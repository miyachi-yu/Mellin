#include "PI0_Data.hh"

#include <Utility/Arguments.hh>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp{
  
  namespace PI0_Data {
    
    //************************************************************************
    //***********************TASSO_PI0_44_1989*******************************
    //************************************************************************
    DataSet TASSO_PI0_44_1989( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_PI0_44_1989.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "TASSO_44_1989_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C42(1989)189";  
      ds.cite() = "Braunschweig:1988hv";
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint TASSO_PI0_44_1989;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax , q =44 ,sys;
	  if( !( iss >> zmin >> zmax >> 
		 TASSO_PI0_44_1989.data() >> 
		 TASSO_PI0_44_1989.stat() >>sys  ) ) 
	  break;
	  
	  
	  TASSO_PI0_44_1989.info() = "e+e-_P0";
	  TASSO_PI0_44_1989.unit() = "";
	  
	  TASSO_PI0_44_1989.kine( zmin, "zmin", "" );
	  TASSO_PI0_44_1989.kine( zmax, "zmax", "" );
	  TASSO_PI0_44_1989.kine( 0.5*(zmin + zmax), "zmid", "" );
	  TASSO_PI0_44_1989.kine( zmax -zmin, "dz", "" );
	  TASSO_PI0_44_1989.kine( q*q, "Q^2", "GeV^2 " );
	  
	  TASSO_PI0_44_1989.syst( sys, "syst" );
	  
	  ds.push_back( TASSO_PI0_44_1989 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    
    //************************************************************************
    //***********************TPC_PI0_29_1985*******************************
    //************************************************************************
    DataSet TPC_PI0_29_1985( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TPC_PI0_29_1985.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "TPC_29_1985_"+ Exp::hadname( h );
      ds.ref()  = "Z.Phys.C27(1985)187";  
      ds.cite() = "Aihara:1984mi";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TPC_PI0_29_1985;
	  
	  //*******select variable*************************************************
	  
	  double xE , q =29;
	  if( !( iss >> xE >>
		 TPC_PI0_29_1985.data() >> 
		 TPC_PI0_29_1985.stat()   ) ) 
	    break;
	  
	  TPC_PI0_29_1985.info() = "e+e-_P0";
	  TPC_PI0_29_1985.unit() = "";
	  
	  TPC_PI0_29_1985.kine( xE, "zmin", "" );
	  TPC_PI0_29_1985.kine( xE, "zmax", "" );
	  TPC_PI0_29_1985.kine( 0.5*(xE + xE), "zmid", "" );
	  TPC_PI0_29_1985.kine( xE -xE, "dz", "" );
	  TPC_PI0_29_1985.kine( q*q, "Q^2", "GeV^2 " );

	  ds.push_back( TPC_PI0_29_1985 );

	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    
    //************************************************************************
    //***********************L3_PI0_91.2_1994*****************************
    //************************************************************************
    DataSet L3_PI0_91_2_1994( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "L3_PI0_91.2_1994.dat";

      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "L3_91.2_1994_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Lett.B328(1994)223";
      ds.cite() = "Acciarri:1994gza";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint L3_PI0_91_2_1994;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 ,sys;
	  if( !( iss >> zmin >> zmax >> 
		 L3_PI0_91_2_1994.data() >> 
		 L3_PI0_91_2_1994.stat()  >> sys) ) 
	    break;
	  
	  L3_PI0_91_2_1994.info() = "e+e-_P0";
	  L3_PI0_91_2_1994.unit() = "";
	  
	  L3_PI0_91_2_1994.kine( zmin, "zmin", "" );
	  L3_PI0_91_2_1994.kine( zmax, "zmax", "" );
	  L3_PI0_91_2_1994.kine( 0.5*(zmin + zmax), "zmid", "" );
	  L3_PI0_91_2_1994.kine( zmax -zmin, "dz", "" );
	  L3_PI0_91_2_1994.kine( q*q, "Q^2", "GeV^2 " );

	  L3_PI0_91_2_1994.syst( sys, "syst" );
	  
	  ds.push_back( L3_PI0_91_2_1994 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    //************************************************************************
    //**************************ALEPH_PI0_91.2_2000***************************
    //************************************************************************
    DataSet ALEPH_PI0_91_2_2000( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "ALEPH_PI0_91.2_2000.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "ALEPH__91.2_2000_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J.C16(2000)613";  
      ds.cite() = "Barate:1999gb,";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint ALEPH_PI0_91_2_2000;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 ,sys;
	  if( !( iss >> zmin >> zmax >>
		 ALEPH_PI0_91_2_2000.data() >> 
		 ALEPH_PI0_91_2_2000.stat() >> sys) )
	    break;
	  
	  ALEPH_PI0_91_2_2000.info() = "e+e-_P0";
	  ALEPH_PI0_91_2_2000.unit() = "";
	  
	  ALEPH_PI0_91_2_2000.kine( zmin, "zmin", "" );
	  ALEPH_PI0_91_2_2000.kine( zmax, "zmax", "" );
	  ALEPH_PI0_91_2_2000.kine( 0.5*(zmin + zmax), "zmid", "" );
	  ALEPH_PI0_91_2_2000.kine( zmax -zmin, "dz", "" );
	  ALEPH_PI0_91_2_2000.kine( q*q, "Q^2", "GeV^2 " );
	  ALEPH_PI0_91_2_2000.syst( sys, "syst" );
	  
	  ds.push_back( ALEPH_PI0_91_2_2000 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }

    //************************************************************************
    //***********************OPAL_PI0_91.2_1998*******************************
    //************************************************************************
    DataSet OPAL_PI0_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_PI0_91.2_1998.dat" ;
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "OPAL_91.2_1998_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J.C5(1998)411";  
      ds.cite() = "Ackerstaff:1998ap";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint OPAL_PI0_91_2_1998;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ))
	       break;
	  
	  OPAL_PI0_91_2_1998.info() = "e+e-_P0";
	  OPAL_PI0_91_2_1998.unit() = "";
	  
	  OPAL_PI0_91_2_1998.kine( zmin, "zmin", "" );
	  OPAL_PI0_91_2_1998.kine( zmax, "zmax", "" );
	  OPAL_PI0_91_2_1998.kine( 0.5*(zmin + zmax), "zmid", "" );
	  OPAL_PI0_91_2_1998.kine( zmax -zmin, "dz", "" );
	  OPAL_PI0_91_2_1998.kine( q*q, "Q^2", "GeV^2 " );
	  OPAL_PI0_91_2_1998.syst( sys, "syst" );

	  OPAL_PI0_91_2_1998.data( data ,"e+e-_P0" ,"");
	  OPAL_PI0_91_2_1998.stat( stat);
	  
	  ds.push_back( OPAL_PI0_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }

    //***********************************************************************
    //***********************DELPHI_PI0_91.2_1996******************************
    //***********************************************************************
    DataSet DELPHI_PI0_91_2_1996( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_PI0_91.2_1995.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
     
      DataSet ds;
      ds.info() = "DELPHI_91.2_1996_"+ Exp::hadname( h );
      ds.ref()  = "Z.Phys.C69(1996)561";  
      ds.cite() = "Adam:1995rf";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint DELPHI_PI0_91_2_1996;

     //*******select variable*************************************************
	  
	  double zmax ,zmin, zmean ,q =91.2 , data ,stat,sys;
	  if( !( iss >> zmax>>zmin>>zmean>> data >> stat >>sys ))
	       break;
	  
	  DELPHI_PI0_91_2_1996.info() = "e+e-_P0";
	  DELPHI_PI0_91_2_1996.unit() = "";
	  
	  DELPHI_PI0_91_2_1996.kine( zmax, "zmin", "" );
	  DELPHI_PI0_91_2_1996.kine( zmin, "zmax", "" );
	  DELPHI_PI0_91_2_1996.kine( 0.5*(zmin + zmax), "zmid", "" );
	  DELPHI_PI0_91_2_1996.kine( zmax - zmin, "dz", "" );
	  DELPHI_PI0_91_2_1996.kine( q*q, "Q^2", "GeV^2 " );
	  DELPHI_PI0_91_2_1996.syst( sys, "syst" );

	  DELPHI_PI0_91_2_1996.data( data ,"e+e-_P0" ,"");
	  DELPHI_PI0_91_2_1996.stat( stat);
	  
	  ds.push_back( DELPHI_PI0_91_2_1996 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }

    //************************************************************************
    //***********************DELPHI_PI0_inclusive_91.2_1998******************
    //************************************************************************
    DataSet DELPHI_PI0_inclusive_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_PI0_inclusive_91.2_1998.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "DELPHI_inclusive_91.2_1998_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J>C5(1998)585";  
      ds.cite() = "Abreu:1998vq";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint DELPHI_PI0_inclusive_91_2_1998;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ))
	    break;
	  
	  DELPHI_PI0_inclusive_91_2_1998.info() = "e+e-_P0";
	  DELPHI_PI0_inclusive_91_2_1998.unit() = "";
	  
	  DELPHI_PI0_inclusive_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_PI0_inclusive_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_PI0_inclusive_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz", "" );
	  DELPHI_PI0_inclusive_91_2_1998.kine( 0.5*(2*zmax/q + 2*zmin/q), "zmid", "" );
	  DELPHI_PI0_inclusive_91_2_1998.kine( q*q, "Q^2", "GeV^2 " );
	  DELPHI_PI0_inclusive_91_2_1998.syst( sys*q/2, "syst" );
	  
	  DELPHI_PI0_inclusive_91_2_1998.data( data*q/2 ,"e+e-_P0" ,"");
	  DELPHI_PI0_inclusive_91_2_1998.stat( stat*q/2);
	  
	  ds.push_back( DELPHI_PI0_inclusive_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }

    //************************************************************************
    //***********************DELPHI_PI0_b_91.2_1998**************************
    //************************************************************************
    DataSet DELPHI_PI0_b_91_2_1998( const Exp::HADRON& h ) throw( int ) {

      string path = data_path + "DELPHI_PI0_b_91.2_1998.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "DELPHI_b_91.2_1998_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J_C5(1998)585";  
      ds.cite() = "Abreu:1998vq";
      
      ds.norm()   = 0.05;
      ds.ncoef()  = 0;
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint DELPHI_PI0_b_91_2_1998;
	  
      //*******select variable*************************************************
	  	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ))
	       break;
	  
	  DELPHI_PI0_b_91_2_1998.info() = "e+e-_P0_b_enriched";
	  DELPHI_PI0_b_91_2_1998.unit() = "";
	  
	  DELPHI_PI0_b_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_PI0_b_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_PI0_b_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz", "" );
	  DELPHI_PI0_b_91_2_1998.kine( 0.5*(2*zmax/q + 2*zmin/q), "zmid", "" );
	  DELPHI_PI0_b_91_2_1998.kine( q*q, "Q^2", "GeV^2 " );
	  DELPHI_PI0_b_91_2_1998.syst( sys*q/2, "syst" );

	  DELPHI_PI0_b_91_2_1998.data( data*q/2 ,"e+e-_P0_b_enriched" ,"");
	  DELPHI_PI0_b_91_2_1998.stat( stat*q/2);
	  
	  ds.push_back( DELPHI_PI0_b_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
  }
}
