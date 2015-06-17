#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>
#include <IntegXsec/XsecIntegrand.hh>
#include <FragXsec_T/FragXsec.hh>
#include <Xsec/xXsec.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "StrFunc.hh"
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

  int nx = 6;
  //  int nx = 8;
  double aa = 3.0;
  vector< double > x( 0 );
  for( double lx = - aa; lx < 0.0; lx += aa / nx )
    x.push_back( pow( 10.0, lx ) );
  
  double offset = args.get( "offset", 2.0  );  
  double angle  = args.get( "angle",  0.55  );  
  double length = args.get( "length", 15.0 );  
  
  try {
    
    Timer timer;

    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    // x-space STRFunc implementation
    int n_( 4 ), m_( ( args.hasOpt( "xShortInteg" ) ? 0 : 6 ) );
    double precision_( ( args.hasOpt( "xShortInteg" ) ? 1.0E-2 : 1.0E-4 ) );
    IntegFragXsec::StrFunc xcs( args, QCD::Flavor::HADRON( 1 ), 
				n_, m_, precision_ );
    // 4, 0, 1.0E-2 -> integ param ( not xPDF param!! )
    /*
    IntegFragXsec::StrFunc xcs( args, QCD::Flavor::HADRON( 1 ), 6, 8, 1.0E-2
				, 10.0, 2.0, 0.55 );
    */
    xcs.setGngrid( ( args.hasOpt( "xShortInteg" ) ? 12 : 2 ) );
    if( args.hasOpt( "logInteg" ) ) xcs.setloginteg( true );
    // x * cs ( \pi^{\pm} cs = 2 * cs )
    
    // Mellin-x-space STRFunc implementation
    FragXsec_T::FragXsec xsa( args, QCD::Flavor::HADRON( 1 ) );
    xsa.xsecComp( new IntegFragXsec::nXsecComp
		  ( args, QCD::Flavor::HADRON( 1 ) ) );
    xsa.xsecComp()->coeff()->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( xsa.xsecComp()->vevo()[0] )->constNf( false );
    if( args.hasOpt( "logInteg" ) ) 
      dynamic_cast< IntegXsec::xXsec* >( xsa.xsecComp()->coeff() )
	->setloginteg( true );
    dynamic_cast< IntegXsec::xXsec* >( xsa.xsecComp()->coeff() )
      ->setGngrid( args.get( "ngrid", 12 ) );

    Xsection::xXsec xxsa( &xsa, 4, 6, 1.0E-4 );           // MellinInversion
    xxsa.setParameter( offset, angle );
    xxsa.upper( length );
    // cs by xxsa( 1.0 ) ( \pi^{\pm} cs = 2 * cs )


    // Mellin-space STRFunc implementation
    FragXsec_T::FragXsec mxs( args, QCD::Flavor::HADRON( 1 ) );
    mxs.xsecComp()->coeff()->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( mxs.xsecComp()->vevo()[0] )->constNf( false );

    Xsection::xXsec xmxs( &mxs, 4, 5, 1.0E-4 );            // MellinInversion
    xmxs.setParameter( offset, angle );
    xmxs.upper( length );
    // cs

    if( 0 ) {
    } else {
      
      if( args.hasOpt( "wncalc" ) ){
	
	//start Mellin implementation
	cout << "Mellin space implementation" << endl;
	
	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  
	  if( iq2 == 0 ){
	    cout << setw(4)  << "Q^2" << setw(18) << "cs_(N=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    mxs.setQ2( q2[ iq2 ] );
	    double n = 2.0;
	    cout << setw(4) << q2[ iq2 ]
	    << setw(18) << mxs( complex< double >( n, 0.0 ) ) << flush;
	    
	    timer.start();
	    for( int i = 0; i < x.size(); i++ ){
	      cout << setw( 13 ) << 2.0 * xmxs( x[ i ] ) << flush;
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
	    cout << setw(4)  << "Q^2" << setw(18) << "cs_(N=2)" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    xcs.setQ2( q2[ iq2 ] );
	    double n = 2.0;
	    //	for( double n = 1.0; n < 10.0; n+=0.1 ){
	    cout << setw(4) << q2[ iq2 ]
		 << setw(18) << "******" << flush;
	    //	}
	    
	    timer.start();
	    for( int i = 0; i < x.size(); i++ ){
	      cout << setw( 13 ) <<  2.0 * xcs( x[ i ] ) / x[ i ] << flush;
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
	  cout << setw(4)  << "Q^2" << setw(18) << "cs_(N=2)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 
	
	{
	  
	  xsa.setQ2( q2[ iq2 ] );
	  double n = 2.0;
	  /*
	  //	  for ConIntegPDF 1/ y ( y )^n dy case ( direct comparison!! )
	  dynamic_cast< IntegXsec::xXsec* >( f1n.xsecComp()->coeff() )
	    ->setparams( 0.0 );
	  cout << setw(4) << q2[ iq2 ]
	       << setw(18) << f1n( complex< double >( n, 0.0 ) ) << flush;
	  */
	  //	  for ConIntegPDF 1/ y ( y / x )^n dy case 
	  cout << setw(4) << q2[ iq2 ]
	       << setw(18) << "******" << flush;
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    if( ! args.hasOpt( "zeroInteg" ) ){
	      dynamic_cast< IntegXsec::xXsec* >( xsa.xsecComp()->coeff() )
		->setparams( x[ i ] );
	    } else {
	      // should be equal to the above...
	      // this is available for ConIntegPDF 1/ y ( y )^n dy case 
	      dynamic_cast< IntegXsec::xXsec* >( xsa.xsecComp()->coeff() )
		->setparams( 0.0 );
	    }
	    //	  for ConIntegPDF 1/ y ( y / x )^n dy case 
	    cout << setw( 13 ) <<  2.0 * xxsa( 1.0 ) << flush;

	    /*
	    //	  for ConIntegPDF 1/ y ( y )^n dy case 
	    cout << setw( 13 ) <<  2.0 * xxsa( x[ i ] )
		 << flush;
	    */
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
