/*!
  \brief  class implementation for Romberg integration
 */
#include "Romberg.hh"
#include "PolyInt.hh"

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

Romberg::Romberg( const int& K, const double& precision ) :
  trap_(), jmax_( 30 ), K_( K ), eps_( precision )
{
}

Romberg::~Romberg() {
}

/*
  Implementation is based on Numerical Receipt in C
*/
double Romberg::integral( const double& min, const double& max, 
			  RealFunction& f ) {
  PolyInt pl( K_ );        // Polynominal Interpolation
  double x = 1.0;
  for( int j = 1; j <= jmax_; j++ ){
    pl[ x ] = trap_.integral( min, max, f, j );
    if( j > K_ ){          // Perfom extrapolation to infinit grid number
      pl.calculated( false );
      double ss  = pl( 0.0 );
      double dss = pl.df( 0.0 );
      if( fabs( dss ) < eps_ * fabs( ss ) ) return ss;
    }
    x *= 0.25;
  }
  cerr << __FILE__ << ":" << __LINE__ << "\texceed maximum iteration .... "
       << endl;
  return 0.0;
}

complex< double > Romberg::cintegral( const double& min, const double& max, 
		    ComplexFunction& f ){
  return complex< double >( 0.0, 0.0 );
}

