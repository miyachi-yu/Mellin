#include "LaguerreQuadrature.hh"

using namespace std;
using namespace Transform;


LaguerreQuadrature::LaguerreQuadrature( const int& n, const double& alpha ) :
  lag_( n, alpha )
{
}

LaguerreQuadrature::~LaguerreQuadrature( ){
}

double LaguerreQuadrature::integral( const double& min, const double& max, 
				      RealFunction& f ){
  
  vector< double >& x = lag_.absissa();
  vector< double >& w = lag_.weight();
  double v = 0.0;
  for( int i = 0; i < x.size(); i++ ) v += w[ i ] * f( x[ i ] );
  return v;
}

complex< double > 
LaguerreQuadrature::cintegral( const double& min, const double& max, 
				ComplexFunction& f ){
  vector< double >& x = lag_.absissa();
  vector< double >& w = lag_.weight();
  complex< double > v( 0.0, 0.0 );
  for( int i = 0; i < x.size(); i++ ) v += w[ i ] * f( x[ i ] );
  return v;
}

double LaguerreQuadrature::integral( const double& xmin, const double& xmax, 
				      const double& zmin, const double& zmax, 
				      RealFunction& f ) {
  vector< double >& x = lag_.absissa();
  vector< double >& w = lag_.weight();
  double v = 0.0;
  for( int i = 0; i < x.size(); i++ ) 
    for( int j = 0; j < x.size(); j++ ) 
      v += w[ i ] * w[ j ] * f( x[ i ], x[ j ] );
  return v;
}
    
complex< double > 
LaguerreQuadrature::cintegral( const double& xmin, const double& xmax, 
				const double& zmin, const double& zmax, 
				ComplexFunction& f ){
  vector< double >& x = lag_.absissa();
  vector< double >& w = lag_.weight();
  complex< double > v = 0.0;
  for( int i = 0; i < x.size(); i++ ) 
    for( int j = 0; j < x.size(); j++ ) 
      v += w[ i ] * w[ j ] * f( x[ i ], x[ j ] );
  return v;
}


