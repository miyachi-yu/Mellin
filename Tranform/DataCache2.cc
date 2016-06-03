// $Id: DataCache2.cc,v 1.4 2009/05/12 05:37:58 miyachi Exp $
/*!
  \file    DataCache2.cc
  \version $Revision: 1.4 $
  \date    $Date: 2009/05/12 05:37:58 $
  \author  $Author: miyachi $
  \brief   Data cache class implementation
*/
#include "DataCache2.hh"

#include <iostream>
using namespace std;
using namespace Transform;

int DataCache2::maxsize_ = -1;

// ---- sub classess : DataCache2::Grid ---- //
DataCache2::Grid::Grid( const complex< double >& n, 
			const complex< double >& m, 
			const double& q2 ) :
  n_( n ), m_( m ), Q2_( q2 )
{
}

DataCache2::Grid::Grid( const Grid& grid ) :
  n_( grid.n_ ), m_( grid.m_ ), Q2_( grid.Q2_ )
{
}

DataCache2::Grid::~Grid(){
}

DataCache2::Grid& DataCache2::Grid::operator=( const Grid& grid ){
  if( this != &grid ){
    n_  = grid.n_;
    m_  = grid.m_;
    Q2_ = grid.Q2_;
  }
  return *this;
}

bool DataCache2::Grid::operator==( const Grid& grid ) const {
  return ( n_ == grid.n_ && m_ == grid.m_ &&  Q2_ == grid.Q2_ );
}

bool DataCache2::Grid::operator< ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ < grid.Q2_ : 
	   ( m_  != grid.m_ ? m_ < grid.m_ : n_ < grid.n_ ) );
}

bool DataCache2::Grid::operator> ( const Grid& grid ) const {
  return ( Q2_ != grid.Q2_  ? Q2_ > grid.Q2_ : 
	   ( m_  != grid.m_ ? m_ > grid.m_ : n_ > grid.n_ ) );
}

ostream& Transform::operator<<( ostream& os, const DataCache2::Grid& grid ){
  return ( os << "(" << grid.n_ << ", " << grid.Q2_ << ")" << flush );
}


// ---- DataCache2 ---- //
DataCache2::DataCache2( const bool& newmeth ) : 
  cache_( ), cnew_( ), newmeth_( newmeth ), ndata_( 0 )
{
}

DataCache2::~DataCache2() {
}

bool DataCache2::cached( const DataCache2::Grid& grid ){
  
  if( newmeth_ ){
    
    map< double, map< CKey, map< CKey, vector< complex< double > > > > >::iterator 
      itr = cnew_.find( grid.Q2() );
    if( itr == cnew_.end() ) return false;
    
    map< CKey, map< CKey, vector< complex< double > > > >::iterator 
      jtr = itr->second.find( grid.n() ) ;
    if( jtr == itr->second.end() ) return false;
    
    map< CKey, vector< complex< double > > >::iterator ktr = jtr->second.find( grid.m() );
    return ( ktr == jtr->second.end() ? false : true );
    
  }
  map< Grid, vector< complex< double > > >::iterator itr;
  itr = cache_.find( grid );
  return ( itr == cache_.end() ? false : true );
}

bool DataCache2::cached( const complex< double >& n, 
			 const complex< double >& m, 
			 const double& q2 ) {
  return this->cached( Grid( n, m, q2 ) );
}

vector< complex< double > >& 
DataCache2::cache( const complex< double >& n, 
		   const complex< double >& m, 
		   const double& q2 ){
  return ( newmeth_ ? cnew_[ q2 ][ n ][ m ] : cache_[ Grid( n, m, q2 ) ] );
  //  vector< complex < double > >& data = 
  //    cache_[ Grid( n, m, q2 ) ];
  //  cout << "cache: " << flush;
  //  for( int i = 0; i < data.size(); i++ ) cout << data[ i ] << " " << flush;
  //  cout << endl;
  //  return cache_[ Grid( n, m, q2 ) ];
}

void DataCache2::cache( const complex< double >& n, 
			const complex< double >& m, 
			const double& q2,
			const complex< double >& data   ){
  vector< complex< double > > vd( 1, data );
  this->cache( n, m, q2, vd );
}


void DataCache2::cache( const complex< double >& n, 
			const complex< double >& m, 
			const double& q2,
			vector< complex< double > >& data   ){
  
  if( cached( n, m, q2 ) ) return ;
  
  if( maxsize_ > 0 && ndata_ > maxsize_ ) this->clear();

  vector< complex< double > >& d = 
    ( newmeth_ ? cnew_[ q2 ][ n ][ m ] : cache_[ Grid( n, m, q2 ) ] );
  
  d.resize( data.size() );
  for( int i = 0; i < data.size(); i++ ) d[ i ] = data[ i ];

  ndata_++;
  
}

void DataCache2::clear() {
  if( newmeth_ ){
    
    for( map< double, map< CKey, map< CKey, vector< complex< double > > > > >::iterator 
	   itr = cnew_.begin();
	 itr != cnew_.end();
	 itr++ ){
      for( map< CKey, map< CKey, vector< complex< double > > > >::iterator 
	     jtr = itr->second.begin();
	   jtr != itr->second.end();
	   jtr++ ){
	for( map< CKey, vector< complex< double > > >::iterator ktr = jtr->second.begin();
	     ktr != jtr->second.end();
	     ktr++ ){
	  ktr->second.resize( 0 );
	}
	jtr->second.clear();
      }
      itr->second.clear();
    }
    cnew_.clear();
  } else {
    for( map< Grid, vector< complex< double > > >::iterator itr = cache_.begin();
	 itr != cache_.end();
	 itr++ ){
      itr->second.resize( 0 );
    }
    cache_.clear();
  }
  ndata_ = 0;
}

ostream& Transform::operator<<( ostream& os, DataCache2& cache ){
  return os;
}
