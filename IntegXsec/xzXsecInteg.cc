#include "xzXsecInteg.hh"
#include "XsecBase.hh"
#include "XsecIntegrand.hh"

#include <iostream>
#include <iomanip>

#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

using namespace std;
using namespace Transform;
using namespace IntegXsec;

xzXsecInteg::xzXsecInteg( XsecBase* xzx, const int& low, const int& high, 
			  const double& precision ) throw( int ):
  IntegFunc(), cfunc_( xzx ) //  IntegFunc( low, high )
{
  this->initialization( low, high, precision );
}

double xzXsecInteg::operator()( const double& x ){
  return this->integration()->integral( this->lower(), this->upper(), 
					this->cfunc_.setx( x ) );
}

complex< double > xzXsecInteg::operator()( const complex< double >& n ){
  return this->integration()->cintegral( this->lower(), this->upper(), 
					 this->cfunc_.setn( n ) );
}

double xzXsecInteg::CFunc::operator()( const double& z ){
  return (*xzXsec_->xsecInteg())( x_, z ); 
}

complex< double > xzXsecInteg::CFunc::operator()( const complex< double >& m ){
  return (*xzXsec_->xsecInteg())( n_, m ); 
}

void xzXsecInteg::initialization( const int& low, const int& high, 
				 const double& precision )
{
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  
  if( low != 0 ){
    dynamic_cast< LegQuadrature* >( GridInt->method1() )->setngrid( low );
  } else{
    GridInt->setmethod1( ( Integration* ) NULL ); 
  }

  if( high != 0 ){
    dynamic_cast< LegQuadrature* >( GridInt->method2() )->setngrid( high );
  } else{
    GridInt->setmethod2( ( Integration* ) NULL ); 
  }

  GridInt->precision( precision );
  GridInt->ngrid( 4 );
}

void xzXsecInteg::setloginteg( const bool& logint )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  
  if( GridInt->method1() )
    dynamic_cast< LegQuadrature* >( GridInt->method1() )
      ->setloginteg( logint );
  if( GridInt->method2() )
    dynamic_cast< LegQuadrature* >( GridInt->method2() )
      ->setloginteg( logint );
}

void xzXsecInteg::setGngrid( const int& ngrid )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  GridInt->ngrid( ngrid );
}

