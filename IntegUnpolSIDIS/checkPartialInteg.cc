#include <iostream>
#include <iomanip>
#include <cmath>
#include "PartialInteg.hh"
#include <Tranform/ComplexFunction.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/Xsec.hh>
#include <Tranform/DInverseMellin.hh>
#include <Tranform/IntegFunc.hh>

using namespace std;
using namespace IntegUnpolSIDIS;

class imazu_func: public Transform::RealFunction{
public:
  imazu_func(){};
  virtual ~imazu_func(){};

  virtual double operator()( const double& x ){ return 0.0; }
  virtual double operator()( const double& x, const double& z ){
    //    return x;
    return x * z;
  }
};

//class Cimazu_func: public Transform::ComplexFunction{
class Cimazu_func: public Xsection::Xsec{
public:
  Cimazu_func(){};
  virtual ~Cimazu_func(){};

  virtual complex< double > operator()( const complex< double >& n )
  { 
    //    cout << abs( n ) << endl;
    return 1.0 / ( n + 1.0 ); 
  }
  virtual complex< double > operator()( const complex< double >& n, 
					const complex< double >& m ){
    //    cout << abs( n ) << " " << abs( m ) << endl;
    return 1.0 / ( n + 1.0 ) * 1.0 / ( m + 1.0 );//    return x * z;
  }
};

int main(){

  imazu_func yoshi;
  PartialInteg PIng( &yoshi, 4, 0, 4, 1.0E-2 );
  //  PartialInteg PIng( &yoshi, 4, 6, 4, 1.0E-2 );
  //  PartialInteg PIng();
  //  PIng.setfunc( yoshi );
  PIng.getCFunc()->setPIP( 1 );

  for( int i = 0; i < 5; i++ ){
    double x_ = 1.0;
    double z_ = 5.0 * i;
    /*
    cout << yoshi( x_, z_ ) << endl;
    cout << PIng.getCFunc()->getPIP() << " " 
	 << (*PIng.getCFunc()->getfunc())( x_, z_ ) << endl;
    cout << (*PIng.getCFunc())( 8.0 ) << " yoshi "
	 << PIng.getCFunc()->getx() << endl;
    */
    PIng.setPIntegRange( i * 1.0 + 3.0, 10.0 );
    cout << PIng.min() << " min; " << PIng.max() << " max; " << flush;
    cout << " integstart" << flush;
    cout << " : result "<< PIng( 1.0 ) << flush;
    cout << " integend" << endl;
  }
  cout << endl;


  std::vector< double > aaa( 1, 0.0 ), bbb( 1, 0.0 );
  Cimazu_func cyoshi;
  Transform::IntegFunc integ;
  /*
  Xsection::xXsec Dyoshi( &cyoshi, 4, 6, 1.0E-4, 20.0, 0.0, 0.90 );
  Xsection::xzXsec Dyoshi2( &cyoshi, 4, 6, 1.0E-4, 20.0, 0.0, 0.90 );
  */
  //  /*
  Transform::InverseMellin Dyoshi( &cyoshi, integ.integration() );
  Dyoshi.setParameter( 0.0, 0.90 ); Dyoshi.upper( 20.0 );
  Transform::DInverseMellin Dyoshi2( &cyoshi, integ.integration() );
  std::vector< double > offset_( 2, 0.0 );
  std::vector< double > phi_( 2, 0.90 );
  Dyoshi2.setParameter( offset_, phi_ ); 
  Dyoshi2.upper( 20.0 );  Dyoshi2.integrand().upper( 20.0 );
  //  */
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
    cout << setw( 15 ) << sqrt( Dyoshi2( aaa, aaa ) ) << flush;
    //    cout << setw( 15 ) << sqrt( Dyoshi2( aaa, aaa ) ) << endl;
  }
  cout << endl;

  return 0;
}
