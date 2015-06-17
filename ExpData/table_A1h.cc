#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "HERMES_SIDIS.hh"
#include "COMPASS_PLB680_217.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace HERMES_SIDIS;

int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  try {
    
    DataSet hermesA1h_p_pip = 
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 30 ) );
    
    DataSet compass_A1d_pip = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::PIp );
    DataSet compass_A1d_pim = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::PIm );
    DataSet compass_A1d_kap = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::Kp );
    DataSet compass_A1d_kam = COMPASS_PLB680_217::A1h( Exp::DEU, Exp::Km );
    
    ofstream ofs( "A1h_p_pip.xml" );
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    ofs << hermesA1h_p_pip << endl;
    ofs << "</expdata>" << endl;

    DataSet hermesA1h_p_pim =
      HERMES_SIDIS::A1h( Exp::TARGET( 1 ), Exp::HADRON( 31 ) );
    ofstream ofs1( "A1h_p_pim.xml" );
    ofs1 << ExpData::xmlHeader() << endl;
    ofs1 << "<expdata version=\"1.0\" >" << endl;
    ofs1 << hermesA1h_p_pim << endl;
    ofs1 << "</expdata>" << endl;
    
    DataSet hermesA1h_d_pip = 
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 30 ) );
    ofstream ofs2( "A1h_d_pip.xml" );
    ofs2 << ExpData::xmlHeader() << endl;
    ofs2 << "<expdata version=\"1.0\" >" << endl;
    ofs2 << hermesA1h_d_pip << endl;
    ofs2 << compass_A1d_pip << endl;
    ofs2 << "</expdata>" << endl;
    
    DataSet hermesA1h_d_pim =
      HERMES_SIDIS::A1h( Exp::TARGET( 3 ), Exp::HADRON( 31 ) );
    ofstream ofs3( "A1h_d_pim.xml" );
    ofs3 << ExpData::xmlHeader() << endl;
    ofs3 << "<expdata version=\"1.0\" >" << endl;
    ofs3 << hermesA1h_d_pim << endl;
    ofs2 << compass_A1d_pim << endl;
    ofs3 << "</expdata>" << endl;
    
    /*
      DataSet SMCA1h_p_hp = 
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 1 ), Exp::HADRON( 20 ) );
      
      DataSet SMCA1h_p_hm =
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 1 ), Exp::HADRON( 21 ) );
      
      DataSet SMCA1h_d_hp = 
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 3 ), Exp::HADRON( 20 ) );
    
      DataSet SMCA1h_d_hm =
      HERMES_SIDIS::A1h_SMC( Exp::TARGET( 3 ), Exp::HADRON( 21 ) );
    */
    
    /*
      ofs << SMCA1h_p_hp << endl;
      ofs << SMCA1h_p_hm << endl;
      ofs << SMCA1h_d_hp << endl;
      ofs << SMCA1h_d_hm << endl;
    */
    
  }
  
  catch( int err ){
  }
  
  return 0;
}
