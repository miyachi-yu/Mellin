#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Exp;

namespace EMC {
  
  DataSet g1p() throw( int ) {
    
    string path = data_path + "EMC_1989_g1p.dat";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
      } else {
      cout << "Loading " << path << endl;
    }
    
    DataSet ep;
    ep.info() = "EMC,1989,g1p";
    ep.ref()  = "Nucl.Phys.B328(1989)1";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      if( buff[0] == '-' ) count = 0;
      if( count < 0 || count > 11 ) continue;

      istringstream ist( buff );
      DataPoint dp;
      
      double x, q2, a1p, da1p, sa1p, g1p, dg1p, sg1p;
      if( ! ( ist 
	      >> x >> q2 
	      >> a1p >> da1p >> sa1p 
	      >> dp.data() >> dp.stat() >> sg1p ) ) continue;
      
      dp.info() = "g1p";
      dp.unit() = "";
      dp.kine(    x,    "x", "" );
      dp.kine(   q2,  "Q^2", "GeV^2" );
      dp.syst( sg1p, "syst" );
      
      ep.push_back( dp );
      
      count++ ;
    }
    return ep;
  }
  
}
