// $Id: Li2Int.cc,v 1.7 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2Int.cc
  \version $Revision: 1.7 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Implementation of Integral function on Li2 ( case 1 )
 */
#include "Li2Int.hh"
#include "GridIntegration.hh"

#include <cstdlib>

using namespace std;
using namespace Transform;

CKey Li2Int::key;
complex< double > Li2Int::data( 0.0, 0.0 );
Utility::LookupTable< CKey, complex< double > > 
Li2Int::cache( Li2Int::key, Li2Int::data, "li2int.db" );

bool Li2Int::cacheReady = false;

void Li2Int::initCache() {
  if( Li2Int::cacheReady ) return;
  atexit( Li2Int::closeCache );
  Li2Int::cacheReady = true;
}

void Li2Int::closeCache() {
  Li2Int::cache.close();
}

Li2Int::Li2Int() : 
  IntegFunc()
{
  if( ! Li2Int::cacheReady ) Li2Int::initCache();
  //  dynamic_cast< GridIntegration* >( this->integration() )->precision( 1.0E-4 );
}

// ------------------------------------------------------------- //
complex< double > Li2Int::Integrand::operator()( const complex< double >& z ){
  double x = z.real();
  if( x == 0 ) return 0;
  return pow( x, n_ - 1.0 ) * li2_( x ) / ( 1.0 + x );
}

// ------------------------------------------------------------- //
complex< double > Li2Int::operator()( const complex< double >& n ){
  if( ! Li2Int::cache.get( Li2Int::key = CKey(n), Li2Int::data ) ) { 
    Li2Int::data = this->integration()->cintegral( 0, 1.0, fnc_.n( n ) );
    Li2Int::cache.put();
  }
  return Li2Int::data ;
}
