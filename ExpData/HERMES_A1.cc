#include "HERMES_A1.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;


namespace Exp{
  
  namespace HERMES_A1 {
    
    static bool seed_init = false;
    
    DataSet A1( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "HERMES_A1_2006.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "HERMES,2006,A1_" + Exp::tarname( t );
      ds.ref()  = "hep/ex0609039(2006)";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1;
	  
	  //*******select variable*************************************************
	  
	  double x, q2,sys, par;
	  if( !( iss >> x >> q2 >> A1.data() >> A1.stat() >> sys >> par) ) break;
	  
	  A1.info() = "A1_" + Exp::tarname( t );
	  A1.unit() = "";
	  
	  A1.kine( x, "x", "" );
	  A1.kine( q2, "Q^2", "GeV^2" );
	  A1.syst( sys, "syst" );
	  A1.syst( par, "par" );
	  
	  ds.push_back( A1 );
	}
	
	if( count != -1 ) count++;
      }
      
      
      string cov_file =
	data_path + "HERMES_A1_2006_cov_" + 
	( t == Exp::PRO ? "p" : "d" ) + ".dat";

      ifstream ifs2( cov_file.c_str() );
      if( ! ifs2 ) return ds;
      
      struct tm tinfo;
      if( ! seed_init ) {
	srandom( time( NULL ) );
	seed_init = true;
      }
      
      long int emID = random();
      
      // resize ems container
      for( int iset = 0; iset < ds.size(); iset++ ){
	ds[ iset ].emNumber() = emID;
	ds[ iset ].ems().resize( ds.size(), 0.0 );
      }
      
      int icov, jcov;
      double cov;
      while( ifs2 >> icov >> jcov >> cov ){
	
       	// cout << setw(4) << icov-- << setw(4) << jcov-- << setw(12) << cov << endl;
	icov--;
	jcov--;
	
	ds[ icov ].emid() = icov ;
	ds[ icov ].ems()[ jcov ] = cov * ds[ icov ].stat() * ds[ jcov ].stat();

      }
      
      return ds;  
    }
    
    
    DataSet A1n( const Exp::TARGET& t ) throw( int ) {

      string path = data_path + "HERMES_A1n_1997.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "HERMES,1997,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Lett.B404(1997)383";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1n;
	  
	  //*******select variable*************************************************
	  
	  double xmin,xmax,xmean, q2,sys;
	  if( !( iss >> xmin >> xmax >> xmean  >> q2 >> A1n.data() >> A1n.stat() >> sys) ) break;
	  
	  A1n.info() = "A1_" + Exp::tarname( t );
	  A1n.unit() = "";
	  
	  A1n.kine( xmin, "x min", "" );
	  A1n.kine( xmax, "x max", "" );
	  A1n.kine( xmean, "x", "xmean" );
	  A1n.kine( q2, "Q^2", "GeV^2" );
	  A1n.syst( sys, "syst" );
	  
	  
	  ds.push_back( A1n );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
  
}
