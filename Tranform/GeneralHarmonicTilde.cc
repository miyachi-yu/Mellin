#include "GeneralHarmonicTilde.hh"
#include <cmath>

using namespace std;
using namespace Transform;

GeneralHarmonicTilde::GeneralHarmonicTilde() : 
  pg_( 0 ), s_(), sp_(), li2int_(), hgint_()  {
}

GeneralHarmonicTilde::~GeneralHarmonicTilde() {
}

complex< double > 
GeneralHarmonicTilde::operator()( const complex< double >& n,
				  const int& eta ){
  if( n.imag() == 0.0 && n.real() > 0 && fmod( n.real(), 1.0 ) == 0.0 ){
    complex< double > val( 0.0, 0.0 );
    for( double j = 1; j <= n.real(); j++ ){
      val += pow( -1.0, j ) / pow( j, 2 ) * s_( 1, j );
    }
    return val;
  }
  
  return - 0.625 * s_.zeta[ 3 ] +
    ( eta > 0 ? 1.0 : -1.0 ) * 
    ( s_( 1, n ) / pow( n, 2 ) 
      - 0.5 * s_.zeta[ 2 ] * ( pg_( 0, ( n + 1.0 ) / 2.0 ) 
			       - pg_( 0,  n / 2.0 ) )
      + li2int_( n ) );
}

complex< double >
GeneralHarmonicTilde::operator()( const int& l,
				  const complex< double >& n,
				  const int& eta ){
  
  if( n.imag() == 0.0 && n.real() > 0 && fmod( n.real(), 1.0 ) == 0.0 ){
    complex< double > val( 0.0, 0.0 );
    for( double j = 1; j <= n.real(); j++ ) 
      val += pow( -1.0, j ) / pow( j, l );
    return val;
  }
  return sp_( l, 0.5 * n, eta ) * pow( 2.0, 1 - l ) - s_( l, n );
}

complex< double > 
GeneralHarmonicTilde::operator()( const int& i,
				  const int& j,
				  const complex< double >& n,
				  const int& eta ){
  
  if( i == 1 && j == 2 )
    return 
      - s_( 1, 2, n ) 
      + 0.5 * s_.zeta[ 2 ] * ( s_( 1, n ) - (*this)( 1, n, eta ) )
      + hgint_( n );
  
  return complex< double >( 0.0, 0.0 );
}

