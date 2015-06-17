#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "XYZ.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace XYZ;

int main( int argc, char* argv[] ){


  Arguments *args = Arguments::instance( argc, argv );
  
  try {
    
    DataSet hermesA1h_p_00 = 
      XYZ::A1h( XYZ::TARGET( 1 ), 
			 XYZ::HADRON( 00 ) );

/*    DataSet hermesA1h_n_00 =
	 XYZ::A1h( XYZ::TARGET( 2 ), 
	 XYZ::HADRON( 00 ) );*/

/*    DataSet hermesA1h_d_00 = 
      XYZ::A1h( XYZ::TARGET( 3 ), 
      XYZ::HADRON( 00 ) );*/

/*    DataSet hermesA1h_d_pim =
      XYZ::A1h( XYZ::TARGET( 2 ), 
      XYZ::HADRON( 00 ) );*/
    
    ofstream ofs( "XYZ.xml" );
    
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    
    ofs << hermesA1h_p_00 << endl;
//    ofs << hermesA1h_n_00 << endl;
//    ofs << hermesA1h_d_00 << endl;
//    ofs << hermesA1h_d_pim << endl;

    ofs << "</expdata>" << endl;
    
  }
  
  catch( int err ){
  }

  return 0;
}
