#include "DbKey.hh"

using namespace std;
using namespace Transform;

DbKey::DbKey() : 
  n1_( 0.0, 0.0 ), x1_( 0.0 )
{
}

DbKey::DbKey( const std::complex< double >& n1, const double& x1 ) : 
  n1_( n1 ), x1_( x1 )
{
}

DbKey::DbKey( const DbKey& key ) :
  n1_( key.n1_ ), x1_( key.x1_ )
{
}
    
DbKey::~DbKey(){
}

DbKey& DbKey::operator=( const DbKey& key ) {
  if( this != &key ){
    n1_ = key.n1_;
    x1_ = key.x1_;
  }
  return *this;
}

bool DbKey::operator<( const DbKey& key )  const {
  if( n1_ != key.n1_ ) return ( n1_ < key.n1_ );
  return ( x1_ < key.x1_ );
}

bool DbKey::operator>( const DbKey& key )  const {
  if( n1_ != key.n1_ ) return ( n1_ > key.n1_ );
  return ( x1_ > key.x1_ );
}

bool DbKey::operator==( const DbKey& key ) const {
  return ( n1_ != key.n1_ || x1_ != key.x1_ ? 
	   false : true );
}
