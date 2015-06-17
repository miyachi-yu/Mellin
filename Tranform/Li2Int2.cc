// $Id: Li2Int2.cc,v 1.5 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2Int2.cc
  \version $Revision: 1.5 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Implementation of Integral function on Li2 ( case 2 )
 */
#include "Li2Int2.hh"

#include <cstdlib>
using namespace std;
using namespace Transform;

CKey Li2Int2::key;
complex< double > Li2Int2::data( 0.0, 0.0 );
Utility::LookupTable< CKey, complex< double > > 
Li2Int2::cache( Li2Int2::key, Li2Int2::data, "li2int2.db" );

bool Li2Int2::cacheReady = false;

void Li2Int2::initCache() {
  if( Li2Int2::cacheReady ) return;
  atexit( Li2Int2::closeCache );
  Li2Int2::cacheReady = true;
}

void Li2Int2::closeCache() {
  Li2Int2::cache.close();
}

Li2Int2::Li2Int2() {
  if( ! Li2Int2::cacheReady ) Li2Int2::initCache();
}

complex< double > Li2Int2::Integrand::operator()( const complex< double >& z ){
  double x = z.real();
  if( x == 0 ) return 0;
  return ( pow( x, n_ ) - 1.0 ) * li2_( x ) / ( x - 1 );
}

complex< double > Li2Int2::operator()( const complex< double >& n ){
  if( ! Li2Int2::cache.get( Li2Int2::key = CKey(n), Li2Int2::data ) ) { 
    Li2Int2::data = this->integration()->cintegral( 0, 1.0, fnc_.n( n ) );
    Li2Int2::cache.put();
  }
  return Li2Int2::data ;
}
