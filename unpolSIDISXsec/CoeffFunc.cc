#include "CoeffFunc.hh"
#include "Charge.hh"

#include <Xsec/CKernel.hh>
#include <Xsec/Wilsons.hh>

using namespace std;
using namespace Utility;
using namespace unpolSIDISXsec;
using namespace Xsection;
using namespace QCD;

CoeffFunc::CoeffFunc( Arguments& args, 
		      const Xsection::Charge::TARGET& n ) :
  CKernel( args ), Xsecfact_( 1.0 ), cf_2( NULL ),
  X_( 0.1 ), S_( 100 ), direct_( false ),
  xcache_1_(), xcache_2_(), xcache_3_(), 
  xcache2_1_(), xcache2_2_(), xcache2_3_(),
  cdata_1_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  cdata_2_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  cdata_3_( 13 * 13, ( complex< double > )( 0.0, 0.0 ) ),
  usecc_( 3, true )
{
  this->charge_     = new Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

void CoeffFunc::update2(){
  //  Xsecfact_ =  8.0 * M_PI * pow( 1.0 / 137.0 , 2 ) / pow( q2(), 2 );
}

void CoeffFunc::update(){
  
  if( this->dimension() != 2 ) throw( 1 );
  
  //! y * x set 
  //  double y = ( direct_ ? q2() / S_ / X_ : q2() / S_ );
  
  int nf = static_cast< int >( this->nf() );
  
  for( int i = - nf; i <= nf; i++ ){
    
    int par1 = ( i == 0 ? 2 : ( i < 0 ? 1 : 0 ) ); // get parton type 1
    int idx = i + 6;
    
    for( int j = - nf; j <= nf; j++ ){
      
      int jdx = j + 6;
      int ijdx = 13 * idx + jdx;
      int par2 = ( j == 0 ? 2 : ( j < 0 ? 1 : 0 ) ); // get parton type 2
      
      if( ( i * j != 0 && i != j ) || ( i == 0 && j == 0 ) ){ 
	cdata_1_[ ijdx ] = complex< double >( 0.0, 0.0 );
	cdata_2_[ ijdx ] = complex< double >( 0.0, 0.0 );
	cdata_3_[ ijdx ] = complex< double >( 0.0, 0.0 );
	continue;
      }
      
      complex< double > nn = this->getN();
      complex< double > mm = this->getM();
      complex< double > dev;
      
      Coefficient& clo   = *( ( *wilsons() ) [ par1 ][ par2 ][ 0 ] );
      Coefficient& cnlo  = *( ( *wilsons() ) [ par1 ][ par2 ][ 1 ] );
      Coefficient& clo2  = *( ( *wilsons2() ) [ par1 ][ par2 ][ 0 ] );
      Coefficient& cnlo2 = *( ( *wilsons2() ) [ par1 ][ par2 ][ 1 ] );
      
      double charge = vchar()[ ( j != 0 ? jdx : idx ) ];
      
      // ( y^2 / 2 * 2 F1 + ( 1 - y ) * ( FL + 2F1 ( F2/x ) ) //
      // 2 F1 part
      complex< double >& cc1 = vCoeff()[ idx ][ jdx ];
      if( usecc_[ 0 ] ){
	dev = ( direct_ ? nn : nn - 2.0 );
	//	complex< double > dev = nn;
	cc1 = 
	  clo( dev, mm ) + 
	  ( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo( dev, mm ) : 0.0 );
	cc1 *= charge;
	//	cc1 *= 0.5 * y * y;
      } else {
	cc1 = complex< double >( 0.0, 0.0 );
      }
      
      // 2 F2 part
      complex< double >& cc2 = vCoeff2()[ idx ][ jdx ];
      if( usecc_[ 1 ] ){
	dev = nn;
	cc2 = 
	  clo2( dev, mm ) + 
	  ( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo2( dev, mm ) : 0.0 );
	cc2 *= charge;
	//	if( direct_ ) cc2 *= 1.0 - y;
      } else {
	cc2 = complex< double >( 0.0, 0.0 );
      }
      
      complex< double >& cc3 = vCoeff3()[ idx ][ jdx ];
      if( usecc_[ 2 ] ){
	dev = ( direct_ ? nn : nn - 1.0 );
	cc3 = complex< double >( 0.0, 0.0 );
	if( ! direct_ ){
	  cc3 = 
	    clo2( dev, mm ) + 
	    ( getOrder() == Scheme::NLO ? alpha_pi_2() * cnlo2( dev, mm ) : 0.0 ) ;
	  cc3 *= charge;
	  //	  cc3 *= - y;
	}
      } else {
	cc3 =  complex< double >( 0.0, 0.0 );
      }
      
      if( ! direct_ ){
	cdata_1_[ ijdx ] = cc1;
	cdata_2_[ ijdx ] = cc2;
	cdata_3_[ ijdx ] = cc3;
      }
    }
  }
  
  if( ! direct_ ){
    if( usecc_[ 0 ] ) xcache2_1_.cache( getN(), getM(), q2(), cdata_1_ );
    if( usecc_[ 1 ] ) xcache2_2_.cache( getN(), getM(), q2(), cdata_2_ );
    if( usecc_[ 2 ] ) xcache2_3_.cache( getN(), getM(), q2(), cdata_3_ );
  }

  this->applyY();
  
}

void CoeffFunc::setMoment( const complex< double >& N ){
  
  this->dimension( 1 );
  this->setN( N );

  double Q2 = this->q2();
  
  if( ( usecc_[ 0 ] && ! xcache_1_.cached( N, Q2 ) ) || 
      ( usecc_[ 1 ] && ! xcache_2_.cached( N, Q2 ) ) ||
      ( usecc_[ 2 ] && ! xcache_3_.cached( N, Q2 ) ) ){
    this->update();
    return;
  }
  
  vector< complex< double > >& buff_1 = xcache_1_.cache( N, Q2 );
  vector< complex< double > >& buff_2 = xcache_2_.cache( N, Q2 );
  vector< complex< double > >& buff_3 = xcache_3_.cache( N, Q2 );
  
  int nf = static_cast< int >( this->nf() );
  for( int i = - nf; i <= nf; i++ ){
    int idx = i + 6;
    if( usecc_[ 0 ] ) this->vCoeff()[ idx ][ idx ]  = buff_1[ idx ];
    if( usecc_[ 1 ] ) this->vCoeff2()[ idx ][ idx ] = buff_2[ idx ];
    if( usecc_[ 2 ] ) this->vCoeff3()[ idx ][ idx ] = buff_3[ idx ];
  }
  
  this->applyY();

}

void CoeffFunc::setMoment( const complex< double >& N, 
			   const complex< double >& M ){
  
  this->dimension( 2 );
  this->setN( N );
  this->setM( M );

  double Q2 = this->q2();
  
  if( ( usecc_[ 0 ] && ! xcache2_1_.cached( N, M, Q2 ) ) || 
      ( usecc_[ 1 ] && ! xcache2_2_.cached( N, M, Q2 ) ) ||
      ( usecc_[ 2 ] && ! xcache2_3_.cached( N, M, Q2 ) ) ){
    this->update();
    return;
  }
  
  vector< complex< double > >& buff_1 = xcache2_1_.cache( N, M, Q2 );
  vector< complex< double > >& buff_2 = xcache2_2_.cache( N, M, Q2 );
  vector< complex< double > >& buff_3 = xcache2_3_.cache( N, M, Q2 );
  
  int nf = static_cast< int >( this->nf() );
  for( int i = - nf; i <= nf; i++ ) {
    int idx = i + 6;
    for( int j = - nf; j <= nf; j++ ){
      int jdx = j + 6;
      int ijdx =  13 * idx + jdx;
      if( usecc_[ 0 ] ) vCoeff()[ idx ][ jdx ]  = buff_1[ ijdx ];
      if( usecc_[ 1 ] ) vCoeff2()[ idx ][ jdx ] = buff_2[ ijdx ];
      if( usecc_[ 2 ] ) vCoeff3()[ idx ][ jdx ] = buff_3[ ijdx ];
    }
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
  double y = ( direct_ ? q2() / S_ / X_ : q2() / S_ );
  int nf = static_cast< int >( this->nf() );
  for( int i = - nf; i <= nf; i++ ){
    int idx = i + 6;
    for( int j = - nf; j <= nf; j++ ){
      int jdx = j + 6;
      int ijdx = 13 * idx + jdx;
      // 2 F1 part
      vCoeff()[ idx ][ jdx ] *= 0.5 * y * y;
      // 2 F2 part
      if( direct_ )   vCoeff2()[ idx ][ jdx ] *= 1.0 - y;
      if( ! direct_ ) vCoeff3()[ idx ][ jdx ] *= - y;
    }
  }
}
