// $Id: Pvalue.cc,v 1.6 2010/03/06 10:13:12 miyachi Exp $
#include "Pvalue.hh"

#include <iomanip>

using namespace Transform;
using namespace std;

// ---------------------------------------------------------------------
Pvalue::Integrant::Integrant( const int& k ) :
  RealFunction(),
  k_( 0 ), odd_( true )
{
  this->k( k );
}

Pvalue::Integrant::~Integrant( ) {
}

void Pvalue::Integrant::k( const int& k ){
  k_   = k;
  odd_ = ( ( k_ % 2 ) == 1 );
}


/*
 */
double Pvalue::Integrant::operator()( const double& x ){
  static double c = 1.0 / sqrt( 2.0 * M_PI );
  double rval = 0.0;
  if( odd_ ){
    rval = c * exp( - 0.5 * x ) * pow( x, -0.5 ); 
    for( int n = k_ - 2; n > 0; n -= 2 ) rval *= x / n;
  } else {
    rval = 0.5 * exp( - 0.5 * x );
    for( int i = 1; i < k_ / 2; i++ ) rval *= 0.5 * x / i;
  }
  return rval ;
}

// ---------------------------------------------------------------------
Pvalue::Pvalue( const double& k ) :
  RealFunction(), IntegFunc( ), f_( k )
{
  this->lower( 0.0 );
}

Pvalue::~Pvalue( ) {
}

double Pvalue::operator()( const double& x ) {
  /*
  double k  = this->k();

  double dk = sqrt( k );

  double k1 = k - dk;
  double k2 = k + dk;
  double cl = 0.0;
  
  if( x < k1 ) {
    cl = this->integration()->integral( 0.0, x, f_ );
  } else if ( x < k2 ) {
    cl = 
      this->integration()->integral( 0.0, k1, f_ ) +
      this->integration()->integral( k1, x, f_ );
  } else {
    cl = 
      this->integration()->integral( 0.0, k1, f_ ) +
      this->integration()->integral( k1, k2, f_ ) +
      this->integration()->integral( k2, x, f_ );
  }
  */
  return 1.0 - this->integration()->integral( 0.0, x, f_ );
  //  double cl = this->integration()->integral( 0.0, x, f_ );
  //  double p = 1.0 - cl;
  //  return ( p < 0.0 ? 0.0 : ( p > 1.0 ? 1.0 : p ) );
  
}

void Pvalue::k( const double& k ){
  f_.k( k );
}
