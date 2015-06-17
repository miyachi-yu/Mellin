//
// mkExxpData.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <yoshiyuki@yoshiyuki-laptop>
// 
// Started on  Sat Jan 10 20:09:32 2009 Yoshiyuki Miyachi
// Started on  Sat Jan 10 20:09:32 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include <Utility/Arguments.hh>

#include "ExpData.hh"
#include "DataSet.hh"

#include "PI0_Data.hh"
#include "PIpm_Data.hh"
#include "Kpm_Data.hh"
#include "P_Data.hh"
#include "hpm_Data.hh"
#include "A1_Data.hh"
#include "HERMES_multiplicities.hh"
#include "HERMES_A1.hh"
#include "EMC_Multi.hh"
#include "Belle.hh"

using namespace std;

class ExpFlag {
public:
  
  ExpFlag( const vector< string >& name ) : 
    hermes( false ), tasso( false ), tpc( false ), l3( false ),
    aleph( false ), opal( false ), delphi( false ), sld( false ),
    emc( false ), nmc( false ), smc( false ), compass( false ),
    e142( false ), e143( false ), e154( false ), e155( false ), e156( false ),
    jlab( false )
  {
    
    for( int i = 0; i < name.size(); i++ ){
      if( name[ i ] == "HERMES" )  hermes  = true;
      if( name[ i ] == "TASSO" )   tasso   = true;
      if( name[ i ] == "TPC" )     tpc     = true;
      if( name[ i ] == "L3" )      l3      = true;
      if( name[ i ] == "ALEPH" )   aleph   = true;
      if( name[ i ] == "DELPHI" )  delphi  = true;
      if( name[ i ] == "SLD" )     sld     = true;
      if( name[ i ] == "OPAL" )    opal    = true;
      if( name[ i ] == "EMC" )     emc     = true;
      if( name[ i ] == "NMC" )     nmc     = true;
      if( name[ i ] == "SMC" )     smc     = true;
      if( name[ i ] == "COMPASS" ) compass = true;
      if( name[ i ] == "E142" )    e142    = true;
      if( name[ i ] == "E143" )    e143    = true;
      if( name[ i ] == "E154" )    e154    = true;
      if( name[ i ] == "E155" )    e155    = true;
      if( name[ i ] == "E156" )    e156    = true;
      if( name[ i ] == "JLAB" )    jlab    = true;
      if( name[ i ] == "Belle" )    belle    = true;
    }
  }
  
  bool tasso;
  bool tpc;
  bool l3;
  bool aleph;
  bool opal;
  bool delphi;
  bool sld;
  bool hermes;
  bool emc;
  bool nmc;
  bool smc;
  bool compass;
  bool e142;
  bool e143;
  bool e154;
  bool e155;
  bool e156;
  bool jlab;
  bool belle;

};

void Usage(){

  Utility::Arguments& args = Utility::Arguments::ref();
  
  args.usage( "frag",  "", "hadron type for fragXsec (pion,kaon,proton,hadron,pi0,ALL)");
  args.usage( "multi", "", 
	      "target-hadron type for multiplicity (p-pion,p-kaon,d-pion,d-kaon,p-proton,d-proton,ALL)");
  //  args.usage( "multiType", "Q2", "multiplicity data type (Z or Q2)" );
  args.usage( "A1", "", "p,d,n,p-pion,p-kaon,p-hadron,d-pion,d-kaon,d-hadron,ALL" );
  args.usage( "oexpdata", "expdata.xml", "default output file name" );
  
  args.usage( "experiment", "ALL", "Experiments to be included." );
  
  args.usage( "lambda", "1.0", "weight for chi2 calculation" );
  
}

// ------------------------------------------------------------------- //
void a1( vector< Exp::DataSet >& vset, 
	 const vector< string >& type,
	 const ExpFlag& eflag	    ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  try{ 
    
    for( int i = 0; i < type.size(); i++ ){
      
      ARG_LOG << "** -- Loading" << setw(8) << type[ i ] << " data"
	      << endl;
      
      Exp::TARGET n;
      Exp::HADRON hp = Exp::undefined;
      Exp::HADRON hm = Exp::undefined;
      
      if( type[ i ] == "p" ){
	n = Exp::PRO; hp = Exp::undefined; hm = Exp::undefined; 
      } else if( type[ i ] == "d" ){
	n = Exp::DEU; hp = Exp::undefined; hm = Exp::undefined; 
      } else if( type[ i ] == "n" ){
	n = Exp::NEU; hp = Exp::undefined; hm = Exp::undefined; 
      } else if( type[ i ] == "p-pion" ){
	n = Exp::PRO; hp = Exp::PIp; hm = Exp::PIm; 
      } else if ( type[ i ] == "p-kaon" ){
	n = Exp::PRO; hp = Exp::Kp; hm = Exp::Km; 
      } else if ( type[ i ] == "p-proton" ){
	n = Exp::PRO; hp = Exp::PRp; hm = Exp::PRm; 
	ARG_ERR << "hadron " << type[ i ] << "is not yet implemented" << endl;
	continue;
      } else if ( type[ i ] == "d-pion" ){
	n = Exp::DEU; hp = Exp::PIp; hm = Exp::PIm; 
      } else if ( type[ i ] == "d-kaon" ){
	n = Exp::DEU; hp = Exp::Kp; hm = Exp::Km; 
      } else if ( type[ i ] == "d-proton" ){
	n = Exp::DEU; hp = Exp::PRp; hm = Exp::PRm; 
	ARG_ERR << "hadron " << type[ i ] << "is not yet implemented" << endl;
	continue;
      } else {
	ARG_ERR << "hadron " << type[ i ] << "is not yet implemented" << endl;
	continue;
      }
      
      if( eflag.hermes ){
	if( hp == Exp::undefined ) { 
	  if( n == Exp::NEU ) vset.push_back( Exp::HERMES_A1::A1n( n ) );
	  else vset.push_back( Exp::HERMES_A1::A1( n ) );
	} else {
	  
	}
      } else {
      }
      
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error " << error << endl;
    throw( error );
  }
  
}
  // ------------------------------------------------------------------- //
void multi( vector< Exp::DataSet >& vset, 
	    const vector< string >& type,
	    const ExpFlag& eflag	    ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  //  bool zdata = ( args.get( "multiType", "Q2" ) == "Z" );
  
  try{ 

    for( int i = 0; i < type.size(); i++ ){
      
      //      ARG_LOG << "** -- Loading" << setw(8) << type[ i ] << " data (" 
      //	      << args.get( "multiType", "Z" ) << ")" << endl;
      
      Exp::TARGET n;
      Exp::HADRON hp;
      Exp::HADRON hm;
      
      if( type[ i ] == "p-pion" ){
	n = Exp::PRO; hp = Exp::PIp; hm = Exp::PIm; 
      } else if ( type[ i ] == "p-kaon" ){
	n = Exp::PRO; hp = Exp::Kp; hm = Exp::Km; 
      } else if ( type[ i ] == "p-proton" ){
	n = Exp::PRO; hp = Exp::PRp; hm = Exp::PRm; 
	ARG_ERR << "hadron " << type[ i ] << " is not yet implemented" << endl;
	continue;
      } else if ( type[ i ] == "d-pion" ){
	n = Exp::DEU; hp = Exp::PIp; hm = Exp::PIm; 
      } else if ( type[ i ] == "d-kaon" ){
	n = Exp::DEU; hp = Exp::Kp; hm = Exp::Km; 
      } else if ( type[ i ] == "d-proton" ){
	n = Exp::DEU; hp = Exp::PRp; hm = Exp::PRm; 
	ARG_ERR << "hadron " << type[ i ] << " is not yet implemented" << endl;
	continue;
      } else if ( type[ i ] == "p-hadron" ){
	n = Exp::PRO; hp = Exp::Hp; hm = Exp::Hm; 
      } else if ( type[ i ] == "d-hadron" ){
	n = Exp::DEU; hp = Exp::Hp; hm = Exp::Hm; 
      } else {
	ARG_ERR << "hadron " << type[ i ] << " is not yet implemented" << endl;
	continue;
      } 
      
      //      if( zdata ) { 
      /*
      } else {
	if( eflag.hermes ){
	  vset.push_back( Exp::HERMES_MultiQ2::Multi( n, hp ) );
	  vset.push_back( Exp::HERMES_MultiQ2::Multi( n, hm ) );
	}
      }
	*/
      if( eflag.hermes && n != Exp::NEU ){

	if( hp == Exp::PIp || hp == Exp::Kp )
	  vset.push_back( Exp::HERMES_multiplicities::multiplicities( n, hp ) );
	
	if( hm == Exp::PIm || hm == Exp::Km )
	  vset.push_back( Exp::HERMES_multiplicities::multiplicities( n, hm ) );

      }
      
      if( eflag.emc && ( hp == Exp::Hp && hm == Exp::Hm && n != Exp::NEU ) ){
	vset.push_back( Exp::EMC_Multi::Multi( n, hp ) );
	vset.push_back( Exp::EMC_Multi::Multi( n, hm ) );
      }
      
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error " << error << endl;
    throw( error );
  }

}

// ------------------------------------------------------------------- //
void fragXsec( vector< Exp::DataSet >& vset, 
	       const vector< string >& type,
	       const ExpFlag& eflag ) throw( int ) {
  
  try{ 
    
    for( int i = 0; i < type.size(); i++ ){
      
      ARG_LOG << "** -- Loagind" << setw(8) << type[ i ] << " data" << endl;
      
      if( type[ i ] == "pi0" ){
	
	if( eflag.tasso ) vset.push_back( Exp::PI0_Data::TASSO_PI0_44_1989() );
	if( eflag.tpc   ) vset.push_back( Exp::PI0_Data::TPC_PI0_29_1985() );
	if( eflag.l3    ) vset.push_back( Exp::PI0_Data::L3_PI0_91_2_1994() );
	if( eflag.aleph ) vset.push_back( Exp::PI0_Data::ALEPH_PI0_91_2_2000() );
	if( eflag.opal  ) vset.push_back( Exp::PI0_Data::OPAL_PI0_91_2_1998() );
	if( eflag.delphi ) vset.push_back( Exp::PI0_Data::DELPHI_PI0_91_2_1996() );
	//	vset.push_back( Exp::PI0_Data::DELPHI_PI0_inclusive_91_2_1998() );
	//	vset.push_back( Exp::PI0_Data::DELPHI_PI0_b_91_2_1998() );

      } else if( type[ i ] == "pion" ) {

	if( eflag.tasso ) {
	  vset.push_back( Exp::PIpm_Data::TASSO_PIpm_44_1989() );
	  vset.push_back( Exp::PIpm_Data::TASSO_PIpm_34_1989() );
	}
	
	if( eflag.tpc   ) vset.push_back( Exp::PIpm_Data::TPC_PIpm_29_1988() );

	if( eflag.sld ) {
	  vset.push_back( Exp::PIpm_Data::SLD_PIpm_inclusive_91_2_1999() );
	  vset.push_back( Exp::PIpm_Data::SLD_PIpm_c_91_2_1999() );
	  vset.push_back( Exp::PIpm_Data::SLD_PIpm_b_91_2_1999() );
	}
	
	if( eflag.aleph ) vset.push_back( Exp::PIpm_Data::ALEPH_PIpm_91_2_1995() );
	
	if( eflag.opal  ) vset.push_back( Exp::PIpm_Data::OPAL_PIpm_91_2_1994() );
	
	if( eflag.delphi ) {
	  vset.push_back( Exp::PIpm_Data::DELPHI_PIpm_inclusive_91_2_1998() );
	  vset.push_back( Exp::PIpm_Data::DELPHI_PIpm_b_91_2_1998() );
	}	
	
	if( eflag.belle ) vset.push_back( Exp::Belle::pion() );

      } else if( type[ i ] == "kaon" ) {
	
	if( eflag.tasso ) {
	  vset.push_back( Exp::Kpm_Data::TASSO_Kpm_44_1989() );
	  vset.push_back( Exp::Kpm_Data::TASSO_Kpm_34_1989() );
	}
	
	if( eflag.tpc   ) vset.push_back( Exp::Kpm_Data::TPC_Kpm_29_1988() );
	
	if( eflag.sld ) {
	  vset.push_back( Exp::Kpm_Data::SLD_Kpm_inclusive_91_2_1999() );
	  vset.push_back( Exp::Kpm_Data::SLD_Kpm_c_91_2_1999() );
	  vset.push_back( Exp::Kpm_Data::SLD_Kpm_b_91_2_1999() );
	}
	
	if( eflag.aleph ) vset.push_back( Exp::Kpm_Data::ALEPH_Kpm_91_2_1995() );

	if( eflag.opal  ) vset.push_back( Exp::Kpm_Data::OPAL_Kpm_91_2_1994() );

	if( eflag.delphi ) {
	  //	  vset.push_back( Exp::Kpm_Data::DELPHI_Kpm_91_2_1995() );
	  vset.push_back( Exp::Kpm_Data::DELPHI_Kpm_inclusive_91_2_1998() );
	  vset.push_back( Exp::Kpm_Data::DELPHI_Kpm_b_91_2_1998() );
	}
	
	if( eflag.belle ) vset.push_back( Exp::Belle::kaon() );

      } else if( type[ i ] == "proton" ) {
    
	if( eflag.tasso ) {
	  vset.push_back( Exp::P_Data::TASSO_P_44_1989() );
	  vset.push_back( Exp::P_Data::TASSO_P_34_1989() );
	}
	
	if( eflag.tpc   ) vset.push_back( Exp::P_Data::TPC_P_29_1988() );

	if( eflag.sld ) {
	  vset.push_back( Exp::P_Data::SLD_P_inclusive_91_2_1999() );
	  vset.push_back( Exp::P_Data::SLD_P_c_91_2_1999() );
	  vset.push_back( Exp::P_Data::SLD_P_b_91_2_1999() );
	}
	
	if( eflag.aleph ) vset.push_back( Exp::P_Data::ALEPH_P_91_2_1995() );
	if( eflag.opal  ) vset.push_back( Exp::P_Data::OPAL_P_91_2_1994() );
	
	if( eflag.delphi ) {
	  vset.push_back( Exp::P_Data::DELPHI_P_inclusive_91_2_1998() );
	  vset.push_back( Exp::P_Data::DELPHI_P_b_91_2_1998() );
	}
	
      } else if( type[ i ] == "hadron" ) {
	
	if( eflag.tasso ) {
	  vset.push_back( Exp::hpm_Data::TASSO_hpm_44_1990() );
	  vset.push_back( Exp::hpm_Data::TASSO_hpm_35_1990() );
	}
	
	if( eflag.tpc   ) vset.push_back( Exp::hpm_Data::TPC_hpm_29_1988() );
	
	if( eflag.sld ) vset.push_back( Exp::hpm_Data::SLD_hpm_91_2_1999() );
	
	if( eflag.aleph ) vset.push_back( Exp::hpm_Data::ALEPH_hpm_91_2_1998() );
	
	if( eflag.opal  ) {
	  //	vset.push_back( Exp::hpm_Data::OPAL_hpm_transverse_91_2_1995() );
	  //	vset.push_back( Exp::hpm_Data::OPAL_hpm_longitudinal_91_2_1995() );
	  vset.push_back( Exp::hpm_Data::OPAL_hpm_b_91_2_1999() );
	  vset.push_back( Exp::hpm_Data::OPAL_hpm_inclusive_91_2_1999() );
	  vset.push_back( Exp::hpm_Data::OPAL_hpm_c_91_2_1999() );
	}
	
	if( eflag.delphi ) vset.push_back( Exp::hpm_Data::DELPHI_hpm_91_2_1996() );

      } else {
	
	ARG_ERR << "hadron " << type[ i ] << "is not yet implemented" << endl;
	
      }
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error " << error << endl;
    throw( error );
  }

}



int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( args.hasOpt( "help" ) ){
    Usage();
    return 1;
  }  

  // ------------------------------------------------------------------- //
  //                             message                                 //
  // ------------------------------------------------------------------- //
  ARG_LOG << args << endl;
  
  // ------------------------------------------------------------------- //
  //                             configuration                           //
  // ------------------------------------------------------------------- //
  
  // --- fragXsec data
  vector< string > fragType( 0 );
  fragType = args.get( "frag", ",", fragType );
  
  if( fragType.size() > 0 && fragType[ 0 ] == "ALL" ){
    fragType.clear(); // reset stored data
    fragType.push_back( "pion" );
    fragType.push_back( "kaon" );
    fragType.push_back( "proton" );
    fragType.push_back( "hadron" );
    fragType.push_back( "pi0" );
  }
  
  if( fragType.size() > 0 ){
    ARG_LOG << "** FragXsec data will be generated for " << endl; 
    for( int i = 0; i < fragType.size(); i++ ){
      ARG_LOG << "** -- " << fragType[ i ] << endl;
    }
  }
  
  // --- multiplicity data
  vector< string > multiType( 0 );
  multiType = args.get( "multi", ",", multiType );
  
  if( multiType.size() > 0 && multiType[ 0 ] == "ALL" ){
    multiType.clear(); // reset stored data
    multiType.push_back( "p-pion" );
    multiType.push_back( "p-kaon" );
    multiType.push_back( "p-proton" );
    multiType.push_back( "p-hadron" );
    multiType.push_back( "d-pion" );
    multiType.push_back( "d-kaon" );
    multiType.push_back( "d-proton" );
    multiType.push_back( "d-hadron" );
  }
  
  if( multiType.size() > 0 ){
    ARG_LOG << "** Multiplicity data will be generated for " << endl; 
    for( int i = 0; i < multiType.size(); i++ ){
      ARG_LOG << "** -- " << multiType[ i ] << endl;
    }
  }
  
  // --- A1 or A1h data
  vector< string > a1Type( 0 );
  a1Type = args.get( "A1", ",", a1Type );
  
  if( a1Type.size() > 0 && a1Type[ 0 ] == "ALL" ){
    a1Type.clear();
    a1Type.push_back( "p" );
    a1Type.push_back( "d" );
    a1Type.push_back( "n" );
    a1Type.push_back( "p-hadron" );
    a1Type.push_back( "p-pion" );
    a1Type.push_back( "p-kaon" );
    a1Type.push_back( "d-hadron" );
    a1Type.push_back( "d-pion" );
    a1Type.push_back( "d-kaon" );
  }
  
  if( a1Type.size() > 0 ){
    ARG_LOG << "** A1 data will be generated for " << endl; 
    for( int i = 0; i < a1Type.size(); i++ ){
      ARG_LOG << "** -- " << a1Type[ i ] << endl;
    }
  }
  
  // --- Experiment
  vector< string > expType( 1, "ALL" );
  expType = args.get( "experiment", ",", expType );
  if( expType.size() > 0 && expType[ 0 ] == "ALL" ){
    expType.clear();
    expType.push_back( "HERMES" );
    expType.push_back( "TPC" );
    expType.push_back( "TASSO" );
    expType.push_back( "L3" );
    expType.push_back( "ALEPH" );
    expType.push_back( "DELPHI" );
    expType.push_back( "OPAL" );
    expType.push_back( "SLD" );
    expType.push_back( "EMC" );
    expType.push_back( "NMC" );
    expType.push_back( "SMC" );
    expType.push_back( "COMPASS" );
    expType.push_back( "E142" );
    expType.push_back( "E143" );
    expType.push_back( "E154" );
    expType.push_back( "E155" );
    expType.push_back( "E156" );
    expType.push_back( "JLAB" );
  }
  ExpFlag eflag( expType );
  
  if( fragType.size()  == 0 &&
      multiType.size() == 0 &&
      a1Type.size()    == 0 ){
    
    cerr << endl;
    cerr << endl;
    cerr << "+---------------------------------------------------------------------+" << endl;
    cerr << "|                                                                     |+" << endl;
    cerr << "|   You must specify data type with --frag and/or --multi options.    ||" << endl;
    cerr << "|                                                                     ||" << endl;
    cerr << "+---------------------------------------------------------------------+|" << endl;
    cerr << " +---------------------------------------------------------------------+" << endl;
    cerr << endl;
    cerr << endl;
    
    Usage();
    
    return 4;
  }
  
  // ------------------------------------------------------------------- //
  //                             load data                               //
  // ------------------------------------------------------------------- //
  Exp::ExpData edata;

  try {
    
    if( fragType.size() > 0  ) fragXsec( edata.data(), fragType,  eflag );
    if( multiType.size() > 0 ) multi(    edata.data(), multiType, eflag );
    if( a1Type.size() > 0    ) a1(       edata.data(), a1Type,    eflag );
    
  }
  
  catch( int error ){
    ARG_ERR << "catch error " << error << endl;
    return 3;
  }
  
  if( edata.data().size() == 0 ){
    ARG_ERR << "No Exp::DataSet object created. Please check your options." << endl;
    Usage();
    return 4;
  }
  
  // ------------------------------------------------------------------- //
  //                            output file                              //
  // ------------------------------------------------------------------- //
  string ofilename = args.get( "oexpdata", "expdata.xml" );
  ofstream ofs( ofilename.c_str() );
  if( ! ofs ){
    ARG_ERR << "fail to open " << ofilename << endl;
    return 2;
  } else {
    ARG_LOG << "** open " << ofilename << endl;
  }
  
  // ------------------------------------------------------------------- //
  //                           chi2 weight                               //
  // ------------------------------------------------------------------- //
  edata.version() = args.get( "version", "1.0" );

  // ------------------------------------------------------------------- //
  //                           version info                              //
  // ------------------------------------------------------------------- //
  edata.lambda()  = args.get( "lambda", 1.0 );

  ofs << edata << endl;

  return 0;
}

