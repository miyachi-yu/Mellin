/*!
  \file  testF1h.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libunpolSIDISXsec.so.

  You may get some feeling how to use polSIDISXsec::StructureFunction class.

*/
#include <Utility/Timer.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

#include <iostream>
#include <iomanip>

#include <unpolDISXsec/StrFunc.hh>

#include "StrFunc.hh"
#include "xXsec.hh"
#include "xzXsec.hh"

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Utility;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  Transform::LaguerreQuadrature laguer( args.get( "nlag", 5 ), -0.5 );

  // ------------------------------------------------------------ //
  //                  Grid integration max depth                  //
  // ------------------------------------------------------------ //
  ARG_LOG << "Set Max depth to be " 
	  << ( Transform::GridIntegration::maxDepth_ =
	       args.get( "maxDepth", Transform::GridIntegration::maxDepth_ ) )
	  << endl;


  // ------------------------------------------------------------ //
  //                  Set center of energy                        //
  // ------------------------------------------------------------ //
  double s = 56.2;
  ARG_LOG << "Total Energy s = " << setw(12) 
	  << ( s = args.get( "s", s ) ) << endl;
  
  // ------------------------------------------------------------ //
  //                  Set z range                                 //
  // ------------------------------------------------------------ //
  vector< double > zz_( 2, 0.2 );
  zz_[ 0 ] = 0.8; zz_[ 1 ] = 0.2;
  zz_ = args.get( "zrange", ",", zz_ );
  ARG_LOG << "z range: "
	  << setw(6) << zz_[ 0 ]
	  << " - "
	  << setw(6) << zz_[ zz_.size() > 1 ? 1 : 0 ]
	  << endl;

  try {
    
    Timer timer;
    
    // ------------------------------------------------------------ //
    //                  Set Q2                                      //
    // ------------------------------------------------------------ //
    vector< double > q2(0);
    q2.push_back(   1.0 );
    q2.push_back(   2.0 );
    q2.push_back(   5.0 );
    q2.push_back(  10.0 );
    q2.push_back(  20.0 );
    q2.push_back(  50.0 );

    // ------------------------------------------------------------ //
    //                  Set number of x and value of x              //
    // ------------------------------------------------------------ //
    //    int nx = 6;
    int nx = 8;
    double aa = 2.0;
    vector< double > x( 0 );
    for( double lx = - aa; lx < 0.0; lx += aa / nx )
      x.push_back( pow( 10.0, lx ) );
    
    // --------------------------------------------------------------
    //                      Structure function
    // -------------------------------------------------------------
    unpolDISXsec::StrFunc   f1( args, Xsection::Charge::p );
    f1.setS( s );
    unpolSIDISXsec::StrFunc f1h( args, Xsection::Charge::p );
    f1h.setS( s );
    if( args.hasOpt( "Dcalc" ) ) {
      f1.setDirect( true );
      f1h.setDirect( true );
    }
    
    // --------------------------------------------------------------
    //                       Mellin inversion
    // -------------------------------------------------------------
    unpolSIDISXsec::xzXsec* f1hx;
    unpolSIDISXsec::xXsec*  f1hx2;
    unpolSIDISXsec::xXsec*  f1x;
    
    // integration path setting
    vector< double > offset( 2, 3.5 ); 
    vector< double > angle( 2, 0.55 ); 
    vector< double > length( 2, 8.0 );
    
    // for z inversion
    offset[ 1 ] = 3.0;
    angle[ 1 ] = 0.55;
    length[ 1 ] = 8.0;
    
    if( args.hasOpt( "Dcalc" ) ) {
    } else {
    }
    
    offset = args.get( "offset", " ", offset );  
    angle  = args.get( "angle",  " ", angle );
    length = args.get( "length", " ", length );
    
    // --------------------------------------------------------------
    //            Lagurange Quadrature Integration Setting
    // -------------------------------------------------------------
    vector< int > leg1( 1, 6 );
    vector< int > leg2( 1, 8 );
    leg1 = args.get( "leg1", ",", leg1 );
    leg2 = args.get( "leg2", ",", leg2 );
    
    
    // --------------------------------------------------------------
    //            Grid Integration configuration
    // -------------------------------------------------------------
    double precision = args.get( "precision", 5.0E-3 );
    int ngrid = args.get( "ngrid", 2 );
    
    if( args.hasOpt( "laguerre" ) ){
      //      f1hx = new unpolSIDISXsec::xzXsec( &f1h, &laguer );
      throw( 1 );
      
    } else {
      
      f1hx = new unpolSIDISXsec::xzXsec( &f1h, leg1[0], leg2[0], precision );
      f1hx->setGngrid( ngrid );
      f1hx->setParameter( offset, angle );
      f1hx->upper( length );
      
      f1hx2 = new unpolSIDISXsec::xXsec( &f1h, leg1[0], leg2[0], precision );
      f1hx2->setGngrid( ngrid );
      f1hx2->setParameter( offset, angle );
      f1hx2->upper( length );
      
      Transform::GridIntegration* grid =
	dynamic_cast< Transform::GridIntegration* >( f1hx2->integration() );

      if( leg1.size() > 1 ){
	dynamic_cast< Transform::LegQuadrature* >
	  ( grid->method1() )->setngrid( leg1[0], leg1[1] );
      }

      if( leg2.size() > 1 ){
	dynamic_cast< Transform::LegQuadrature* >
	  ( grid->method2() )->setngrid( leg2[0], leg2[1] );
      }


      f1x = new unpolSIDISXsec::xXsec( &f1, leg1[0], leg2[0], precision );
      f1x->setGngrid( ngrid );
      f1x->setParameter( offset, angle );
      f1x->upper( length );
      
    }
    
    complex< double > n( 4.0, 0.0 ), m( 4.0, 0.0 );
    if( args.hasOpt( "Dcalc" ) ) n = m = complex< double >( 2.0, 0.0 ); 
    
    /*
      std::vector< double > xx_( 1, 0.5 ), zz_( 1, 0.5 );
      std::vector< double > mom( 2, 0.0 );
    */
    
    vector< double > xx_( 1, 0.5 ),  mom( 2, 0.0 );
    mom[ 1 ] = 1.0;
    
    f1hx->setMomParam( mom );
    f1hx2->setMomParam( mom );
    
    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
      
      if( iq2 == 0.0 ){
        cout << setw(4)  << "Q^2" 
             << setw(10) << "g_1h (N=" << n.real() << ", M=" 
	     << m.real() << ")" ;
        for( int i = 0; i < x.size(); i++ ) {
          cout << setw(12) << x[ i ] << flush;
        }
        cout << endl;
      }
      
      
      {
	f1h.setQ2( q2[ iq2 ] );
	f1h.clearCache();
	cout 
	  << setw(4) << f1h.q2() 
	  << setw(17) << "DInverseMellin" 
	  //	  << setw(17) << f1h( n, m ) 
	  << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  xx_[ 0 ] = x[ i ]; 
	  cout << setw(12) << (*f1hx)( xx_, zz_ ) << flush;
	}
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec )<< " sec" << endl;      
      }

      {
	f1h.setQ2( q2[ iq2 ] );
	f1h.clearCache();
	cout 
	  << setw(4) << "" 
	  << setw(17) << "InverseMellin" 
	  << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  xx_[ 0 ] = x[ i ]; 
	  cout << setw(12) << (*f1hx2)( xx_, zz_ ) << flush;
	}
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec )<< " sec" << endl;      
      }


      {
	f1.setQ2( q2[ iq2 ] );
	f1h.setQ2( q2[ iq2 ] );
	f1h.clearCache();
	cout 
	  << setw(4) << "" 
	  << setw(17) << "Multiplicity" 
	  << flush;
	
	timer.start();
	for( int i=0; i < x.size(); i++ ){
	  xx_[ 0 ] = x[ i ]; 
	  cout << setw(12) << (*f1hx2)( xx_, zz_ ) / (*f1x)( xx_ ) << flush;
	}
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec )<< " sec" << endl;      
      }
      
      cout << endl;
      
    }
    
  }

  catch ( int error ) {
  }
  
  return 0;
}
