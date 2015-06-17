#include "IntegFunc.hh"
#include "GridIntegration.hh"
#include "LegQuadrature.hh"

using namespace std;
using namespace Transform;

IntegFunc::IntegFunc( const int& n, const int& m, const int& ngrid,
		      const double& precision ) : 
  Transformer()
{  
  this->integration( new GridIntegration( ( n != 0 ? new LegQuadrature( n ) 
					    : NULL ),
					  ( m != 0 ? new LegQuadrature( m ) 
					    : NULL ),
					  ngrid, precision ) );
}

IntegFunc::IntegFunc( Integration* integration ) : Transformer()
{
  this->integration( integration );
}

IntegFunc::~IntegFunc() {
  if( dynamic_cast< GridIntegration* >( this->integration() ) ){
    delete dynamic_cast< GridIntegration* >( this->integration() )->method1();
    delete dynamic_cast< GridIntegration* >( this->integration() )->method2();
    delete  this->integration();
  }
}
