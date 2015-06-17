/*!
  \file  testG1h.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libpolSIDIS.so.

  You may get some feeling how to use polSIDIS::StrFunc class.

 */
#include <Utility/Timer.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/CKernel.hh>
#include "Evolution/PDFx.hh"

#include <iostream>
#include <iomanip>

#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;

int main( int argc, char* argv[] ) {

  //  create corresponding arg object through static function instance 
  //  in Arguments class   
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
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
    
    vector< double > vlength_( 2, 0.0 );
    vector< double > voffset_( 2, 0.0 );
    vector< double > vangle_( 2, 0.0 );
    vlength_[ 0 ] = args.get( "PPDFlength", 10.0 );
    vlength_[ 1 ] = args.get( "FFlength", 10.0 );
    voffset_[ 0 ] = args.get( "PPDFoffset", 2.0 );
    voffset_[ 1 ] = args.get( "FFoffset", 2.0 );
    vangle_[ 0 ] = args.get( "PPDFangle", 0.55 );
    vangle_[ 1 ] = args.get( "FFangle", 0.55 );

    //    int nx = 6;
    int nx = 8;
    double aa = 2.0;
    vector< double > x( 0 );
    for( double lx = - aa; lx < 0.0; lx += aa / nx )
      x.push_back( pow( 10.0, lx ) );
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    polSIDIS::StrFunc g1h( args, Xsection::Charge::p );
    g1h.xsecComp()->coeff()->constNf( false );
    dynamic_cast< polPDF::Evo* >
      ( g1h.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( g1h.xsecComp()->vevo()[ 1 ] )->constNf( false );

    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-4, 10.0, 2.0, 0.55); // LO, NLO
    g1hx.setParameter( voffset_, vangle_ );
    g1hx.upper( vlength_ );
    // MellinInversion

    // --------------------------------------------------------------
    //     performance in moment calculation
    // --------------------------------------------------------------

    complex< double > n( 2.0, 0.0 ), m( 2.0, 0.0 );

    /*
    std::vector< double > xx_( 1, 0.5 ), zz_( 1, 0.5 );
    std::vector< double > mom( 2, 0.0 );
    */
    ///*
    std::vector< double > xx_( 1, 0.5 ), zz_( 2, 0.2 );
    std::vector< double > mom( 2, 0.0 );
    zz_[ 0 ] = 0.8; zz_[ 1 ] = 0.2;
    mom[ 1 ] = 1.0;
    //*/    
    g1hx.setMomParam( mom );
    
    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
      
      if( iq2 == 0.0 ){
	cout << setw(4)  << "Q^2" 
	     << setw(15) << "g_1h (N=2, M=2)" ;
	for( int i = 0; i < x.size(); i++ ) {
	  cout << setw(13) << x[ i ] << flush;
	}
	cout << endl;
      }
      
      g1h.setQ2( q2[ iq2 ] );
      
      cout << setw(4) << g1h.q2()
	   << setw(15) << g1h( n, m ) << flush;
      
      timer.start();
      //      /*
      for( int i=0; i < x.size(); i++ ){
	xx_[ 0 ] = x[ i ]; 
	cout << setw(13) << xx_[ 0 ] * g1hx( xx_, zz_ ) 
	  * ( zz_[ 0 ] - zz_[ 1 ] ) / 2.0 << flush;
      }
      //      */
      /*
      for( int i=0; i < x.size(); i++ ){
	xx_[ 0 ] = x[ i ]; zz_[ 0 ] = x[ i ];
	cout << setw(13) << xx_[ 0 ] * zz_[ 0 ] * g1hx( xx_, zz_ )
	  / 2.0 << flush;
      }
      */
      timer.stop();
      
      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << endl;
    }
    
  }
  
  catch ( int error ) {
  }

  return 0;
}
