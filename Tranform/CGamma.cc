// $Id: CGamma.cc,v 1.10 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    CGamma.cc
  \version $Revision: 1.10 $
  \author  $Author: miyachi $
  \brief   Complex Gamma Function Class (Implementation)
 */
#include "CGamma.hh"

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;
using namespace Transform;

CKey CGamma::key;
complex< double > CGamma::data;
Utility::LookupTable< CKey, complex< double > >* CGamma::cache;
bool CGamma::cacheReady = false;

void CGamma::initCache() {
  if( CGamma::cacheReady ) return ;
  CGamma::cache = 
    new Utility::LookupTable< CKey, complex< double > >( CGamma::key,
							 CGamma::data,
							 "gamma.db" );
  atexit( CGamma::closeCache );
  CGamma::cacheReady = true;
}

void CGamma::closeCache(){
  CGamma::cache->close();
  delete CGamma::cache;
}

complex< double > CGamma::Integrand::operator()( const double& t ){
  return pow( log( 1.0 / t ), z_ - 1.0 );
}

DataCache CGamma::dcache_;

vector< complex< double > > CGamma::cdata_( 1, complex< double >( 0.0, 0.0 ) );

CGamma::CGamma() : 
  IntegFunc(), integrand_()
{
  // CGamma::initCache();
  // dcache_.clear();
}

CGamma::~CGamma(){
}

complex< double > CGamma::operator()( const complex< double >& z ) {
  
  if( z.imag() == 0.0 && z.real() <= 0.0 && floor( z.real() ) == z.real() ){
    return complex< double >( 1.0E+308, 0.0 );
  }
  
  //  return exp( lg( z ) );
  
  
  //  CGamma::key = CKey( z );
  //  if( ! CGamma::cache->get( CGamma::key, CGamma::data ) ) {
  if( ! dcache_.cached( z, 0.0 ) ){
    //CGamma::data = exp( lg( z ) );
    /*  
	( z.real() < 0 ? 
	(*this)( z + 1.0 ) / z :
	this->integration()->cintegral( 0.0, 1.0, integrand_.set( z ) ) );      
    */
    // CGamma::cache->put();
    cdata_[ 0 ] = exp( lg( z ) );
    dcache_.cache( z, 0.0, cdata_ );
    return cdata_[ 0 ];
  }
  
  return dcache_.cache( z, 0.0 )[ 0 ];
  
  //  return CGamma::data;
}

