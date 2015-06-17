#include "Laguerre.hh"

using namespace std;
using namespace Transform;

Laguerre::Laguerre( const int& n, const double& alpha ) :
  f1_( - n, alpha + 1.0 ), prefix_( 0.0 ), gamma_(), 
  a_( alpha ), n_( n ),
  x_( 0 ), w_( 0 )
{
  this->set( n, alpha );
}

Laguerre::~Laguerre(){
}

void Laguerre::set( const int& n, const double& alpha ){
  double a = alpha + 1.0;
  prefix_ = ( gamma_( n + a ) / gamma_( n + 1.0 ) / gamma_( a ) ).real();
  f1_.set( - n, a );
  n_ = n;
  a_ = alpha;
  this->getAbsissa();
}

double Laguerre::operator()( const double& x ){
  return prefix_ * f1_( x ).real();
}

void Laguerre::getAbsissa(){
  
  // clear array
  x_.resize( 0 );
  
  // at first, determine absissa....
  double x0 = 0.0;
  for( int i = 0; i < n_; i++ ){
    if( ( x0 = this->findZeroCross( x0 ) ) < 0.0 ) break;
    x_.push_back( x0 ); 
  }
  
  // resize weight array
  w_.resize( x_.size() );
  
  // modify myself for n - 1 
  double pback = prefix_;
  prefix_ = ( gamma_( n_ + a_ ) / gamma_( n_ ) / gamma_( a_ + 1.0 ) ).real();
  f1_.set( - n_ + 1.0, a_ + 1.0 );
  
  for( int i = 0; i < x_.size(); i++ ){
    double l = (*this)( x_[i] );             // L_{n-1}^{alpha}( x_i )
    w_[ i ]  = ( gamma_( n_ + a_ ) ).real() * x_[ i ];
    w_[ i ] /= ( gamma_( n_ + 1.0 ) ).real() * ( n_ + a_ ) * l * l;
  }
  
  // set back
  prefix_ = pback;
  f1_.set( - n_, a_ + 1.0 );
  
}

double Laguerre::findZeroCross( double x, const double& dx ){
  static double max = 1000.0;
  double current = (*this)( x );
  double next;
  
  while( current * ( next = (*this)( x += dx ) ) > 0.0 ) {
    if( x > max ) return -max;
    current = next;
  }
  return ( dx < 1.0E-5 ? x : this->findZeroCross( x - dx, 0.1 * dx ) );
}
