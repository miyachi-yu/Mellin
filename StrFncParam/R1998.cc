// $Id: R1998.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    R1990.cc
  \brief   Class implementation of R1998 parameterization
 */
#include "R1998.hh"
#include <cmath>

using namespace std;
using namespace StrFncParam;

R1998::R1998() : 
  R1990(),
  a( 6, 0.0 ), c( 6, 0.0 )
{
  b.resize( 6, 0.0 );

  a[ 0 ] =  0.0485;
  a[ 1 ] =  0.5470;
  a[ 2 ] =  2.0621;
  a[ 3 ] = -0.3804;
  a[ 4 ] =  0.5090;
  a[ 5 ] = -0.0285;

  b[ 0 ] =  0.0481;
  b[ 1 ] =  0.6114;
  b[ 2 ] = -0.3509;
  b[ 3 ] = -0.4611;
  b[ 4 ] =  0.7172;
  b[ 5 ] = -0.0317;
  
  c[ 0 ] =  0.0577;
  c[ 1 ] =  0.4644;
  c[ 2 ] =  1.8288;
  c[ 3 ] = 12.3708;
  c[ 4 ] =-43.1043;
  c[ 5 ] = 41.7415;
  
}

R1998::~R1998() {
}

double R1998::eval( const double& x ){
  th = this->theta( x ) / log( q2_ / lm2 );
  return ( this->Ra(x) + this->Rb(x) + this->Rc(x) ) / 3.0 ;
}

double R1998::evalError( const double& x ){
  return 
    0.0078 - 0.013 * x +
    ( 0.070 - 0.39 * x + 0.70 * x * x ) / ( 1.7 + q2_ );
}

double R1998::Ra( const double& x ) {
  return 
    a[0] * th +
    a[1] *
    pow( pow( q2_, 4 ) + pow( a[2], 4 ), -0.25 ) *
    ( 1.0 + a[3] * x + a[4] * x * x ) * 
    pow( x, a[5] ) ;
}

double R1998::Rb( const double& x ) {
  return 
    b[0] * th +
    ( b[1]/q2_ + b[2] / ( q2_ * q2_ + 0.09 ) ) *
    ( 1.0 + b[3] * x + b[4] * x * x ) *
    pow( x, b[5] ) ;
}

double R1998::Rc( const double& x ) {
  double qth = c[3] + c[4] * x * x + c[ 5 ] * x * x * x;
  return 
    c[0] * th +
    c[1] / sqrt( pow( q2_ - qth, 2 ) + c[2] * c[2] );
}
