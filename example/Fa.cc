#include "Fa.hh"

#include <cmath>

Fa::Fa( double a, double b ) : a_( a ), b_( b ) {
}

Fa::~Fa(){
}

double Fa::operator() ( const double& x ){
  return ( x <= 0.0 || x >= 1.0 ? 0.0 : pow( x, a_ ) * pow( 1 - x, b_ ) );
}
