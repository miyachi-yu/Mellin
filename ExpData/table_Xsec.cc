#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "FragXsec.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;
using namespace FragXsec;

namespace HadronProduction {
  extern DataSet read_data( string& path, string& type, string& info ) 
    throw( int );
};

int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  try {
    
    DataSet FragXsec_CHpm = 
      FragXsec::Xsec( "../Data/ALEPH_CHARGE.dat", Exp::HADRON( 22 ) );
    FragXsec_CHpm.info() = "ALEPH_CH";
    FragXsec_CHpm.ref() = "Phys.Lett.357B,(1995)487";

    DataSet FragXsec_PRpm = 
      FragXsec::Xsec( "../Data/ALEPH_PPbar.dat", Exp::HADRON( 52 ) );
    FragXsec_PRpm.info() = "ALEPH_PPbar";
    FragXsec_PRpm.ref() = "Zeit.Phys.C66,(1995)355";

    string type = "e+e_PI";
    string info = "ALEPH_PI";
    string file = "../Data/ALEPH_PI.dat";
    DataSet FragXsec_PIpm = 
      HadronProduction::read_data( file, type, info );
    FragXsec_PIpm.ref() = "Zeit.Phys.C66,(1995)355";

    type = "e+e_KA";
    info = "ALEPH_KA";
    file = "../Data/ALEPH_KA.dat";
    DataSet FragXsec_KApm = 
      HadronProduction::read_data( file, type, info );
    FragXsec_KApm.ref() = "Zeit.Phys.C66,(1995)355";

    ofstream ofs( "Xsec.xml" );
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    if( args->hasOpt( "CHpm" ) ) 
      ofs << FragXsec_CHpm << endl;
    if( args->hasOpt( "PRpm" ) ) 
      ofs << FragXsec_PRpm << endl;
    if( args->hasOpt( "PIpm" ) ) 
      ofs << FragXsec_PIpm << endl;
    if( args->hasOpt( "KApm" ) ) 
      ofs << FragXsec_KApm << endl;
    ofs << "</expdata>" << endl;

  }
  
  catch( int err ){
  }

  return 0;
}
