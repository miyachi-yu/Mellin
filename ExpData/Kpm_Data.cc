#include "Kpm_Data.hh"

#include <Utility/Arguments.hh>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp{
  
  namespace Kpm_Data {
    
    //************************************************************************
    //***********************TASSO_Kpm_44_1989*******************************
    //************************************************************************
    DataSet TASSO_Kpm_44_1989( const Exp::HADRON& h ) throw( int ) {

      string path = data_path + "TASSO_Kpm_44_1989.dat";
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
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_Kpm_44_1989;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax , q =44;
	  if( !( iss >> xpmin >> xpmax >> 
		 TASSO_Kpm_44_1989.data() >> 
		 TASSO_Kpm_44_1989.stat()  ) ) 
	  break;
	  
	 	  
	  TASSO_Kpm_44_1989.info() = "e+e-_KA";
	  TASSO_Kpm_44_1989.unit() = "";
	  
	  TASSO_Kpm_44_1989.kine( xpmin, "zmin", "" );
	  TASSO_Kpm_44_1989.kine( xpmax, "zmax", "" );
	  TASSO_Kpm_44_1989.kine( xpmax - xpmin, "dz", "" );
	  TASSO_Kpm_44_1989.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TASSO_Kpm_44_1989.kine( q*q, "Q^2", "GeV^2" );
	 
	  
	  ds.push_back( TASSO_Kpm_44_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TASSO_Kpm_34_1989*******************************
    //************************************************************************
    DataSet TASSO_Kpm_34_1989( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_Kpm_34_1989.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
            
      DataSet ds;
      ds.info() = "TASSO_34_1989_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C42(1989)189";  
      ds.cite() = "Braunschweig:1988hv";
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_Kpm_34_1989;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,q =34;
	  if( !( iss >> xpmin >> xpmax >> 
		 TASSO_Kpm_34_1989.data() >> 
		 TASSO_Kpm_34_1989.stat() ) )
	  break;
	  
 
	  
	  TASSO_Kpm_34_1989.info() = "e+e-_KA";
	  TASSO_Kpm_34_1989.unit() = "";
	  
	  TASSO_Kpm_34_1989.kine( xpmin, "zmin", "" );
	  TASSO_Kpm_34_1989.kine( xpmax, "zmax", "" );
	  TASSO_Kpm_34_1989.kine( xpmax - xpmin, "dz", "" );
	  TASSO_Kpm_34_1989.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TASSO_Kpm_34_1989.kine( q*q, "Q^2", "GeV^2" );
	  
	  
	  ds.push_back( TASSO_Kpm_34_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TPC_Kpm_29_1988*******************************
    //************************************************************************
    DataSet TPC_Kpm_29_1988( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TPC_Kpm_29_1988.dat";
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
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TPC_Kpm_29_1988;

      //*******select variable*************************************************
	  
	  double xpmax, xpmin , q =29;
	  if( !( iss >> xpmin >> xpmax >>
		 TPC_Kpm_29_1988.data() >> 
		 TPC_Kpm_29_1988.stat()   ) ) 
	  break;
	  
 
	  
	  TPC_Kpm_29_1988.info() = "e+e-_KA";
	  TPC_Kpm_29_1988.unit() = "";
	  
	  TPC_Kpm_29_1988.kine( xpmin, "zmin", "" );
	  TPC_Kpm_29_1988.kine( xpmax, "zmax", "" );
	  TPC_Kpm_29_1988.kine( xpmax - xpmin, "dz", "" );
	  TPC_Kpm_29_1988.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  TPC_Kpm_29_1988.kine( q*q, "Q^2", "GeV^2" );
	  
	  
	  ds.push_back( TPC_Kpm_29_1988 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_Kpm_inclusive_91.2_1999*********************
    //************************************************************************
    DataSet SLD_Kpm_inclusive_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_Kpm_inclusive_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "SLD__inclusive_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rev.D59(1999)052001";  
      ds.cite() = "Abe:1998zs";
      ds.norm()   = 0.017; // %
      ds.ncoef()  = 1; 
      ds.ncerr()  = 1;
      ds.ncstat() = "free";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_Kpm_inclusive_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 SLD_Kpm_inclusive_91_2_1999.data() >> 
		 SLD_Kpm_inclusive_91_2_1999.stat() >> sys  ) ) 
	  break;
	  

	  
	  SLD_Kpm_inclusive_91_2_1999.info() = "e+e-_KA";
	  SLD_Kpm_inclusive_91_2_1999.unit() = "";
	  
	  SLD_Kpm_inclusive_91_2_1999.kine( xpmin, "zmin", "" );
	  SLD_Kpm_inclusive_91_2_1999.kine( xpmax, "zmax", "" );
	  SLD_Kpm_inclusive_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  SLD_Kpm_inclusive_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  SLD_Kpm_inclusive_91_2_1999.kine( q*q, "Q^2", "GeV^2" );
	  SLD_Kpm_inclusive_91_2_1999.syst( sys, "syst" );
	  
	  
	  ds.push_back( SLD_Kpm_inclusive_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_Kpm_c_91.2_1999*****************************
    //************************************************************************
    DataSet SLD_Kpm_c_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_Kpm_c_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "SLD_c_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rev.D59(1999)052001";  
      ds.cite() = "Abe:1998zs";
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_Kpm_c_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,sys;
	  if( !( iss >> xpmin >> xpmax >> 
		 SLD_Kpm_c_91_2_1999.data() >> 
		 SLD_Kpm_c_91_2_1999.stat()  ) ) 
	  break;
	  
	  SLD_Kpm_c_91_2_1999.info() = "e+e-_KA_c_enriched";
	  SLD_Kpm_c_91_2_1999.unit() = "";
	  
	  SLD_Kpm_c_91_2_1999.kine( xpmin, "zmin", "" );
	  SLD_Kpm_c_91_2_1999.kine( xpmax, "zmax", "" );
	  SLD_Kpm_c_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  SLD_Kpm_c_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  SLD_Kpm_c_91_2_1999.kine( q*q, "Q^2", "GeV^2" );

	  ds.push_back( SLD_Kpm_c_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_Kpm_b_91.2_1999*******************************
    //************************************************************************
    DataSet SLD_Kpm_b_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_Kpm_b_91.2_1999.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
            
      DataSet ds;
      ds.info() = "SLD__b_91.2_1999_"+ Exp::hadname( h );
      ds.ref()  = "Phys.Rev.D59(1999)052001";  
      ds.cite() = "Abe:1998zs";
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_Kpm_b_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2;
	  if( !( iss >> xpmin >> xpmax >> 
		 SLD_Kpm_b_91_2_1999.data() >> 
		 SLD_Kpm_b_91_2_1999.stat()) )
	  break;
	  
	  SLD_Kpm_b_91_2_1999.info() = "e+e-_KA_b_enriched";
	  SLD_Kpm_b_91_2_1999.unit() = "";

	  SLD_Kpm_b_91_2_1999.kine( xpmin, "zmin", "" );
	  SLD_Kpm_b_91_2_1999.kine( xpmax, "zmax", "" );
	  SLD_Kpm_b_91_2_1999.kine( xpmax - xpmin, "dz", "" );
	  SLD_Kpm_b_91_2_1999.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  SLD_Kpm_b_91_2_1999.kine( q*q, "Q^2", "GeV^2" );

	  ds.push_back( SLD_Kpm_b_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //**************************ALEPH_Kpm_91.2_1995***************************
    //************************************************************************
    DataSet ALEPH_Kpm_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "ALEPH_Kpm_91.2_1995.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "ALEPH__91.2_1995_"+ Exp::hadname( h );
      ds.ref()  = "Z.Phys.C66(1995)355";  
      ds.cite() = "Buskulic:1994ft";
      ds.norm()   = 0.05; // %
      ds.ncoef()  = 1.0; 
      ds.ncerr()  = 1;
      ds.ncstat() = "free"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint ALEPH_Kpm_91_2_1995;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 ALEPH_Kpm_91_2_1995.data() >> 
		 ALEPH_Kpm_91_2_1995.stat() >> sys) )
	  break;
	  
	  ALEPH_Kpm_91_2_1995.info() = "e+e-_KA";
	  ALEPH_Kpm_91_2_1995.unit() = "";

	  ALEPH_Kpm_91_2_1995.kine( xpmin, "zmin", "" );
	  ALEPH_Kpm_91_2_1995.kine( xpmax, "zmax", "" );
	  ALEPH_Kpm_91_2_1995.kine( xpmax - xpmin, "dz", "" );
	  ALEPH_Kpm_91_2_1995.kine( 0.5 * ( xpmax + xpmin ), "zmid", "" );
	  ALEPH_Kpm_91_2_1995.kine( q*q, "Q^2", "GeV^2" );
	  ALEPH_Kpm_91_2_1995.syst( sys, "syst" );

	  ds.push_back( ALEPH_Kpm_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************OPAL_Kpm_91.2_1994*******************************
    //************************************************************************
    DataSet OPAL_Kpm_91_2_1994( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_Kpm_91.2_1994.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "OPAL_91.2_1994_"+ Exp::hadname( h );
      ds.ref()  = "Z. Phys.C63(1994)181";  
      ds.cite() = "Akers:1994ez";
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_Kpm_91_2_1994;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  OPAL_Kpm_91_2_1994.info() = "e+e-_KA";
	  OPAL_Kpm_91_2_1994.unit() = "";
	  
	  OPAL_Kpm_91_2_1994.kine( xpmin*2/q, "zmin", "" );
	  OPAL_Kpm_91_2_1994.kine( xpmax*2/q, "zmax", "" );
	  OPAL_Kpm_91_2_1994.kine(  xpmax*2/q - xpmin*2/q, "dz", "" );
	  OPAL_Kpm_91_2_1994.kine( 0.5 * ( xpmax*2/q + xpmin*2/q ), "zmid", "" );

	  OPAL_Kpm_91_2_1994.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_Kpm_91_2_1994.syst( sys*q/2, "syst" );

	  OPAL_Kpm_91_2_1994.data( data*q/2, "e+e-_KA", "");
	  OPAL_Kpm_91_2_1994.stat( stat*q/2);
	  
	  
	  ds.push_back( OPAL_Kpm_91_2_1994 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //***********************************************************************
    //***********************DELPHI_Kpm_91.2_1995******************************
    //***********************************************************************
    DataSet DELPHI_Kpm_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_Kpm_91.2_1995.dat" ;
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }

      DataSet ds;
      ds.info() = "DELPHI_91.2_1995_"+ Exp::hadname( h );
      ds.ref()  = "Eur.Phys.J>C5(1998)585";  
      ds.cite() = "Abreu:1998vq";
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint DELPHI_Kpm_91_2_1995;

     //*******select variable*************************************************
	  
	  double xpmean ,  q =91.2 , data ,stat;
	  if( !( iss >> xpmean>> data >> stat ))
	       break;
	  
	  DELPHI_Kpm_91_2_1995.info() = "e+e-_KA";
	  DELPHI_Kpm_91_2_1995.unit() = "";
	  
	  DELPHI_Kpm_91_2_1995.kine( xpmean, "zmin", "" );
	  DELPHI_Kpm_91_2_1995.kine( xpmean, "zmax", "" );
	  DELPHI_Kpm_91_2_1995.kine( 0.0,    "dz", "" );
	  DELPHI_Kpm_91_2_1995.kine( xpmean, "zmid", "" );
	  DELPHI_Kpm_91_2_1995.kine( q*q, "Q^2", "GeV^2" );

	  
	  DELPHI_Kpm_91_2_1995.data( data, "e+e-_KA", "");
	  DELPHI_Kpm_91_2_1995.stat( stat);
	  
	  
	  ds.push_back( DELPHI_Kpm_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************DELPHI_Kpm_inclusive_91.2_1998******************
    //************************************************************************
    DataSet DELPHI_Kpm_inclusive_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_Kpm_inclusive_91.2_1998.dat";
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
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed"; 
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint DELPHI_Kpm_inclusive_91_2_1998;

     //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
	  DELPHI_Kpm_inclusive_91_2_1998.info() = "e+e-_KA";
	  DELPHI_Kpm_inclusive_91_2_1998.unit() = "";
	  
	  DELPHI_Kpm_inclusive_91_2_1998.kine( 2*xpmin/q, "zmin", "" );
	  DELPHI_Kpm_inclusive_91_2_1998.kine( 2*xpmax/q, "zmax", "" );
	  DELPHI_Kpm_inclusive_91_2_1998.kine( 2*xpmax/q - 2*xpmin/q, "dz", "" );
	  DELPHI_Kpm_inclusive_91_2_1998.kine( 0.5 * ( 2*xpmax/q + 2*xpmin/q ), "zmid", "" );

	  DELPHI_Kpm_inclusive_91_2_1998.kine( q*q, "Q^2", "GeV^2" );
	  DELPHI_Kpm_inclusive_91_2_1998.syst( sys*q/2, "syst" );

	  DELPHI_Kpm_inclusive_91_2_1998.data( data*q/2, "e+e-_KA", "");
	  DELPHI_Kpm_inclusive_91_2_1998.stat( stat*q/2);
	  
	  ds.push_back( DELPHI_Kpm_inclusive_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************DELPHI_Kpm_b_91.2_1998**************************
    //************************************************************************
    DataSet DELPHI_Kpm_b_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_Kpm_b_91.2_1998.dat";
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
      ds.norm()   = 0; // %
      ds.ncoef()  = 0.0; 
      ds.ncerr()  = 0;
      ds.ncstat() = "fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint DELPHI_Kpm_b_91_2_1998;

      //*******select variable*************************************************
	  	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  DELPHI_Kpm_b_91_2_1998.info() = "e+e-_KA_b_enriched";
	  DELPHI_Kpm_b_91_2_1998.unit() = "";
	  
	  DELPHI_Kpm_b_91_2_1998.kine( 2*xpmin/q, "zmin", "" );
	  DELPHI_Kpm_b_91_2_1998.kine( 2*xpmax/q, "zmax", "" );
	  DELPHI_Kpm_b_91_2_1998.kine( 2*xpmax/q - 2*xpmin/q, "dz", "" );
	  DELPHI_Kpm_b_91_2_1998.kine( 0.5 * ( 2*xpmax/q + 2*xpmin/q ), "zmid", "" );
	  DELPHI_Kpm_b_91_2_1998.kine( q*q, "Q^2", "GeV^2" );
	  DELPHI_Kpm_b_91_2_1998.syst( sys*q/2, "syst" );

	  DELPHI_Kpm_b_91_2_1998.data( data*q/2, "e+e-_KA_b_enriched", "" );
	  DELPHI_Kpm_b_91_2_1998.stat( stat*q/2);
	  
	  
	  ds.push_back( DELPHI_Kpm_b_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
	  }
     }
  
}
