#include "HyperGeomInt.hh"

#include <cstdlib>

using namespace std;
using namespace Transform;

complex< double > 
HyperGeomInt::Integrand::operator()( const complex< double >& n ){
  return (*this)( n.real() );
}

complex< double > 
HyperGeomInt::Integrand::operator()( const double& x ){
  
  double a = 1.0 - x;
  complex< double > A = pow( x, 0.5 * n_ );
  complex< double > B = ( 1.0 - A ) / a;
  double lx = log( x );
  double sx = sqrt( x );
  double C = 0.5 * log( x ) - 1.0;
  double D = log( a );
  
  return 
    0.25 * B * lx * D / x
    + 0.125 * ( 2.0 - ( 2.0 + n_ * a ) * A ) / a / a * lx * li2_( x )
    + 0.25 * pow( x, -1.5 ) * C * D * log( ( 1.0 - sx ) / ( 1.0 + sx ) )
    + 0.25 * pow( x, 0.5 * n_ - 1.0 ) * C * D * hyper_( sqrt( x ) ) /
    ( n_ + 1.0 ) ;
}

CKey HyperGeomInt::key;
complex< double > HyperGeomInt::data( 0.0, 0.0 );
Utility::LookupTable< CKey, complex< double > > 
HyperGeomInt::cache( HyperGeomInt::key, HyperGeomInt::data, "hgint.db" );

bool HyperGeomInt::cacheReady = false;

void HyperGeomInt::initCache() {
  if( HyperGeomInt::cacheReady ) return;
  atexit( HyperGeomInt::closeCache );
  HyperGeomInt::cacheReady = true;
}

void HyperGeomInt::closeCache() {
  HyperGeomInt::cache.close();
}

HyperGeomInt::HyperGeomInt() {
  if( ! HyperGeomInt::cacheReady ) HyperGeomInt::initCache();
  this->lower( 0.0 );
  this->upper( 1.0 );
}

complex< double > HyperGeomInt::operator()( const complex< double >& n ){
  CKey k( n );
  if( ! HyperGeomInt::cache.get( HyperGeomInt::key = k, 
				 HyperGeomInt::data ) ) {
    HyperGeomInt::data = 
      0.5 * li2int2_( n / 2.0 ) +
      this->integration()->cintegral( this->lower(), this->upper(),
				      func_.n( n ) );
    HyperGeomInt::cache.put();
  }
  return HyperGeomInt::data ;
}
