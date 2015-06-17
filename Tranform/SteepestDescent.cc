// $Id: SteepestDescent.cc,v 1.2 2008/07/17 10:09:21 miyachi Exp $
/*!
  \file   SteepestDescent.cc
  \brief  Class implementation of a steepest descent contour in Mellin inversion
 */
#include "SteepestDescent.hh"

#include <cmath>

using namespace std;
using namespace Transform;

SteepestDescent::SteepestDescent() :
  c0_( 1.5 ), c1_( -1.0 ) {
}

SteepestDescent::~SteepestDescent() {
}

complex< double > SteepestDescent::operator()( const double& t ){
  return complex< double >( c0_ + c1_ * t * t, t );
}

complex< double > SteepestDescent::differential( const double& t ){
  return complex< double >( 2.0 * c1_ * t, 1.0 );
}
