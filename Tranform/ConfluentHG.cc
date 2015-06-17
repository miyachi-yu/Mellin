// $Id: ConfluentHG.cc,v 1.5 2008/07/29 08:59:10 miyachi Exp $
#include "ConfluentHG.hh"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;


ConfluentHG::Integrand::Integrand( const complex< double >& a, 
				   const complex< double >& b ) : 
  a_( a ), b_( b ), z_( 0.0, 0.0 )
{
}

ConfluentHG::Integrand::~Integrand(){
}

complex< double > ConfluentHG::Integrand::operator() ( const double& t ){
  return exp( z_ * t ) * pow( t, a_ - 1.0 ) * pow( 1.0 - t,  b_ - a_ - 1.0 );
}

ConfluentHG::ConfluentHG( const complex< double >& a,
			  const complex< double >& b ) :
  IntegFunc( ), gamma_(), c_( 0.0, 0.0 ),
  integrand_( a, b ),
  a_( a ), b_( b )
{
  this->set( a, b );
}

ConfluentHG::~ConfluentHG(){
}

void ConfluentHG::set( const complex< double >& a, 
		       const complex< double >& b ){
  /*
    double d = b - a;
    if( ( a <= 0.0 && floor( a ) == a ) ||
    ( d <= 0.0 && floor( d ) == d ) ) {
    c_ = complex< double >( 0.0, 0.0 );
    } else {
    c_ = gamma_( b ) / gamma_( d ) / gamma_( a );
    }
  */
  a_ = a;
  b_ = b;
  c_ = gamma_( b ) / gamma_( b - a ) / gamma_( a );
  integrand_.set( a, b );
}

complex< double > ConfluentHG::operator()( const complex< double >& z ){
  
  // if a is negative integer, one can use polynominal expression.
  // elements in the summention over k = -a.real is always zero.
  // one only need to sum up to k = a.real from k = 0.

  if( a_.imag() == 0.0 && a_.real() <= 0.0
      && floor( a_.real() ) == a_.real() ){
    
    double a = a_.real();
    complex< double > abc( 1.0, 0.0 );
    complex< double > v( 1.0, 0.0 );
    
    for( double k = 1.0; k <= - a ; k++ ){
      abc *= ( a + k - 1.0 ) / ( b_ + k - 1.0 ) / k ;
      v += abc * pow( z, k );
    }    
    return v;
    //  } else  if( b_.imag() == 0.0 && floor( b_.real() ) == a_.real() ){
    //    return 
  } else {
    
    if( c_ == complex< double >( 0.0, 0.0 ) )
      return complex< double >( 0.0, 0.0 );
    integrand_.set( z );
    return  
    c_ * 
      this->integration()->cintegral( 0.0, 1.0, integrand_ );
  }
  
}
