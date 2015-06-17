#include "DInverseMellinIntegrand.hh"

using namespace std;
using namespace Transform;

DInverseMellinIntegrand::DInverseMellinIntegrand( ComplexFunction *g ) : 
     Transformer(), integinteg_( g ){ 
}

DInverseMellinIntegrand::~DInverseMellinIntegrand(){ 
}

DInverseMellinIntegrand& DInverseMellinIntegrand::set( ComplexFunction *g ) {
     integinteg_.set( g );
     return *this;
}

DInverseMellinIntegrand& DInverseMellinIntegrand::set
( const vector< double >& x, const vector< double >& z ){
  integinteg_.set( x, z );
  return *this;
}

double DInverseMellinIntegrand::operator() ( const double& r ){
  return 
    this->integration()->integral( this->lower(), this->upper(), 
				   integinteg_.setR( r ) );
}
