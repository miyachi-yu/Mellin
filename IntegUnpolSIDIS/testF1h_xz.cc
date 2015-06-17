#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <IntegXsec/XsecIntegrand.hh>
#include <unpolSIDIS/StrFunc.hh>
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
    IntegUnpolSIDIS::StrFunc xf1hh( args, IntegXsec::Charge::p, 
				    QCD::Flavor::PIp, 4, 0, 1.0E-2,
				    10.0, 2.0, 0.55 );
    xf1hh.setGngrid( 4 ); 
    if( args.hasOpt( "logInteg" ) )  xf1hh.setloginteg( true ); 
    // 2 x * z * f1h

    // Mellin-x-space STRFunc implementation
    unpolSIDIS::StrFunc f1hn( args, Xsection::Charge::p );
    //    args.set( "PInteg", 0 );
    //    args.set( "ShortInteg", 0 );
    f1hn.xsecComp( new IntegUnpolSIDIS::nXsecComp( args, 
						   IntegXsec::Charge::p,
						   QCD::Flavor::PIp ) );
    f1hn.xsecComp()->coeff()->constNf( false );
    dynamic_cast< unpolPDF::Evo* >
      ( f1hn.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( f1hn.xsecComp()->vevo()[ 1 ] )->constNf( false );

    if( args.hasOpt( "logInteg" ) )
      dynamic_cast< IntegXsec::xzXsec* >( f1hn.xsecComp()->coeff() )
	->setloginteg( true );
    dynamic_cast< IntegXsec::xzXsec* >( f1hn.xsecComp()->coeff() )
      ->setGngrid( args.get( "ngrid", 12 ) );

    Xsection::xzXsec f1hxz( &f1hn, 4, 6, 1.0E-4 );        // MellinInversion
    f1hxz.setParameter( voffset, vangle );
    f1hxz.upper( vlength );
    // 2 f1h

    // Mellin-space STRFunc implementation
    unpolSIDIS::StrFunc f1hnn( args, Xsection::Charge::p );
    f1hnn.xsecComp()->coeff()->constNf( false );
    dynamic_cast< unpolPDF::Evo* >
      ( f1hnn.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( f1hnn.xsecComp()->vevo()[ 1 ] )->constNf( false );

    Xsection::xzXsec f1hxx( &f1hnn, 4, 6, 1.0E-4 );
    f1hxx.setParameter( voffset, vangle );
    f1hxx.upper( vlength );

    vector< double > xx_( 1, 0.0 ), zz_( 1, 0.0 ), mom( 2, 0.0 );
    f1hxx.setMomParam( mom );
    // 2 f1h  

    if( 0 ) {
    } else {
      
      if( args.hasOpt( "wncalc" ) ){

	//start Mellin implementation
	cout << "Mellin space implementation" << endl;

	complex< double > n( 2.0, 0.0 ), m( 2.0, 0.0 );
	
	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  
	  if( iq2 == 0 ){
	    cout << setw(4)  << "Q^2" << setw(15) << "f1h(N=2, M=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    f1hnn.setQ2( q2[ iq2 ] );
	    
	    cout << setw(4) << q2[ iq2 ]
		 << setw(15) << f1hnn( n, m ) << flush;
	    
	    timer.start();
	    for( int i=0; i < x.size(); i++ ){
	      xx_[ 0 ] = zz_[ 0 ] = x[ i ]; 
	      cout << setw(13) 
		   << xx_[ 0 ] * zz_[ 0 ] * f1hxx( xx_, zz_ ) / 2.0 << flush;
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
	    cout << setw(4)  << "Q^2" << setw(15) << "f1h(N=2, M=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    xf1hh.setQ2( q2[ iq2 ] );
	    
	    cout << setw(4) << q2[ iq2 ]
		 << setw(15) << "******" << flush;
	    
	    timer.start();
	    for( int i = 0; i < x.size(); i++ ){
	      cout << setw( 13 ) <<  xf1hh( x[ i ], x[ i ] ) / 2.0 << flush;
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
	  cout << setw(4)  << "Q^2" << setw(15) << "f1h(N=2, M=2)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 
	
	{
	  
	  f1hn.setQ2( q2[ iq2 ] );
	  
	  cout << setw(4) << q2[ iq2 ]
	       << setw(15) << "******" << flush;
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    dynamic_cast< IntegXsec::xzXsec* >( f1hn.xsecComp()->coeff() )
	      ->setparams( x[ i ], x[ i ] );
	    cout << setw( 13 ) 
		 <<  x[ i ] * x[ i ] * f1hxz( unitv, unitv ) / 2.0 
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
