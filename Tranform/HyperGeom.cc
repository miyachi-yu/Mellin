#include "HyperGeom.hh"
#include "GridIntegration.hh"
#include "LegQuadrature.hh"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace Transform;

DataCache::Grid   HyperGeom::key;
complex< double > HyperGeom::data( 0.0, 0.0 );
Utility::LookupTable< DataCache::Grid, complex< double > > 
HyperGeom::cache( HyperGeom::key, HyperGeom::data, "hg.db" );

bool HyperGeom::cacheReady = false;

void HyperGeom::initCache() {
  if( HyperGeom::cacheReady ) return;
  atexit( HyperGeom::closeCache );
  HyperGeom::cacheReady = true;
}

void HyperGeom::closeCache() {
  HyperGeom::cache.close();
}

complex< double > 
HyperGeom::Integrand::operator()( const complex< double >& n ){
  return (*this)( n.real() );
}

complex< double > 
HyperGeom::Integrand::operator()( const double& x ){
  return
    exp( ( n_.real() + 1.0 ) * x ) * 
    polar( 1.0, n_.imag() * x ) /
    ( 1 - exp( 2.0 * x ) * pow( z_, 2 ) );
}

HyperGeom::HyperGeom() {
  if( ! HyperGeom::cacheReady ) HyperGeom::initCache();
  // The lower limit -1000.0 is not enough to converge HyperGeometric 
  // function itself. However quantity appeared in Anomalous Dimension
  // is integral of the Hyper Geometric function. The integral 
  // is converged with -1.0E+3, so we take this number...
  this->lower( -1.0E+3 );
  this->upper(  0.0 );
}

complex< double > HyperGeom::operator()( const double& z ){

  DataCache::Grid k( n_, z );
  if( ! HyperGeom::cache.get( HyperGeom::key = k, HyperGeom::data ) ) {
    HyperGeom::data = 
      2.0 * ( n_ + 1.0 ) * 
      this->integration()->cintegral( this->lower(), this->upper(),
				      func_.set( n_, z ) );
    HyperGeom::cache.put();
  }
  return HyperGeom::data ;
  
  /*
  return ( HyperGeom::cache.find( key ) != HyperGeom::cache.end() ?
	   HyperGeom::cache[ key ] :
	   HyperGeom::cache[ key ] = 
	   2.0 * ( n_ + 1.0 ) * 
	   this->integration()->cintegral( this->lower(), this->upper(),
					   func_.set( n_, z ) ) );
  */
}

complex< double > HyperGeom::operator()( const complex< double >& n ){
  return (*this)( n.real() );
}

// ------------------------------------------------------------ //
