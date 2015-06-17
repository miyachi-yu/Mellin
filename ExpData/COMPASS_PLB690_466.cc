#include "COMPASS_PLB690_466.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;


namespace Exp{
  
  namespace COMPASS_PLB690_466 {
    
    static bool seed_init = false;
    
    DataSet A1( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "COMPASS_A1p_2010.dat";
      
      if( t != Exp::PRO ){
	cerr << "This data file (" << path 
	     << ") only contains proton data"
	     << endl;
	throw( 2 );
      }
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "COMPASS,2010,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys. Lett. B690 (2010) 466";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( str.size() == 0 )           continue;
	if( str.substr( 0, 1 ) == "*" ) continue;
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1;
	  
	  //*******select variable*************************************************
	  
	  double xmin, xmax, x, q2, sys, g1, g1sta, g1sys;
	  if( !( iss >> xmin >> xmax >> x >> q2 
		 >> A1.data() >> A1.stat() >> sys
		 >> g1 >> g1sta >> g1sys ) ) {
	    
	    cout << "done" << endl;
	    break;
	  }
	  
	  cout << xmin << "\t"
	       << xmax << "\t"
	       << x << "\t"
	       << q2 << "\t"
	       << A1.data() << "\t"
	       << A1.stat() << "\t"
	       << sys 
	       << endl;
	  
	  A1.info() = "A1_" + Exp::tarname( t );
	  A1.unit() = "";
	  
	  A1.kine( xmin, "xmin", "" );
	  A1.kine( xmax, "xmax", "" );
	  A1.kine( x,    "x",    "" );
	  A1.kine( q2,   "Q^2", "GeV^2" );
	  A1.syst( sys, "syst" );
	  
	  ds.push_back( A1 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }    
  }
}
