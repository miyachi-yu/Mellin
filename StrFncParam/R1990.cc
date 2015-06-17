// $Id: R1990.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    R1990.cc
  \brief   Class implementation of R1990 parameterization
 */
#include "R1990.hh"

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace StrFncParam;

R1990::R1990() :
  lm2( 0.04 ),     // ( 0.2 GeV )^2
  b( 3, 0.0 )
{
  /*
    The values can be found in Phys. Lett. B250 (1990) 193.
    Some attention must be paid when one check the KEK scanned 
    version. The value of b1 is given as 0.635!!
   */
  b[0] =  0.0635; 
  b[1] =  0.5747;
  b[2] = -0.3534;
}

R1990::~R1990(){
}

double R1990::eval( const double& x ){
  double r = b[0] * this->theta( x ) / log( q2_ / lm2 );
  r += b[1] / q2_;
  r += b[2] / ( q2_ * q2_ + 0.09 );
  return r;
}

double R1990::evalError( const double& x ){
  return 0.0;
}

double R1990::operator()( const double& x ){
  return this->eval( x );
}

double R1990::operator()( const double& x, const double& q2 )
{
  q2_ = q2;
  return (*this)( x );
}

double R1990::error( const double& x ){
  return this->evalError( x );
}

double R1990::error( const double& x, const double& q2 )
{
  q2_ = q2;
  return this->error( x );
}

double R1990::theta( const double& x ){
  static double a = 0.125 * 0.125;
  return 1.0 + 12.0 * a * q2_ / ( q2_ + 1.0 ) / ( a + x * x );
}
