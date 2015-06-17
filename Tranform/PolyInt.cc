/*!
  \brief  PolyInt class implementation
 */
#include "PolyInt.hh"
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

PolyInt::PolyInt( const int& K ) :
  RealFunction(),
  map< double, double >(),
  K_( K ),
  calculated_( false ), x_( 0.0 ), f_( 0.0 ), df_( 0.0 ),
  c_( 0 ), d_( 0 ), xa_( 0 )
{
}

PolyInt::PolyInt( const PolyInt& poly ) :
  RealFunction(),
  map< double, double >( poly ),
  K_( poly.K_ ),
  calculated_( poly.calculated_ ),
  x_( poly.x_ ), f_( poly.f_ ), df_( poly.df_ ),
  c_( poly.c_ ), d_( poly.d_ ), xa_( poly.xa_ )
{
}

PolyInt::~PolyInt() {
}

PolyInt& PolyInt::operator=( const PolyInt& poly ){
  if( this != &poly ){
    this->clear();
    this->insert( poly.begin(), poly.end() );
    K_ = poly.K_;
    calculated_ = poly.calculated_;
    x_ = poly.x_; 
    f_ = poly.f_; 
    df_ = poly.df_;
    c_ = poly.c_; 
    d_ = poly.d_; 
    xa_ = poly.xa_;
  }
  return *this;
}

/*
  Try to find nearest point, and copy stored information 
  for interpolation
*/
int PolyInt::nearest() {
  
  // find iterator just above x_
  PolyInt::iterator str = this->upper_bound( x_ ); 
  
  // move iteator forward by half of K_
  for( int i = 0; i < K_ / 2 && str != this->end() ;  i++ ) str++;
  for( int i = 0; i < K_ &&     str != this->begin(); i++ ) str--;
  
  // prepare grid points and initial value of C, D tables
  int ns = 0, idx = 0;
  double diff;
  xa_.clear(); c_.clear(); d_.clear();
  for( PolyInt::iterator itr = str; idx < K_ && itr != this->end();
       itr++, idx++ ){
    double d = fabs( x_ - itr->first );
    if( idx == 0 || d < diff ){ diff = d; ns = idx; }
    xa_.push_back( itr->first );
    c_.push_back( itr->second );
    d_.push_back( itr->second );
  }

  return ns;
}

/*
  To evaluate, C_{m,i}, D_{m,i}: m-th correction to i-th point.
  m runs from 1 to N - 1
  i runs from 0 to N - M - 1
  
  This implementation is based on sample code in 
  Numerical Receipt in C.
*/
double PolyInt::interpolate(){
  calculated_ = false;             // set calculation status   
  int ns = this->nearest();        // find nearest point
  double f = c_[ ns-- ];           // set the most corse value
  int n = xa_.size();
  for( int m = 1; m < n; m++ ){ 
    for( int i = 0; i < n - m; i++ ){
      if( xa_[ i ] == xa_[ i + m ] ) {
	cerr << __FILE__ << ":" << __LINE__ << "\tcalculation error" << endl;
	return 0.0;
      }
      double den = ( c_[ i + 1 ] - d_[ i ] ) / ( xa_[ i ] - xa_[ i + m ] );
      c_[ i ] = ( xa_[ i ]     - x_ ) * den;
      d_[ i ] = ( xa_[ i + m ] - x_ ) * den;
    }
    f += ( df_ = ( 2 * ns < ( n - m ) ? c_[ ns + 1 ] : d_[ ns-- ] ) );
  }
  calculated_ = true;
  return f;
}

double PolyInt::operator()( const double& x ){
  if( this->size() == 0 ) return 0.0;
  if( x == x_ && calculated_ ) return f_;
  x_ = x; 
  return f_ = this->interpolate();
}

double PolyInt::f( const double& x ) {
  if( x != x_ || ! calculated_ ) return (*this)( x );
  return f_;
}

double PolyInt::df( const double& x ) {
  if( x != x_ || ! calculated_ ) (*this)( x );
  return df_;
}
