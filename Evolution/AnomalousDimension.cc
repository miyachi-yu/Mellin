// $Id: AnomalousDimension.cc,v 1.25 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file    AnomalousDimension.cc
  \version $Revision: 1.25 $
  \author  $Author: miyachi $
  \brief   Implementation of Anomalous Dimension Base class
 */
#include "AnomalousDimension.hh"
#include "Kernel.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

GeneralHarmonic      AnomalousDimension::s_;
GeneralHarmonicPrime AnomalousDimension::sp_;
GeneralHarmonicTilde AnomalousDimension::st_;

AnomalousDimension::AnomalousDimension( Kernel* k ) : 
  TF(),
  name_( "AD" ), kernel_( k ), q2_( 1.0 ), Nf_( 3 ),
  p_( 2, vector< ComplexFunction* >( 7, ( ComplexFunction* ) NULL ) ),
  useDB_( true ),
  key_( 2, vector< DataCache::Grid >( 7, DataCache::Grid() ) ),
  data_( 2, vector< complex< double > >( 7, complex< double >( 0.0, 0.0 ) ) ),
  cache_( 2, 
	  vector < Utility::LookupTable< DataCache::Grid, complex< double > >* >
	  ( 7, ( Utility::LookupTable< DataCache::Grid, complex< double > >* ) NULL ) )
{
}

AnomalousDimension::~AnomalousDimension(){
  this->closeDB();
}

void AnomalousDimension::openDB(){
  for( int i = 0; i < cache_.size(); i++ ) 
    for( int j = 0; j < cache_[ i ].size(); j++ ){
      string dbname = this->db_name( (TYPE) j, (Scheme::ORDER) i );
      cache_[ i ][ j ] =
	new Utility::LookupTable< DataCache::Grid, complex< double > >
    	( key_[ i ][ j ], data_[ i ][ j ], dbname ); 
    } 
}

void AnomalousDimension::closeDB(){
  for( int i = 0; i < cache_.size(); i++ ) 
    for( int j = 0; j < cache_[ i ].size(); j++ ){
      if( cache_[ i ][ j ] ) {
	cache_[ i ][ j ]->close();
	delete cache_[ i ][ j ];
	cache_[ i ][ j ] = NULL;
      }
    } 
}

void AnomalousDimension::nf( const double& n ){
  Nf_ = n;
  TF = 0.5 * Nf_;
}

complex< double > 
AnomalousDimension::r( const AnomalousDimension::TYPE& type,  
		       const Scheme::ORDER& order, 
		       const complex< double >& n ){
  
  if( order == Scheme::NLO && kernel_->alpha().scheme().order() != Scheme::NLO )
    return complex< double >( 0.0, 0.0 );
  
  if( p_[ order ][ type ] == NULL ) return complex< double >( 0.0, 0.0 );
  
  ComplexFunction *f = p_[ order ][ type ];
  
  // if it is required not to use the external DB file
  if( ! useDB_ ) return (*f)( n );
  
  // if the external DB was not created, create DB.
  if( cache_[ order ][ type ] == NULL ) this->openDB();
  
  Utility::LookupTable< DataCache::Grid, complex< double > > *c
    = cache_[ order ][ type ];
  
  DataCache::Grid k( n, TF );
  if( ! c->get( key_[ order ][ type ] = k, data_[ order ][ type ] ) ){
    data_[ order ][ type ] = (*f)( n );
    c->put();
    //    cout << setw(10) << "Insert:" << flush;
  } else {
    //    cout << setw(10) << "Found:" << flush;
  }
  /*
    cout << setw(18) << data_[ order ][ type ] 
       << setw( 5) << "for" 
       << setw( 5) << this->name( type )
       << ( order == Scheme::LO ? " LO " : " NLO " )
       << setw(20) << k 
       << endl;
  */
  return data_[ order ][ type ];
}

string AnomalousDimension::name( AnomalousDimension::TYPE type ){
  switch( type ){
  case Pqq:   return "qq";
  case Pqg:   return "qg";
  case Pgq:   return "gq";
  case Pgg:   return "gg";
  case Pnsp:  return "ns+";
  case Pnsm:  return "ns-";
  case Pnsv:  return "nsv";
  }
  return "NULL";
}

string AnomalousDimension::db_name( AnomalousDimension::TYPE type, 
				    Scheme::ORDER order ){

  string strType = this->name( type );
  string strOrder( order == Scheme::LO ? "LO" : "NLO" );
  string suffix( this->kernel() ? 
		 ( this->kernel()->constNf() ? "_fixNf.db" : "_dynamicNf.db" ) :
		 ".db" );
  
  return name_ + "_" + strType + "_" + strOrder + suffix;
}

namespace Evolution {
  ostream& operator<<( ostream& os, AnomalousDimension& ad ){
    
    os << "############################################################" << endl;
    os << "#               Anomalous Dimensions                       #" << endl;
    os << "############################################################" << endl;
    
    complex< double > n = ad.kernel()->n();
    os << "N:            " << n       << endl;
    
    os << "# ------ special functions ------" << endl;
    os << "S_m(N):                        ";
    for( int i=1; i<4; i++ ) os << setw(16) << ad.s_( i, n );
    os << endl;
    
    os << "S'_m(N/2,+1):( = S_m(N/2))     ";
    for( int i=1; i<4; i++ ) os << setw(16) << ad.sp_( i, n / 2.0, +1 );
    os << endl;
    
    os << "S'_m(N/2,-1):( = S_m((N-1)/2) )";
    for( int i=1; i<4; i++ ) os << setw(16) << ad.sp_( i, n / 2.0, +1 );
    os << endl;
    
    os << "S~_m(N,+1):                    ";
    for( int i=1; i<4; i++ ) os << setw(16) << ad.st_( i, n, +1 );
    os << endl;
    
    os << "S~_m(N,-1):                    ";
    for( int i=1; i<4; i++ ) os << setw(16) << ad.st_( i, n, -1 );
    os << endl;
    
    os << "S~(N,+1):                      "
       << setw(16) << ad.st_( n, +1 ) << endl;
    
    os << "S~(N,-1):                      "
       << setw(16) << ad.st_( n, -1 ) << endl;
    
    os << "S_{2,1}(N):                    "
       << setw(16) << ad.s_( 2, 1, n ) << endl;
    
    os << "S_{1,2}(N):                    "
       << setw(16) << ad.s_( 1, 2, n ) << endl;
    
    os << "S~_{1,2}(N,+1):                "
       << setw(16) << ad.st_( 1, 2, n, +1 ) << endl;
    
    os << "S~_{1,2}(N,-1):                "
       << setw(16) << ad.st_( 1, 2, n, -1 ) << endl;
    
    complex< double > rqq0 = ad.r( AnomalousDimension::Pqq,  Scheme::LO,  n );
    complex< double > rqq1 = ad.r( AnomalousDimension::Pqq,  Scheme::NLO, n );
    complex< double > rqg0 = ad.r( AnomalousDimension::Pqg,  Scheme::LO,  n );
    complex< double > rqg1 = ad.r( AnomalousDimension::Pqg,  Scheme::NLO, n );
    complex< double > rgq0 = ad.r( AnomalousDimension::Pgq,  Scheme::LO,  n );
    complex< double > rgq1 = ad.r( AnomalousDimension::Pgq,  Scheme::NLO, n );
    complex< double > rgg0 = ad.r( AnomalousDimension::Pgg,  Scheme::LO,  n );
    complex< double > rgg1 = ad.r( AnomalousDimension::Pgg,  Scheme::NLO, n );
    complex< double > nsp0 = ad.r( AnomalousDimension::Pnsp, Scheme::LO,  n );
    complex< double > nsp1 = ad.r( AnomalousDimension::Pnsp, Scheme::NLO, n );
    complex< double > nsm0 = ad.r( AnomalousDimension::Pnsm, Scheme::LO,  n );
    complex< double > nsm1 = ad.r( AnomalousDimension::Pnsm, Scheme::NLO, n );
    
    os << "*********************************************************" << endl;
    os << "*   Anomalous Dimentison " << endl;
    os << "    " << setw(18) << "LO" << setw(18) << "NLO" << endl;
    os << "qq :" << setw(18) << rqq0 << setw(18) << rqq1 << endl;
    os << "qg :" << setw(18) << rqg0 << setw(18) << rqg1 << endl;
    os << "gq :" << setw(18) << rgq0 << setw(18) << rgq1 << endl;
    os << "gg :" << setw(18) << rgg0 << setw(18) << rgg1 << endl;
    os << "NS+:" << setw(18) << nsp0 << setw(18) << nsp1 << endl;
    os << "NS-:" << setw(18) << nsm0 << setw(18) << nsm1 << endl;
    
    if( n == complex< double >( 1.0, 0.0 ) ){
      os << "****************************************************************" << endl;
      os << "*  check for N = 1, case" << endl;
      os << "rqq0 - 32/3*(S1(1) - 1) = 0         "
	 << setw(18) << rqq0 - 32.0 * ( ad.s_( 1, n ) - 1.0 ) / 3.0 << endl;
      os << "rgg0 - 9/4 rqq0 + 22 - 8/3 TF = 0   "
	 << setw(18) 
	 << rgg0 - 9.0 / 4.0 * rqq0 + 22.0 - 8.0 / 3.0 * ad.TF
	 << endl;
      os << "rqg1 = 0                            "
	 << setw(18) << rqg1
	 << endl;
      os << "rns- = 0 (unpolarized)              "
	 << setw(18) << nsm1 << endl;
      os << "rns+ = 0 (polarized)                "
	 << setw(18) << nsp1 << endl;
      os << "rqq1 - 24 CF TF = 0                 "
	 << setw(18) << rqq1 - 32.0 *  ad.TF
	 << endl;
      
    }
    
    return os;
  }
}
