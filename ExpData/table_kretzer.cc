#include "DataSet.hh"
#include "ExpData.hh"
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

int main( int argc, char* argv[] ) {

  Arguments *args = Arguments::instance( argc, argv );
  
  list< string > exps;
  //  exps.push_back( "ALEPH" );
  exps.push_back( "SLD" );
  exps.push_back( "TPC" );
  
  list< string > hads;
  hads.push_back( "PI" );
  hads.push_back( "KA" );

  for( list< string >::iterator htr = hads.begin();
       htr != hads.end(); htr++ ){
    
    string ofile = "kretzer";
    if( args->hasOpt( "full" ) )  ofile += "_fullM";
    if( args->hasOpt( "short" ) )  ofile += "_shortM";
    if( args->hasOpt( "Zbin" ) )  ofile += "_Z";
    ofile += "_" + *htr;
    ofile += ".xml";

    ofstream ofs( ofile.c_str() );
        
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    
    for( list< string >::iterator etr = exps.begin();
	 etr != exps.end(); etr++ ){
      
      string file = data_path + *etr + "_" + *htr + ".dat";
      string type = "e+e-_" + *htr;
      string info = *etr + "_" + *htr ;
      DataSet ds = HadronProduction::read_data( file, type, info );
      
      ofs << ds << endl;
      
      if( *etr == "SLD" ){
	// c and b enriched sample
	file = data_path + *etr + "_C_" + *htr + ".dat";
	type = "e+e-_" + *htr + "_c_enriched";
	DataSet dsc = HadronProduction::read_data( file, type, info );
	ofs << dsc << endl;
	
	file = data_path + *etr + "_B_" + *htr + ".dat";
	type = "e+e-_" + *htr + "_b_enriched";
	DataSet dsb = HadronProduction::read_data( file, type, info );
	ofs << dsb << endl;
      }

    }

    if( !args->hasOpt( "Zbin" ) ){

      if( *htr == "PI" ){
	DataSet MultiQ2_p_pip = 
	  HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 30 ) );
	
	DataSet MultiQ2_p_pim =
	  HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 31  ) );
      
	DataSet MultiQ2_d_pip = 
	  HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 30 ) );
	
	DataSet MultiQ2_d_pim =
	  HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 31 ) );
	
	if( args->hasOpt( "short" ) ){
	  ofs << MultiQ2_p_pip << endl;
	  ofs << MultiQ2_p_pim << endl;
	} else if( args->hasOpt( "full" ) ){
	  ofs << MultiQ2_p_pip << endl;
	  ofs << MultiQ2_p_pim << endl;
	  ofs << MultiQ2_d_pip << endl;
	  ofs << MultiQ2_d_pim << endl;
	}
      }
      
      if( *htr == "KA" ){
	DataSet MultiQ2_p_pip = 
	  HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 40 ) );
	
	DataSet MultiQ2_p_pim =
	  HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 41  ) );
	DataSet MultiQ2_d_pip = 
	  HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 40 ) );
      
	DataSet MultiQ2_d_pim =
	  HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 41 ) );
	if( args->hasOpt( "short" ) ){
	  ofs << MultiQ2_p_pip << endl;
	  ofs << MultiQ2_p_pim << endl;
	} else if( args->hasOpt( "full" ) ){
	  ofs << MultiQ2_p_pip << endl;
        ofs << MultiQ2_p_pim << endl;
        ofs << MultiQ2_d_pip << endl;
        ofs << MultiQ2_d_pim << endl;
	}
      }
  
    } else {

      if( *htr == "PI" ){
	DataSet MultiZ_p_pip = 
	  HERMES_MultiZ::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 30 ) );
	
	DataSet MultiZ_p_pim =
	  HERMES_MultiZ::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 31  ) );
      
	DataSet MultiZ_d_pip = 
	  HERMES_MultiZ::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 30 ) );
	
	DataSet MultiZ_d_pim =
	  HERMES_MultiZ::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 31 ) );
	
	if( args->hasOpt( "short" ) ){
	  ofs << MultiZ_p_pip << endl;
	  ofs << MultiZ_p_pim << endl;
	} else if( args->hasOpt( "full" ) ){
	  ofs << MultiZ_p_pip << endl;
	  ofs << MultiZ_p_pim << endl;
	  ofs << MultiZ_d_pip << endl;
	  ofs << MultiZ_d_pim << endl;
	}
      }
      
      if( *htr == "KA" ){
	DataSet MultiZ_p_pip = 
	  HERMES_MultiZ::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 40 ) );
	
	DataSet MultiZ_p_pim =
	  HERMES_MultiZ::Multi( Exp::TARGET( 1 ), 
				 Exp::HADRON( 41  ) );
	DataSet MultiZ_d_pip = 
	  HERMES_MultiZ::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 40 ) );
      
	DataSet MultiZ_d_pim =
	  HERMES_MultiZ::Multi( Exp::TARGET( 3 ), 
				 Exp::HADRON( 41 ) );
	if( args->hasOpt( "short" ) ){
	  ofs << MultiZ_p_pip << endl;
	  ofs << MultiZ_p_pim << endl;
	} else if( args->hasOpt( "full" ) ){
	  ofs << MultiZ_p_pip << endl;
        ofs << MultiZ_p_pim << endl;
        ofs << MultiZ_d_pip << endl;
        ofs << MultiZ_d_pim << endl;
	}
      }

    }
    
    ofs << "</expdata>" << endl;
    ofs.close();
  }
  
  return 0;
}
