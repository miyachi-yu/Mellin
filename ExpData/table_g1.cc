#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;

namespace E155 {
  extern DataSet g1p() throw( int );
  extern DataSet g1n() throw( int );
  extern DataSet g1d() throw( int );
}

namespace E154 {
  extern DataSet g1n() throw( int );
}

namespace E142 {
  extern DataSet g1n() throw( int );
}

namespace HERMES {
  extern DataSet g1p() throw( int );
  extern DataSet g1d() throw( int );
  extern DataSet g1n() throw( int );
}

namespace SMC {
  extern DataSet g1p() throw( int );
  extern DataSet g1d() throw( int );
}

namespace EMC {
  extern DataSet g1p() throw( int );
}

namespace E143 {
  extern DataSet g1p() throw( int );
  extern DataSet g1d() throw( int );
}

namespace COMPASS {
  extern DataSet g1d( const string& path,
		      const string& info,
		      const string& ref ) throw( int );
}

int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  try {
    
    DataSet emcg1p     = EMC::g1p();
    DataSet e143g1p    = E143::g1p();
    DataSet smcg1p     = SMC::g1p();
    DataSet e155g1p    = E155::g1p();
    DataSet hermesg1p  = HERMES::g1p();

    
    DataSet smcg1d     = SMC::g1d();
    DataSet e143g1d    = E143::g1d();
    DataSet e155g1d    = E155::g1d();
    DataSet hermesg1d  = HERMES::g1d();
    
    //    DataSet e155g1n    = E155::g1n();
    DataSet e142g1n   = E142::g1n();
    DataSet e154g1n   = E154::g1n();
    DataSet hermesg1n = HERMES::g1n();
    
    DataSet compass05 = 
      COMPASS::g1d( data_path + "COMPASS_2005_g1d",
		    "COMPASS,2005,g1d",
		    "Phys.Lett.B612,154(2005)" );
    
    DataSet compass07 = 
      COMPASS::g1d( data_path + "COMPASS_2007_g1d",
		    "COMPASS,2007,g1d",
		    "Phys.Lett.B647,8(2007)" );
    
    ofstream ofs("g1.xml");
    
    ofs << ExpData::xmlHeader() << endl;
    ofs << "<expdata version=\"1.0\" >" << endl;
    
    ofs << emcg1p     << endl;
    ofs << e143g1p    << endl;
    ofs << smcg1p     << endl;
    ofs << e155g1p    << endl;
    ofs << hermesg1p  << endl; 

    ofs << smcg1d     << endl;
    ofs << e143g1d    << endl;
    ofs << e155g1d    << endl;
    ofs << hermesg1d  << endl;
    ofs << compass05  << endl;
    ofs << compass07  << endl;

    ofs << e142g1n    << endl;
    ofs << e154g1n    << endl;
    ofs << hermesg1n    << endl;
    ofs << "</expdata>" << endl;
    
  }
  
  catch( int err ){
  }

  return 0;
}
