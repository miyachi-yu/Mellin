#include "xzXsec.hh"
#include "Xsec.hh"
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>
#include <cmath>

using namespace std;
using namespace Transform;
using namespace Xsection;

xzXsec::xzXsec( Xsec *xc,
	      const int& low, const int& high, const double& precision,
		const double& length, const double& offset, 
	      const double& rangle ) : 
  DInverseMellin( xc, NULL ), 
  xsec_( xc )
{
  this->initialization( low, high, precision, length, offset, rangle );
}

xzXsec::xzXsec( Xsec* xc, Transform::Integration* integ ) :
  DInverseMellin( xc, integ ),
  xsec_( xc )
{
  throw( 1 );
}

xzXsec::~xzXsec() {
  if( dynamic_cast< GridIntegration* >( this->integration() )->method1() ) 
  delete dynamic_cast< GridIntegration* >( this->integration() )->method1();
  if( dynamic_cast< GridIntegration* >( this->integration() )->method2() )
  delete dynamic_cast< GridIntegration* >( this->integration() )->method2();
  delete  this->integration();
}

void xzXsec::initialization( const int& low, const int& high, 
			     const double& precision,
			     const double& length, const double& offset, 
			     const double& rangle ){

  
  // create integration object
  GridIntegration* integ =  new GridIntegration( ( Integration* ) NULL,
					     ( Integration* ) NULL,
					     4, precision );
  
  if( low != 0 ) integ->setmethod1( new LegQuadrature( low ) );
  if( high != 0 ) integ->setmethod2( new LegQuadrature( high ) );
  
  // register the integration object to this and also integrand
  this->integration( integ );
  this->integrand().integration( integ );
  
  // set integration path length
  this->upper( length );
  this->integrand().upper( length );
  
  // set integration path conditions
  std::vector< double > offset_( 2, offset );
  std::vector< double > phi_( 2, rangle );
  //  offset_[ 1 ] = offset_[ 0 ] + 0.1;
  
  this->setParameter( offset_, phi_ );
  
}

void xzXsec::setGngrid( const int& ngrid )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  GridInt->ngrid( ngrid );
}

void xzXsec::setloginteg( const bool& logint )
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
