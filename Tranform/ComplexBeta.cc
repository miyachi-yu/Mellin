#include "ComplexBeta.hh"

using namespace std;
using namespace Transform;

complex< double > 
ComplexBeta::Integrand::operator()( const double& x ){
  return pow( x, m ) * pow( 1.0 - x, n );
}

ComplexBeta::ComplexBeta( const double& alpha, const double& beta ) :
  ComplexFunction(), IntegFunc( 6, 8, 2, 1.0E-5 ),
  integrand_(), a_( alpha )
{
  this->lower( 0.0 );
  this->upper( 1.0 );
  this->set( alpha, beta );
}

ComplexBeta::ComplexBeta( const ComplexBeta& cb ) :
  ComplexFunction(), IntegFunc( 6, 8, 2, 1.0E-5 ),
  integrand_(), a_( cb.a_ )
{
  this->lower( 0.0 );
  this->upper( 1.0 );
  integrand_.m = cb.integrand_.m;
}

ComplexBeta::~ComplexBeta(){
}

ComplexBeta& ComplexBeta::operator=( const ComplexBeta& cb ){
  if( this != &cb ){
    a_ = cb.a_ ;
    integrand_.m = cb.integrand_.m;
  }
}

complex< double > ComplexBeta::operator()( const complex< double >& n ){
  integrand_.n = n + a_ - 1.0;
  return this->integration()->cintegral( 0.0, 1.0, integrand_ );
};

void ComplexBeta::set( const double& a, const double& b ){
  a_ = a ;
  integrand_.m = complex< double >( b, 0.0 );
}
