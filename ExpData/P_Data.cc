#include "P_Data.hh"

#include <Utility/Arguments.hh>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp{
  
  namespace P_Data {
    
    //************************************************************************
    //***********************TASSO_P_44_1989*******************************
    //************************************************************************
    DataSet TASSO_P_44_1989( const Exp::HADRON& h ) throw( int ) {

      string path = data_path + "TASSO_P_44_1989.dat";
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
      ds.norm()   = 0.06; // %
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
	  DataPoint TASSO_P_44_1989;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax , q =44 ,data ,stat;
	  if( !( iss >> xpmin >> xpmax >> 
		 data >> stat ))

	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  TASSO_P_44_1989.info() = " e+e-_PR";
	  TASSO_P_44_1989.unit() = "";
	  
	  TASSO_P_44_1989.kine( zmin, "zmin", "" );
	  TASSO_P_44_1989.kine( zmax, "zmax", "" );
	  TASSO_P_44_1989.kine( zmax - zmin, "dz", "" );
	  TASSO_P_44_1989.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  TASSO_P_44_1989.kine( q*q, "Q^2", "GeV^2" );

	  TASSO_P_44_1989.data(data*xp2z, "e+e-_PR","");
	  TASSO_P_44_1989.stat(stat*xp2z);
	  
	  ds.push_back( TASSO_P_44_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TASSO_P_34_1989*******************************
    //************************************************************************
    DataSet TASSO_P_34_1989( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TASSO_P_34_1989.dat";
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
      ds.norm()   = 0.06; // %
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
	  DataPoint TASSO_P_34_1989;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,q =34 ,data ,stat;
	  if( !( iss >> xpmin >> xpmax >> 
		 data>> 
		 stat))
	       break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  TASSO_P_34_1989.info() = " e+e-_PR";
	  TASSO_P_34_1989.unit() = "";
	  
	  TASSO_P_34_1989.kine( zmin, "zmin", "" );
	  TASSO_P_34_1989.kine( zmax, "zmax", "" );
	  TASSO_P_34_1989.kine( zmax - zmin, "dz", "" );
	  TASSO_P_34_1989.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  TASSO_P_34_1989.kine( q*q, "Q^2", "GeV^2" );

	  TASSO_P_34_1989.data(data*xp2z, "e+e-_PR","");
	  TASSO_P_34_1989.stat(stat*xp2z);
 	  
	  ds.push_back( TASSO_P_34_1989 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************TPC_P_29_1988*******************************
    //************************************************************************
    DataSet TPC_P_29_1988( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "TPC_P_29_1988.dat";
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
	  DataPoint TPC_P_29_1988;

      //*******select variable*************************************************
	  
	  double xpmax, xpmin , q =29 ,data ,stat;
	  if( !( iss >> xpmin >> xpmax >>
		 data >> 
		 stat))
	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  TPC_P_29_1988.info() = " e+e-_PR";
	  TPC_P_29_1988.unit() = "";
	  
	  TPC_P_29_1988.kine( zmin, "zmin", "" );
	  TPC_P_29_1988.kine( zmax, "zmax", "" );
	  TPC_P_29_1988.kine( zmax - zmin, "dz", "" );
	  TPC_P_29_1988.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  TPC_P_29_1988.kine( q*q, "Q^2", "GeV^2" );

	  TPC_P_29_1988.data(data*xp2z, "e+e-_PR" ,"");
	  TPC_P_29_1988.stat(stat*xp2z);
 
	  ds.push_back( TPC_P_29_1988 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_P_inclusive_91.2_1999*********************
    //************************************************************************
    DataSet SLD_P_inclusive_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_P_inclusive_91.2_1999.dat";
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
	  DataPoint SLD_P_inclusive_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 data >> stat >> sys ) )
	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  SLD_P_inclusive_91_2_1999.info() = " e+e-_PR";
	  SLD_P_inclusive_91_2_1999.unit() = "";
	  
	  SLD_P_inclusive_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_P_inclusive_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_P_inclusive_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_P_inclusive_91_2_1999.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  SLD_P_inclusive_91_2_1999.kine( q*q, "Q^2", "GeV^2" );

	  SLD_P_inclusive_91_2_1999.data(data*xp2z, "e+e-_PR","");
	  SLD_P_inclusive_91_2_1999.stat(stat*xp2z);
	  SLD_P_inclusive_91_2_1999.syst( sys*xp2z, "syst" );
	  
	  
	  ds.push_back( SLD_P_inclusive_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_P_c_91.2_1999*****************************
    //************************************************************************
    DataSet SLD_P_c_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_P_c_91.2_1999.dat";
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
	  DataPoint SLD_P_c_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,data,stat;
	  if( !( iss >> xpmin >> xpmax >> 
		 data>>
		 stat))
	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  SLD_P_c_91_2_1999.info() = "e+e-_PR_c_enriched";
	  SLD_P_c_91_2_1999.unit() = "";
	  
	  SLD_P_c_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_P_c_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_P_c_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_P_c_91_2_1999.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  SLD_P_c_91_2_1999.kine( q*q, "Q^2", "GeV^2" );

	  SLD_P_c_91_2_1999.data(data*xp2z, "e+e-_PR_c_enriched","");
	  SLD_P_c_91_2_1999.stat(stat*xp2z);
 
	  
	  ds.push_back( SLD_P_c_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************SLD_P_b_91.2_1999*******************************
    //************************************************************************
    DataSet SLD_P_b_91_2_1999( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "SLD_P_b_91.2_1999.dat";
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
	  DataPoint SLD_P_b_91_2_1999;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,data,stat;
	  if( !( iss >> xpmin >> xpmax >> 
		 data >> 
		 stat))
	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  SLD_P_b_91_2_1999.info() = "e+e-_PR_b_enriched";
	  SLD_P_b_91_2_1999.unit() = "";
	  
	  SLD_P_b_91_2_1999.kine( zmin, "zmin", "" );
	  SLD_P_b_91_2_1999.kine( zmax, "zmax", "" );
	  SLD_P_b_91_2_1999.kine( zmax - zmin, "dz", "" );
	  SLD_P_b_91_2_1999.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  SLD_P_b_91_2_1999.kine( q*q, "Q^2", "GeV^2" );

	  SLD_P_b_91_2_1999.data(data*xp2z, "e+e-_PR_b_enriched","");
	  SLD_P_b_91_2_1999.stat(stat*xp2z);

	  ds.push_back( SLD_P_b_91_2_1999 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
 
    //************************************************************************
    //**************************ALEPH_P_91.2_1995***************************
    //************************************************************************
    DataSet ALEPH_P_91_2_1995( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "ALEPH_P_91.2_1995.dat" ;
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
	  DataPoint ALEPH_P_91_2_1995;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 ,data ,stat,sys;
	  if( !( iss >> xpmin >> xpmax >>
		 data>>stat >>sys))

	  break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938*2/q;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double xp2z  =zmean/xmean;
	 	  
	  ALEPH_P_91_2_1995.info() = " e+e-_PR";
	  ALEPH_P_91_2_1995.unit() = "";
	  
	  ALEPH_P_91_2_1995.kine( zmin, "zmin", "" );
	  ALEPH_P_91_2_1995.kine( zmax, "zmax", "" );
	  ALEPH_P_91_2_1995.kine( zmax - zmin, "dz", "" );
	  ALEPH_P_91_2_1995.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	  ALEPH_P_91_2_1995.kine( q*q, "Q^2", "GeV^2" );

	  ALEPH_P_91_2_1995.data(data*xp2z, "e+e-_PR","");
	  ALEPH_P_91_2_1995.stat(stat*xp2z);
	  ALEPH_P_91_2_1995.syst( sys*xp2z, "syst" );
	  
	  ds.push_back( ALEPH_P_91_2_1995 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************OPAL_P_91.2_1994*******************************
    //************************************************************************
    DataSet OPAL_P_91_2_1994( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "OPAL_P_91.2_1994.dat";
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
	  DataPoint OPAL_P_91_2_1994;

      //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double p2z  =zmean/xmean*q/2;
	 	  
	  OPAL_P_91_2_1994.info() = " e+e-_PR";
	  OPAL_P_91_2_1994.unit() = "";
	  
	  OPAL_P_91_2_1994.kine( zmin*2/q, "zmin", "" );
	  OPAL_P_91_2_1994.kine( zmax*2/q, "zmax", "" );
	  OPAL_P_91_2_1994.kine( zmax*2/q - zmin*2/q, "dz", "" );
	  OPAL_P_91_2_1994.kine( 0.5 * ( zmax*2/q + zmin*2/q ), "zmid", "" );
	  OPAL_P_91_2_1994.kine( q*q, "Q^2", "GeV^2" );
	  OPAL_P_91_2_1994.syst( sys*p2z, "syst" );

	  OPAL_P_91_2_1994.data( data*p2z ,"e+e-_PR","");
	  OPAL_P_91_2_1994.stat( stat*p2z);
	  
	  
	  ds.push_back( OPAL_P_91_2_1994 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }

    //************************************************************************
    //***********************DELPHI_P_inclusive_91.2_1998******************
    //************************************************************************
    DataSet DELPHI_P_inclusive_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_P_inclusive_91.2_1998.dat";
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
	  DataPoint DELPHI_P_inclusive_91_2_1998;

     //*******select variable*************************************************
	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double p2z  =zmean/xmean*q/2;
	 	  
	  DELPHI_P_inclusive_91_2_1998.info() = " e+e-_PR";
	  DELPHI_P_inclusive_91_2_1998.unit() = "";
	  
	  DELPHI_P_inclusive_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_P_inclusive_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_P_inclusive_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz", "" );
	  DELPHI_P_inclusive_91_2_1998.kine( 0.5 * ( 2*zmax/q + 2*zmin/q ), "zmid", "" );
	  DELPHI_P_inclusive_91_2_1998.kine( q*q, "Q^2", "GeV^2" );
	  DELPHI_P_inclusive_91_2_1998.syst( sys*p2z, "syst" );

	  DELPHI_P_inclusive_91_2_1998.data( data*p2z ,"e+e-_PR","");
	  DELPHI_P_inclusive_91_2_1998.stat( stat*p2z);
	  
	  
	  ds.push_back( DELPHI_P_inclusive_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }

      return ds;  
    }
 
    //************************************************************************
    //***********************DELPHI_P_b_91.2_1998**************************
    //************************************************************************
    DataSet DELPHI_P_b_91_2_1998( const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "DELPHI_P_b_91.2_1998.dat";
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
	  DataPoint DELPHI_P_b_91_2_1998;

      //*******select variable*************************************************
	  	  
	  double xpmin, xpmax ,  q =91.2 , data ,stat ,sys;
	  if( !( iss >> xpmin >> xpmax >> data >> stat >>sys ))
	       break;
	  
	  //xp->z
	  double xmean = (xpmin+xpmax)/2;
	  
	  double mp =0.938;

	  double zmax  =sqrt( pow(xpmax ,2) + pow(mp ,2));
	  double zmin  =sqrt( pow(xpmin ,2) + pow(mp ,2));
	  double zmean =(zmax+zmin)/2;
	  double p2z  =zmean/xmean*q/2;
	 	  
	  DELPHI_P_b_91_2_1998.info() = "e+e-_PR_b_enriched";
	  DELPHI_P_b_91_2_1998.unit() = "";
	  
	  DELPHI_P_b_91_2_1998.kine( 2*zmin/q, "zmin", "" );
	  DELPHI_P_b_91_2_1998.kine( 2*zmax/q, "zmax", "" );
	  DELPHI_P_b_91_2_1998.kine( 2*zmax/q - 2*zmin/q, "dz", "" );
	  DELPHI_P_b_91_2_1998.kine( 0.5 * ( 2*zmax/q + 2*zmin/q ), "zmid", "" );
	  DELPHI_P_b_91_2_1998.kine( q*q, "Q^2", "GeV^2" );
	  DELPHI_P_b_91_2_1998.syst( sys*p2z, "syst" );
	  
	  DELPHI_P_b_91_2_1998.data( data*p2z ,"e+e-_PR_b_enriched","");
	  DELPHI_P_b_91_2_1998.stat( stat*p2z);
	  
	  ds.push_back( DELPHI_P_b_91_2_1998 );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
  }
  
}
