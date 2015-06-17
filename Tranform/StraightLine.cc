// $Id: StraightLine.cc,v 1.4 2008/10/17 09:59:32 miyachi Exp $
/*!
  \brief  class implementation of straight line contour in Mellin inversion
 */
#include "StraightLine.hh"

#include <cmath>

using namespace std;
using namespace Transform;

StraightLine::StraightLine() :
  c0_( 2.0, 0.0 ), e_( cos( 0.5 * M_PI ), sin( 0.5 * M_PI ) ) {
}

StraightLine::~StraightLine() {
}

void StraightLine::set( const double& c0, const double& phi ){
  c0_ = complex< double >( c0, 0.0 );
  e_  = complex< double >( cos( phi * M_PI ), sin( phi * M_PI ) );
  ce_ = conj( e_ );
}

void StraightLine::set( const double& c0 ){
  c0_ = complex< double >( c0, 0.0 );
}

complex< double > StraightLine::operator()( const double& t ) {
  return c0_ + ( t > 0 ? t * e_ : fabs( t ) * ce_ );
}

complex< double > StraightLine::differential( const double& t ) {
  return ( t > 0 ? e_ : ce_ );
}

double StraightLine::angle(){
  return arg( e_ ) / M_PI;
}
