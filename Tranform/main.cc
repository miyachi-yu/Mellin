// $Id: main.cc,v 1.5 2008/05/23 02:08:03 miyachi Exp $
/*!
  \file    main.cc
  \version $Revision: 1.5 $
  \date    $Date: 2008/05/23 02:08:03 $
  \author  $Author: miyachi $
  \brief   test program for libTransform.so

  This is an example for libTransform.so library.


  One must set enviroment variable LD_LIBRARY_PATH to include the
  directory which contains libTransform.so.


 */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "Mellin.hh"
#include "CBeta.hh"
#include "Trapezoid.hh"
#include "Rectangle.hh"
#include "LegQuadrature.hh"
#include "GridIntegration.hh"
#include "CPolyGamma.hh"

using namespace std;
using namespace Transform;

/*!
  \class Fa
  \brief f(x) = x^alpha * ( 1 - x )^beta

  This is a test RealFunction chaild class.  The function
  f(x) = x^alpha * ( 1 - x )^beta
  is Mellin transformed to Beta( beta + 1, alpha + z ) function,
  which is implemented as CBeta.

 */
class Fa : public RealFunction {
public:
  Fa( double a, double b ) : a_( a ), b_( b ) {};
  virtual ~Fa() {}
  
  virtual double operator() ( const double& x ) {
    return ( x <= 0.0 || x >= 1.0 ? 0.0 : pow( x, a_ ) * pow( 1 - x, b_ ) );
  }
  
private:
  double a_;
  double b_;
};

int main( int argc, char* argv[] ){
  
  if( argc != 3 ) {
    cerr << "USAGE: " << argv[0] << " alpha beta" << endl;
    return -1;
  }
  
  double alpha = atof( argv[1] ), beta = atof( argv[2] );
  
  Fa f(   alpha, beta );   // x^alpha * ( 1 - x )^beta
  CBeta b( alpha, beta );  // Beta( beta + 1, z + alpha )
  
  // crates two integration methods to achieve the required 
  // precision....
  
  //  Trapezoid meth1;
  //  Rectangle meth2;
  LegQuadrature meth1(4);  // Gauss Legendre with 4 inter points
  LegQuadrature meth2(6);  // Gauss Legendre with 6 inter points
  
  // Create integration method using the previously created methods.
  GridIntegration grid( &meth1, &meth2, 4, 0.001 );
  
  // Mellin conversion of the real function f with the integration method
  Mellin M( &grid );
  M.upper( 1.0 );
  
  ComplexFunction& g = M.transform( &f ); 
    
  // set upper integration edge for x^alpha (1-x)^beta function
  
  // calculate the relative difference in abs( ).....
  // it may be interesting to plot phase difference...
  for ( double i = -10.0; i < 10.0; i += 0.5 ){
    cout << setw( 4 ) << i ;
    for ( double r = 1.0; r < 10.0; r += 0.5 ){
      complex< double > z( r, i );
      cout << setw( 8 ) << setprecision(2) 
	   << abs( g( z ) - b( z )) / abs( b( z ) ) << flush; 
    }
    cout << endl;
  }
  
  // test for PolyGamma function
  CPolyGamma polyg( 1 );
  cout << polyg( complex< double >( 10.8, 0.0 ) ) << endl;
  
  return 0;
}
