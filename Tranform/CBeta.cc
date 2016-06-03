// $Id: CBeta.cc,v 1.10 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file    CBeta.cc
  \version $Revision: 1.10 $
  \date    $Date: 2008/12/02 08:13:15 $
  \author  $Author: miyachi $
  \brief   Class implementation of complex beta function.
 */

#include "CBeta.hh"
#include "GridIntegration.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace Transform;

CBeta::DbKey CBeta::key;
complex< double > CBeta::data;
Utility::LookupTable< CBeta::DbKey, complex< double > > CBeta::cache( CBeta::key, CBeta::data, "cbeta.db" );
bool CBeta::registered = false;
void CBeta::atExitFunc(){
  CBeta::cache.close();
}

void CBeta::Integrand::m( const double& b ){
  m_ = complex< double >( b, 0.0 );
}

void CBeta::Integrand::n( const complex< double >& z ){
  n_ = z - complex< double >( 1.0, 0.0 );
}

complex< double > 
CBeta::Integrand::operator()( const double& x ){
  return pow( x, m_ ) * pow( 1.0 - x, n_ );
}

CBeta::CBeta( double alpha, double beta ) : 
  ComplexFunction(), IntegFunc( 6, 8, 2, 1.0E-5 ),
  gamma_(), integrand_(),
  meth_( cgamma ),
  a_( alpha ), b_( beta )
{
  this->set( alpha, beta );
  if( CBeta::registered == false ) {
    atexit( CBeta::atExitFunc );
    CBeta::registered = true;
  }
}

CBeta::CBeta( const CBeta& cbeta ) : 
  ComplexFunction(), IntegFunc( 6, 8, 2, 1.0E-5 ),
  gamma_(), integrand_(),
  meth_( cbeta.meth_ ),
  a_( cbeta.a_ ), b_( cbeta.b_ )
{
  integrand_.m( b_ );
  if( CBeta::registered == false ) {
    atexit( CBeta::atExitFunc );
    CBeta::registered = true;
  }
}

CBeta::~CBeta() {
}

CBeta& CBeta::operator=( const CBeta& cbeta ){
  if( this != &cbeta ){
    a_ = cbeta.a_ ;
    b_ = cbeta.b_ ;
    integrand_.m( b_ );
    meth_ = cbeta.meth_;
  }
  return *this;
}

complex< double > CBeta::operator()( const complex< double >& z ) {
  
  if( meth_ == direct ){
    integrand_.n( z + a_ );
    CBeta::key = DbKey( integrand_.n_, integrand_.m_ );
    if( ! CBeta::cache.get( CBeta::key, CBeta::data ) ) {
      CBeta::data = this->integration()->cintegral( 0.0, 1.0, integrand_ );
      CBeta::cache.put();
    }
    
    return CBeta::data;
  }
  complex< double > c = 1.0 / gamma_( z + a_ + b_ + 1.0 );
  return ( std::isnan( c.real() ) ? 0.0 : 
	   c * gamma_( b_ + 1.0 ) * gamma_( z + a_ ) );
}

void CBeta::set( const double& a, const double& b ){
  a_ = a;
  b_ = b;
  integrand_.m( b_ );
  //  meth_ = ( a_ < 100.0 && fabs( b_ ) < 60.0 ? cgamma : direct );
  meth_ = ( a_ < 170.0 && b_ < 170.0 ? cgamma : direct );
}

double CBeta::x( const double& v ){
  return pow( v, a_ ) * pow( 1.0 - v, b_ );
}


