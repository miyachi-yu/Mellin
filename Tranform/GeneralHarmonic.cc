// $Id: GeneralHarmonic.cc,v 1.4 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file     GeneralHarmonic.cc
  \version  $Revision: 1.4 $
  \date     $Date: 2008/12/02 08:13:16 $
  \author   $Author: miyachi $
  \brief    Implementation of General Harmonic Number function class
*/
#include "GeneralHarmonic.hh"
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

double GeneralHarmonic::zeta[4] = { 0.0, 0.577216, M_PI*M_PI/6.0, 1.202057 };
CKey GeneralHarmonic::key[4];
complex< double > GeneralHarmonic::data[4];
Utility::LookupTable< CKey, complex< double > >* GeneralHarmonic::cache[4] = {
  NULL, NULL, NULL, NULL
};
bool GeneralHarmonic::cacheReady = false;

void GeneralHarmonic::initCache() {
  if( GeneralHarmonic::cacheReady ) return ;
  for( int i=0; i<4; i++ ){
    ostringstream dbname;
    dbname << "GeneralHarmonic" << i << ".db" ;
    GeneralHarmonic::cache[ i ] = 
      new Utility::LookupTable< CKey, complex< double > >( GeneralHarmonic::key[i],
						  GeneralHarmonic::data[i],
						  dbname.str().c_str() );
  }
  atexit( GeneralHarmonic::closeCache );
  GeneralHarmonic::cacheReady = true;
}

void GeneralHarmonic::closeCache(){
  for( int i=0; i<4; i++ ){
    GeneralHarmonic::cache[ i ]->close();
    delete GeneralHarmonic::cache[ i ];
  }
}

GeneralHarmonic::GeneralHarmonic() : 
  pg_( 1 ), li2int_() {
  if( ! GeneralHarmonic::cacheReady ) GeneralHarmonic::initCache();
}

GeneralHarmonic::~GeneralHarmonic() {
}

complex< double > 
GeneralHarmonic::operator() ( const int& m, 
			      const complex< double >& n ) {
  
  Utility::LookupTable< CKey, complex< double > >& c = *( GeneralHarmonic::cache[ m ] );
  CKey&                                   k = GeneralHarmonic::key[ m ];
  complex< double >&                      d = GeneralHarmonic::data[ m ];
  
  if( c.get( k = CKey( n ), d ) ) return d;
  
  d = 0.0;
  if( n.imag() == 0.0 && n.real() > 0.0 && 
      ( fmod( n.real(), 1.0 ) == 0.0 ||  fmod( n.real(), 1.0 ) == 1.0 ) ){
    for( double j = 1 ; j <= n.real(); j++ ) d += pow( j, -m );
  } else {
    d = zeta[ m ] 
      + pow( -1.0, m - 1 ) * pg_( m - 1, n + 1.0 ) 
      * ( m == 3 ? 0.5 : 1.0 );
  }
  c.put();                                               //  store to cache
  
  return d;
}

complex< double > GeneralHarmonic::operator()( const int& i, const int& j,
					       const complex< double >& n ){
  GeneralHarmonic& S = *this;
  
  
  if( i == 1 && j == 2 ){
    return S( 1, n ) * S( 2, n ) + S( 3, n ) - S( 2, 1, n );
  }
  
  if( i == 2 && j == 1 ){
    return zeta[ 2 ] * S( 1, n ) - li2int_( n );
  }
  
  return complex< double >( 0.0, 0.0 );
}

