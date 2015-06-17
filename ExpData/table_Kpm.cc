#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "Kpm_Data.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace Kpm_Data;

int main( int argc, char* argv[] ){
     
Arguments *args = Arguments::instance( argc, argv );
 
  try {
	  
    DataSet TASSO_Kpm_34_1989 = 
    Kpm_Data::TASSO_Kpm_34_1989( Exp::HADRON( 42 ) );
    ofstream ofs1( "Kpm_Data.xml" );
    ofs1 << ExpData::xmlHeader() << endl;
    ofs1 << "<expdata version=\"1.0\" >" << endl;
    ofs1 << TASSO_Kpm_34_1989 << endl;


    DataSet TASSO_Kpm_44_1989 = 
    Kpm_Data::TASSO_Kpm_44_1989( Exp::HADRON( 42 ) );
    ofs1 << TASSO_Kpm_44_1989 << endl;


    DataSet TPC_Kpm_29_1988 = 
    Kpm_Data::TPC_Kpm_29_1988( Exp::HADRON( 42 ) );
    ofs1 << TPC_Kpm_29_1988 << endl;

    DataSet SLD_Kpm_inclusive_91_2_1999 = 
    Kpm_Data::SLD_Kpm_inclusive_91_2_1999( Exp::HADRON( 42 ) );
    ofs1 << SLD_Kpm_inclusive_91_2_1999 << endl;


    DataSet SLD_Kpm_c_91_2_1999 = 
    Kpm_Data::SLD_Kpm_c_91_2_1999( Exp::HADRON( 42 ) );
    ofs1 << SLD_Kpm_c_91_2_1999 << endl;


    DataSet SLD_Kpm_b_91_2_1999 = 
    Kpm_Data::SLD_Kpm_b_91_2_1999( Exp::HADRON( 42 ) );
    ofs1 << SLD_Kpm_b_91_2_1999 << endl;


    DataSet  OPAL_Kpm_91_2_1994 = 
    Kpm_Data:: OPAL_Kpm_91_2_1994( Exp::HADRON( 42 ) );
    ofs1 <<  OPAL_Kpm_91_2_1994 << endl;

    DataSet ALEPH_Kpm_91_2_1995 = 
    Kpm_Data::ALEPH_Kpm_91_2_1995( Exp::HADRON( 42 ) );
    ofs1 << ALEPH_Kpm_91_2_1995 << endl;


    DataSet DELPHI_Kpm_91_2_1995 = 
    Kpm_Data::DELPHI_Kpm_91_2_1995( Exp::HADRON( 42 ) );
    ofs1 << DELPHI_Kpm_91_2_1995 << endl;
    
    DataSet DELPHI_Kpm_b_91_2_1998 = 
    Kpm_Data::DELPHI_Kpm_b_91_2_1998( Exp::HADRON( 42 ) );
    ofs1 << DELPHI_Kpm_b_91_2_1998 << endl;


    DataSet DELPHI_Kpm_inclusive_91_2_1998 = 
    Kpm_Data::DELPHI_Kpm_inclusive_91_2_1998( Exp::HADRON( 42 ) );
    ofs1 << DELPHI_Kpm_inclusive_91_2_1998 << endl;


    ofs1 << "</expdata>" << endl;

  }
  
  catch( int err ){
  }

  return 0;
}
