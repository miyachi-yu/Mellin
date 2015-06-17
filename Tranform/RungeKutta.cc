#include "RungeKutta.hh"
#include "RealFunction.hh"

using namespace std;
using namespace Transform;

RungeKutta::Differential::~Differential(){
}

RungeKutta::RungeKutta( Differential *f ) :
  f_( f ) {
}

RungeKutta::~RungeKutta() {
}

double RungeKutta::operator()( const double& y, const double& t, 
			       const double& dt ) const {
  double dt2 = 0.5 * dt;
  double k1  = dt * (*f_)( t,       y );
  double k2  = dt * (*f_)( t + dt2, y + 0.5 * k1 );
  double k3  = dt * (*f_)( t + dt2, y + 0.5 * k2 );
  double k4  = dt * (*f_)( t + dt,  y + k3 );
  
  return ( k1 + 2.0 * k2 + 2.0 * k3 + k4 ) / 6.0 ;
}
