// $Id: CPolyLog.cc,v 1.9 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    CPolyLog.cc
  \version $Revision: 1.9 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Implementation of CPolyLog special function class
 */
#include "CPolyLog.hh"
#include "GridIntegration.hh"
#include <cstdlib>

using namespace std;
using namespace Transform;

CGamma CPolyLog::G_;

CPolyLog::CPolyLog() : 
  IntegFunc(), func_(), cache_( false ), q2_( 1.0 ), sIsReal_( false ), sIsInt_( false )
{
  //  if( ! CPolyLog::cacheReady ) CPolyLog::initCache();
  dynamic_cast< GridIntegration* >( this->integration() )->ngrid( 40 );
  dynamic_cast< GridIntegration* >( this->integration() )->precision( 1.0E-7 );
}

complex< double >
CPolyLog::Integrand::operator()( const complex< double >& t ){

  return
    ( sIsPosi_ ?
      pow( t, s_ - 1.0 ) / ( ( bose_ ? -1.0 : 1.0 ) + exp( t ) / z_ ) :
      pow( t, s_ ) * sin( 0.5 * s_ * M_PI - t * log( -z_ ) ) / sinh( M_PI * t ) );
}

CPolyLog& CPolyLog::set( const std::complex< double >& s ) {
  sIsReal_ = ( s.imag() == 0.0 );
  if( sIsReal_ ) {
    sIsInt_ = ( s.real() - int( s.real() ) == 0.0 );

  } else {
    sIsInt_ = false;
  }
  func_.s_ = s;
  func_.sIsPosi_ = ( s.real() > 0.0 );
  return *this;
}

complex< double > CPolyLog::ints( const complex< double >& z ){
  switch( int( func_.s_.real() ) ) {
  case 1:
    return - log( 1.0 - z );
  case 0:
    return z / ( 1.0 - z );
  case -1:
    return z / pow( 1.0 - z, 2 );
  case -2:
    return z * ( 1.0 + z ) / pow( 1.0 - z, 3 );
  case -3:
    return z * ( 1.0 + 4.0 * z + z * z ) / pow( 1.0 - z, 4 );
  case -4:
    return z * ( 1.0 + z ) * ( 1.0 + 10.0 * z + z * z ) / pow( 1.0 - z, 5 );
  }
  return this->normal( z );
}

complex< double > CPolyLog::normal( const complex< double >& z ){
  if( func_.sIsPosi_ ) return this->posiS( z );
  return this->negaS( z );
}


complex< double > CPolyLog::negaS( const complex< double >& z ){
   return this->integration()->cintegral( 0.0, M_PI, func_ );
} 

complex< double > CPolyLog::posiS( const complex< double >& z ){
  
  func_.bose_ = ( z.imag() == 0.0 && z.real() > 1.0 ? false : true );
  func_.z_    = ( func_.bose_ ? z : -1.0 * z );
  
  double upper = 0.0;
  if( func_.bose_ ) {
    upper = 2.0 * abs( func_.s_.real() );
  } else {
    double b = func_.s_.real() - 1.0;
    double a = log( func_.z_.real() );
    if( b < 0.0 ){
      upper = a;
    } else {
      double x = b * exp( a - b );
      
      //      cout << "x = " << x << ", "<< flush;
      
      double w = ( x > 1.0 ?
		   log( x ) - log( log( x ) ) :
		   x - pow( x, 2 ) + 1.5 * pow( x, 3 ) - 8.0 / 3.0 * pow( x, 4 ) );
      upper = w + b;
    }
    upper *= 2.0;
  }
  
  upper *= 4.0;
  //  cout << "Upper: " << upper << endl;
  //  double upper = 20.0;
  
  return ( func_.bose_ ? 1.0 : -1.0 ) *
    this->integration()->cintegral( 0.0, upper, func_ ) / G_( func_.s_ );
}

complex< double > CPolyLog::operator()( const complex< double >& z ){
  
  if( cache_.cached( func_.s_, z, q2_ ) ) {
    return cache_.cache( func_.s_, z, q2_ )[ 0 ];
  }

  complex< double > val;
  if( sIsInt_ ){
    val = this->ints( z );
  } else {
    val = this->normal( z );
  }
  
  cache_.cache( func_.s_, z, q2_, val );
  return val;
}

