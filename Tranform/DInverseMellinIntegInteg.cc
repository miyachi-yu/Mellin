#include "DInverseMellinIntegInteg.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

FactorFunc* FactorFunc::set( const vector< double >& x, 
			     const double& mom ){
  x_ = x; 
  mom_ = mom; 
  return this;
}

DInverseMellinIntegInteg::DInverseMellinIntegInteg( ComplexFunction *g ) : 
  g_( g ), 
  c_( 2, 1.8 ), 
  zc_( 2, complex< double >( 0.0, 0.0 ) ),
  czc_( 2, complex< double >( 0.0, 0.0 ) ),
  e_( 2, complex< double>( cos( 0.65 * M_PI ), sin( 0.65 * M_PI ) ) ), 
  ce_( 2,complex< double>( cos( 0.65 * M_PI ), sin(-0.65 * M_PI ) ) ), 
  temp_( 2, complex< double >( 0.0, 0.0 ) ),
  mom_( 2, 0.0 ),   
  Fac_( 2, (FactorFunc*) NULL ),
  FacORG_( 4, (FactorFunc*) NULL )
{ 
  FacORG_[ 0 ] = new Fac_org();
  FacORG_[ 1 ] = new Fac_org();
  FacORG_[ 2 ] = new Fac_mom();
  FacORG_[ 3 ] = new Fac_mom();
}

DInverseMellinIntegInteg::~DInverseMellinIntegInteg(){
  for( int i = 0; i < 4; i++ ) if( FacORG_[ i ] ) delete FacORG_[ i ];
}

void DInverseMellinIntegInteg::setParameter( const vector< double >& c, 
					     const vector< double >& phi ){
  if ( c.size() == 0 || phi.size() == 0 ) return;
  // assign contents of c to local variable c_
  c_[ 0 ] = c[ 0 ];
  c_[ 1 ] = ( c.size() == 1 ? c[ 0 ] : c[ 1 ] );
  
  for( int i = 0; i < e_.size(); i++ ){
    double p = ( phi.size() < i + 1 ? phi[ 0 ] : phi[ i ] ) * M_PI ;
    e_[ i ] = complex< double >( cos( p ), sin( p ) );
    ce_[ i ] = conj( e_[ i ] );
  }
}

DInverseMellinIntegInteg& DInverseMellinIntegInteg::set( ComplexFunction *g ) {
  g_ = g;
  return *this;
}

DInverseMellinIntegInteg& 
DInverseMellinIntegInteg::set( const vector< double >& x, 
			       const vector< double >& z ){

  if ( x.size() > 2 || z.size() > 2 ) return *this;  

  // In order to throw, the function has to be declear with throw( int ).
  //     if ( x.size() > 2 || z.size() > 2 ) throw( 1 );

  Fac_[ 0 ] = 
    ( x.size() == 1 ?
      FacORG_[ 0 ]->set( x ) : 
      FacORG_[ 2 ]->set( x, ( mom_[ 0 ] != 0.0 ? mom_[ 0 ] : 1.0 ) ) );
  
  Fac_[ 1 ] = 
    ( z.size() == 1 ? 
      FacORG_[ 1 ]->set( z ) :
      FacORG_[ 3 ]->set( z, ( mom_[ 1 ] != 0.0 ? mom_[ 1 ] : 1.0 ) ) );

  return *this;
}

DInverseMellinIntegInteg& DInverseMellinIntegInteg::setR( const double& r ) {
  zc_[ 1 ]  = c_[ 1 ] + r * e_[ 1 ];
  czc_[ 1 ] = c_[ 1 ] + r * ce_[ 1 ];
  temp_[ 0 ] = (*Fac_[ 1 ])( zc_[ 1 ] ) * e_[ 1 ];
  temp_[ 1 ] = (*Fac_[ 1 ])( czc_[ 1 ] ) * ce_[ 1 ];
  //  temp_[ 0 ] = - 0.5 / M_PI / M_PI * (*Fac_[ 1 ])( zc_[ 1 ] ) * e_[ 1 ];
  //  temp_[ 1 ] = - 0.5 / M_PI / M_PI * (*Fac_[ 1 ])( czc_[ 1 ] ) * ce_[ 1 ];
  // move the factor 1/ 2 pi^2 to DInverseMellin.cc
  //     zc_[ 0 ] = c_[ 0 ] + r * e_[ 0 ];
  return *this;
}

double DInverseMellinIntegInteg::operator() ( const double& r ){
  //  cout << r << endl;
  zc_[ 0 ] = c_[ 0 ] + r * e_[ 0 ];
  //     zc_[ 1 ] = c_[ 1 ] + r  * e_[ 1 ];
  
  return ( (*Fac_[ 0 ])( zc_[ 0 ] ) * e_[ 0 ] *  
	   ( temp_[ 0 ] * (*g_)( zc_[ 0 ], zc_[ 1 ] )  
	     - temp_[ 1 ] * (*g_)( zc_[ 0 ], czc_[ 1 ] ) )
	   ).real();

}

complex< double > 
DInverseMellinIntegInteg::Fac_org::operator()( const complex< double >& zc ){
  return pow( x_[ 0 ], - zc );
}

complex< double > 
DInverseMellinIntegInteg::Fac_mom::operator()( const complex< double >& zc ){
  complex< double > mzc = mom_ - zc;
  return ( pow( x_[ 0 ], mzc ) -  pow( x_[ 1 ], mzc ) ) / mzc / ( x_[ 0 ] - x_[ 1 ] );
  //     cout <<  x_.size() << " " << x_[ 0 ] << " " <<  x_[ 1 ] << " "
  //	  << mom_ << endl;
  //     return ( pow( x_[ 0 ], mom_ - zc ) -  pow( x_[ 1 ], mom_ - zc ) )
  //	  / ( mom_ - zc );
  //     cout << x_[ 0 ] << " " << x_[ 1 ] << " " << mom_ << endl;
}

