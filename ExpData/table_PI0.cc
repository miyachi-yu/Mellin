#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "PI0_Data.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace PI0_Data;

int main( int argc, char* argv[] ){
     
Arguments *args = Arguments::instance( argc, argv );
 
  try {
    
    DataSet TASSO_PI0_44_1989 = 
    PI0_Data::TASSO_PI0_44_1989(  Exp::HADRON( 33 ) );
    ofstream ofs2( "PI0_Data.xml" );
    ofs2 << ExpData::xmlHeader() << endl;
    ofs2 << "<expdata version=\"1.0\" >" << endl;
    ofs2 << TASSO_PI0_44_1989 << endl;


    DataSet TPC_PI0_29_1985 = 
    PI0_Data::TPC_PI0_29_1985( Exp::HADRON( 33 ) );
    ofs2 << TPC_PI0_29_1985 << endl;

    DataSet L3_PI0_91_2_1994 = 
    PI0_Data::L3_PI0_91_2_1994( Exp::HADRON( 33 ) );
    ofs2 << L3_PI0_91_2_1994 << endl;

    DataSet  OPAL_PI0_91_2_1998 = 
    PI0_Data:: OPAL_PI0_91_2_1998( Exp::HADRON( 33 ) );
    ofs2 <<  OPAL_PI0_91_2_1998 << endl;

    DataSet ALEPH_PI0_91_2_2000 = 
    PI0_Data::ALEPH_PI0_91_2_2000( Exp::HADRON( 33 ) );
    ofs2 << ALEPH_PI0_91_2_2000 << endl;

    DataSet DELPHI_PI0_91_2_1996 = 
    PI0_Data::DELPHI_PI0_91_2_1996( Exp::HADRON( 33 ) );
    ofs2 << DELPHI_PI0_91_2_1996 << endl;


    ofs2 << "</expdata>" << endl;
  }
  
  catch( int err ){
  }

  return 0;
}
