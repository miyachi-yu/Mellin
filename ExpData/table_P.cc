#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "P_Data.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace P_Data;

int main( int argc, char* argv[] ){
     
Arguments *args = Arguments::instance( argc, argv );
 
  try {
	  
    DataSet TASSO_P_34_1989 = 
    P_Data::TASSO_P_34_1989( Exp::HADRON( 52 ) );
    ofstream ofs1( "P_Data.xml" );
    ofs1 << ExpData::xmlHeader() << endl;
    ofs1 << "<expdata version=\"1.0\" >" << endl;
    ofs1 << TASSO_P_34_1989 << endl;


    DataSet TASSO_P_44_1989 = 
    P_Data::TASSO_P_44_1989( Exp::HADRON( 52 ) );
    ofs1 << TASSO_P_44_1989 << endl;


    DataSet TPC_P_29_1988 = 
    P_Data::TPC_P_29_1988( Exp::HADRON( 52 ) );
    ofs1 << TPC_P_29_1988 << endl;

    DataSet SLD_P_inclusive_91_2_1999 = 
    P_Data::SLD_P_inclusive_91_2_1999( Exp::HADRON( 52 ) );
    ofs1 << SLD_P_inclusive_91_2_1999 << endl;

    DataSet SLD_P_c_91_2_1999 = 
    P_Data::SLD_P_c_91_2_1999( Exp::HADRON( 52 ) );
    ofs1 << SLD_P_c_91_2_1999 << endl;

    DataSet SLD_P_b_91_2_1999 = 
    P_Data::SLD_P_b_91_2_1999( Exp::HADRON( 52 ) );
    ofs1 << SLD_P_b_91_2_1999 << endl;

    DataSet  OPAL_P_91_2_1994 = 
    P_Data:: OPAL_P_91_2_1994( Exp::HADRON( 52 ) );
    ofs1 <<  OPAL_P_91_2_1994 << endl;

    DataSet ALEPH_P_91_2_1995 = 
    P_Data::ALEPH_P_91_2_1995( Exp::HADRON( 52 ) );
    ofs1 << ALEPH_P_91_2_1995 << endl;

    DataSet DELPHI_P_b_91_2_1998 = 
    P_Data::DELPHI_P_b_91_2_1998( Exp::HADRON( 52 ) );
    ofs1 << DELPHI_P_b_91_2_1998 << endl;

    DataSet DELPHI_P_inclusive_91_2_1998 = 
    P_Data::DELPHI_P_inclusive_91_2_1998( Exp::HADRON( 52 ) );
    ofs1 << DELPHI_P_inclusive_91_2_1998 << endl;

    ofs1 << "</expdata>" << endl;

  }
  
  catch( int err ){
  }

  return 0;
}
