/*!

  Mellin Inversion test code.
  Let's try to perform Mellin inversion on the some known complex function.
  Here CBeta will be used, because the relation is known as written in
  the explanation of CBeta class.


  $Id: doubleInversion.cc,v 1.7 2008/08/25 09:47:14 miyachi Exp $
 */
#include "InverseMellin.hh"
#include "DInverseMellin.hh"
#include "GridIntegration.hh"
#include "LegQuadrature.hh"
#include "CBeta.hh"
#include <Utility/Timer.hh>
#include <Utility/Arguments.hh>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

class Func : public ComplexFunction {
public:
  Func() : beta_( 1.0, 1.0 ) {}
  virtual ~Func() {}
  
  virtual complex< double > operator()( const complex< double >& n ){
    return beta_( n );
  }
  
  virtual complex< double > operator()( const complex< double >& n,
				const complex< double >& m ){
    return beta_( n ) * beta_( m );
  }
  
  virtual double real( const double& x  ){
    return x * ( 1.0 - x );
  }
  
  virtual double real( const double& x, const double& z  ){
    return x * ( 1.0 - x ) * z * ( 1.0 - z );
  }
  
private:
  CBeta beta_;
  
};

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  Func f;
  
  GridIntegration integ( new LegQuadrature( 4 ), new LegQuadrature( 6 ),
			 4, 1.0E-4 );
  
  double offset = args.get( "offset", 2.0 );
  double angle  = args.get( "angle", 0.55 );
  double length = args.get( "length", 20.0 );
  
  vector< double > c( 2, offset );
  vector< double > phi( 2, angle );
  
  InverseMellin inv( &f, &integ );
  inv.setParameter( c, phi );
  inv.upper( length );
  
  DInverseMellin dinv( &f, &integ );
  dinv.setParameter( c, phi );
  dinv.upper( length );
  
  //  if( 0 )
  {

    for( double x = 0.1 ; x < 0.95; x += 0.1 ){
      
      cout 
	<< setw(4) << x
	<< setw(12) << f.real( x )
	<< setw(12) << inv( x )
	<< endl;
      
    }
  }
  
  //  if( 0 ) 
    {

    Utility::Timer timer;
    
    for( double x = 0.1 ; x < 0.95; x += 0.1 ){
      
      cout << "--- x = " << setw(4) << x << endl;
      for( double z = 0.1 ; z < 0.95; z += 0.1 ){
	cout << setw(12) << z << flush;
      }	
      cout << endl;
      
      // reference
      for( double z = 0.1 ; z < 0.95; z += 0.1 ){
	cout << setw(12) << f.real( x, z ) << flush;
      }	
      cout << endl;
      
      // test for InverseMellin
      timer.start(); 
      for( double z = 0.1 ; z < 0.95; z += 0.1 ){
	cout << setw(12) << inv( x, z ) << flush;
      }
      cout << setw(12) << timer.check( Utility::Timer::Msec ) << endl;
      
      // test for DInverseMellin
      timer.start();
      for( double z = 0.1 ; z < 0.95; z += 0.1 ){
	vector< double > xv( 1, x );
	vector< double > zv( 1, z );
	cout << setw(12) << dinv( xv, zv ) << flush;
      }
      cout << setw(12) << timer.check( Utility::Timer::Msec ) << endl;
      
    }
    
  }
  
  {
    
    vector< double > zrange;
    if( args.get( "zmax", 0.8 ) == 0.0 ){
      zrange.resize( 1 ); 
    } else {
      zrange.resize( 2 );
      zrange[ 1 ] = args.get( "zmax", 0.8 );
    }
    zrange[ 0 ] = args.get( "zmin", 0.1 );
    
    cout << "-----------------------------------------------------------------\n" 
	 << "-   Partial integral check\n"
	 << "-   zmin: " << setw(6) << zrange[0] << endl
	 << "-   zmax: " << setw(6) << zrange[1] << endl
	 << "-----------------------------------------------------------------\n"
	 << endl;

    cout << endl;
    for( double x = 0.1 ; x < 0.95; x += 0.1 ){
      cout << setw(12) << x << flush;
    }	
    cout << endl;

    Utility::Timer timer;
    
    {
      timer.start();
      for( double x = 0.1 ; x < 0.95; x += 0.1 ){
	vector< double > xrange( 1, x );
	cout << setw(12) << inv( xrange, zrange ) << flush;
      }
      cout << setw(12) << timer.check( Utility::Timer::Msec ) << endl;
    }
    
    {
      timer.start();
      for( double x = 0.1 ; x < 0.95; x += 0.1 ){
	vector< double > xrange( 1, x );
	cout << setw(12) << dinv( xrange, zrange ) << flush;
      }
      cout << setw(12) << timer.check( Utility::Timer::Msec ) << endl;
    }
    
  }
  delete integ.method1();
  delete integ.method2();
  
  return 0;
};
