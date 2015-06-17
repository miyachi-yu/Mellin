#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "PIpm_Data.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace PIpm_Data;

int main( int argc, char* argv[] ){
     
Arguments *args = Arguments::instance( argc, argv );
 
  try {
	  
    DataSet TASSO_PIpm_34_1989 = 
    PIpm_Data::TASSO_PIpm_34_1989( Exp::HADRON( 32 ) );
    ofstream ofs1( "PIpm_Data.xml" );
    ofs1 << ExpData::xmlHeader() << endl;
    ofs1 << "<expdata version=\"1.0\" >" << endl;
    ofs1 << TASSO_PIpm_34_1989 << endl;
   

    DataSet TASSO_PIpm_44_1989 = 
    PIpm_Data::TASSO_PIpm_44_1989( Exp::HADRON( 32 ) );
    ofs1 << TASSO_PIpm_44_1989 << endl;

    DataSet TPC_PIpm_29_1988 = 
    PIpm_Data::TPC_PIpm_29_1988( Exp::HADRON( 32 ) );
    ofs1 << TPC_PIpm_29_1988 << endl;


    DataSet SLD_PIpm_inclusive_91_2_1999 = 
    PIpm_Data::SLD_PIpm_inclusive_91_2_1999( Exp::HADRON( 32 ) );
    ofs1 << SLD_PIpm_inclusive_91_2_1999 << endl;

    DataSet SLD_PIpm_c_91_2_1999 = 
    PIpm_Data::SLD_PIpm_c_91_2_1999( Exp::HADRON( 32 ) );
    ofs1 << SLD_PIpm_c_91_2_1999 << endl;

    DataSet SLD_PIpm_b_91_2_1999 = 
    PIpm_Data::SLD_PIpm_b_91_2_1999( Exp::HADRON( 32 ) );
    ofs1 << SLD_PIpm_b_91_2_1999 << endl;

    DataSet  OPAL_PIpm_91_2_1994 = 
    PIpm_Data:: OPAL_PIpm_91_2_1994( Exp::HADRON( 32 ) );
    ofs1 <<  OPAL_PIpm_91_2_1994 << endl;

    DataSet ALEPH_PIpm_91_2_1995 = 
    PIpm_Data::ALEPH_PIpm_91_2_1995( Exp::HADRON( 32 ) );
    ofs1 << ALEPH_PIpm_91_2_1995 << endl;

    DataSet DELPHI_PIpm_b_91_2_1998 = 
    PIpm_Data::DELPHI_PIpm_b_91_2_1998( Exp::HADRON( 32 ) );

    ofs1 << DELPHI_PIpm_b_91_2_1998 << endl;


    DataSet DELPHI_PIpm_inclusive_91_2_1998 = 
    PIpm_Data::DELPHI_PIpm_inclusive_91_2_1998( Exp::HADRON( 32 ) );
    ofs1 << DELPHI_PIpm_inclusive_91_2_1998 << endl;


    ofs1 << "</expdata>" << endl;

  }
  
  catch( int err ){
  }

  return 0;
}
