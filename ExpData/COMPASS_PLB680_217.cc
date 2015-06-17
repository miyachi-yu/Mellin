#include "COMPASS_PLB680_217.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;


namespace Exp{
  
  namespace COMPASS_PLB680_217 {
    
    static bool seed_init = false;
    
    DataSet A1( const Exp::TARGET& t ) throw( int ) {

      string path = data_path + "COMPASS_A1h_2009.dat";
      
      if( t != Exp::DEU ){
	cerr << "This data file (" << path 
	     << ")only contains deuteron data"
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
      ds.info() = "COMPASS,2009,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys. Lett. B680 (2009) 217";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( str.size() == 0 ) continue;
	if( str.substr( 0, 1 ) == "*" ) continue;
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1;
	  
	  //*******select variable*************************************************
	  
	  double xmin, xmax, x, q2, sys;
	  if( !( iss >> xmin >> xmax >> x >> q2 >> A1.data() >> A1.stat() >> sys ) ) break;
	  
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
    
    DataSet A1h( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "COMPASS_A1h_2009.dat";
      
      if( t != Exp::DEU ){
	cerr << "This data file (" << path 
	     << ") only contains deuteron data. ("
	     << t << ")"
	     << endl;
	throw( 2 );
      }
      
      if( h != Exp::PIp && h != Exp::PIm && 
	  h != Exp::Kp && h != Exp::Km  ){
	cerr << "This data file (" << path 
	     << ") only contains PIp, PIm, KAp, KAm"
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
      ds.info() = "COMPASS,2009,A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
      ds.ref()  = "Phys. Lett. B680 (2009) 217";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( str.size() == 0 )           continue; // null line
	if( str.substr( 0, 1 ) == "*" ) continue; // comment line
	if( str.size() > 3 &&
	    atoi( str.substr( 0, 1 ).c_str() ) == tar &&
	    atoi( str.substr( 2, 2 ).c_str() ) == h  ) count = 0;
	
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1h;
	  
	  //*******select variable*************************************************
	  double xmin,xmax,xmean, q2,a1, sta, sys;
	  if( !( iss >> xmin >> xmax >> xmean  >> q2 >> a1 >> sta >> sys) ) {
	    cout << "done" << endl;
	    break;
	  }
	  
	  cout << xmin << "\t"
	       << xmax << "\t"
	       << xmean << "\t"
	       << q2 << "\t"
	       << a1 << "\t"
	       << sta << "\t"
	       << sys 
	       << endl;
	  
	  A1h.info() = "A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
	  
	  A1h.unit() = "";
	  
	  A1h.data() = a1;
	  A1h.stat() = sta;
	  A1h.kine( xmin, "x min", "" );
	  A1h.kine( xmax, "x max", "" );
	  A1h.kine( xmean, "x",    "" );
	  A1h.kine( q2, "Q^2", "GeV^2" );
	  A1h.syst( sys, "syst" );
	  ds.push_back( A1h );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
}
