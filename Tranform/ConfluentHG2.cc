#include "ConfluentHG2.hh"

#include <cmath>

using namespace std;
using namespace Transform;

complex< double > ConfluentHG2::Integrand::operator()( const double& t ){
  return exp( - z_ * t ) * 
    pow( t, a_ - 1.0 ) * pow( 1.0 + t, b_ - a_ - 1.0 );
}

ConfluentHG2::ConfluentHG2( const complex< double >& a, 
			    const complex< double >& b ) : 
  IntegFunc(), 
  a_( a ), b_( b ),
  ga_( 0.0 ), gamma_( ), integrand_( a, b )
{
  this->set( a, b );
  this->upper( 100.0 );
}

ConfluentHG2::~ConfluentHG2(){
}

void ConfluentHG2::set( const complex< double >& a,
			const complex< double >& b ){
  a_ = a;
  b_ = b;
  integrand_.set( a, b );
  ga_ = 1.0 / gamma_( a );
}

complex< double > ConfluentHG2::operator()( const complex< double >& z ){
  if( a_.real() > 0 && z.real() > 0 )
    return ga_ * 
      this->integration()->cintegral( 0.0, this->upper(), integrand_.z( z ) );
  
  return complex< double >( 0.0, 0.0 );
  
}
