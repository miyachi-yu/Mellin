#include "MCInt.hh"
#include "TrapInt.hh"
#include "GridIntegration.hh"

#include <iostream>
#include <iomanip>

using namespace std;

class F1 : public Transform::RealFunction {
public:
  
  F1( const double& a1 = 1.0, const double& b1 = 1.0 ) : 
    a( a1 ), b( b1 ), xp( 0 ) {
  }

  virtual ~F1(){}
  
  double operator()( const double& x ){
    //xp.push_back( x );
    // return sin(x) / x;
    //    return pow( x, a ) * exp( - b * x );
    return x*x;
  }
  
  double operator()( const double& x, const double& y ){
    //xp.push_back( x );
    // return sin(x) / x;
    //    return pow( x, a ) * exp( - b * x );
    return x*x + y*y;
  }
  
  double a;
  double b;
  
  vector< double > xp;
  
};

int main( int argc, char* argv[] ){
  
  Transform::MCInt   mcint;
  Transform::TrapInt trap;
  F1 f1;
  
  double min = 0.0;
  double max = 1.0;
  
  cout << setw(12) << mcint.integral( min, max, f1 )
       << setw(12) << trap.integral( min, max, f1 )
       << endl;

  cout << "------------------------------------------------------" << endl;
  
  for( int i = 100; i < 100000; i+=100 ){
    cout << setw(12) << i << flush;
    mcint.n() = i;
    cout << setw(12) << mcint.integral( min, max, f1 )
	 << setw(12) << trap.integral( min, max, f1 )
	 << endl;
  }

  cout << "------------------------------------------------------" << endl;
  
  for( int i = 100; i < 100000; i+=100 ){
    cout << setw(12) << i << flush;
    mcint.n() = i;
    cout << setw(12) << mcint.integral( min, max, min, max, f1 )
	 << setw(12) << trap.integral( min, max, min, max, f1 )
	 << endl;
  }
  
  
  cout << "------------------------------------------------------" << endl;
  
  Transform::MCInt mc1(10);
  Transform::MCInt mc2(20);
  Transform::GridIntegration grid( &mc1, &mc2, 2, 0.001 );
  cout << setw(12) << grid.integral( min, max, f1 )
       << setw(12) << trap.integral( min, max, f1 )
       << setw(12) << grid.integral( min, max, min, max, f1 )
       << endl;
  
  
  return 0;
}
