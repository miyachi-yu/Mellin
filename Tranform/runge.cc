#include "RungeKutta.hh"
#include "RealFunction.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace Transform;

class TestDiff : public RungeKutta::Differential {
public:
  virtual ~TestDiff(){}
  virtual double operator()( const double& t, const double& y ) const {
    return  y;
  };  
};

class Exp : public RealFunction {
public:
  
  Exp( const double& x0 = 0.0, const double& y0 = 1.0) :
    x0_( x0 ), y0_( y0 ), dx_( 0.01 ), f_(), rg_( &f_ ) {
  }
  virtual ~Exp(){}
  
  virtual double operator()( const double& x ) {
    int nx = static_cast< int >( fabs( x - x0_ ) / dx_ );
    double y = y0_;
    for( int i = 0; i < nx; i++ ) y += rg_( y, x0_ + i * dx_, dx_ );
    return y;
  }
  
private:
  double x0_;
  double y0_;
  double dx_;
  TestDiff f_;
  RungeKutta rg_;
  
};

int main( int argc, char* argv[] ){
  
  Exp f;
  
  for( double x = 0.0; x < 10.0; x += 0.5 ){
    std::cout 
      << std::setw(10) << x
      << std::setw(14) << f( x )
      << std::setw(14) << exp( x )
      << std::endl;
  }
  
  return 0;
}
