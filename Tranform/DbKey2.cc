#include "DbKey2.hh"

using namespace std;
using namespace Transform;

DbKey2::DbKey2() : 
  n1_( 0.0, 0.0 ), n2_( 0.0, 0.0 ), x1_( 0.0 ), x2_( 0.0 )
{
}

DbKey2::DbKey2( const std::complex< double >& n1, 
	      const std::complex< double >& n2,
	      const double& x1, const double& x2 ) : 
  n1_( n1 ), n2_( n2 ), x1_( x1 ), x2_( x2 )
{
}

DbKey2::DbKey2( const DbKey2& key ) :
  n1_( key.n1_ ), n2_( key.n2_ ), x1_( key.x1_ ), x2_( key.x2_ )
{
}
    
DbKey2::~DbKey2(){
}

DbKey2& DbKey2::operator=( const DbKey2& key ) {
  if( this != &key ){
    n1_ = key.n1_;
    n2_ = key.n2_;
    x1_ = key.x1_;
    x2_ = key.x2_;
  }
  return *this;
}

bool DbKey2::operator<( const DbKey2& key )  const {
  if( n1_ != key.n1_ ) return ( n1_ < key.n1_ );
  if( x1_ != key.x1_ ) return ( x1_ < key.x1_ );
  if( n2_ != key.n2_ ) return ( n2_ < key.n2_ );
  return ( x2_ < key.x2_ );
}

bool DbKey2::operator>( const DbKey2& key )  const {
  if( n1_ != key.n1_ ) return ( n1_ > key.n1_ );
  if( x1_ != key.x1_ ) return ( x1_ > key.x1_ );
  if( n2_ != key.n2_ ) return ( n2_ > key.n2_ );
  return ( x2_ > key.x2_ );
}

bool DbKey2::operator==( const DbKey2& key ) const {
  return ( n1_ != key.n1_ || n2_ != key.n2_ ||
	   x1_ != key.x1_ || x2_ != key.x2_ ? 
	   false : true );
}
