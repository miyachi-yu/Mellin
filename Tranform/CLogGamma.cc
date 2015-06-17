// $Id: CLogGamma.cc,v 1.4 2008/07/10 12:39:43 miyachi Exp $
/*!
  \file    CLogGamma.cc
  \version $Revision: 1.4 $
  \author  $Author: miyachi $
  \brief   Complex Log Gamma Function Class (Implementation)
*/
#include "CLogGamma.hh"

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

double CLogGamma::a[ 10 ] = {
  8.333333333333333E-02,  -2.777777777777778E-03,
  7.936507936507937E-04,  -5.952380952380952E-04,
  8.417508417508418E-04,  -1.917526917526918E-03,
  6.410256410256410E-03,  -2.955065359477124E-02,
  1.796443723688307E-01,  -1.39243221690590E+00
};


CLogGamma::~CLogGamma() {
}

std::complex< double > 
CLogGamma::operator()( const std::complex< double >& z ) {
  
  //  z.real() and z.imag() are inline functions
  //  so that one does not need to prepare extra variables
  //  to speed up the program
  
  // if the given z is a pole location, return large value
  if( z.real() <= 0.0 && 
      z == std::complex< double >( (int) z.real(), 0.0 ) ) 
    return std::complex< double >( 1.0E+300, 0.0 );
  
  // if the given z is the left hand of std::complex plane,
  if( z.real() < 0.0 ) {
    
    complex< double > zp = -1.0 * z;
    complex< double > g  = (*this)( zp );
    
    complex< double > 
      s( - sin( M_PI * zp.real() ) * cosh( M_PI * zp.imag() ),
	 - cos( M_PI * zp.real() ) * sinh( M_PI * zp.imag() ) );
    
    complex< double > 
      cg( log( M_PI / ( abs( zp ) * abs( s ) ) ), 
	  - arg( zp ) - ( arg( s ) + ( arg( s )< 0.0 ? 2.0 * M_PI : 0.0 ) ) );
    
    return cg - g;
  }
  
  // if the given std::complex stays in 0 < Re(z) <= 6.0,
  if( z.real() <= 6.0 ){
    
    int na = (int)( 7.0 - z.real() );
    
    // calculate the value at  Re(z) about 7
    std::complex< double > g = 
      (*this)( std::complex< double >( z.real() + na, z.imag() ) );
    
    // then apply the correction
    for( int j = 0; j < na; j++ ){
      double xj = z.real() + j;
      g -= std::complex< double >( 0.5 * log( xj * xj + z.imag() * z.imag() ),
				   atan( z.imag() / xj ) );
    }
    return g;
  }
  
  // if not, using the formula.
  double z1  = abs( z ); 
  double th  = arg( z );
  
  static double log2pi2 = 0.5 * log( 2.0 * M_PI );
  double lz1 = log( z1 );
  double zr5 = z.real() - 0.5;
  
  std::complex< double > 
    g( zr5 * lz1 - th * z.imag() - z.real() + log2pi2,
       th * zr5 + z.imag() * lz1 - z.imag() );
  
  //  std::cout << g << std::endl;
  
  double t  = 1.0 / z1;
  double t2 = t * t;
  for( int k = 0; k < 10; k++ ){
    //    std::cout << "t: " << t << std::endl;
    g +=
      std::complex< double >( a[k] * t * cos( ( 2.0 * k + 1.0 ) * th ),
			      -1.0 * a[k] * t * sin( ( 2.0 * k + 1.0 ) * th ) );
    t *= t2;
  }
  
  //  std::cout << g << std::endl;
  
  return g;
}  
