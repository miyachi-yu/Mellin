#include <Utility/Arguments.hh>
#include "ConfluentHG.hh"
#include "ConfluentHG2.hh"
#include "ErrorFunction.hh"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  // test 0
  // this should correspond 
  // ( 2 e^x ( x - 1.0 ) + 2 ) / x^2
  // see 
  // http://demonstrations.wolfram.com/ConfluentHypergeometricFunctions/
  
  if( 1 ){
    
    double a = 2.0;
    double b = 3.0;
    double upper = args.get( "upper", 100.0 );

    Transform::ConfluentHG hg( a, b );
    Transform::ConfluentHG2 conf2( a, b );
    conf2.upper( upper );
    
    ostringstream ostr;
    ostr << "("<< a << ", " << b << ")";
    
    cout 
      << setw(10) << "x"
      << setw(18) << ( "1F1" + ostr.str() )
      << setw(12) << "f"
      << setw(18) << "1F1/f = 1.0"
      << setw(18) << ( "U" + ostr.str() )
      << endl;
    
    for( double x = -5.0; x < 5.0; x += 0.25 ){
      
      double f =
	( x != 0.0 ? 
	  ( 2.0 * exp( x ) * ( x - 1.0 ) + 2.0 ) / x / x : 
	  1.0 );
      
      cout 
	<< setw(10) << x
	<< setw(18) << hg( x )
	<< setw(12) << f
	<< setw(18) << hg( x ) / f
	<< setw(18) << conf2( x )
	<< endl;
    }
    
  }

  if( 0 ) {     // test 1

    double a = -1.0, b = 0.5;
    
    Transform::ConfluentHG hg1(     a, b );
    Transform::ConfluentHG hg2( b - a, b );
    
    cout
      << setw(10) << "x"
      << setw(18) << "F1=F(a;b;-x)"
      << setw(18) << "F2=F(b-a;b;x)"
      << setw(18) << "F2/F1"
      << setw(18) << "exp(x)"
      << setw(18) << "F2/F1/exp(x)=1.0"
      << endl;
    
    for( double x = 0.1; x < 5.0; x += 0.1 ){
      
      cout << setw( 10 ) << x  << flush;
      
      complex< double > h1;
      complex< double > h2;
      complex< double > r;
      complex< double > e;
      
      cout << setw(18) << ( h1 = hg1( -x ) ) << flush;
      cout << setw(18) << ( h2 = hg2(  x ) ) << flush;
      cout << setw(18) << ( r  = ( h1 != 0.0 ? h2 / h1 : 0.0 ) )  << flush;
      cout << setw(18) << ( e  = exp( x ) ) << flush;
      cout << setw(18) << r / e  << flush;
      cout << endl;
    }

  }


  if( 1 ){  // ------------------ test 2 --------------------
    Transform::ErrorFunction erf;
    Transform::ConfluentHG    f1( 0.5, 1.5 ); 
    Transform::ConfluentHG    f2( 1.0, 1.5 ); 
    
    for( double x = -2.0; x < 2.0; x += 0.1 ){
      
      double x2 = x * x;
      static double c = 2.0 / sqrt( M_PI );
      cout 
	<< setw(10) << x
	<< setw(18) << erf( x )
	<< setw(18) << c * x * f1( - x2 )
	<< setw(18) << c * x * exp( - x2 ) * f2( x2 )
	<< endl;
      
    }  
  }
  
  return 0;
};
