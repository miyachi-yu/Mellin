#include "CKernel.hh"
#include "Wilsons.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Xsection;

CKernel::CKernel( Arguments& args ) :
  CKernelBase(),
  q2_( 0.1 ),
  Nf_( 3.0 ), 
  charge_( NULL ), 
  alpha_( args ),
  order_( alpha_.scheme().order() ), 
  Nf_qfactor_( 1.0 ),
  alpha_pi_2_( 0.0 ),
  constNf_( false ),
  vchar_( 13, 0.0 ),
  cache_(),
  cache2_(),
  cdata_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ) 
{
}

CKernel::~CKernel(){
}

void CKernel::setMoment( const complex< double >& N ){
  dim_ = 1;
  N_ = N;
  
  if( ! cache_.cached( N_, q2_ ) ) 
    {
    this->update();
    return;
  }
  
  int nf = static_cast< int >( Nf_ );
  vector< complex< double > >& buff = cache_.cache( N_, q2_ );
  for( int i = - nf; i <= nf; i++ ) vcoeff_[ i + 6 ][ i + 6 ] = buff[ i + 6 ];
  
}

void CKernel::setMoment( const complex< double >& N, 
			 const complex< double >& M ){
  dim_ = 2;
  N_   = N;
  M_   = M;
  
  if( ! cache2_.cached( N_, M_, q2_ ) ) 
    {
      this->update();
      return;
    }
  
  int nf = static_cast< int >( Nf_ );
  vector< complex< double > >& buff = cache2_.cache( N_, M_, q2_ );
  
  for( int i = - nf; i <= nf; i++ ) 
    for( int j = - nf; j <= nf; j++ )
      vcoeff_[ i + 6 ][ j + 6 ] = buff[ 13 * ( i + 6 ) + j + 6 ];
  
}

void CKernel::setQ2( const double& q2 ){
  
  if( q2_ == q2 ) return;
  q2_ = q2;
  
  double alp_pi = ( order_ == Scheme::NLO ? alpha_( q2_ ) / M_PI : 0.0 );
  
  alpha_pi_2_ = 0.5 * alp_pi;  // 0.5 * alpha( Q2 ) / M_PI
  // 1.0 + alpha( Q2 ) / M_PI
  // this will be used somewhere... in deed in FragXsec_T/Charge.cc
  alpha_pi_1_ = 1.0 + alp_pi;
  
  if( ! constNf_ ) Nf_ = Alpha::flavor()( sqrt( q2_ ) * Nf_qfactor_ );
  
  int nf = static_cast< int >( Nf_ );
  for( int i = - nf; i <= nf; i++ ) 
    vchar_[ i + 6 ] = (*charge_)( Flavor::PARTON( i ) );
  
  //  this->update();
  this->update2();
}

void CKernel::update(){

  /* 
     coefficients between different flavors are out of scope...
     which have to be included p p -> jet, direct photon process...
     for the purpose it is necessary to increse cf_ matrix
     just by one unit because of flavor independence of QCD 
  */      
  
  int nf = static_cast< int >( Nf_ );
  for( int i = - nf; i <= nf; i++ ){
    
    int par1 = ( i == 0 ? 2 : ( i < 0 ? 1 : 0 ) ); // get parton type 1
    
    if( dim_ == 1 ){

      Coefficient& clo  = *( (*cf_) [ par1 ][ par1 ][ 0 ] );
      Coefficient& cnlo = *( (*cf_) [ par1 ][ par1 ][ 1 ] );
      
      complex< double >& cc = vcoeff_[ i + 6 ][ i + 6 ];
      cc = clo( N_ ) + 
	( order_ == Scheme::NLO ? alpha_pi_2_ * cnlo( N_ ) : 0.0 );
      cc *= vchar_[ i + 6 ];
      
      // store also in data buffer
      cdata_[ i + 6 ] = cc;
      
    } else if( dim_ == 2 ) {
      
      for( int j = - nf; j <= nf; j++ ){
	
	int par2 = ( j == 0 ? 2 : ( j < 0 ? 1 : 0 ) ); // get parton type 2
	
	complex< double >& cc = vcoeff_[ i + 6 ][ j + 6 ];
	
	if( ( i * j != 0 && i != j ) || ( i == 0 && j == 0 ) ){ 
	  cc = complex< double >( 0.0, 0.0 );
	  cdata_[ 13 * ( i + 6 ) + j + 6 ] = cc;
	  continue;
	}
	
	Coefficient& clo  = *( (*cf_) [ par1 ][ par2 ][ 0 ] );
	Coefficient& cnlo = *( (*cf_) [ par1 ][ par2 ][ 1 ] );
	
	cc = clo( N_, M_ ) + 
	  ( order_ == Scheme::NLO ? alpha_pi_2_ * cnlo( N_, M_ ) : 0.0 );
	
	cc *= vchar_[ ( j != 0 ? j + 6 : i + 6 ) ] ;

	// copy also in cached data buffer
	cdata_[ 13 * ( i + 6 ) + j + 6 ] = cc;
	
      }
    }
  }  
  
  // cache data in memory
  if( dim_ == 1 ) cache_.cache( N_, q2_, cdata_ );
  if( dim_ == 2 ) cache2_.cache( N_, M_, q2_, cdata_ );
  
}

namespace Xsection {
  
  ostream& operator<<( ostream& os, CKernel& ck ){
    cout << "Xsection::CKernel information" << endl;
    cout << "Q2 = " << setw(10) << ck.q2_ << endl;
    cout << "NF = " << setw(10) << ck.Nf_ << endl;
    return os;
  }
}
