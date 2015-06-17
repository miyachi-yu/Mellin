#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "hpm_Data.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace hpm_Data;

int main( int argc, char* argv[] ){
     
Arguments *args = Arguments::instance( argc, argv );
 
  try {
	  
    DataSet TASSO_hpm_35_1990 = 
    hpm_Data::TASSO_hpm_35_1990( Exp::HADRON( 22 ) );
    ofstream ofs1( "hpm_Data.xml" );
    ofs1 << ExpData::xmlHeader() << endl;
    ofs1 << "<expdata version=\"1.0\" >" << endl;
    ofs1 << TASSO_hpm_35_1990 << endl;


    DataSet TASSO_hpm_44_1990 = 
    hpm_Data::TASSO_hpm_44_1990( Exp::HADRON( 22 ) );
    ofs1 << TASSO_hpm_44_1990 << endl;


    DataSet TPC_hpm_29_1988 = 
    hpm_Data::TPC_hpm_29_1988( Exp::HADRON( 22 ) );
    ofs1 << TPC_hpm_29_1988 << endl;

    DataSet SLD_hpm_91_2_1999 = 
    hpm_Data::SLD_hpm_91_2_1999( Exp::HADRON( 22 ) );
    ofs1 << SLD_hpm_91_2_1999 << endl;

    /*
    DataSet  OPAL_hpm_transverse_91_2_1995 = 
    hpm_Data:: OPAL_hpm_transverse_91_2_1995( Exp::HADRON( 22 ) );
    ofs1 <<  OPAL_hpm_transverse_91_2_1995 << endl;

    DataSet  OPAL_hpm_longitudinal_91_2_1995 = 
    hpm_Data:: OPAL_hpm_longitudinal_91_2_1995( Exp::HADRON( 22 ) );
    ofs1 <<  OPAL_hpm_longitudinal_91_2_1995 << endl;
    */

    DataSet  OPAL_hpm_inclusive_91_2_1999 = 
    hpm_Data:: OPAL_hpm_inclusive_91_2_1999( Exp::HADRON( 22 ) );
    ofs1 <<  OPAL_hpm_inclusive_91_2_1999 << endl;

    DataSet  OPAL_hpm_c_91_2_1999 = 
    hpm_Data:: OPAL_hpm_c_91_2_1999( Exp::HADRON( 22 ) );
    ofs1 <<  OPAL_hpm_c_91_2_1999 << endl;

    DataSet  OPAL_hpm_b_91_2_1999 = 
    hpm_Data:: OPAL_hpm_b_91_2_1999( Exp::HADRON( 22 ) );
    ofs1 <<  OPAL_hpm_b_91_2_1999 << endl;

    DataSet ALEPH_hpm_91_2_1998 = 
    hpm_Data::ALEPH_hpm_91_2_1998( Exp::HADRON( 22 ) );
    ofs1 << ALEPH_hpm_91_2_1998 << endl;

    DataSet DELPHI_hpm_91_2_1996 = 
    hpm_Data::DELPHI_hpm_91_2_1996( Exp::HADRON( 22 ) );
    ofs1 << DELPHI_hpm_91_2_1996 << endl;

    ofs1 << "</expdata>" << endl;
  }
  
  catch( int err ){
  }

  return 0;
}
