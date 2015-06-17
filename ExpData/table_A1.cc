#include "ExpData.hh"
#include "DataSet.hh"
#include "EMC_A1.hh"
#include "E143_A1.hh"
#include "E155_A1.hh"
#include "HERMES_A1.hh"
#include "SMC_PhysRevD58_A1.hh"
#include "A1_Data.hh"
#include "HERMES_SIDIS.hh"
#include "COMPASS_PLB680_217.hh"
#include "COMPASS_PLB690_466.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;

int main( int argc, char* argv[] ){


  Arguments *args = Arguments::instance( argc, argv );
  
  try {

//proton Target
    DataSet emc_p_ = 
      EMC_A1::A1( Exp::TARGET( 1 ) );

    DataSet e155_p_ =
      E155_A1::A1( Exp::TARGET( 1 ) );

    DataSet e143_p_ = 
      E143_A1::A1( Exp::TARGET( 1 ) );

    DataSet hermes_p_ =
      HERMES_A1::A1( Exp::TARGET( 1 ) );

    DataSet smc_p_ = 
      SMC_PhysRevD58_A1::A1( Exp::TARGET( 1 ) );

    DataSet clas_p_ = 
      A1_Data::A1CLAS( Exp::TARGET( 1 ) );

//neutron Target
    DataSet e155_n_ =
      E155_A1::A1( Exp::TARGET( 2 ) );

    DataSet e143_n_ = 
      E143_A1::A1( Exp::TARGET( 2 ) );

    DataSet hermes_n_ =
      HERMES_A1::A1n( Exp::TARGET( 2 ) );

    DataSet e142_n_ = 
      A1_Data::A1e142( Exp::TARGET( 2 ) );

    DataSet e154_n_ = 
	  A1_Data::A1e154( Exp::TARGET( 2 ) );

    DataSet jlab_n_ = 
      A1_Data::A1( Exp::TARGET( 2 ) );

//deuteron Target
    DataSet e155_d_ = 
      E155_A1::A1( Exp::TARGET( 3 ) );

    DataSet e143_d_ = 
      E143_A1::A1( Exp::TARGET( 3 ) );

    DataSet hermes_d_ =
      HERMES_A1::A1( Exp::TARGET( 3 ) );

    DataSet smc_d_ = 
      SMC_PhysRevD58_A1::A1( Exp::TARGET( 3 ) );
    
    DataSet compass_p_ = COMPASS_PLB690_466::A1( Exp::PRO );

    DataSet compass_d_ = 
      A1_Data::A1compass( Exp::TARGET( 3 ) );


    //HERMES_SIDIS
    DataSet hermesA1h_p_pip = 
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 30 ) );

    DataSet hermesA1h_p_pim =
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 31 ) );

    DataSet hermesA1h_d_pip = 
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 30 ) );

    DataSet hermesA1h_d_pim =
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 31 ) );

    DataSet hermesA1h_p_kap = 
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 40 ) );

    DataSet hermesA1h_p_kam =
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 41 ) );

    DataSet hermesA1h_d_kap = 
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 40 ) );

    DataSet hermesA1h_d_kam =
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 41 ) );

    DataSet hermesA1h_p_hp = 
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 20 ) );

    DataSet hermesA1h_p_hm =
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 21 ) );

    DataSet hermesA1h_d_hp = 
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 20 ) );

    DataSet hermesA1h_d_hm =
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 21 ) );

//SMC_SIDIS
    DataSet SMCA1h_p_hp = 
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 1 ), Exp::HADRON( 20 ) );
    
    DataSet SMCA1h_p_hm =
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 1 ), Exp::HADRON( 21 ) );
    
    DataSet SMCA1h_d_hp = 
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 3 ), Exp::HADRON( 20 ) );
    
    DataSet SMCA1h_d_hm =
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 3 ), Exp::HADRON( 21 ) );
    
    // compass A1h
    DataSet compassA1h_d_pip = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::PIp );
    DataSet compassA1h_d_pim = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::PIm );
    DataSet compassA1h_d_kap = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::Kp );
    DataSet compassA1h_d_kam = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::Km );
    

    cout << "Creating A1_A1h.xml" << endl;

    ofstream ofs( "A1_A1h.xml" );
    //    ofstream ofs( "A1.xml" );
    
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    
//proton Target
    
    ofs << emc_p_ << endl;
    ofs << e155_p_ << endl;
    ofs << e143_p_ << endl;
    ofs << hermes_p_ << endl;
    ofs << smc_p_ << endl;
    //    ofs << clas_p_ << endl;
    
//neutoron Target

    ofs << e155_n_ << endl;
    ofs << e143_n_ << endl;
    ofs << hermes_n_ <<endl;
    ofs << e142_n_ << endl;
    ofs << e154_n_ << endl;
    ofs << jlab_n_ << endl;

//deuteron Target

    ofs << e155_d_ << endl;
    ofs << e143_d_ << endl;
    ofs << hermes_d_ << endl;
    ofs << smc_d_ << endl;
    ofs << compass_p_ << endl;
    ofs << compass_d_ << endl;

//HERMES_SIDIS

    ofs << hermesA1h_p_pip << endl;
    ofs << hermesA1h_p_pim << endl;
    ofs << hermesA1h_d_pip << endl;
    ofs << hermesA1h_d_pim << endl;
    ofs << hermesA1h_p_kap << endl;
    ofs << hermesA1h_p_kam << endl;
    ofs << hermesA1h_d_kap << endl;
    ofs << hermesA1h_d_kam << endl;
    ofs << hermesA1h_p_hp << endl;
    ofs << hermesA1h_p_hm << endl;
    ofs << hermesA1h_d_hp << endl;
    ofs << hermesA1h_d_hm << endl;

//SMC_SIDIS

    ofs << SMCA1h_p_hp << endl;
    ofs << SMCA1h_p_hm << endl;
    ofs << SMCA1h_d_hp << endl;
    ofs << SMCA1h_d_hm << endl;

    // COMPASS SIDIS
    ofs << compassA1h_d_pip << endl;
    ofs << compassA1h_d_pim << endl;
    ofs << compassA1h_d_kap << endl;
    ofs << compassA1h_d_kam << endl;

    ofs << "</expdata>" << endl;
    
  }
  
  catch( int err ){
  }

  return 0;
}
