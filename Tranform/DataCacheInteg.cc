#include "DataCacheInteg.hh"

#include <iostream>
using namespace std;
using namespace Transform;

int DataCacheInteg::maxsize_ = -1;

// ---- sub classess : DataCacheInteg::Grid ---- //
DataCacheInteg::Grid::Grid( const DbKey& n, const double& q2 ) :
  n_( n ), Q2_( q2 )
{
}

DataCacheInteg::Grid::Grid( const Grid& grid ) :
  n_( grid.n_ ), Q2_( grid.Q2_ )
{
}

DataCacheInteg::Grid::~Grid(){
}

DataCacheInteg::Grid& DataCacheInteg::Grid::operator=( const Grid& grid ){
  if( this != &grid ){
    n_  = grid.n_;
    Q2_ = grid.Q2_;
  }
  return *this;
}

bool DataCacheInteg::Grid::operator==( const Grid& grid ) const {
  return ( n_ == grid.n_ &&  Q2_ == grid.Q2_ );
}

bool DataCacheInteg::Grid::operator< ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ < grid.Q2_ : n_ < grid.n_ );
}

bool DataCacheInteg::Grid::operator> ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ > grid.Q2_ : n_ > grid.n_ );
}

ostream& Transform::operator<<( ostream& os, const DataCacheInteg::Grid& grid )
{
  return ( os << "(" << grid.n_ << ", " << grid.Q2_ << ")" << flush );
}


// ---- DataCacheInteg ---- //
DataCacheInteg::DataCacheInteg() : 
  cache_( )
{
}

DataCacheInteg::DataCacheInteg( const DataCacheInteg& cache ) : 
  cache_( cache.cache_ )
{
}

DataCacheInteg::~DataCacheInteg() {
}

DataCacheInteg& DataCacheInteg::operator=( const DataCacheInteg& cache ){
  if( this != &cache ){
    cache_ = cache.cache_;
  }
  return *this;
}

bool DataCacheInteg::cached( const DataCacheInteg::Grid& grid ){
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

bool DataCacheInteg::cached( const DbKey& n, const double& q2 ) {
  return this->cached( Grid( n, q2 ) );
}

bool DataCacheInteg::cached( const complex< double >& n1, const double& x1, 
			     const double& q2 ) {
  return this->cached( DbKey( n1, x1 ), q2 );
}

vector< complex< double > >& 
DataCacheInteg::cache( const DbKey& n, const double& q2 ){
  return cache_[ Grid( n, q2 ) ];
}

vector< complex< double > >& 
DataCacheInteg::cache( const complex< double >& n1, const double& x1, 
		       const double& q2 ){
  return this->cache( DbKey( n1, x1 ), q2 );
}

void DataCacheInteg::cache( const DbKey& n, const double& q2,
			    vector< complex< double > >& data   ){
  if( cached( n, q2 ) ) return ;

  if( maxsize_ > 0 && cache_.size() > maxsize_ ) cache_.clear();
  
  vector< complex< double > >& d = cache_[ Grid( n, q2 ) ];
  d.resize( data.size() );
  for( int i = 0; i < data.size(); i++ ) d[ i ] = data[ i ];
  
}

void DataCacheInteg::cache( const complex< double >& n1, const double& x1,
			    const double& q2,
			    vector< complex< double > >& data   ){
  this->cache( DbKey( n1, x1 ), q2, data );
}

void DataCacheInteg::clear() {
  cache_.clear();
}

ostream& Transform::operator<<( ostream& os, DataCacheInteg& cache ){
  return os;
}
