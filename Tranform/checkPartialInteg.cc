#include "DInverseMellin.hh"
#include "InverseMellin.hh"
#include "IntegFunc.hh"
#include "ComplexFunction.hh"
#include "GridIntegration.hh"
#include "LegQuadrature.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

class Cimazu_func: public Transform::ComplexFunction{
public:
  Cimazu_func(){};
  virtual ~Cimazu_func(){};

  virtual std::complex< double > operator()( const std::complex< double >& n )
  { 
    //    cout << abs( n ) << endl;
    return 1.0 / ( n + 1.0 ); 
  }
  virtual std::complex< double > operator()( const std::complex< double >& n, 
					     const std::complex< double >& m ){
    //    cout << abs( n ) << " " << abs( m ) << endl;
    return 1.0 / ( n + 1.0 ) * 1.0 / ( m + 1.0 );//    return x * z;
  }
};

using namespace std;
using namespace Transform;

int main(){

  std::vector< double > aaa( 1, 0.0 ), bbb( 1, 0.0 );
  Cimazu_func cyoshi;
  IntegFunc integ;

  /*
  dynamic_cast< LegQuadrature* >( dynamic_cast< GridIntegration* >
                                  ( integ.integration() )->method1() )
    ->setloginteg( true );
  dynamic_cast< LegQuadrature* >( dynamic_cast< GridIntegration* >
                                  ( integ.integration() )->method2() )
    ->setloginteg( true );
  */
  double c0 = 0.0; double phi = 0.90; double length = 20.0;

  InverseMellin Dyoshi( &cyoshi, integ.integration() );
  Dyoshi.setParameter( c0, phi ); Dyoshi.upper( length );
  
  DInverseMellin Dyoshi2( &cyoshi, integ.integration() );
  std::vector< double > offset_( 2, c0 );
  std::vector< double > phi_( 2, phi );
  Dyoshi2.setParameter( offset_, phi_ ); 
  Dyoshi2.upper( length );  Dyoshi2.integrand().upper( length );

  double xmin = 0.01; double xmax = 1.0; double segm = 4.0;
  double dx = exp( ( log( xmax ) - log( xmin ) ) / segm );

  for( double xx = xmin; xx <= xmax; xx *= dx ){
    cout << setw( 15 ) << xx << flush;
  }
    cout << endl;

  for( double xx = xmin; xx <= xmax; xx *= dx ){
    cout << setw( 15 ) << Dyoshi( xx ) << flush;
  }
  cout << endl;

  for( double xx = xmin; xx <= xmax; xx *= dx ){
    aaa[ 0 ] = xx;
    //    cout << setw( 15 ) << sqrt( Dyoshi2( aaa, aaa ) ) << flush;
    cout << setw( 15 ) << sqrt( Dyoshi2( aaa, aaa ) ) << flush;
  }

  cout << endl;

  return 0;
}
