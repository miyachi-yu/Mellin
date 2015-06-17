// $Id: CPolyGamma.cc,v 1.10 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file     CPolyGamma.cc
  \version  $Revision: 1.10 $
  \date     $Date: 2008/12/02 08:13:16 $
  \author   $Author: miyachi $
  \brief    Implementation of PolyGamma function class
 */
#include "CPolyGamma.hh"

#include <cmath>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace Transform;

CKey CPolyGamma::key[4];
complex< double > CPolyGamma::data[4];
Utility::LookupTable< CKey, complex< double > >* CPolyGamma::cache[4] = {
  NULL, NULL, NULL, NULL
};
bool CPolyGamma::cacheReady = false;

void CPolyGamma::initCache() {
  if( CPolyGamma::cacheReady ) return ;
  for( int i=0; i<4; i++ ){
    ostringstream dbname;
    dbname << "cpolygamma" << i << ".db" ;
    CPolyGamma::cache[ i ] = 
      new Utility::LookupTable< CKey, complex< double > >( CPolyGamma::key[i],
						  CPolyGamma::data[i],
						  dbname.str().c_str() );
  }
  atexit( CPolyGamma::closeCache );
  CPolyGamma::cacheReady = true;
}

void CPolyGamma::closeCache(){
  for( int i=0; i<4; i++ ){
    CPolyGamma::cache[ i ]->close();
    delete CPolyGamma::cache[ i ];
  }
}

CPolyGamma::CPolyGamma( int m ) : 
  m_( m ), mcore_( 1 ), gamma_() {
  if( ! CPolyGamma::cacheReady ) CPolyGamma::initCache();
  this->setmcore();
}

CPolyGamma::CPolyGamma( const CPolyGamma& pg ){
  m_     = pg.m_;
  mcore_ = pg.mcore_;
}

CPolyGamma::~CPolyGamma() {
}

CPolyGamma& CPolyGamma::operator=( const CPolyGamma& pg ){
  if( this != &pg ){
    m_     = pg.m_;
    mcore_ = pg.mcore_;
  }
  return *this;
}

complex< double > CPolyGamma::operator() ( const complex< double >& n ) {
  
  Utility::LookupTable< CKey, complex< double > >& c = *( CPolyGamma::cache[ m_ ] );
  CKey&                                   k = CPolyGamma::key[ m_ ];
  complex< double >&                      d = CPolyGamma::data[ m_ ];
  
  if( c.get( k = CKey( n ), d ) ) return d;
  
  // If n is enough large, try to use approxymated form
  d = ( abs( n ) > 10.0 ? 
	this->largeN( n ) :
	(*this)( n + ( n.real() > 0 ? 1.0 : -1.0 ) ) - 
	( (double) mcore_ ) * pow( n, -1 - m_ ) );
  
  c.put();
  return d;
}

complex< double > CPolyGamma::largeN( const complex< double >& n ){

  switch( m_ ){
  case 3:
    //  As given in Handbook of Special Functions 
    static double c09 = 4.0 / 3.0;
    return 
      2.0 * pow( n, -3 ) + 3.0 * pow( n,-4 ) + 2.0 * pow( n, -5 )
      - pow( n, -7 ) + c09 * pow( n, -9 ) -3.0 * pow( n, -11 )
      + 10.0 * pow( n, -13 );
  case 2:
    // from Nucl. Phys. B465, 267
    static double c10 = -3.0 / 20.0;
    static double c12 = -5.0 / 48.0;
    return
      - pow( n, -2 ) 
      - pow( n, -3 ) 
      - pow( n, -4 ) / 2.0
      + pow( n, -6 ) / 6.0
      - pow( n, -8 ) / 16.0 
      + c10 * pow( n, -10 ) 
      + c12 * pow( n, -12 );
    
    /*  As given in Handbook of Special Functions 
	static double c56 = 5.0 / 6.0;
	return 
	- pow( n, -2 ) - pow( n, -3 ) - 0.5 * pow( n, -4 ) + pow( n, -6 ) / 6.0 
	- pow( n, -8 ) / 6.0 - 0.3 * pow( n, -10 )  - c56 * pow( n, -12 );
    */
  case 1:
    //  As given in Handbook of Special Functions 
    return 
      1.0 / n + 0.5 * pow( n, -2 ) + pow( n, -3 ) / 6.0 - pow( n, -5 ) / 30.0 
      + pow( n, -7 ) / 42.0 - pow( n, -9 ) / 30.0;
  case 0:
    //  As given in Handbook of Special Functions 
    return 
      log( n ) - 0.5 / n + pow( n, -4 ) / 120.0 - pow( n, -6 ) / 256.0;
  }

  // in case m_ > 3, the values already becomes enough small to be ignored.
  return 0.0;
}

void CPolyGamma::setmcore(){
  if( m_ == 0 ) mcore_ = 0;
  mcore_ = 1;
  for( int i = m_; i > 0; i-- ) mcore_ *= -1 * i;
}
