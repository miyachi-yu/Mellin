/*!
  \file  testF1.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libunpolDISXsec.so.

  You may get some feeling how to use polDISXsec::StructureFunction class.

*/
#include <Utility/Timer.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <unpolDIS/StrFunc.hh>

#include <iostream>
#include <iomanip>
#include <map>

#include "StrFunc.hh"
#include "xXsec.hh"

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Utility;

map< double, vector< double > > cmap_;

void loadSteepestContour( Xsection::xXsec* f_, vector< double >& x_ );

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  Transform::LaguerreQuadrature laguer( args.get( "nlag", 5 ), -0.5 );

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
    
    double s = args.get( "s", 56.2 );

    //    int nx = 6;
    int nx = 8;
    //    double aa = 1.0;
    double aa = 2.0;
    vector< double > x( 0 );
    for( double lx = - aa; lx < 0.0; lx += aa / nx )
      x.push_back( pow( 10.0, lx ) );
    
    double offset; double angle; double length;
    if( !args.hasOpt( "CTEQ6" ) ){
      offset = args.get( "offset", 2.0  );  
      angle  = args.get( "angle",  0.55  );  
      length = args.get( "length", 10.0 );  
    } else {
      offset = args.get( "offset", 2.0  );  
      angle  = args.get( "angle",  0.50 );  
      length = args.get( "length", 20.0 );  
    }

    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    unpolDISXsec::StrFunc f1( args, Xsection::Charge::p );
    f1.setS( s );
    if( args.hasOpt( "Dcalc" ) ) f1.setDirect( true );

    unpolDISXsec::xXsec* f1x;

    if( args.hasOpt( "laguerre" ) ){
      f1x = new unpolDISXsec::xXsec( &f1, &laguer );
      f1.setQ2( args.get( "LQ2", 100.0 ) ); 
      // temporary set q2 for laguerre //
      loadSteepestContour( f1x, x );
    }else{
      f1x = new unpolDISXsec::xXsec( &f1, 4, 6, 1.0E-4 );
      f1x->setGngrid( 4 );

      double offset_( offset );
      double phi_( angle );
      if( !args.hasOpt( "Dcalc" ) ) offset_ = 3.5;
      //      if( !args.hasOpt( "Dcalc" ) ) phi_ = 0.75;
      f1x->setParameter( offset_, phi_ );
      f1x->upper( length ); 
    }


    //      cout << f1x->contour() << endl;

    //    complex< double > n( 3.0, 0.0 );
    complex< double > n( 4.0, 0.0 );
    if( args.hasOpt( "Dcalc" ) ) n = complex< double >( 2.0, 0.0 ); 

    // for xmoment calc
    std::vector< double > mx_( 2, 0.0 ); 
    double dx = args.get( "xmomR", 0.001 );

    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){

      if( iq2 == 0.0 ){
        cout << setw(4)  << "Q^2" 
             << setw(14) << "g_1h ( N=" << n.real() << " )" ;
        for( int i = 0; i < x.size(); i++ ) {
          cout << setw(13) << x[ i ] << flush;
        }
        cout << endl;
      }
      
      f1.setQ2( q2[ iq2 ] );
      
      cout << setw(4) << f1.q2()
	   << setw(17) << f1( n ) << flush;
      
      timer.start();

      for( int i=0; i < x.size(); i++ ){
	// for xmoment calc
	if( args.hasOpt( "xmoment" )  || args.hasOpt( "mom" ) ){
	  mx_[ 1 ] = x[ i ] * ( 1.0 + dx );
	  mx_[ 0 ] = x[ i ] * ( 1.0 - dx );
	  //	  mx_[ 1 ] = x[ i ] + dx;
	  //	  mx_[ 0 ] = x[ i ] - dx;
	}
	// Lets set the contour parameter
	if( args.hasOpt( "laguerre" ) ){
	  f1x->steepestDescent( cmap_[ x[ i ] ][ 0 ], cmap_[ x[ i ] ][ 1 ],
				cmap_[ x[ i ] ][ 2 ] );
	}
	cout << setw(13) << 
	  ( args.hasOpt( "xmoment" ) || args.hasOpt( "mom" ) ?
	    (*f1x)( mx_ ) : (*f1x)( x[ i ] ) ) << flush;
      }
      timer.stop();
      
      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << endl;      
    }
    
  }

  catch ( int error ) {
    if( error == 10 ) cerr << "second parameter must be negative"
			   << endl;
  }
  
  return 0;
}


void loadSteepestContour( Xsection::xXsec* f_, vector< double >& x_ ){

  map< double, vector< double > >& cm = cmap_;
  
  for( int i = 0; i < x_.size(); i ++ ){

    double xx_ = x_[ i ];
    cm[ xx_ ] = vector< double >( 3, 0.0 );

    f_->F().x( xx_ );
    f_->F().setmin( 6.0 );
    cm[ xx_ ][ 0 ] = f_->F().c0();
    cm[ xx_ ][ 1 ] = f_->F().c1( cm[ xx_ ][ 0 ] );
    cm[ xx_ ][ 2 ] = f_->F().c2( cm[ xx_ ][ 0 ] );

    cout << cm[ xx_ ][ 0 ] << " "
	 << cm[ xx_ ][ 1 ] << " "
	 << cm[ xx_ ][ 2 ] << " "
	 << xx_ << endl;

    // second parameter must be negative, otherwise .....
    if( cm[ xx_ ][ 1 ] > 0.0 ) throw( 10 );
    
  }

}
