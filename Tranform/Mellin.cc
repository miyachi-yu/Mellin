#include "Mellin.hh"

using namespace std;
using namespace Transform;

Mellin::Mellin( RealFunction *f, Integration *inter ) :
  Transformer(),
  integrand_( f )
{
  this->integration( inter );
  this->upper( 1E+4 );
  this->lower( 0.0 );
}

Mellin::Mellin( Integration *inter ) :
  Transformer(),
  integrand_()
{
  this->integration( inter );
  this->upper( 1E+4 );
  this->lower( 0.0 );
}

Mellin::~Mellin(){
}

complex< double > Mellin::operator() ( const complex< double >& z ) {
  return 
    this->integration()->cintegral( this->lower(), 
				    this->upper(), integrand_.set( z ) );
}

complex< double > Mellin::operator() ( const complex< double >& n,
				       const complex< double >& m ) {
  return 
    this->integration()->cintegral( this->lower(), this->upper(), 
				    this->lower(), this->upper(),
				    integrand_.set( n, m ) );
}

ComplexFunction& Mellin::transform( RealFunction *f ) {
  integrand_.set( f );
  return *this;
}
