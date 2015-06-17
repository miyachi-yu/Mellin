#include "ScaleFactor.hh"

#include <cmath>

using namespace std;

ScaleFactor::ScaleFactor() :
  vector< double >( 0 ),
  min_( 0 ), max_( 0 )
{
  this->push_back( 0.98000 );
  this->push_back( 0.61250 );
  this->push_back( 0.41800 );
  this->push_back( 0.29550 );
  this->push_back( 0.20900 );
  this->push_back( 0.14750 );
  this->push_back( 0.09975 );
  this->push_back( 0.06325 );
  this->push_back( 0.04180 );
  this->push_back( 0.02955 );
  this->push_back( 0.01930 );
  this->push_back( 0.01020 );
  this->push_back( 0.00240 );
  
  min_.resize( this->size(), 0.0 );
  max_.resize( this->size(), 0.0 );
  
  double lq2max = 2.4;
  double lq2min = 0.6;
  double step   = 0.15;

  for( int i = 0; i < this->size(); i++ ){
    
    min_[ i ] = pow( 10.0, lq2min - step *  i );
    max_[ i ] = pow( 10.0, lq2max - step *  i );
    
    min_[ i ] = ( min_[ i ] < 1.0 ? 1.0 : min_[ i ] );
    max_[ i ] = ( max_[ i ] < 1.0 ? 1.0 : max_[ i ] );
    
  }
}

ScaleFactor::~ScaleFactor() {
}

int ScaleFactor::index( const double& x ){
  int i;
  for( i = 0; i < this->size(); i++ ) if( x > (*this)[ i ] ) break;
  return i;
}

double ScaleFactor::offset( const double& x ){
  double step = 1.2;
  return step * this->index( x );
}

double ScaleFactor::q2min( const double& x ){
  return min_[ this->index( x ) ];
}

double ScaleFactor::q2max( const double& x ){
  return max_[ this->index( x ) ];
}
