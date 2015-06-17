/*!
  \file  testERR.cc
  \brief Example program for error calculation with ErrCalc class.

  This program create FragXsec_T::FragXsec for error calculation,
  so that one has to give at least options for FragXsec construction.
  --FF, --FFK, --FFR ( also for Kretzer, or DSS type )
  

  
 */
#include <Utility/Timer.hh>
#include <Xsec/xXsec.hh>
#include <Evolution/PDFx.hh>
#include <FragXsec_T/FragXsec.hh>
#include <Tranform/Matrix.hh>
#include <TApplication.h>
#include <time.h>

#include "ErrCalc.hh"
#include "PDFCorrCalc.hh"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;

/*!
  get a default error matrix if the given initial distribution does
  not contain PdfErrorMatrix.
*/
PdfErrMatrix seterrm();

int main( int argc, char* argv[] ) {

  Arguments& args = *( Arguments::instance( argc, argv ) );
  TApplication TA( "browser", &argc, argv ); //needed if you use root
  
  try {
    
    Timer timer;

    vector< double > q2(0);
    q2.push_back(   1.0 );
    q2.push_back(   2.0 );
    q2.push_back(   5.0 );
    q2.push_back(  10.0 );
    q2.push_back(  20.0 );
    q2.push_back(  50.0 );
    q2.push_back( 200.0 );
    q2.push_back( 500.0 );
    
    //    int nx = 6;
    int nx = 8;
    double aa = 2.0;
    vector< double > x( 0 );
    for( double lx = - aa; lx < 0.0; lx += aa / nx )
      x.push_back( pow( 10.0, lx ) );
    
    //set xsec//
    FragXsec_T::FragXsec cs( args, QCD::Flavor::PIp );
    Xsection::xXsec xcs( &cs, 4, 6, 1.0E-4, 10.0, 2.0, 0.55); // LO, NLO
    
    //set error//
    ErrorCalc::ErrCalc eCalc( &xcs );
    //    if( eCalc.getErrMatrix().size() == 0 ) eCalc.setErrMatrix( seterrm() );
    
    // get Error Matrix Container
    map< int, QCD::PdfErrMatrix >& ematrices = eCalc.ematrices();
    for( map< int, QCD::PdfErrMatrix >::iterator itr = ematrices.begin();
	 itr != ematrices.end();
	 itr++ ){
      
      ARG_LOG << "# --------------------------------------------------- #\n" 
	      << "#                   Error Matrix                      #\n"
	      << "# --------------------------------------------------- #\n" 
	      << itr->second
	      << endl;
    }

    
    //set corr//
    ErrorCalc::PDFCorrCalc corCalc( &cs );
    //    if( corCalc.getErrMatrix().size() == 0 ) corCalc.setErrMatrix( seterrm() );
    
    complex< double > n( 2.0, 0.0 );
    
    // ----------------------------------------------------------------------
    //                         xsec calc start  
    // ----------------------------------------------------------------------
    if( 1 ) {

      ostringstream ostr;
      ostr << "Xsec ( N=" << n.real() << " )";
      cout << setw(4)  << "Q^2" << setw(15) << ostr.str();
      for( int i = 0; i < x.size(); i++ ) cout << setw(13) << x[ i ] << flush;
      cout << endl;
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	cs.setQ2( q2[ iq2 ] );  // set Q2
	
	// Xsec output      
	cout << setw(4) << cs.q2() << setw(15) << cs( n ) << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  cout << setw(13) << 2.0 * xcs( x[ i ] ) << flush;
	}
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec ) << " sec";
	cout << endl;
	
	// Xsec Error output     
	cout << setw(4) << cs.q2()
	     << setw(15) << 2.0 * sqrt( eCalc( n ) ) << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  // ErrCalc returns sigma^2
	  cout << setw(13) << 2.0 * sqrt( eCalc( x[ i ] ) ) << flush;
	}
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec ) << " sec";
	cout << endl;
	
      }
    }

    if( 0 ){
      
      corCalc.setPID_1( Flavor::PARTON( 1 ) );
      //    corCalc.setPID_2( Flavor::PARTON( 0 ) );
      corCalc.setPID_2( Flavor::PARTON( 1 ) );
      corCalc.setcvout( true );
      corCalc.setnom( true );
      corCalc.setflout( true );
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	cs.setQ2( q2[ iq2 ] );
	//    complex< double > tmp = corCalc( n );
	double tmp = corCalc( 0.01, 0.7 );
	sleep( 1 );
      }
    }
    
    
    //pdf(ff) calc//
    if( 0 ){
      eCalc.setPID( Flavor::PARTON( 1 ) );
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	if( iq2 == 0.0 ){
	  cout << setw(4)  << "Q^2" 
	       << setw(12) << "FF ( N=" << n.real() << " )" ;
	  for( int i = 0; i < x.size(); i++ ) {
	    cout << setw(13) << x[ i ] << flush;
	  }
	  cout << endl;
	}
	
	cs.setQ2( q2[ iq2 ] );
	
	// pdf direct
	eCalc.setPDFdrct( true );
	
	// pdf output      
	cout << setw(4) << cs.q2()
	     << setw(15) << eCalc( n ) << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  cout << setw(13) << x[ i ] * eCalc( x[ i ] ) << flush;
	}
	timer.stop();
	
	double msec = timer.check( Timer::Msec );
	cout << setw(10) << msec / 1000.0 << " sec";
	cout << endl;
	
	// pdf Error output     
	eCalc.setPDFdrct( false ); // pdf error error
	
	cout << setw(4) << cs.q2()
	     << setw(15) << sqrt( eCalc( n ) ) << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  // ErrCalc returns sigma^2
	  cout << setw(13) << x[ i ] * sqrt( eCalc( x[ i ] ) ) << flush;
	}
	timer.stop();
	
	msec = timer.check( Timer::Msec );
	cout << setw(10) << msec / 1000.0 << " sec";
	cout << endl;
	
      }
    }
  }

  catch ( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << flush;
    if( error == 8 ) 
      cerr << " error matrix is not positive definite!!" << flush;
    cerr << endl;
  }
  
  return 0;
}

PdfErrMatrix seterrm(){
  
  PdfErrMatrix errmtx;
  errmtx.resize( 16, vector< double >( 16, 0.0 ) );
  
  std::vector< int > par( 0 );
  par.push_back( 1 );
  par.push_back( 2 );
  par.push_back( 3 );
  par.push_back( 5 );
  par.push_back( 6 );
  par.push_back( 7 );
  
  std::vector< std::vector< double > > 
    pem( par.size(), 
	 std::vector< double >( par.size(), 0.0 ) );

  pem[ 0 ][ 0 ] =     6.53138e-05;
  pem[ 1 ][ 0 ] =     -0.00111445;
  pem[ 1 ][ 1 ] =         0.68971;
  pem[ 2 ][ 0 ] =     -0.00501075;
  pem[ 2 ][ 1 ] =         1.95226;
  pem[ 2 ][ 2 ] =         5.68084;
  pem[ 3 ][ 0 ] =    -7.19115e-05;
  pem[ 3 ][ 1 ] =     0.000809509;
  pem[ 3 ][ 2 ] =      0.00470423;
  pem[ 3 ][ 3 ] =     8.30695e-05;
  pem[ 4 ][ 0 ] =     0.000891787;
  pem[ 4 ][ 1 ] =       0.0400945;
  pem[ 4 ][ 2 ] =       0.0983689;
  pem[ 4 ][ 3 ] =    -0.000995603;
  pem[ 4 ][ 4 ] =        0.019748;
  pem[ 5 ][ 0 ] =     0.000515708;
  pem[ 5 ][ 1 ] =       0.0598512;
  pem[ 5 ][ 2 ] =        0.174765;
  pem[ 5 ][ 3 ] =    -0.000581082;
  pem[ 5 ][ 4 ] =       0.0179088;
  pem[ 5 ][ 5 ] =       0.0207829;
  
  for( int i = 0; i < par.size(); i++ ) 
    for( int j = 0; j < par.size(); j++ ) if( i >= j ){
	
	int tmpi = par[ i ] - 1;
	int tmpj = par[ j ] - 1;
	//      cout << tmpi << " " << tmpj;
	errmtx[ tmpi ][ tmpj ] = pem[ i ][ j ];
	errmtx[ tmpj ][ tmpi ] = pem[ i ][ j ];
	//      cout << tmpi << " " << tmpj << " " << flush;
	//      cout << errmtx[ tmpj ][ tmpi ] << endl;
  }
  
  //  cout << errmtx << endl;
  return errmtx;
  
};
