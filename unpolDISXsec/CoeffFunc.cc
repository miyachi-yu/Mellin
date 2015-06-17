#include "CoeffFunc.hh"

#include <Xsec/CKernel.hh>
#include <Xsec/Wilsons.hh>

using namespace std;
using namespace Utility;
using namespace unpolDISXsec;
using namespace Xsection;
using namespace QCD;

CoeffFunc::CoeffFunc( Arguments& args, 
		      const Xsection::Charge::TARGET& n ) :
  CKernel( args ), Xsecfact_( 1.0 ), cf_2( NULL ),
  X_( 0.1 ), x1_( 0.0 ), x2_( 1.0 ), S_( 100 ), direct_( false ), 
  xmom_( false ), zmom_( false ),
  xcache_1_(), xcache_2_(), xcache_3_(), 
  xcache2_1_(), xcache2_2_(), xcache2_3_(),
  cdata_1_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  cdata_2_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  cdata_3_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  usecc_( 3, true )
{
  this->charge_     = new Xsection::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

void CoeffFunc::update2(){
  //  Xsecfact_ =  8.0 * M_PI * pow( 1.0 / 137.0 , 2 ) / pow( q2(), 2 );
}

void CoeffFunc::update(){
  
  if( dimension() != 1 ) throw( 1 );
  
  if( ! direct_ && xmom_ ) throw( 3 );
  
  //! y * x set 
  //  double y = ( direct_ && !xmom_ ? q2() / S_ / X_  : q2() / S_ );
  
  int nf = static_cast< int >( this->nf() );
  complex< double > nn = getN();
  complex< double > dev;
  
  for( int i = - nf; i <= nf; i++ ){
    
    int par1 = ( i == 0 ? 2 : ( i < 0 ? 1 : 0 ) ); // get parton type 1
    int idx = i + 6;
    
    complex< double >& cc1 = vCoeff()[ idx ][ idx ];
    complex< double >& cc2 = vCoeff2()[ idx ][ idx ];
    complex< double >& cc3 = vCoeff3()[ idx ][ idx ];
    
    Coefficient& clo  = *( ( *wilsons() ) [ par1 ][ par1 ][ 0 ] );
    Coefficient& cnlo = *( ( *wilsons() ) [ par1 ][ par1 ][ 1 ] );
    
    Coefficient& clo2  = *( ( *wilsons2() ) [ par1 ][ par1 ][ 0 ] );
    Coefficient& cnlo2 = *( ( *wilsons2() ) [ par1 ][ par1 ][ 1 ] );
    
    double& charge = vchar()[ idx ];
    
    // ( y^2 / 2 * 2 F1 + ( 1 - y ) * ( FL + 2F1 ( F2/x ) ) //
    // 2 F1 part
    dev = ( direct_ ? nn : nn - 2.0 );
    cc1 = 
      clo( dev ) + 
      ( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo( dev ) : 0.0 );
    
    cc1 *= ( direct_ && xmom_ ? factor( -2.0 ) : 1.0 );
    cc1 *= charge;

    //    cc1 *= 0.5 * y * y;

    // 2 F2 part
    dev = nn;
    cc2 = 
      clo2( dev ) + 
      ( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo2( dev ) : 0.0 );
    cc2 *= charge;
    
    //    if( direct_ )
    //      cc2 *= ( xmom_ ? factor( 0.0 ) - y * factor( -1.0 ) : 1.0 - y );
    
    dev = ( direct_ ? nn : nn - 1.0 );
    cc3 = 0.0;
    if( ! direct_ ){
      cc3 = 
	clo2( dev ) +
	( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo2( dev ) : 0.0 );
      cc3 *= charge;
      //      cc3 *= - y;
    }
    
    if( ! direct_ ){
      cdata_1_[ idx ] = cc1;
      cdata_2_[ idx ] = cc2;
      cdata_3_[ idx ] = cc3;
    }    
    
  }
  
  if( ! direct_ ){
    xcache_1_.cache( getN(), q2(), cdata_1_ );
    xcache_2_.cache( getN(), q2(), cdata_2_ );
    xcache_3_.cache( getN(), q2(), cdata_3_ );
  }
  
  this->applyY();
}

complex< double > CoeffFunc::factor( const double& order ){
  complex< double > nn = getN();
  double torder = order + 1.0; // first moment only
  nn -= torder;
  return ( pow( x1_, - nn ) - pow( x2_, - nn ) ) / nn / ( x2_ - x1_ );
}

void CoeffFunc::setX1X2( const double& x1, const double& x2 ) throw( int ){
  //  if( xmom_ != true || x2 <= x1 ) throw( 1 ); 
  if( x2 <= x1 ) throw( 1 ); 
  // x2 > x1 and xmom_ should be already true 
  // ( for the consistency with xmom_ in xXsec ) reference!
  x1_ = x1; x2_ = x2;
}


void CoeffFunc::setMoment( const complex< double >& N ){

  dimension( 1 );
  setN( N );
  
  if( ! xcache_1_.cached( N, q2() ) || 
      ! xcache_2_.cached( N, q2() ) ||
      ! xcache_3_.cached( N, q2() ) ) {
    this->update();
    return;
  }
  
  int nf = static_cast< int >( this->nf() );
  vector< complex< double > >& buff_1 = xcache_1_.cache( N, q2() );
  vector< complex< double > >& buff_2 = xcache_2_.cache( N, q2() );
  vector< complex< double > >& buff_3 = xcache_3_.cache( N, q2() );

  for( int i = - nf; i <= nf; i++ ){
    int idx = i + 6;
    vCoeff()[ idx ][ idx ]  = buff_1[ idx ];
    vCoeff2()[ idx ][ idx ] = buff_2[ idx ];
    vCoeff3()[ idx ][ idx ] = buff_3[ idx ];
  }

  this->applyY();
}

void CoeffFunc::usecc( const int& i ){
  if( i < 0 || i > 2 ) return;
  for( int j = 0; j < usecc_.size(); j++ ) usecc_[ j ] = ( i == j );
}

void CoeffFunc::usecc(){
  for( int j = 0; j < usecc_.size(); j++ ) usecc_[ j ] = true;
}

void CoeffFunc::applyY(){
  //! y * x set 
  double y = ( direct_ && !xmom_ ? q2() / S_ / X_  : q2() / S_ );
  
  int nf = static_cast< int >( this->nf() );
  for( int i = - nf; i <= nf; i++ ){
    int idx = i + 6;
    // ( y^2 / 2 * 2 F1 + ( 1 - y ) * ( FL + 2F1 ( F2/x ) ) //
    // 2 F1 part
    vCoeff()[ idx ][ idx ] *= 0.5 * y * y;
    // 2 F2 part
    if( direct_ ) 
      vCoeff2()[ idx ][ idx ] *= 
	( xmom_ ? factor( 0.0 ) - y * factor( -1.0 ) : 1.0 - y );
    if( ! direct_ ) 
      vCoeff3()[ idx ][ idx ] *= - y;
  }
}

void CoeffFunc::setMoment( const complex< double >& N, 
			   const complex< double >& M ){
  dimension( 2 );
  setN( N );
  setM( M );
  
  if( ! xcache2_1_.cached( N, M, q2() ) || 
      ! xcache2_2_.cached( N, M, q2() ) || 
      ! xcache2_3_.cached( N, M, q2() ) ) {
    this->update();
    return;
  }
  
  int nf = static_cast< int >( this->nf() );
  vector< complex< double > >& buff_1 = xcache2_1_.cache( N, M, q2() );
  vector< complex< double > >& buff_2 = xcache2_2_.cache( N, M, q2() );
  vector< complex< double > >& buff_3 = xcache2_3_.cache( N, M, q2() );
  
  for( int i = - nf; i <= nf; i++ ) {
    int idx = i + 6;
    for( int j = - nf; j <= nf; j++ ){
      int jdx = jdx;
      int ijdx = 13 * idx + jdx;
      vCoeff()[ idx ][ jdx ]  = buff_1[ ijdx ];
      vCoeff2()[ idx ][ jdx ] = buff_2[ ijdx ];
      vCoeff3()[ idx ][ jdx ] = buff_3[ ijdx ];
    }
  }
}
