#include "SMC_PhysRevD58_A1.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{
  
  namespace SMC_PhysRevD58_A1 {

    DataSet A1( const Exp::TARGET& t ) throw( int ) {

      string path = data_path + "SMC_PhysRevD58_A1_1998.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "SMC,1998,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Rev.D.58(1998)112001";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1;
	  
	  //*******select variable*************************************************
	  
	  double x, q2;
	  if( !( iss >> x >> q2 >> A1.data() >> A1.stat() ) ) break;
	  
	  A1.info() = "A1_" + Exp::tarname( t );
	  A1.unit() = "";
	  
	  A1.kine( x, "x", "" );
	  A1.kine( q2, "Q^2", "GeV^2" );
	  
	  ds.push_back( A1 );
	}
	
	if( count != -1 ) count++;
      }
      
     return ds;  
    }

  }
  
}

