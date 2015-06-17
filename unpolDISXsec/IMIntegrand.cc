#include <Tranform/InverseMellin.hh>
#include <iostream>

#include "IMIntegrand.hh"

using namespace std;
using namespace Transform;
using namespace unpolDISXsec;

IMIntegrand::IMIntegrand( ComplexFunction *f ) :
  Transform::InverseMellinIntegrand( f ),
  xmom_( false ), zmom_( false )
{
}

complex< double > 
IMIntegrand::operator()( const complex< double >& n ){
  
  if( ! f_ ) return complex< double >( 0.0, 0.0 );
  
  complex< double > rval = (*f_)( n );
  
  if( xmom_ )       return rval; 
  
  if( ! partialx_ ) return pow( x1_, - n ) * rval;
  
  complex< double > N = momx_ - n;
  return ( pow( x2_, N ) - pow( x1_, N ) ) / N / dx_ * rval;
}

complex< double > 
IMIntegrand::operator()( const complex< double >& n,
			 const complex< double >& m ){
  
  if( !f_ ) return complex< double >( 0.0, 0.0 );
  
  complex< double > weightx = 1.0;
  
  if( xmom_ ){  // this may be ! xmom_
    if( partialx_ ){
      complex< double > nx = momx_ - n;
      weightx = ( pow( x2_, nx ) - pow( x1_, nx ) ) / nx / dx_ ;
    } else {
      weightx = pow( x1_, - n );
    }
  }
  
  complex< double > weightz = 1.0;
  
  if( zmom_ ){  // this may be ! zmom_
    if( partialz_ ){
      complex< double > nz = momz_ - m;
      weightz = ( pow( z2_, nz ) - pow( z1_, nz ) ) / nz / dz_ ;
    } else {
      weightz = pow( z1_, - m );
    }
  }
  
  return weightx * weightz * (*f_)( n, m );
}

