#include "ConvIntegrand.hh"
#include "IntegXsecComp.hh"
#include <cmath>

using namespace IntegXsec;
using namespace Transform;
using namespace std;

ConvIntegrand::ConvIntegrand( IntegXsecComp* xc ):
  xc_( xc ), dim_( xc_->vparam().size() ), 
  g_( 0 ), delta_( 0 ), plusf_( 0 ), logplusf_( 0 )
{
  //  cout << xc_->vparam().size() << endl;
  //  cout << xc_->vparam()[ 0 ].size() << endl;
  //  cout << xc_->vparam()[ 1 ].size() << endl;
  for( int i = 0; i < dim_ ; i++ ){
    g_.push_back( new ConvIntegPDF( xc_->vparam()[ i ] ) );
    delta_.push_back( new DeltaFunc( g_[ i ], xc_->vparam()[ i ] ) );
    plusf_.push_back( new PlusFunc( g_[ i ], xc_->vparam()[ i ] ) );
    logplusf_.push_back( new LogPlusFunc( g_[ i ], xc_->vparam()[ i ] ) );
  }
  //  cout << g_[ 0 ]->getparam().size() << endl;
}

ConvIntegrand::~ConvIntegrand(){
  for( int i = 0; i < dim_; i++ ){
    delete g_[ i ]; delete delta_[ i ]; 
    delete plusf_[ i ]; delete logplusf_[ i ];
  }
}

ConvIntegrand& ConvIntegrand::setxPDF( vector< RealFunction* > vf ){
  if( vf.size() != dim_ ) throw( 1 );
  for( int i = 0; i < dim_; i++ ) g_[ i ]->setxPDF( vf[ i ] );
  if( !(this->xpdfcheck()) ) throw( 1 );
  return *this;
}

ConvIntegrand& ConvIntegrand::setxPDF( RealFunction* f, RealFunction* g ){
  g_[ 0 ]->setxPDF( f ); 
  //  if( g_[ 1 ] != NULL ) g_[ 1 ]->setxPDF( g ); 
  if( g_.size() > 1 ) g_[ 1 ]->setxPDF( g ); 
  return *this;
}

ConvIntegrand& ConvIntegrand::setcPDF( vector< ComplexFunction* > vf ){
  if( vf.size() != dim_ ) throw( 1 );
  for( int i = 0; i < dim_; i++ ) g_[ i ]->setcPDF( vf[ i ] );
  if( !(this->cpdfcheck()) ) throw( 1 );
  return *this;
}

ConvIntegrand& ConvIntegrand::setcPDF( ComplexFunction* f, ComplexFunction* g )
{
  g_[ 0 ]->setcPDF( f ); 
  //  if( g_[ 1 ] != NULL ) g_[ 1 ]->setcPDF( g ); 
  if( g_.size() > 1 ) g_[ 1 ]->setcPDF( g ); 
  return *this;
}

double ConvIntegrand::rescale( const int& i ) throw( int ){
  if( xc_ == NULL || i >= xc_->vvevo().size() ) return 0.0;
  return log( xc_->q2() / xc_->vvevo()[ i ]->Q2() );
  // xc_->q2() == xc_->coeff()->q2()
  // xc_->vvevo() always returns an appropriate non-zero vevo 
  // even in case of the current xc_ ( IntegXsecComp not XsecCompBase ) 
  // itself does not contain any evos 
  // ( see vevo() func in IntegXsecComp.hh ) 
}

void ConvIntegrand::setConvIntfunc( vector< ConvIntegPDF* > g ){
  if( g.size() != g_.size() ) throw( 1 );//return;
  g_ = g;
}

bool ConvIntegrand::xpdfcheck(){
  for( int i = 0; i < dim_; i++ ) 
    if( g_[ i ]->getxPDF() == NULL ) return false;
  return true;
}

bool ConvIntegrand::cpdfcheck(){
  for( int i = 0; i < dim_; i++ ) 
    if( g_[ i ]->getcPDF() == NULL ) return false;
  return true;
}

/*
double ConvIntegrand::operator()( const double& x ){ 
  return 0.0;
}

double ConvIntegrand::operator()( const double& x, const double& z )
{ 
  if( dim_ != 2 ) return 0.0;
  return 0.0;
}

complex< double > ConvIntegrand::operator()( const complex< double >& n ){
  return complex< double >( 0.0, 0.0 );
}

complex< double > ConvIntegrand::operator()( const complex< double >& n, 
					     const complex< double >& m ){
  if( dim_ != 2 ) return complex< double >( 0.0, 0.0 );
  return complex< double >( 0.0, 0.0 );
}
*/
