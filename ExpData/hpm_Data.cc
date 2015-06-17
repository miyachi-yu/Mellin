#include "hpm_Data.hh"

#include <Utility/Arguments.hh>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp{
  
  namespace hpm_Data {
    
    //************************************************************************
    //***********************TASSO_hpm_44_1990*******************************
    //************************************************************************
    DataSet TASSO_hpm_44_1990( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_hpm_44_1990.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
     
      DataSet ds;
      ds.info() = "TASSO_44_1990_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C47(1990)187";  
      ds.cite() = "Braunschweig:1990yd";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_hpm_44_1990;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax , q =44;
	  if( !( iss >> xpmin >> xpmax >> 
		 TASSO_hpm_44_1990.data() >> 
		 TASSO_hpm_44_1990.stat()  ) ) 
	  break;
	  
	 	  
	  TASSO_hpm_44_1990.info() = "e+e-_HA";
	  TASSO_hpm_44_1990.unit() = "";
	  
	  TASSO_hpm_44_1990.kine( xpmin, "zmin", "" );
	  TASSO_hpm_44_1990.kine( xpmax, "zmax", "" );
	  TASSO_hpm_44_1990.kine( xpmax - xpmin, "dz", "" );
	  TASSO_hpm_44_1990.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TASSO_hpm_44_1990.kine( q*q, "Q^2", "GeV^2" );
	  
	  ds.push_back( TASSO_hpm_44_1990 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TASSO_hpm_35_1990*******************************
    //************************************************************************
    DataSet TASSO_hpm_35_1990( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_hpm_35_1990.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "TASSO_35_1990_"+ Exp::hadname( h );
      ds.ref()  = "Z.PhysC47(1990)187";
      ds.cite() = "Braunschweig:1990yd";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_hpm_35_1990;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,q =35;
	  if( !( iss >> xpmin >> xpmax >> 
		 TASSO_hpm_35_1990.data() >> 
		 TASSO_hpm_35_1990.stat() ) )
	  break;
	  
	  TASSO_hpm_35_1990.info() = "e+e-_HA";
	  TASSO_hpm_35_1990.unit() = "";
	  
	  TASSO_hpm_35_1990.kine( xpmin, "zmin", "" );
	  TASSO_hpm_35_1990.kine( xpmax, "zmax", "" );
	  TASSO_hpm_35_1990.kine( xpmax - xpmin, "dz", "" );
	  TASSO_hpm_35_1990.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TASSO_hpm_35_1990.kine( q*q, "Q^2", "GeV^2" );
	  
	  
	  ds.push_back( TASSO_hpm_35_1990 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TPC_hpm_29_1988*******************************
    //************************************************************************
    DataSet TPC_hpm_29_1988( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TPC_hpm_29_1988.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "TPC_29_1988_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rev.Lett.61(1988)1263";  
      ds.cite() = "Aihara:1988su";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TPC_hpm_29_1988;

      //*******select variable*************************************************
	  
	  double xpmax, xpmin , q =29;
	  if( !( iss >> xpmin >> xpmax >>
		 TPC_hpm_29_1988.data() >> 
		 TPC_hpm_29_1988.stat()   ) ) 
	  break;
	  
 
	  
	  TPC_hpm_29_1988.info() = "e+e-_HA";
	  TPC_hpm_29_1988.unit() = "";
	  
	  TPC_hpm_29_1988.kine( xpmin, "zmin", "" );
	  TPC_hpm_29_1988.kine( xpmax, "zmax", "" );
	  TPC_hpm_29_1988.kine( xpmax - xpmin, "dz", "" );
	  TPC_hpm_29_1988.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TPC_hpm_29_1988.kine( q*q, "Q^2", "GeV^2" );
	  
	  ds.push_back( TPC_hpm_29_1988 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_hpm__91.2_1999*********************
    //************************************************************************
    DataSet SLD_hpm_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_hpm_inclusive_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "SLD__91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rev.D59(1999)052001";  
      ds.cite() = "Abe:1998zs";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_hpm_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 SLD_hpm_91_2_1999.data() >> 
		 SLD_hpm_91_2_1999.stat() >> sys  ) ) 
	  break;
	  

	  
	  SLD_hpm_91_2_1999.info() = "e+e-_HA";
	  SLD_hpm_91_2_1999.unit() = "";
	  
	  SLD_hpm_91_2_1999.kine( xpmin, "zmin", "" );
	  SLD_hpm_91_2_1999.kine( xpmax, "zmax", "" );
	  SLD_hpm_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  SLD_hpm_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  SLD_hpm_91_2_1999.kine( q*q, "Q^2", "GeV^2" );
	  SLD_hpm_91_2_1999.syst( sys, "syst" );
	  
	  
	  ds.push_back( SLD_hpm_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }


    //************************************************************************
    //**************************ALEPH_hpm_91.2_1998***************************
    //************************************************************************
    DataSet ALEPH_hpm_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "ALEPH_hpm_91.2_1998.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
        
      DataSet ds;
      ds.info() = "ALEPH_91.2_1998_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rept.294(1998)1";
      ds.cite() = "Barate:1996fi";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint ALEPH_hpm_91_2_1998;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 ALEPH_hpm_91_2_1998.data() >> 
		 ALEPH_hpm_91_2_1998.stat() >> sys) )
	  break;
	  
 
	  
	  ALEPH_hpm_91_2_1998.info() = "e+e-_HA";
	  ALEPH_hpm_91_2_1998.unit() = "";
	  

	  ALEPH_hpm_91_2_1998.kine( xpmin, "zmin", "" );
	  ALEPH_hpm_91_2_1998.kine( xpmax, "zmax", "" );
	  ALEPH_hpm_91_2_1998.kine( xpmax - xpmin, "dz", "" );
	  ALEPH_hpm_91_2_1998.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  ALEPH_hpm_91_2_1998.kine( q*q, "Q^2", "GeV^2" );
	  ALEPH_hpm_91_2_1998.syst( sys, "syst" );
	  
	  
	  ds.push_back( ALEPH_hpm_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
    //************************************************************************
    //***********************OPAL_hpm_transverse_91.2_1995*******************************
    //************************************************************************
    DataSet OPAL_hpm_transverse_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_hpm_transverse_91.2_1995.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1995_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C68(1995)203";  
      ds.cite() = "Akers:1995wt";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_hpm_transverse_91_2_1995;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  OPAL_hpm_transverse_91_2_1995.info() = "e+e-_HA";
	  OPAL_hpm_transverse_91_2_1995.unit() = "";
	  
	  OPAL_hpm_transverse_91_2_1995.kine( xpmin, "zmin", "" );
	  OPAL_hpm_transverse_91_2_1995.kine( xpmax, "zmax", "" );
	  OPAL_hpm_transverse_91_2_1995.kine( xpmax - xpmin, "dz", "" );
	  OPAL_hpm_transverse_91_2_1995.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  OPAL_hpm_transverse_91_2_1995.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_hpm_transverse_91_2_1995.syst( sys, "syst" );

	  OPAL_hpm_transverse_91_2_1995.data( data, "e+e-_HA", "");
	  OPAL_hpm_transverse_91_2_1995.stat( stat);
	  
	  
	  ds.push_back( OPAL_hpm_transverse_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************OPAL_hpm_91.2_1995*******************************
    //************************************************************************
    DataSet OPAL_hpm_longitudinal_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_hpm_longitudinal_91.2_1995.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1995_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C68(1995)203";  
      ds.cite() = "Akers:1995wt";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_hpm_longitudinal_91_2_1995;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  OPAL_hpm_longitudinal_91_2_1995.info() = "e+e-_HA";
	  OPAL_hpm_longitudinal_91_2_1995.unit() = "";
	  
	  OPAL_hpm_longitudinal_91_2_1995.kine( xpmin, "zmin", "" );
	  OPAL_hpm_longitudinal_91_2_1995.kine( xpmax, "zmax", "" );
	  OPAL_hpm_longitudinal_91_2_1995.kine( xpmax - xpmin, "dz", "" );
	  OPAL_hpm_longitudinal_91_2_1995.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  OPAL_hpm_longitudinal_91_2_1995.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_hpm_longitudinal_91_2_1995.syst( sys, "syst" );

	  OPAL_hpm_longitudinal_91_2_1995.data( data , "e+e-_HA", "");
	  OPAL_hpm_longitudinal_91_2_1995.stat( stat);
	  
	  
	  ds.push_back( OPAL_hpm_longitudinal_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
	  }

   //************************************************************************
   //***********************OPAL_hpm_91.2_1999*******************************
   //************************************************************************
    DataSet OPAL_hpm_b_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path =  data_path + "OPAL_hpm_b_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J(1999)369";  
      ds.cite() = "Ackerstaff:1998hz";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_hpm_b_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
	  OPAL_hpm_b_91_2_1999.info() = "e+e-_HA_b_enriched";
	  OPAL_hpm_b_91_2_1999.unit() = "";
	  
	  OPAL_hpm_b_91_2_1999.kine( xpmin, "zmin", "" );
	  OPAL_hpm_b_91_2_1999.kine( xpmax, "zmax", "" );
	  OPAL_hpm_b_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  OPAL_hpm_b_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  OPAL_hpm_b_91_2_1999.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_hpm_b_91_2_1999.syst( sys, "syst" );
	  
	  OPAL_hpm_b_91_2_1999.data( data, "e+e-_HA_b_enriched", "" );
	  OPAL_hpm_b_91_2_1999.stat( stat );
	  
	  
	  ds.push_back( OPAL_hpm_b_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
	  }
    
    //************************************************************************
    //***********************OPAL_hpm_91.2_1999*******************************
    //************************************************************************
    DataSet OPAL_hpm_inclusive_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_hpm_inclusive_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J(1999)369";  
      ds.cite() = "Ackerstaff:1998hz";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_hpm_inclusive_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  OPAL_hpm_inclusive_91_2_1999.info() = "e+e-_HA";
	  OPAL_hpm_inclusive_91_2_1999.unit() = "";
	  
	  OPAL_hpm_inclusive_91_2_1999.kine( xpmin, "zmin", "" );
	  OPAL_hpm_inclusive_91_2_1999.kine( xpmax, "zmax", "" );
	  OPAL_hpm_inclusive_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  OPAL_hpm_inclusive_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  OPAL_hpm_inclusive_91_2_1999.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_hpm_inclusive_91_2_1999.syst( sys, "syst" );

	  OPAL_hpm_inclusive_91_2_1999.data( data, "e+e-_HA", "");
	  OPAL_hpm_inclusive_91_2_1999.stat( stat);
	  
	  
	  ds.push_back( OPAL_hpm_inclusive_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
	  }

    //***********************************************************************
    //***********************DELPHI_hpm_91.2_1995******************************
    //***********************************************************************
    DataSet DELPHI_hpm_91_2_1996( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_hpm_91.2_1996.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      
      DataSet ds;
      ds.info() = "DELPHI_91.2_1996_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J>C5(1998)585";  
      ds.cite() = "Abreu:1998vq";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint DELPHI_hpm_91_2_1996;

     //*******select variable*************************************************
	  
	  double xpmax ,xpmin,  q =91.2 , data ,stat,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >> sys ) )
	       break;
	  
 
	  
	  DELPHI_hpm_91_2_1996.info() = "e+e-_HA";
	  DELPHI_hpm_91_2_1996.unit() = "";
	  
	  DELPHI_hpm_91_2_1996.kine( xpmin, "zmin", "" );
	  DELPHI_hpm_91_2_1996.kine( xpmax, "zmax", "" );
	  DELPHI_hpm_91_2_1996.kine( xpmax - xpmin, "dz", "" );
	  DELPHI_hpm_91_2_1996.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  DELPHI_hpm_91_2_1996.kine( q*q, "Q^2", "GeV^2" );


	  DELPHI_hpm_91_2_1996.data( data, "e+e-_HA", "" );
	  DELPHI_hpm_91_2_1996.stat( stat);
	  
	  DELPHI_hpm_91_2_1996.syst( sys, "syst" );
	  ds.push_back( DELPHI_hpm_91_2_1996 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************OPAL_hpm_91.2_1999*******************************
    //************************************************************************
    DataSet OPAL_hpm_c_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_hpm_c_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J(1999)369";  
      ds.cite() = "Ackerstaff:1998hz";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count==-1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_hpm_c_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >> sys ) )
	    break;
	  
	  OPAL_hpm_c_91_2_1999.info() = "e+e-_HA_c_enriched";
	  OPAL_hpm_c_91_2_1999.unit() = "";
	  
	  OPAL_hpm_c_91_2_1999.kine( xpmin, "zmin", "" );
	  OPAL_hpm_c_91_2_1999.kine( xpmax, "zmax", "" );
	  OPAL_hpm_c_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  OPAL_hpm_c_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  OPAL_hpm_c_91_2_1999.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_hpm_c_91_2_1999.syst( sys, "syst" );
	  
	  OPAL_hpm_c_91_2_1999.data( data, "e+e-_HA_c_enriched", "");
	  OPAL_hpm_c_91_2_1999.stat( stat);
	  
	  ds.push_back( OPAL_hpm_c_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
  }
  
}
