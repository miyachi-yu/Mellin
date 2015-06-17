#include "PStrFunc.hh"
#include "XsecComp.hh"
#include <IntegXsec/XsecIntegrand.hh>

using namespace std;
using namespace Transform;
using namespace IntegPolSIDIS;

PStrFunc::PStrFunc( Xsection::XsecCompBase* xc, Utility::Arguments& args, 
		  const IntegXsec::Charge::TARGET& n,
		  const int& low, const int& high, const double& precision )
  throw( int ): StrFunc( xc, args, n, low, high, precision ), 
		PartialInteg( dynamic_cast< Transform::ComplexFunction* >
			      ( this ), 
			      dynamic_cast< IntegXsec::xzXsec* >( this )
			      ->integration() ),
		nxz_( complex< double >( 0.0, 0.0 ) )
{
  if( ! dynamic_cast< PartialInteg* >( this )->integration() ) throw( 1 );
}

void PStrFunc::setMoment( const std::complex< double >& N, 
			  const std::complex< double >& M ){
  N_ = N;
  M_ = M;

  if( !this->xsecInteg() ) throw( 1 );

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
  // X(Z)_ : given setx_zPI (partial integral variable is kept 0.0 )

  cdata_.clear();
}

complex< double > PStrFunc::getCvalue( const int& i, const int& j ){

  if( ! this->setActflav( i, j ) ) return complex< double >( 0.0, 0.0 ); 
  // setActflav can be kept as default

  // stored in memory ?
  if( cache2_.cached( N_, M_, X_, Z_, q2() ) )
    return cache2_.cache( N_, M_, X_, Z_, q2() )[ actwc2_[ acti_ ][ actj_ ] ];

  // not calculated before in the loop ? 
  if( cdata_[ actwc2_[ acti_ ][ actj_ ] ] == complex< double >( 0.0, 0.0 ) ){

    PartialInteg* pint_ = dynamic_cast< PartialInteg* >( this );

    this->setflavors( acti_, actj_ );
    // note not i(j) + 6 & setflavors in xzXsec not in XsecIntegrand
    cdata_[ actwc2_[ acti_ ][ actj_ ] ] = (*pint_)( nxz_ );
  }

  return cdata_[ actwc2_[ acti_ ][ actj_ ] ];

}

void PStrFunc::setx_zPI( const double& x ){
  nxz_ = complex< double >( x, 0.0 );
  this->setPIntegParam( 2 ); // z partial integ
  X_ = x; Z_ = 0.0;
}

void PStrFunc::setz_xPI( const double& z ){
  nxz_ = complex< double >( z, 0.0 );
  this->setPIntegParam( 1 ); // x partial integ
  X_ = 0.0; Z_ = z;
}
