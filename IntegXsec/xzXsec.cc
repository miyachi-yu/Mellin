#include "xzXsec.hh"
#include "XsecIntegrand.hh"
#include "CKernel.hh"

#include <iostream>
#include <iomanip>

#include <Evolution/Kernel.hh>

using namespace std;
using namespace Transform;
using namespace IntegXsec;

xzXsec::xzXsec( const int& low, const int& high, const double& precision )
  throw( int ) : XsecBase(), Transformer(), 
		 xzXIntg_( this, low, high, precision ),
		 i_( 0 ), j_( 0 )
{
  this->integration( xzXIntg_.integration() );
}

xzXsec::~xzXsec(){
}

double xzXsec::operator()( const double& x, const double& z )
{
  if( ! this->xsecInteg() ) throw( 1 );
  if( this->xsecInteg()->vevo().size() != 2 ) throw( 1 );
  this->setparams( x, z ); 
  // through XsecIntegrand (including "lower" "upper" set)

  return this->integration()->integral( this->lower(), this->upper(), 
					this->xzXIntg_ );
}

complex< double > xzXsec::operator()
  ( const complex< double >& n, const complex< double >& m ){
  // for service as a partial integrand

  this->setparams( n.real(), m.real() );

  vector< vector< complex< double > > >& vvpar =  this->xsecInteg()->vparam();
  vvpar[ 0 ][ 1 ] = N_;
  vvpar[ 1 ][ 1 ] = M_;

  this->xsecInteg()->setflavors( i_, j_ ); // note not i(j) + 6 
  return this->integration()
    ->cintegral( this->lower(), this->upper(), this->xzXIntg_ );
}

void xzXsec::setparams( const double& x, const double& z )
{
  if( !this->xsecInteg() ) throw( 1 );

  this->lower( x ); this->upper( 1.0 );
  this->xzXIntg_.lower( z ); this->xzXIntg_.upper( 1.0 );

  vector< complex< double > > vpar1( 2, x ); 
  vector< complex< double > > vpar2( 2, z ); 
  vector< vector< complex< double > > >& vvpar = this->xsecInteg()->vparam();

  if( vvpar.size() == 0 ){ 
    vvpar.resize( 2, vpar1 );
    vvpar[ 1 ] = vpar2;
  }
  if( vvpar.size() != 2 ) throw( 2 );
  //  if( vvpar.size() != 2 )
  //    { vvpar.resize( 2, vpar1 ); vvpar[ 1 ] = vpar2; return; }
  vvpar[ 0 ] = vpar1;
  vvpar[ 1 ] = vpar2;
}

void xzXsec::setMoment( const std::complex< double >& N, 
			const std::complex< double >& M ){

  if( !this->xsecInteg() ) throw( 1 );

  vector< vector< complex< double > > >& vvpar =  this->xsecInteg()->vparam();
  if( vvpar.size() == 0 )
    { cout << "setparams first!" << endl; throw( 2 ); }
  if( vvpar.size() != 2 ) throw( 2 );
  if( vvpar[ 0 ].size() != 2 || vvpar[ 1 ].size() != 2 )
    { cout << "setparams first!" << endl; throw( 2 ); }

  vvpar[ 0 ][ 1 ] = N;
  vvpar[ 1 ][ 1 ] = M;

  N_ = vvpar[ 0 ][ 1 ];
  M_ = vvpar[ 1 ][ 1 ];
  X_ = vvpar[ 0 ][ 0 ].real();
  Z_ = vvpar[ 1 ][ 0 ].real();

  if( cdata_.size() != 0 ) throw( 100 ); 
  cdata_.resize( getWCdim2(), complex< double >( 0.0, 0.0 ) );
  // cdata initialization

  int NF = static_cast< int >( this->xsecInteg()->coeff()->nf() );

  for( int i = - NF; i <= NF; i++ ) 
    for( int j = - NF; j <= NF; j++ ) {

      // for flavor identification calc
      if( Utility::Arguments::ref().hasOpt( "Flav1" ) )
	if( i != Utility::Arguments::ref().get( "Flav1", 1 ) ) continue;
      // vcoeff default 0.0 ( Xsec/CKernelBase )
      if( Utility::Arguments::ref().hasOpt( "Flav2" ) )
	if( j != Utility::Arguments::ref().get( "Flav2", 1 ) ) continue;
      // vcoeff default 0.0 ( Xsec/CKernelBase )

      this->vcoeff_[ i + 6 ][ j + 6 ] = charge( i, j ) * getCvalue( i, j );
      // note CKernel calc (complex) is operated WITHOUT the charge 
      // maltiplication!
    }

  if( ! cache2_.cached( N_, M_, X_, Z_, q2() ) ) 
    cache2_.cache( N_, M_, X_, Z_, q2(), cdata_ );

  cdata_.clear();
}

double xzXsec::charge( const int& i, const int& j ){
  IntegXsec::CKernel* ck_ = dynamic_cast< IntegXsec::CKernel* >
    ( this->xsecInteg()->coeff() );
  if( ck_->getpp_judge() ) return 1.0;
  return ck_->vchar()[ ( j != 0 ? j + 6 : i + 6 ) ];
}

complex< double > xzXsec::getCvalue( const int& i, const int& j ){

  if( ! this->setActflav( i, j ) ) return complex< double >( 0.0, 0.0 );

  // stored in memory ?
  if( cache2_.cached( N_, M_, X_, Z_, q2() ) )
    return cache2_.cache( N_, M_, X_, Z_, q2() )[ actwc2_[ acti_ ][ actj_ ] ];

  // not calculated before in the loop ? 
  if( cdata_[ actwc2_[ acti_ ][ actj_ ] ] == complex< double >( 0.0, 0.0 ) ){

    this->xsecInteg()->setflavors( acti_, actj_ ); // note not i(j) + 6 
    cdata_[ actwc2_[ acti_ ][ actj_ ] ] = this->integration()
      ->cintegral( this->lower(), this->upper(), this->xzXIntg_ );
    ncount_++;
  }

  return cdata_[ actwc2_[ acti_ ][ actj_ ] ];

}

bool xzXsec::setActflav( const int& i, const int& j ){
  // default : SIDIS conditions
  if( i == 0 && j == 0 ) return false; // gg elimination
  if( i * j != 0 && i != j ) return false; // qq'(q'bar) elimination

  (  i == 0 ? acti_ = 0  : acti_ = 1 );
  (  j == 0 ? actj_ = 0  : actj_ = 1 );
  // ug(10) & gu(01) && uu(11) -> qg & gq & qq in wc
  // no difference for q <-> qbar & q <-> q' 

  // Drell-Yan 
  //  if( acti_ * actj_ != 0 ) actj_ = -1; // acti_ = actj_ = 1

  // cdata_.size() = 3 ! ( from getWCdim2() in XsecBase.hh ) 
  if( actwc2_.size() == 0 ){
    actwc2_[ 1 ][ 0 ] = 0; // qg -> cdata_[ 0 ]
    actwc2_[ 0 ][ 1 ] = 1; // gq -> cdata_[ 1 ]
    actwc2_[ 1 ][ 1 ] = 2; // qq -> cdata_[ 2 ]
    // Drell-Yan //
    //    actwc2_[ 1 ][ -1 ] = 2; // qqbar -> cdata_[ 2 ]
  }

  return true;
}
