#include "xXsec.hh"
#include "Xsec.hh"
#include <cmath>
#include <Utility/Arguments.hh>

#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>
#include <Tranform/LaguerreQuadrature.hh>

using namespace Utility;
using namespace Transform;
using namespace Xsection;

xXsec::xXsec( Xsec *xc,
	      const int& low, const int& high, const double& precision,
	      const double& length, const double& offset, 
	      const double& rangle ) : 
  InverseMellin( xc, NULL ), 
  xsec_( xc ), objList_( 0 )
{
  
  this->initialization( low, high, precision, length, offset, rangle );
  
}

xXsec::xXsec( Xsec *xc, Integration *integ ) :
  InverseMellin( xc, integ ), 
  xsec_( xc ), objList_( 0 )
{
}

xXsec::~xXsec() {
  /*
  if( dynamic_cast< GridIntegration* >( this->integration() )->method1() ) 
  delete dynamic_cast< GridIntegration* >( this->integration() )->method1();
  if( dynamic_cast< GridIntegration* >( this->integration() )->method2() )
  delete dynamic_cast< GridIntegration* >( this->integration() )->method2();
  delete  this->integration();
  */
  for( int i = 0; i < objList_.size(); i++ ) if( objList_[ i ] ) 
    delete objList_[ i ];
}

void xXsec::initialization( const int& low, const int& high, 
			    const double& precision,
			    const double& length, const double& offset, 
			    const double& rangle ){
  
  /*
  GridIntegration* integ =  new GridIntegration( ( Integration* ) NULL,
                                             ( Integration* ) NULL,
                                             4, precision );
  if( low != 0 ) integ->setmethod1( new LegQuadrature( low ) );
  if( high != 0 ) integ->setmethod2( new LegQuadrature( high ) );
  this->integration( integ );
  */

  objList_.resize( 3 );
  objList_[ 0 ] = ( low != 0 ? new LegQuadrature( low ) : NULL );
  objList_[ 1 ] = ( high != 0 ? new LegQuadrature( high ) : NULL );
  objList_[ 2 ] = new GridIntegration( objList_[ 0 ], objList_[ 1 ], 4, precision );
  
  this->integration( objList_[ 2 ] );
  this->setParameter( offset, rangle );
  this->upper( length );
  
}

void xXsec::setGngrid( const int& ngrid )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  GridInt->ngrid( ngrid );
}

void xXsec::setloginteg( const bool& logint )
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

double xXsec::chiSquare( Exp::DataSet& ds ){
  /*
  ds.chiSquare() = 0.0;
  for( Exp::DataSet::iterator itr = ds.begin();
  itr != ds.end();
       itr++ ) ds.chiSquare() += this->chiSquare( *itr );
  */
}

double xXsec::chiSquare( Exp::DataPoint& dp ) {
  dp.model() = this->eval( dp );
  return dp.chiSquare();
}
