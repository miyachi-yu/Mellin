#include "RTransform.hh"
#include "GridIntegration.hh"
#include "LegQuadrature.hh"
#include <Utility/Arguments.hh>

#include <vector>
#include <iostream>

using namespace Transform;
using namespace std;

RTransform::RTransform() :
  integ_( NULL ), K_( NULL )
{
  Utility::Arguments &args = Utility::Arguments::ref();
  
  vector< int > nLeg( 2 );
  nLeg[ 0 ] = 4;
  nLeg[ 1 ] = 6;
  nLeg = args.get( "nleg", ",", nLeg );
  if( nLeg.size() == 1 ){
    nLeg.resize( 2 );
    nLeg[ 1 ] = nLeg[ 0 ] + 2;
  }
  LegQuadrature *leg1 = new LegQuadrature( nLeg[ 0 ] );
  LegQuadrature *leg2 = new LegQuadrature( nLeg[ 1 ] );
  
  integ_ =
    new GridIntegration( leg1, leg2,
				    args.get( "nGrid", 4 ),
				    args.get( "precision", 0.01 ) );
  
  this->upper( args.get( "xmax", 1.0E+2 ) );
  this->lower( args.get( "xmin", 0.0 ) );
  
}

RTransform::~RTransform(){
  delete dynamic_cast< GridIntegration* >( integ_ )->method1();
  delete dynamic_cast< GridIntegration* >( integ_ )->method2();
  delete integ_;
}


double RTransform::operator()( const double& t ){
  if( integ_ == NULL || K_ == NULL ) return 0.0;
  RealFunction &rFunc = K_->set( t );
  return integ_->integral( this->lower(), this->upper(), rFunc );
}

void RTransform::integrand( RealFunction *f ){
  if( K_ ) K_->integrand( f );
}

double RTransform::Kernel::operator()( const double& x ){
  return ( f_ == NULL ? 0.0 : this->eval( x ) * (*f_)( x ) ) ;
}

void RTransform::nLeg( const int& n1, const int& n2 ){
  GridIntegration* gint = dynamic_cast< GridIntegration* >( integ_ );
  dynamic_cast< LegQuadrature* >( gint->method1() )->setngrid( n1 );
  dynamic_cast< LegQuadrature* >( gint->method2() )->setngrid( n2 );
}

void RTransform::precision( const double& p ){
  GridIntegration* gint = dynamic_cast< GridIntegration* >( integ_ );
  gint->precision( p );
}

void RTransform::nGrid( const int& n ){
  GridIntegration* gint = dynamic_cast< GridIntegration* >( integ_ );
  gint->ngrid( n );
}
