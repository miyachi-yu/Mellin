// $Id: TrapInt.cc,v 1.4 2008/07/24 21:28:20 miyachi Exp $
/*!
  \file     TrapInt.cc
  \date     $Date: 2008/07/24 21:28:20 $
  \version  $Revision: 1.4 $
  \author   $Author: miyachi $
  \brief    Implementation of Trapezoid approximation integration class

  $Log: TrapInt.cc,v $
  Revision 1.4  2008/07/24 21:28:20  miyachi
  Double inversion is implemented in also InverseMellin, in bit different way. Acoordingly, the integration classes were modified...

  Revision 1.3  2008/05/23 02:08:02  miyachi
  Huge update. Now each library has own namespace. One has to explcitly specify the namespace.

  Revision 1.2  2008-05-09 06:51:34  miyachi
  Does it help the infinit loop?

  Revision 1.1  2007/04/19 10:01:23  miyachi
  First release of libTransform.so

  Revision 1.2  2007/01/10 02:16:46  miyachi
  The majour differences are:

  - Modify the algorythm of precision checking used in TrapInt class.
    Now it check the difference between trapezoid approximation and
    simple mid-point integration.

  - Add comments for doxygen. Including some CVS tag in the file comment.

 */
#include "TrapInt.hh"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace Transform;


TrapInt::TrapInt( double precision ) :
  precision_( precision )
{
}

TrapInt::~TrapInt(){
}

double TrapInt::integral( const double& min, const double& max, 
			  RealFunction& f ) {
  return this->integral_( min, max, 2, f );
}

double TrapInt::simpleIntegral_( double a, double b, int n, RealFunction& f ) {
  if( n < 1 ) return 0.0;
  double dstep = ( b - a )/n;
  double rval( 0.0 );
  double oldvalue(0.0);
  for( int i=0; i<n; i++ ){
    if( i == 0 ) oldvalue = f( a );
    double newvalue = f( a + (i+1)* dstep );
    rval += 0.5 * ( oldvalue + newvalue ) * dstep ;
    oldvalue = newvalue;
  }
  return rval;
}

double TrapInt::midPoint_( double a, double b, int n, RealFunction& f ) {
  if( n < 1 ) return 0.0;
  double dstep = ( b - a )/n;
  double rval( 0.0 );
  for( int i=0; i<n; i++ ) rval += f( a + 0.5 * (i+1) * dstep ) * dstep ;
  return rval;
}

double TrapInt::integral_( double a, double b, int n, RealFunction& f )  {
  static int depth_(1);
  if( n < 1 ) return 0.0;
  if( a == b ) return 0.0;
  
  // calculate the integral with trapezoid approximation
  double trap = this->simpleIntegral_( a, b, n, f );
  
  // calculate the integral with the mid point
  double mid  = this->midPoint_( a, b, n, f );
  
  // check if the difference of these two methods full fills 
  // the required precision
  if( fabs( trap - mid ) < 1E-30 ||
      fabs( trap - mid ) <= fabs( precision_ * ( trap + mid ) / 2.0 ) )
    return trap;
  
  // divide the subregion into two more sub-sub-region, and
  // repeat the integral
  depth_++;
  double dstep = ( b - a )/n;
  double rval( 0.0 );
  for( int i=0; i<n; i++ ) 
    rval += this->integral_( a + i*dstep, a + (i+1)*dstep, 2, f );
  
  depth_--;
  return rval;
}

complex< double > TrapInt::cintegral( const double& min, const double& max, 
				      ComplexFunction& f ) {
  return this->cintegral_( min, max, 2, f );
}

complex< double > TrapInt::cintegral_( double a, double b, int n, 
				       ComplexFunction& f ) {

  static int depth_(1);
  if( n < 1 ) return 0.0;
  if( a == b ) return complex< double >( 0.0, 0.0 );

  //  for( int i = 0; i < depth_; i++ ) cout << "*" << flush; cout << endl;
  // calculate the integral with trapezoid approximation
  complex< double > trap = this->csimpleIntegral_( a, b, n, f );
  
  // calculate the integral with the mid point
  complex< double > mid  = this->cmidPoint_( a, b, n, f );

  // check if the difference of these two methods full fills 
  // the required precision
  double limit = precision_ * ( abs( trap ) + abs( mid ) ) / 2.0 ;
  double diff  = abs( trap - mid );
  
  //cout << "DIFF: " << diff << "\tLIMIT: " << limit << endl;
  if( limit < 1.0E-15  ) return trap;
  if( diff <= limit ) return trap;
  
  
  // divide the subregion into two more sub-sub-region, and
  // repeat the integral
  depth_++;
  double dstep = ( b - a )/n;
  complex< double > rval( 0.0, 0.0 );
  for( int i=0; i<n; i++ ) 
    rval += this->cintegral_( a + i*dstep, a + (i+1)*dstep, 2, f );
  
  depth_--;
  return rval;
}

complex< double > TrapInt::csimpleIntegral_( double a, double b, int n, 
					     ComplexFunction& f ){
  if( n < 1 ) return 0.0;
  double dstep = ( b - a )/n;
  complex< double > rval( 0.0, 0.0 );
  complex< double > oldvalue( 0.0, 0.0 );
  for( int i=0; i<n; i++ ){
    if( i == 0 ) oldvalue = f( a );
    complex< double > newvalue = f( a + (i+1)* dstep );
    rval += 0.5 * ( oldvalue + newvalue ) * dstep ;
    oldvalue = newvalue;
  }
  return rval;
}

complex< double > TrapInt::cmidPoint_( double a, double b, int n, 
				       ComplexFunction& f ) {
  if( n < 1 ) return 0.0;
  double dstep = ( b - a )/n;
  complex< double > rval( 0.0, 0.0 );
  for( int i=0; i<n; i++ ) rval += f( a + 0.5 * (i+1) * dstep ) * dstep ;
  return rval;
}
