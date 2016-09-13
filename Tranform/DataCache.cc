// $Id: DataCache.cc,v 1.7 2009/05/12 05:37:58 miyachi Exp $
/*!
  \file    DataCache.cc
  \version $Revision: 1.7 $
  \date    $Date: 2009/05/12 05:37:58 $
  \author  $Author: miyachi $
  \brief   Data cache class implementation
*/
#include "DataCache.hh"

#include <iostream>
using namespace std;
using namespace Transform;

int DataCache::maxsize_ = -1;

// ---- sub classess : DataCache::Grid ---- //
DataCache::Grid::Grid( const complex< double >& n, const double& q2 ) :
  n_( n ), Q2_( q2 )
{
}

DataCache::Grid::Grid( const Grid& grid ) :
  n_( grid.n_ ), Q2_( grid.Q2_ )
{
}

DataCache::Grid::~Grid(){
}

DataCache::Grid& DataCache::Grid::operator=( const Grid& grid ){
  if( this != &grid ){
    n_  = grid.n_;
    Q2_ = grid.Q2_;
  }
  return *this;
}

bool DataCache::Grid::operator==( const Grid& grid ) const {
  return ( n_ == grid.n_ &&  Q2_ == grid.Q2_ );
}

bool DataCache::Grid::operator< ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ < grid.Q2_ : n_ < grid.n_ );
}

bool DataCache::Grid::operator> ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ > grid.Q2_ : n_ > grid.n_ );
}

// ---- DataCache ---- //
DataCache::DataCache() : 
  cache_( )
{
}

DataCache::DataCache( const DataCache& cache ) : 
  cache_( cache.cache_ )
{
}

DataCache::~DataCache() {
}

DataCache& DataCache::operator=( const DataCache& cache ){
  if( this != &cache ){
    cache_ = cache.cache_;
  }
  return *this;
}

bool DataCache::cached( const DataCache::Grid& grid ){
  map< Grid, vector< complex< double > > >::iterator itr;
  itr = cache_.find( grid );
  return ( itr == cache_.end() ? false : true );
}

bool DataCache::cached( const complex< double >& n, const double& q2 ) {
  return this->cached( Grid( n, q2 ) );
}

vector< complex< double > >& 
DataCache::cache( const complex< double >& n, const double& q2 ){
  //  vector< complex < double > >& data = cache_[ Grid( n, q2 ) ];
  //  cout << "cache: " << flush;
  //  for( int i = 0; i < data.size(); i++ ) cout << data[ i ] << " " << flush;
  //  cout << endl;
  return cache_[ Grid( n, q2 ) ];
}

void DataCache::cache( const complex< double >& n, const double& q2,
		       vector< complex< double > >& data   ){
  if( cached( n, q2 ) ) return ;

  if( maxsize_ > 0 && cache_.size() > maxsize_ ) cache_.clear();
  
  vector< complex< double > >& d = cache_[ Grid( n, q2 ) ];
  d.resize( data.size() );
  for( int i = 0; i < data.size(); i++ ) d[ i ] = data[ i ];
  
}

void DataCache::clear() {
  for( map< Grid, vector< complex< double > > >::iterator itr = cache_.begin();
       itr != cache_.end();
       itr++ ){
    itr->second.resize( 0 );
    //    itr->second.clear();
  }
  cache_.clear();
}

namespace Transform {
  
  ostream& operator<<( ostream& os, const DataCache::Grid& grid ){
    return ( os << "(" << grid.n_ << ", " << grid.Q2_ << ")" << flush );
  }
  ostream& operator<<( ostream& os, DataCache& cache ){
    return os;
  }

}
