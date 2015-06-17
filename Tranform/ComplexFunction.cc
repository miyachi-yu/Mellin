#include "ComplexFunction.hh"

using namespace std;
using namespace Transform;

ComplexFunction::ComplexFunction(){
}

ComplexFunction::~ComplexFunction(){
}

complex< double >
ComplexFunction::differential( const complex< double >& z,
			       const int& n,
			       const complex< double >& dz ){
  
  if( dz == complex< double >( 0.0, 0.0 ) || n < 0 ) 
    return complex< double >( 0.0, 0.0 );
  
  complex< double > h = 0.5 * dz;
  return
    ( n == 0 ? 
      (*this)( z ) :
      ( this->differential( z + h, n - 1, dz ) - 
	this->differential( z - h, n - 1, dz )    ) / dz );
  
}
