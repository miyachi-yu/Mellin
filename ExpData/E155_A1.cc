#include "E155_A1.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{
  
  namespace E155_A1 {
    
    DataSet A1( const Exp::TARGET& t ) throw( int ) {

      string path =  data_path + "E155_A1_1999.dat";

      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "E155,2000,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Lett.B493(2000)";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1;
	  
	  //*******select variable*************************************************
	  
	  double x ,q2,sys;
	  if( !( iss >> x >> q2 >> A1.data() >> A1.stat() >> sys ) ) break;
	  
	  A1.info() = "A1_" + Exp::tarname( t );
	  A1.unit() = "";
	  
	  A1.kine( x, "x", "" );
	  A1.kine( q2, "Q^2", "GeV^2" );
	  A1.syst( sys, "syst" );
	  
	  ds.push_back( A1 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
  
}
