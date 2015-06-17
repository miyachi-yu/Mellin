#include "DataSet.hh"
#include "ExpData.hh"
#include "HERMES_multiplicities.hh"
#include "HERMES_MultiQ2.hh"
#include "HERMES_MultiZ.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <list>

using namespace std;
using namespace Utility;
using namespace Exp;

namespace HadronProduction {
  extern DataSet read_data( string& path, string& type, string& info ) throw( int );
};

vector< DataSet > MultiXsecData( const string& hadron ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  vector< DataSet > data;
  
  int ih = 0;
  if( hadron == "PI" )      ih = 30;
  else if( hadron == "KA" ) ih = 40;
  else {
    ARG_ERR << "not yet implemented." << endl;
    return data;
  }  
  
  data.push_back( HERMES_multiplicities::multiplicities( Exp::PRO, Exp::HADRON( ih ) ) );
  data.push_back( HERMES_multiplicities::multiplicities( Exp::PRO, Exp::HADRON( ih + 1 ) ) );
  
  if( ! args.hasOpt( "short" ) ){
    data.push_back( HERMES_multiplicities::multiplicities( Exp::DEU, Exp::HADRON( ih ) ) );
    data.push_back( HERMES_multiplicities::multiplicities( Exp::DEU, Exp::HADRON( ih + 1 ) ) );
  }  
  
  /*

  if( ! args.hasOpt( "Zbin" ) ){
    
    data.push_back( HERMES_MultiQ2::Multi( Exp::PRO, 
    Exp::HADRON( ih ) ) );
    
    data.push_back( HERMES_MultiQ2::Multi( Exp::PRO,
    Exp::HADRON( ih + 1  ) ) );
    
    
    if( ! args.hasOpt( "short" ) ){
    
    data.push_back( HERMES_MultiQ2::Multi( Exp::DEU,
    Exp::HADRON( ih ) ) );
    
    data.push_back( HERMES_MultiQ2::Multi( Exp::DEU, 
    Exp::HADRON( ih + 1 ) ) );
      
    }
    
  } else {
    
    data.push_back( HERMES_MultiZ::Multi( Exp::PRO, 
					  Exp::HADRON( ih ) ) );
    
    data.push_back( HERMES_MultiZ::Multi( Exp::PRO,
					  Exp::HADRON( ih + 1  ) ) );
    
    
    if( ! args.hasOpt( "short" ) ){
      
      data.push_back( HERMES_MultiZ::Multi( Exp::DEU,
					    Exp::HADRON( ih ) ) );
      
      data.push_back( HERMES_MultiZ::Multi( Exp::DEU,
					    Exp::HADRON( ih + 1 ) ) );
      
    }

  }
  */
  
  return data;
  
}

vector< DataSet > fragXsecData( vector< string >& exps,
			        const string& hadron ){
  vector< DataSet > data;
  
  for( vector< string >::iterator etr = exps.begin();
       etr != exps.end(); etr++ ){
    
    string file = data_path + *etr + "_" + hadron + ".dat";
    string type = "e+e-_" + hadron;
    string info = *etr + "_" + hadron ;
    
    ARG_LOG << "Load: " << file << endl;
    
    DataSet ds = HadronProduction::read_data( file, type, info );
    
    data.push_back( ds );
    
    if( *etr == "SLD" ){
      // c and b enriched sample
      file = data_path + *etr + "_C_" + hadron + ".dat";
      type = "e+e-_" + hadron + "_c_enriched";
      DataSet dsc = HadronProduction::read_data( file, type, info );
      data.push_back( dsc );
      
      file = data_path + *etr + "_B_" + hadron + ".dat";
      type = "e+e-_" + hadron + "_b_enriched";
      DataSet dsb = HadronProduction::read_data( file, type, info );
      data.push_back( dsb );
    }
  }
  
  return data;
}

void normalization( vector< DataSet >& data ){
  for( int i = 0; i < data.size(); i++ ){
    data[ i ].norm()  = 0.05;
    data[ i ].ncoef() = 0.0;
    data[ i ].ncerr() = 1.0;
    data[ i ].ncstat() = "free";
  }
}

int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );
  
  ARG_LOG
    << "#  --------------------------------------------------------------  #\n" 
    << "# \n"
    << "#  cretate data table for FragXsec_T,\n"
    << "#                          and hadron multiplicity in SIDIS\n" 
    << "#  possible options are \n"
    << "#    --short     table contains frag and multi on proton\n"
    << "#    --fragExp   experiment name for fragXsec datan\n"
    << "#                (ALEPH,DELPHI,OPAL,SLD,TASSO_34,TASSO_44,TPC)\n"
    << "#    --hadron    hadron type (PI,KA,ALL)\n"
    << "#    --prefix    base of output file name (fragXsec_)\n"
    << "#  --------------------------------------------------------------  #\n" 
    << "# \n"
    << args 
    << endl;
  
  // ---------------------------------------------------------------- //
  //                       FragXsec_T experiments                     //
  // ---------------------------------------------------------------- //
  vector< string > exps;
  exps.push_back( "ALEPH" );
  exps.push_back( "DELPHI" );
  exps.push_back( "OPAL" );
  exps.push_back( "SLD" );
  exps.push_back( "TASSO_34" );
  exps.push_back( "TASSO_44" );
  exps.push_back( "TPC" );
  exps.push_back( "Belle" );
  
  exps = args.get( "fragExp", ",", exps );
  
  ARG_LOG 
    << "#\n"
    << "#  Experiment list used for fragXsec Tables:\n"
    << "#  --fragExp=";
  for( int i = 0; i < exps.size(); i++ )
    ARG_LOG << exps[i] << ( i != exps.size() -1 ? "," : "" );
  ARG_LOG << endl;
  
  // ---------------------------------------------------------------- // 
  //                          Hadron type                             //
  // ---------------------------------------------------------------- //
  vector< string > hads;
  hads.push_back( "PI" );
  hads.push_back( "KA" );
  hads.push_back( "ALL" );

  hads = args.get( "hadron", ",", hads );
  
  ARG_LOG 
    << "#\n"
    << "#  Hadron list for fragXsec and Multi Tables:\n"
    << "#  --hadron=";
  for( int i = 0; i < hads.size(); i++ )
    ARG_LOG << hads[i] << ( i != hads.size() -1 ? "," : "" );
  ARG_LOG << endl;
  
  // ---------------------------------------------------------------- // 
  //                     Load Experiment Data                         //
  // ---------------------------------------------------------------- //
  vector< DataSet > fragPI = fragXsecData( exps, "PI" );
  vector< DataSet > fragKA = fragXsecData( exps, "KA" );
  vector< DataSet > multiPI = MultiXsecData( "PI" );
  vector< DataSet > multiKA = MultiXsecData( "KA" );

  //  normalization( fragPI );
  //  normalization( fragKA );
  //  normalization( multiPI );
  //  normalization( multiKA );

  for( vector< string >::iterator htr = hads.begin();
       htr != hads.end(); htr++ ){
    
    string ofile = args.get( "prefix", "fragXsec_" );
    
    if( args.hasOpt( "short" ) ) ofile += "shortM_";
    if( args.hasOpt( "full" ) )  ofile += "fullM_";
    if( args.hasOpt( "Zbin" ) )  ofile += "Z_";
    
    ofile += *htr + ".xml";
    
    ARG_LOG << " .... creating " << ofile << endl;
    
    // open XML file for the given hadron type
    ofstream ofs( ofile.c_str() );
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    
    if( *htr == "PI" || *htr == "ALL" ){
      for( int i = 0; i < fragPI.size(); i++ ) ofs << fragPI[ i ] << endl;
      for( int i = 0; i < multiPI.size(); i++ ) ofs << multiPI[ i ] << endl;
    }
    
    if( *htr == "KA" || *htr == "ALL" ){
      for( int i = 0; i < fragKA.size(); i++ ) ofs << fragKA[ i ] << endl;
      for( int i = 0; i < multiKA.size(); i++ ) ofs << multiKA[ i ] << endl;
    }
    
    ofs << "</expdata>" << endl;
    ofs.close();
  }
  
  return 0;
}
