#include "PIpm_Data.hh"
#include <Utility/Arguments.hh>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp {
  
  namespace PIpm_Data {
    
    //************************************************************************
    //***********************TASSO_PIpm_44_1989*******************************
    //************************************************************************
    DataSet TASSO_PIpm_44_1989( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_PIpm_44_1989.dat";
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
      ds.norm() = 0.06;
      ds.ncoef() =10.0;
      ds.ncerr() =10.0;
      ds.ncstat() ="free";


      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_PIpm_44_1989;

      //*******select variable*************************************************
	  
	  double zmin, zmax , q =44;
	  if( !( iss >> zmin >> zmax >> 
		 TASSO_PIpm_44_1989.data() >> 
		 TASSO_PIpm_44_1989.stat()  ) ) 
	  break;
	  
	 	  
	  TASSO_PIpm_44_1989.info() ="e+e-_PI";
	  TASSO_PIpm_44_1989.unit() = "";
	  
	  TASSO_PIpm_44_1989.kine( zmin, "zmin", "" );
	  TASSO_PIpm_44_1989.kine( zmax, "zmax", "" );
	  TASSO_PIpm_44_1989.kine( zmax - zmin, "dz", "" );
	  TASSO_PIpm_44_1989.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  TASSO_PIpm_44_1989.kine( q*q, "Q^2", "GeV^2 " );

	  
	  
	  ds.push_back( TASSO_PIpm_44_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TASSO_PIpm_34_1989*******************************
    //************************************************************************
    DataSet TASSO_PIpm_34_1989( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_PIpm_34_1989.dat";
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
      
      ds.norm() = 0.06;
      ds.ncoef() =1;
      ds.ncerr() =1;
      ds.ncstat() ="free";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TASSO_PIpm_34_1989;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,q =34;
	  if( !( iss >> zmin >> zmax >> 
		 TASSO_PIpm_34_1989.data() >> 
		 TASSO_PIpm_34_1989.stat() ) )
	  break;
	  
 
	  
	  TASSO_PIpm_34_1989.info() = "e+e-_PI";
	  TASSO_PIpm_34_1989.unit() = "";
	  
	  TASSO_PIpm_34_1989.kine( zmin, "zmin", "" );
	  TASSO_PIpm_34_1989.kine( zmax, "zmax", "" );
	  TASSO_PIpm_34_1989.kine( zmax - zmin, "dz", "" );
	  TASSO_PIpm_34_1989.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  TASSO_PIpm_34_1989.kine( q*q, "Q^2", "GeV^2 " );
	  
	  
	  ds.push_back( TASSO_PIpm_34_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TPC_PIpm_29_1988*******************************
    //************************************************************************
    DataSet TPC_PIpm_29_1988( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TPC_PIpm_29_1988.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint TPC_PIpm_29_1988;

      //*******select variable*************************************************
	  
	  double mean , q =29;
	  if( !( iss >> mean >>
		 TPC_PIpm_29_1988.data() >> 
		 TPC_PIpm_29_1988.stat()   ) ) 
	  break;
	  
 
	  
	  TPC_PIpm_29_1988.info() = "e+e-_PI";
	  TPC_PIpm_29_1988.unit() = "";
	  
	  TPC_PIpm_29_1988.kine( mean, "zmin", "" );
	  TPC_PIpm_29_1988.kine( mean, "zmax", "" );
	  TPC_PIpm_29_1988.kine( mean - mean, "dz", "" );
	  TPC_PIpm_29_1988.kine( 0.5*(mean + mean), "zmid", "" ); 
	  TPC_PIpm_29_1988.kine( q*q, "Q^2", "GeV^2 " );
	  
	  
	  ds.push_back( TPC_PIpm_29_1988 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_PIpm_inclusive_91.2_1999*********************
    //************************************************************************
    DataSet SLD_PIpm_inclusive_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path =  data_path + "SLD_PIpm_inclusive_91.2_1999.dat";
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
      
      ds.norm() = 0.017;
      ds.ncoef() =1;
      ds.ncerr() =1;
      ds.ncstat() ="free";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_PIpm_inclusive_91_2_1999;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 ,sys;
	  if( !( iss >> zmin >> zmax >>
		 SLD_PIpm_inclusive_91_2_1999.data() >> 
		 SLD_PIpm_inclusive_91_2_1999.stat() >> sys  ) ) 
	  break;
	  

	  
	  SLD_PIpm_inclusive_91_2_1999.info() = "e+e-_PI";
	  SLD_PIpm_inclusive_91_2_1999.unit() = "";
	  
	  SLD_PIpm_inclusive_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_PIpm_inclusive_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_PIpm_inclusive_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_PIpm_inclusive_91_2_1999.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  SLD_PIpm_inclusive_91_2_1999.kine( q*q, "Q^2", "GeV^2 " );
	  SLD_PIpm_inclusive_91_2_1999.syst( sys, "syst" );
	  
	  
	  ds.push_back( SLD_PIpm_inclusive_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_PIpm_c_91.2_1999*****************************
    //************************************************************************
    DataSet SLD_PIpm_c_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_PIpm_c_91.2_1999.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_PIpm_c_91_2_1999;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 ,sys;
	  if( !( iss >> zmin >> zmax >> 
		 SLD_PIpm_c_91_2_1999.data() >> 
		 SLD_PIpm_c_91_2_1999.stat()  ) ) 
	  break;

	  SLD_PIpm_c_91_2_1999.info() = "e+e-_PI_c_enriched";
	  SLD_PIpm_c_91_2_1999.unit() = "";
	  
	  SLD_PIpm_c_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_PIpm_c_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_PIpm_c_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_PIpm_c_91_2_1999.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  SLD_PIpm_c_91_2_1999.kine( q*q, "Q^2", "GeV^2 " );
	  
	  ds.push_back( SLD_PIpm_c_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_PIpm_b_91.2_1999*******************************
    //************************************************************************
    DataSet SLD_PIpm_b_91_2_1999( const Exp::HADRON& h ) throw( int ) {

      string path = data_path + "SLD_PIpm_b_91.2_1999.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint SLD_PIpm_b_91_2_1999;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2;
	  if( !( iss >> zmin >> zmax >> 
		 SLD_PIpm_b_91_2_1999.data() >> 
		 SLD_PIpm_b_91_2_1999.stat()) )
	  break;
	  
	  SLD_PIpm_b_91_2_1999.info() = "e+e-_PI_b_enriched";
	  SLD_PIpm_b_91_2_1999.unit() = "";
	  

	  SLD_PIpm_b_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_PIpm_b_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_PIpm_b_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_PIpm_b_91_2_1999.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  SLD_PIpm_b_91_2_1999.kine( q*q, "Q^2", "GeV^2 " );

	  ds.push_back( SLD_PIpm_b_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //**************************ALEPH_PIpm_91.2_1995***************************
    //************************************************************************
    DataSet ALEPH_PIpm_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "ALEPH_PIpm_91.2_1995.dat";
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
      
      ds.norm() = 0.03;
      ds.ncoef() =1;
      ds.ncerr() =1;
      ds.ncstat() ="free";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint ALEPH_PIpm_91_2_1995;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 ,sys;
	  if( !( iss >> zmin >> zmax >>
		 ALEPH_PIpm_91_2_1995.data() >> 
		 ALEPH_PIpm_91_2_1995.stat() >> sys) )
	  break;
	  
 
	  
	  ALEPH_PIpm_91_2_1995.info() = "e+e-_PI";
	  ALEPH_PIpm_91_2_1995.unit() = "";
	  

	  ALEPH_PIpm_91_2_1995.kine( zmin, "zmin", "" );
	  ALEPH_PIpm_91_2_1995.kine( zmax, "zmax", "" );
	  ALEPH_PIpm_91_2_1995.kine( zmax - zmin, "dz", "" );
	  ALEPH_PIpm_91_2_1995.kine( 0.5*(zmin + zmax), "zmid", "" ); 
	  ALEPH_PIpm_91_2_1995.kine( q*q, "Q^2", "GeV^2 " );
	  ALEPH_PIpm_91_2_1995.syst( sys, "syst" );
	  
	  
	  ds.push_back( ALEPH_PIpm_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************OPAL_PIpm_91.2_1994*******************************
    //************************************************************************
    DataSet OPAL_PIpm_91_2_1994( const Exp::HADRON& h ) throw( int ) {
      
      string path =  data_path + "OPAL_PIpm_91.2_1994.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint OPAL_PIpm_91_2_1994;

      //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  OPAL_PIpm_91_2_1994.info() = "e+e-_PI";
	  OPAL_PIpm_91_2_1994.unit() = "";
	  
	  OPAL_PIpm_91_2_1994.kine( 2*zmin/q, "zmin", "" );
	  OPAL_PIpm_91_2_1994.kine( 2*zmax/q, "zmax", "" );
	  OPAL_PIpm_91_2_1994.kine( 2*zmax/q - 2*zmin/q, "dz", "");
	  OPAL_PIpm_91_2_1994.kine( 0.5*(2*zmax/q + 2*zmin/q), "zmid", "");
	  OPAL_PIpm_91_2_1994.kine( q*q, "Q^2", "GeV^2 " );
	  OPAL_PIpm_91_2_1994.syst( sys*q/2, "syst" );

	  OPAL_PIpm_91_2_1994.data( data*q/2, "e+e-_PI", "" );
	  OPAL_PIpm_91_2_1994.stat( stat*q/2);
	  
	  
	  ds.push_back( OPAL_PIpm_91_2_1994 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
    //************************************************************************
    //***********************DELPHI_PIpm_inclusive_91.2_1998******************
    //************************************************************************
    DataSet DELPHI_PIpm_inclusive_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_PIpm_inclusive_91.2_1998.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";

      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar && count == -1 ) count = 0;
	if( count > 0 ){

	  istringstream iss( str );
	  DataPoint DELPHI_PIpm_inclusive_91_2_1998;

     //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ))
	       break;
	  
 
	  
	  DELPHI_PIpm_inclusive_91_2_1998.info() = "e+e-_PI";
	  DELPHI_PIpm_inclusive_91_2_1998.unit() = "";
	  
	  DELPHI_PIpm_inclusive_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_PIpm_inclusive_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_PIpm_inclusive_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz" ,"");
	  DELPHI_PIpm_inclusive_91_2_1998.kine( 0.5*(2*zmax/q + 2*zmin/q), "zmid", "");
	  DELPHI_PIpm_inclusive_91_2_1998.kine( q*q, "Q^2", "GeV^2 " );
	  DELPHI_PIpm_inclusive_91_2_1998.syst( sys*q/2, "syst" );

	  DELPHI_PIpm_inclusive_91_2_1998.data( data*q/2, "e+e-_PI", "");
	  DELPHI_PIpm_inclusive_91_2_1998.stat( stat*q/2);
	  
	  
	  ds.push_back( DELPHI_PIpm_inclusive_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
    
    //************************************************************************
    //***********************DELPHI_PIpm_b_91.2_1998**************************
    //************************************************************************
    DataSet DELPHI_PIpm_b_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_PIpm_b_91.2_1998.dat";
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
      
      ds.norm() = 0;
      ds.ncoef() =0;
      ds.ncerr() =0;
      ds.ncstat() ="fixed";
      
      int tar = 1;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( str.substr( 0, 4 ) == "1 32" && 
	    count == -1 ) count = 0;
	
	if( count != -1 ){
	  
	  istringstream iss( str );
	  DataPoint DELPHI_PIpm_b_91_2_1998;
	  
	  //*******select variable*************************************************
	  
	  double zmin, zmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> zmin >> zmax >> data >> stat >>sys ) ){
	    continue;
	    //	    break;
	  }
	  
	  DELPHI_PIpm_b_91_2_1998.info() = "e+e-_PI_b_enriched";
	  DELPHI_PIpm_b_91_2_1998.unit() = "";
	  
	  DELPHI_PIpm_b_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_PIpm_b_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_PIpm_b_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz" ,"");
	  DELPHI_PIpm_b_91_2_1998.kine( 0.5*(2*zmax/q + 2*zmin/q), "zmid" ,"");
	  DELPHI_PIpm_b_91_2_1998.kine( q*q, "Q^2", "GeV^2 " );
	  DELPHI_PIpm_b_91_2_1998.syst( sys*q/2, "syst" );

	  DELPHI_PIpm_b_91_2_1998.data( data*q/2, "e+e-_PI_b_enriched", "" );
	  DELPHI_PIpm_b_91_2_1998.stat( stat*q/2);
	  
	  
	  ds.push_back( DELPHI_PIpm_b_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
  }
  
}
