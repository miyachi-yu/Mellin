#include "DataSet.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace Exp;

namespace HadronProduction {
  
  DataSet read_data( string& path, string& type, string& info ) throw( int ) {
    
    DataSet ds;
    ifstream ifs( path.c_str() );
    if( ! ifs ) {
      cerr << "Fail to open data file" << endl;
      throw( 1 );
    }
    
    ds.info() = info;
    ds.ref()  = "";
    
    int length = 1024;
    char buff[ length ];
    
    while( ifs.getline( buff, length ) ){
      
      istringstream istr( buff );
      DataPoint dp;
      double xmin, xmax, x, q2, sys;
      
      if( istr >> xmin >> xmax >> q2 >> dp.data() >> dp.stat() ){
	dp.info() = type;
	dp.unit() = "";
	dp.kine( xmin, "zmin", "" );
	dp.kine( xmax, "zmax", "" );
	dp.kine( 0.5 * ( xmax + xmin ), "zmid", "" );
	dp.kine( xmax - xmin, "dz", "" );
	dp.kine( q2 * q2,  "Q^2", "GeV^2" );
	ds.push_back( dp );
      }
    }
    
    return ds;
  }
  
}
