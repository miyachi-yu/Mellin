#include "A1_Data.hh"

#include <Utility/Arguments.hh>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{
  
  namespace A1_Data {
    
    //************************************************************************
    //***********************:JLAB-E-99-117,A1, 2004**************************
    //************************************************************************
    DataSet A1( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "JLAB-E-99-117_A1_2004.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "JLAB-E-99-117,2004,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Rev.C67(2004)065207";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint A1;

	  //*******select variable*************************************************
	  
	  double x, q2,sysu,sysl;
	  if( !( iss >> x >> q2 >> A1.data() >> A1.stat() >> sysu >> sysl ) ) 
	  break;
	  
	  A1.info() = "A1_" + Exp::tarname( t );
	  A1.unit() = "";
	  
	  A1.kine( x, "x", "" );
	  A1.kine( q2, "Q^2", "GeV^2" );
	  A1.syst( sysu, "syst upper" );
	  A1.syst( sysl, "syst lower" );
	  
	  
	  ds.push_back( A1 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //************************E154,1997,A1************************************
    //************************************************************************
    
    DataSet A1e154( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "E154_A1_1997.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "E154,1997,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Rev.Lett.79(1997)26";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1e154;
	  
	  //*******select variable*************************************************
	  
	  double xmin,xmax,xmean, q2,sys;
	  if( !( iss >> xmin >> xmax >> xmean  >> q2 >> A1e154.data() 
		 >> A1e154.stat() >> sys) ) break;
	  
	  A1e154.info() = "A1_" + Exp::tarname( t );
	  A1e154.unit() = "";
	  
	  A1e154.kine( xmin, "x min", "" );
	  A1e154.kine( xmax, "x max", "" );
	  A1e154.kine( xmean, "x", "xmean" );
	  A1e154.kine( q2, "Q^2", "GeV^2" );
	  A1e154.syst( sys, "syst" );
	  
	  ds.push_back( A1e154 );
	}
      
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    //************************************************************************
    //************************COMPASS,2006,A1*********************************
    //************************************************************************
    DataSet A1compass( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "COMPASS_A1_2006.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "COMPASS,2006,A1_" + Exp::tarname( t );
      ds.ref()  = "PL B647,8 (2007)";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint A1compass;

	  //*******select variable*************************************************
	  
	  double x, q2,sys;
	  if( !( iss >> x >> q2 >> A1compass.data() 
		 >> A1compass.stat() >> sys ) ) break;
	  
	  A1compass.info() = "A1_" + Exp::tarname( t );
	  A1compass.unit() = "";
	  
	  A1compass.kine( x, "x", "" );
	  A1compass.kine( q2, "Q^2", "GeV^2" );
	  A1compass.syst( sys, "syst" );
	  
	  ds.push_back( A1compass );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    //************************************************************************
    //************************E142,1996,A1************************************
    //************************************************************************
    
    DataSet A1e142( const Exp::TARGET& t ) throw( int ) {

      string path = data_path + "E142_A1_1996.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "E142,1996,A1_" + Exp::tarname( t );
      ds.ref()  = "Phys.Rev.D54(96)6620";      
      
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) count = 0;
	if( count > 0 ){
	  
	istringstream iss( str );
	DataPoint A1e142;
	
	//*******select variable*************************************************
	
	double xmin,xmax,xmean, q2,sys;
	if( !( iss >> xmin >> xmax >> xmean  >> q2 >> A1e142.data() 
	       >> A1e142.stat() >> sys) ) break;
	
	A1e142.info() = "A1_" + Exp::tarname( t );
	A1e142.unit() = "";
	
	A1e142.kine( xmin, "x min", "" );
	A1e142.kine( xmax, "x max", "" );
	A1e142.kine( xmean, "x", "xmean" );
	A1e142.kine( q2, "Q^2", "GeV^2" );
	A1e142.syst( sys, "syst" );
	
	ds.push_back( A1e142 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    
    //************************************************************************
    //************************CLAS,2006,A1************************************
    //************************************************************************
    DataSet A1CLAS( const Exp::TARGET& t ) throw( int ) {
      
      string path = data_path + "CLAS_A1_2006.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "CLAS,2006,A1_" + Exp::tarname( t );
      ds.ref()  = "NUCL-EX/0605028(PART2)";      
      
      int tar = t;
      string str;
      int count = -1;
      double Wlimit = Utility::Arguments::ref().get( "wlimit", 2.5 );
      double W;
    
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar ) 
	if( atof( str.substr( 2, 5 ).c_str() ) >= Wlimit ){
	  count = 0; W = atof( str.substr( 2, 5 ).c_str() );
	}
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint A1CLAS;
	  
	  //*******select variable*************************************************
	  
	  double xmean, q2, sys;
	  if( !( iss >> xmean  >> q2 >> A1CLAS.data() 
		 >> A1CLAS.stat() >> sys ) ) continue;
	  
	  A1CLAS.info() = "A1_" + Exp::tarname( t );
	  A1CLAS.unit() = "";
	  
	  A1CLAS.kine( xmean, "x", "xmean" );
	  A1CLAS.kine( q2, "Q^2", "GeV^2" );
	  A1CLAS.kine( W, "W", "GeV" );
	  A1CLAS.syst( sys, "syst" );	
	  
	  ds.push_back( A1CLAS );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
  
}
