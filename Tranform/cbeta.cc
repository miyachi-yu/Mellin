#include "CBeta.hh"
#include "ComplexBeta.hh"
#include "IntegFunc.hh"
#include "InverseMellin.hh"
#include "Romberg.hh"

#include <Utility/Arguments.hh>

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  IntegFunc integ( args.get( "leg1", 4 ), args.get( "leg2", 6 ),
		   args.get( "ngrid", 2 ), args.get( "precision", 5.0E-3 ) );

  Romberg romber( args.get( "K", 10 ), args.get( "eps", 1.0E-6 ) );
  
  double alpha = args.get( "alpha", 1.0 );
  double beta  = args.get( "beta",  1.0 );
  
  CBeta       cb1( alpha, beta );
  ComplexBeta cb2( alpha, beta );
  
  InverseMellin b1( &cb1, integ.integration() );
  InverseMellin b2( &cb2, integ.integration() );
  InverseMellin b3( &cb1, &romber );
  
  double offset = args.get( "offset", 1.2 );
  double angle  = args.get( "angle",  0.55 );
  double length = args.get( "length", 15.0 );
  
  b1.setParameter( offset, angle );
  b2.setParameter( offset, angle );
  b3.setParameter( offset, angle );

  b1.upper( length );
  b2.upper( length );
  b3.upper( length );

  if( 1 ){
    
    vector< complex< double > > lz;
    complex< double > c0( offset, 0.0 );
    complex< double > e = 
      complex< double >( cos( angle * M_PI ), sin( angle * M_PI ) );
    
    for( double l = 0.0; l < length; l += length / 10.0 )
      lz.push_back( c0 + l * e );

    cout 
      << setw(32) << "z"
      << setw(32) << "Beta1(z)"
      << setw(32) << "Beta2(z)"
      << endl;
    
    for( int iz = 0; iz < lz.size(); iz++ ){
      cout 
	<< setw(32) << lz[ iz ]  << flush
	<< setw(32) << cb1( lz[ iz ] ) << flush
	<< setw(32) << cb2( lz[ iz ] ) << flush
	<< endl;
    }
    
  }

  if( 1 )
    {
      
      cout << setw( 16 ) << "x"          << flush
	   << setw( 16 ) << "cbeta(a,b)" << flush
	   << setw( 16 ) << "M^{-1}:1" << flush
	   << setw( 16 ) << "M^{-1}:2" << flush
	   << endl;
	
      for( double lx = -2; lx < 0.0; lx += 0.1 ){
	
	double x = pow( 10.0, lx );
	
	cout << setw( 16 ) << x   << flush ;
	cout << setw( 16 ) << cb1.x( x ) << flush;
	cout << setw( 16 ) << b1( x ) << flush;
	//	cout << setw( 16 ) << b2( x ) << flush;
      	cout << setw( 16 ) << b3( x ) << flush;
	cout << endl;
      }
    }
  return 0;
}
