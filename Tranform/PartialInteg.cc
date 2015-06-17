#include "PartialInteg.hh"

#include <iostream>
#include <iomanip>

#include "GridIntegration.hh"
#include "LegQuadrature.hh"

using namespace std;
using namespace Transform;

PartialInteg::PartialInteg( const int& low, const int& high, const int& ngrid, 
			    const double& precision ) throw( int ):
  IntegFunc( low, high, ngrid, precision ), cfunc_(), zmin_( 0.0 ), 
  zmax_( 1.0 )
{}

PartialInteg::PartialInteg( Integration* integration ) : 
  IntegFunc( integration ), cfunc_(), zmin_( 0.0 ), zmax_( 1.0 )
{}

double PartialInteg::operator()( const double& x ){
  if( this->getCFunc()->getfunc() == NULL ) throw( 1 );
  return this->integration()->integral( this->min(), this->max(), 
				 this->getCFunc()->setx( x ) );
}

complex< double > PartialInteg::operator()( const complex< double >& n ){
  if( this->getCFunc()->getnfunc() == NULL ) throw( 1 );
  return this->integration()->cintegral( this->min(), this->max(), 
				 this->getCFunc()->setn( n ) );
}

void PartialInteg::setloginteg( const bool& logint )
{ 

  if( dynamic_cast< GridIntegration* >( this->integration() ) ){
    GridIntegration* GridInt 
      = dynamic_cast< GridIntegration* >( this->integration() );
    if( dynamic_cast< LegQuadrature* >( GridInt->method1() ) )
      dynamic_cast< LegQuadrature* >( GridInt->method1() )
	->setloginteg( logint );
    if( dynamic_cast< LegQuadrature* >( GridInt->method2() ) )
      dynamic_cast< LegQuadrature* >( GridInt->method2() )
	->setloginteg( logint );
  }

}
