#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <IntegXsec/XsecIntegrand.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/XsecComp.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "StrFunc.hh"
#include "nStrFunc.hh"

using namespace Utility;
using namespace std;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  double rootS0_ = 40; 
  int ratio = 1;
  int ratio2 = ratio * ratio;
  double rootS_ = rootS0_ * ratio; 
  double S_ = rootS_ * rootS_;

  vector< double > q2(0);
  q2.push_back(   1.0 * ratio2 );
  q2.push_back(   2.0 * ratio2 );
  q2.push_back(   5.0 * ratio2 );
  q2.push_back(  10.0 * ratio2 );
  q2.push_back(  20.0 * ratio2 );
  q2.push_back(  50.0 * ratio2 );
  q2.push_back( 100.0 * ratio2 );
  q2.push_back( 200.0 * ratio2 );
  q2.push_back( 500.0 * ratio2 );
  
  /*
  int nx = 8;
  double aa = 2.0;
  vector< double > x( 0 );
  for( double lx = - aa; lx < 0.0; lx += aa / nx ) //if( x.size() <= 6 )
    x.push_back( pow( 10.0, lx ) );
  */

  int nx = 8;
  double aa = 0.5;
  vector< double > x( 0 );
  for( double lx = - aa; lx <= aa; lx += 2.0 * aa / nx ) //if( x.size() <= 6 )
    x.push_back( lx );


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
    IntegUnpolDrellYan::StrFunc dcs( args, IntegXsec::Charge::p, 
				     QCD::Flavor::PIp, n_, m_, precision_,
				     10.0, 2.0, 0.55 );
    dcs.setGngrid( ( args.hasOpt( "xShortInteg" ) ? 12 : 2 ) ); 
    if( args.hasOpt( "logInteg" ) )  dcs.setloginteg( true ); 
    // x01 * x02 * s * d sig / d sqrt( tau ) d y //

    // Mellin-x-space STRFunc implementation
    IntegUnpolDrellYan::nStrFunc dcsnx( args, IntegXsec::Charge::p,
					QCD::Flavor::PIp );
    dcsnx.xsecComp()->coeff()->constNf( false );
    dynamic_cast< unpolPDF::Evo* >
      ( dcsnx.xsecComp()->vevo()[ 0 ] )->constNf( false );
    dynamic_cast< unpolPDF::Evo* >
      ( dcsnx.xsecComp()->vevo()[ 1 ] )->constNf( false );

    if( args.hasOpt( "logInteg" ) )
      dynamic_cast< IntegXsec::xzXsec* >( dcsnx.xsecComp()->coeff() )
	->setloginteg( true );
    dynamic_cast< IntegXsec::xzXsec* >( dcsnx.xsecComp()->coeff() )
      ->setGngrid( args.get( "ngrid", 12 ) );

    Xsection::xXsec xdcsnx( &dcsnx, 4, 6, 1.0E-4 );        // MellinInversion
    /* // xzXsec makes the symmetry in p<->p exchange worse //
    Xsection::xzXsec xdcsnx( &dcsnx, 4, 6, 1.0E-4 );        // MellinInversion
    Xsection::xzXsec xdcsnx( &dcsnx, 4, 6, 1.0E-6 );        // MellinInversion
    */
    xdcsnx.setParameter( voffset, vangle );
    xdcsnx.upper( vlength );

    vector< double > mom( 2, 0.0 );
    xdcsnx.setMomParam( mom );    
    // s * d sig / d sqrt( tau ) d y //

    if( 0 ) {
    } else {

      if( args.hasOpt( "wxcalc" ) ){
	//start x-space implementation
	cout << "x-space implementation" << endl;
	
	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  if( iq2 == 0 ){
	    cout << setw(4)  << "sqrt(T)" << setw(18) 
		 << "dsig/dsqrt(T)dy" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	  } 
	  
	  {
	    
	    dcs.setQ2( q2[ iq2 ] );
	    //	  cout << setw(4) << q2[ iq2 ] // q2
	    cout << setw(4) << sqrt( q2[ iq2 ] / S_ ) // sqrt( tau )
	      //	  cout << setw(4) << sqrt( q2[ iq2 ] ) // q
		 << setw(18) << "******" << flush;
	    
	    timer.start();
	    for( int i = 0; i < x.size(); i++ ){
	      double tau = q2[ iq2 ] / S_;
	      double x01 = sqrt( tau ) * exp( x[ i ] );
	      double x02 = sqrt( tau ) * exp( - x[ i ] );
	      // s * d sig / d sqrt( tau ) d y //
	      cout << setw( 13 ) <<  2 * sqrt( tau ) * S_ * dcs( x01, x02 ) 
		/  x01 / x02
		   << flush;
	      /*
	      // d sig / d tau d y //
	      cout << setw( 13 ) <<  
	      dcs( x01, x02 ) /  x02 / x01 
	      << flush;
	      */
	      /*
	      // d sig / d M d y //
	      cout << setw( 13 ) << 
	      2 * sqrt( q2[ iq2] ) / S_ * dcs( x01, x02 ) /  x02 / x01
	      << flush;
	      */
	      /*
	      // M^3 * d sig / d M d y //
	      cout << setw( 13 ) << 
	      pow( sqrt( q2[ iq2] ), 3 ) * 
	      2 * sqrt( q2[ iq2] ) / S_ * dcs( x01, x02 ) /  x02 / x01
	      << flush;
	      */
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
	    cout << setw(4)  << "sqrt(T)" << setw(18) 
		 << "dsig/dsqrt(T)dy" << flush;
	    for( int i = 0; i < x.size(); i++ ) 
	      cout << setw( 13 ) << x[ i ] << flush;
	    cout << endl;
	}

	{

	  dcsnx.setQ2( q2[ iq2 ] );
	  //	  cout << setw(4) << q2[ iq2 ] // q2
	  cout << setw(4) << sqrt( q2[ iq2 ] / S_ ) // sqrt( tau )
	    //	  cout << setw(4) << sqrt( q2[ iq2 ] ) // q
	       << setw(18) << "******" << flush;

	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    double tau = q2[ iq2 ] / S_;
	    double x01 = sqrt( tau ) * exp( x[ i ] );
	    double x02 = sqrt( tau ) * exp( - x[ i ] );
	    // s * d sig / d sqrt( tau ) d y //
	    if( ! args.hasOpt( "ReV" ) ){
	      dynamic_cast< IntegXsec::xzXsec* >
		( dcsnx.xsecComp()->coeff() )->setparams( x01, x02 );
	    } else {
	      dynamic_cast< IntegXsec::xzXsec* >
		( dcsnx.xsecComp()->coeff() )->setparams( x02, x01 );
	    }
	    cout << setw( 13 ) 
		 << 2 * sqrt( tau ) * S_ * xdcsnx( unitv, unitv ) 
		 << flush;
	  }
	  timer.stop();
	  cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;
          cout << setw( 13 ) << dynamic_cast< IntegXsec::xzXsec* >
            ( dcsnx.xsecComp()->coeff() )->getncount() << " calc" << flush;
	  
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
