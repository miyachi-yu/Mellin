#include "RealFunction.hh"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Transform;

DEF_REALFUNC( F1 );
double F1::operator()( const double& x ){
  return pow( x - 0.5, 2 ) + 1.0;
}

DEF_REALFUNC( F2 );
double F2::operator()( const double& x ){
  return cos( x );
}

DEF_REALFUNC( F3 );
double F3::operator()( const double& x ){
  return exp( x ) + exp( -x );
}


class F4 : public Transform::RealFunction {
  
  virtual double operator()( const double& x ){
    return pow( x, 3 );
  }
  
  virtual double operator()( const double& x, const double& z ){
    return pow( x, 3 ) * pow( z, 4 );
  }

};


int main( int argc, char* argv[] ){
  
  F1 f1;
  F2 f2;
  F3 f3;

  cout << f1.minimum( 0.0 ) << endl;
  cout << f2.minimum( 0.0 ) << endl;
  cout << f3.minimum( 10.0 ) << endl;
  
  { // test for 2D differential
    F4 f4;
    
    cout 
      << "differential test...." << endl
      << setw(12) << f4.differential( 1.0, 1.0, 0, 1, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 1, 0, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 1, 1, 0.01, 0.01 ) << endl
      << setw(12) << f4.differential( 1.0, 1.0, 0, 2, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 1, 2, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 2, 2, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 2, 1, 0.01, 0.01 ) << flush
      << setw(12) << f4.differential( 1.0, 1.0, 2, 0, 0.01, 0.01 ) << flush
      << endl;


  }
  
  
  return 0;
}
