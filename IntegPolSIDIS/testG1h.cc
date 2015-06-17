#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <IntegXsec/XsecIntegrand.hh>
#include <polSIDIS/StrFunc.hh>
#include <Xsec/xzXsec.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "StrFunc.hh"
#include "PStrFunc.hh"
#include "nXsecComp.hh"

using namespace Utility;
using namespace std;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  vector< double > q2(0);
  q2.push_back(   1.0 );
  q2.push_back(   2.0 );
  q2.push_back(   5.0 );
  q2.push_back(  10.0 );
  q2.push_back(  20.0 );
  q2.push_back(  50.0 );
  q2.push_back( 200.0 );
  q2.push_back( 500.0 );
  
  //  int nx = 6;
  int nx = 8;
  double aa = 2.0;
  vector< double > x( 0 );
  for( double lx = - aa; lx < 0.0; lx += aa / nx )
    x.push_back( pow( 10.0, lx ) );
  
  double offset = args.get( "offset", 2.0  );  
  double angle  = args.get( "angle",  0.55  );  
  double length = args.get( "length", 15.0 );  

  vector< double > voffset( 2, offset ), vangle( 2, angle ); 
  vector< double > vlength( 2, length ), unitv( 1, 1.0 );  

  try {
    
    Timer timer;
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    // x-space STRFunc implementation
    int n_( 4 ), m_( ( args.hasOpt( "xShortInteg" ) ? 0 : 6 ) );
    double precision_( ( args.hasOpt( "xShortInteg" ) ? 1.0E-2 : 1.0E-4 ) );
    IntegPolSIDIS::StrFunc xg1hh( args, IntegXsec::Charge::p, 
				    QCD::Flavor::PIp, n_, m_, precision_,
				    10.0, 2.0, 0.55 );
    xg1hh.setGngrid( ( args.hasOpt( "xShortInteg" ) ? 12 : 2 ) ); 
    if( args.hasOpt( "logInteg" ) )  xg1hh.setloginteg( true ); 

    IntegPolSIDIS::PartialInteg 
      xg1h( dynamic_cast< Transform::RealFunction* >( &xg1hh ), 
	    4, 0, 4, 1.0E-2 );
    xg1h.setPIntegRange( 0.2, 0.8 );
    // int dz / z * ( 2 x * z * g1h )

    // Mellin-x-space STRFunc implementation
    polSIDIS::StrFunc g1hn( args, Xsection::Charge::p );
    args.set( "PInteg", 0 );
    //    args.set( "ShortInteg", 0 );
    g1hn.xsecComp( new IntegPolSIDIS::nXsecComp( args, 
						   IntegXsec::Charge::p,
						   QCD::Flavor::PIp ) );
    g1hn.xsecComp()->coeff()->constNf( false );
    dynamic_cast< polPDF::Evo* >
      ( g1hn.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( g1hn.xsecComp()->vevo()[ 1 ] )->constNf( false );

    if( args.hasOpt( "logInteg" ) )
      dynamic_cast< IntegXsec::xzXsec* >( g1hn.xsecComp()->coeff() )
	->setloginteg( true );
    dynamic_cast< IntegXsec::xzXsec* >( g1hn.xsecComp()->coeff() )
      ->setGngrid( args.get( "ngrid", 12 ) );

    dynamic_cast< Transform::PartialInteg* >( g1hn.xsecComp()->coeff() )
      ->setPIntegRange( 0.2, 0.8 );

    Xsection::xzXsec g1hxz( &g1hn, 4, 6, 1.0E-4 );        // MellinInversion
    g1hxz.setParameter( voffset, vangle );
    g1hxz.upper( vlength );
    // int dz * ( 2 g1h ) by g1hxz( 1.0, 1.0 )

    // Mellin-space STRFunc implementation
    polSIDIS::StrFunc g1hnn( args, Xsection::Charge::p );
    g1hnn.xsecComp()->coeff()->constNf( false );
    dynamic_cast< polPDF::Evo* >
      ( g1hnn.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( g1hnn.xsecComp()->vevo()[ 1 ] )->constNf( false );

    Xsection::xzXsec g1hxx( &g1hnn, 4, 6, 1.0E-4 );
    g1hxx.setParameter( voffset, vangle );
    g1hxx.upper( vlength );

    vector< double > xx_( 1, 0.5 ), zz_( 2, 0.2 ), mom( 2, 0.0 );
    zz_[ 0 ] = 0.8; zz_[ 1 ] = 0.2; mom[ 1 ] = 1.0;
    g1hxx.setMomParam( mom );
    // int dz * ( 2 g1h ) / ( zmax - zmin )

    if( 0 ) {
    } else {
      
      if( args.hasOpt( "wncalc" ) ){

	//start Mellin implementation
	cout << "Mellin space implementation" << endl;

	complex< double > n( 2.0, 0.0 ), m( 2.0, 0.0 );
	
	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  
	  if( iq2 == 0 ){
	    cout << setw(4)  << "Q^2" << setw(15) << "g1h(N=2, M=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    g1hnn.setQ2( q2[ iq2 ] );
	    
	    cout << setw(4) << q2[ iq2 ]
		 << setw(15) << g1hnn( n, m ) << flush;
	    
	    timer.start();
	    for( int i=0; i < x.size(); i++ ){
	      xx_[ 0 ] = x[ i ]; 
	      cout << setw(13) << xx_[ 0 ] * g1hxx( xx_, zz_ ) 
		* ( zz_[ 0 ] - zz_[ 1 ] ) / 2.0 << flush;
	    }
	    timer.stop();
	    cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;
	    
	  }
	  cout << endl;
	}
	
	cout << endl;
	// end Mellin implementation
      }

      if( args.hasOpt( "wxcalc" ) ){
	//start x-space implementation
	cout << "x-space implementation" << endl;
	
	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  
	  if( iq2 == 0 ){
	    cout << setw(4)  << "Q^2" << setw(15) << "g1h(N=2, M=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {

	    xg1hh.setQ2( q2[ iq2 ] );
	    
	    cout << setw(4) << q2[ iq2 ]
		 << setw(15) << "******" << flush;
	    
	    timer.start();
	    for( int i = 0; i < x.size(); i++ ){
	      cout << setw( 13 ) <<  xg1h( x[ i ] ) / 2.0 << flush;
	    }
	    timer.stop();
	    cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;
	    
	  }
	  cout << endl;
	}
	
	cout << endl;
	// end x-space implementation
      }

      for( int i = 0; i <  args.get( "mxloop", 1 ); i++ ){

      //start Mellin-x implementation
      cout << "Mellin-x-space implementation" << endl;

      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	if( iq2 == 0 ){
	  cout << setw(4)  << "Q^2" << setw(15) << "g1h(N=2, M=2)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 

	{

	  g1hn.setQ2( q2[ iq2 ] );

	  cout << setw(4) << q2[ iq2 ]
	       << setw(15) << "******" << flush;

	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    dynamic_cast< IntegPolSIDIS::PStrFunc* >
	      ( g1hn.xsecComp()->coeff() )->setx_zPI( x[ i ] );
	    cout << setw( 13 ) 
		 <<  x[ i ] * g1hxz( unitv, unitv ) / 2.0 
		 << flush;
	  }
	  timer.stop();
	  cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;

	}
	cout << endl;
      }

      // end Mellin-x implementation      
      }
    }
  }
  
  catch ( int error ) {
  }

  return 0;
}
