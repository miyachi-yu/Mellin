#include "ErrorFunction.hh"

#include <cmath>

using namespace std;
using namespace Transform;

double ErrorFunction::Integrand::operator()( const double& x ){
  return exp( -x * x );
}

ErrorFunction::ErrorFunction() :
  IntegFunc() {
}

double ErrorFunction::operator()( const double& z ){
  static double c = 2.0 / sqrt( M_PI ) ;
  return c * this->integration()->integral( 0.0, z, integrand_ );
}
