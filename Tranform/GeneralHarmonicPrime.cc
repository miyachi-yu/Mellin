#include "GeneralHarmonicPrime.hh"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

GeneralHarmonicPrime::GeneralHarmonicPrime() {
}

GeneralHarmonicPrime::~GeneralHarmonicPrime(){
}


complex< double > 
GeneralHarmonicPrime::operator()( const int& l, 
				  const complex< double >& n,
				  const int& eta ){
  /*
  if( n.imag() == 0.0 && n.real() > 0 && fmod( n.real(), 0.5 ) == 0.0 ){
    complex< double > val( 0.0, 0.0 );
    for( double j = 0.0; j < 2.0 * n.real(); j++ ) 
      val += ( 1.0 + pow( -1.0, j + 1 ) ) * pow( j + 1.0, -l );
    return pow( 2.0, l - 1 ) * val;
  }
  */
  return s_( l, n - ( eta > 0.0 ? 0.0 : 0.5 ) );
}
