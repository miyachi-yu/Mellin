#include "DataCacheInteg2.hh"

#include <iostream>
using namespace std;
using namespace Transform;

int DataCacheInteg2::maxsize_ = -1;

// ---- sub classess : DataCacheInteg2::Grid ---- //
DataCacheInteg2::Grid::Grid( const DbKey2& n, const double& q2 ) :
  n_( n ), Q2_( q2 )
{
}

DataCacheInteg2::Grid::Grid( const Grid& grid ) :
  n_( grid.n_ ), Q2_( grid.Q2_ )
{
}

DataCacheInteg2::Grid::~Grid(){
}

DataCacheInteg2::Grid& DataCacheInteg2::Grid::operator=( const Grid& grid ){
  if( this != &grid ){
    n_  = grid.n_;
    Q2_ = grid.Q2_;
  }
  return *this;
}

bool DataCacheInteg2::Grid::operator==( const Grid& grid ) const {
  return ( n_ == grid.n_ &&  Q2_ == grid.Q2_ );
}

bool DataCacheInteg2::Grid::operator< ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ < grid.Q2_ : n_ < grid.n_ );
}

bool DataCacheInteg2::Grid::operator> ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ > grid.Q2_ : n_ > grid.n_ );
}

ostream& Transform::operator<<( ostream& os, const DataCacheInteg2::Grid& grid )
{
  return ( os << "(" << grid.n_ << ", " << grid.Q2_ << ")" << flush );
}


// ---- DataCacheInteg2 ---- //
DataCacheInteg2::DataCacheInteg2() : 
  cache_( )
{
}

DataCacheInteg2::DataCacheInteg2( const DataCacheInteg2& cache ) : 
  cache_( cache.cache_ )
{
}

DataCacheInteg2::~DataCacheInteg2() {
}

DataCacheInteg2& DataCacheInteg2::operator=( const DataCacheInteg2& cache ){
  if( this != &cache ){
    cache_ = cache.cache_;
  }
  return *this;
}

bool DataCacheInteg2::cached( const DataCacheInteg2::Grid& grid ){
  map< Grid, vector< complex< double > > >::iterator itr;
  /*
  for( itr = cache_.begin(); itr != cache_.end(); itr++ )
    if( itr->first == grid ) {
      cout << grid << "\t cached as " << itr->first << endl;
      return true;
    }
  return false;
  */
  itr = cache_.find( grid );
  //if( itr != cache_.end() ) cout << grid << "\t as " << itr->first << endl;
  return ( itr == cache_.end() ? false : true );
}

bool DataCacheInteg2::cached( const DbKey2& n, const double& q2 ) {
  return this->cached( Grid( n, q2 ) );
}

bool DataCacheInteg2::cached( const complex< double >& n1, 
			      const complex< double >& n2, 
			      const double& x1, const double& x2, 
			      const double& q2 ) {
  return this->cached( DbKey2( n1, n2, x1, x2 ), q2 );
}

vector< complex< double > >& 
DataCacheInteg2::cache( const DbKey2& n, const double& q2 ){
  return cache_[ Grid( n, q2 ) ];
}

vector< complex< double > >& 
DataCacheInteg2::cache( const complex< double >& n1, 
			const complex< double >& n2, 
			const double& x1, const double& x2, 
			const double& q2 ){
  return this->cache( DbKey2( n1, n2, x1, x2 ), q2 );
}

void DataCacheInteg2::cache( const DbKey2& n, const double& q2,
			    vector< complex< double > >& data   ){
  if( cached( n, q2 ) ) return ;

  if( maxsize_ > 0 && cache_.size() > maxsize_ ) cache_.clear();
  
  vector< complex< double > >& d = cache_[ Grid( n, q2 ) ];
  d.resize( data.size() );
  for( int i = 0; i < data.size(); i++ ) d[ i ] = data[ i ];
  
}

void DataCacheInteg2::cache( const complex< double >& n1, 
			     const complex< double >& n2, 
			     const double& x1, const double& x2, 
			     const double& q2,
			     vector< complex< double > >& data   ){
  this->cache( DbKey2( n1, n2, x1, x2 ), q2, data );
}

void DataCacheInteg2::clear() {
  cache_.clear();
}

ostream& Transform::operator<<( ostream& os, DataCacheInteg2& cache ){
  return os;
}
