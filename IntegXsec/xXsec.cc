#include "xXsec.hh"
#include "XsecIntegrand.hh"
#include "CKernel.hh"

#include <iostream>
#include <iomanip>

#include <Evolution/Kernel.hh>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

using namespace std;
using namespace Transform;
using namespace IntegXsec;

xXsec::xXsec( const int& low, const int& high, const double& precision )
  throw( int ) : XsecBase(), IntegFunc(), // IntegFunc( low, high )
		 i_( 0 ), j_( 0 )
{
  this->initialization( low, high, precision );
}

xXsec::~xXsec(){
}

double xXsec::operator()( const double& x )
{
  if( !this->xsecInteg() ) throw( 1 );
  if( this->xsecInteg()->vevo().size() != 1 ) throw( 1 );
  this->setparams( x ); 
  // through XsecIntegrand (including "lower" "upper" set)

  return this->integration()
    ->integral( this->lower(), this->upper(), *( this->xsecInteg() ) );
}

complex< double > xXsec::operator()( const complex< double >& n ){
  // for service as a partial integrand

  this->setparams( n.real() );

  vector< vector< complex< double > > >& vvpar =  this->xsecInteg()->vparam();
  vvpar[ 0 ][ 1 ] = N_;

  this->xsecInteg()->setflavors( i_, j_ ); // note not i(j) + 6 
  return this->integration()
    ->cintegral( this->lower(), this->upper(), *( this->xsecInteg() ) );
}

void xXsec::setparams( const double& x )
{
  if( !this->xsecInteg() ) throw( 1 );

  this->lower( x ); this->upper( 1.0 );

  vector< complex< double > > vpar( 2, x ); 
  vector< vector< complex< double > > >& vvpar = this->xsecInteg()->vparam();

  if( vvpar.size() == 0 ) vvpar.resize( 1, vpar );
  if( vvpar.size() != 1 ) throw( 2 );
  vvpar[ 0 ] = vpar;
}

void xXsec::setMoment( const std::complex< double >& N ){

  if( !this->xsecInteg() ) throw( 1 );

  vector< vector< complex< double > > >& vvpar =  this->xsecInteg()->vparam();
  if( vvpar.size() == 0 )
    { cout << "setparams first!" << endl; throw( 2 ); }
  if( vvpar.size() != 1 ) throw( 2 );
  if( vvpar[ 0 ].size() != 2 )
    { cout << "setparams first!" << endl; throw( 2 ); }

  vvpar[ 0 ][ 1 ] = N;

  N_ = vvpar[ 0 ][ 1 ];
  X_ = vvpar[ 0 ][ 0 ].real();

  if( cdata_.size() != 0 ) throw( 100 );
  cdata_.resize( getWCdim1(), complex< double >( 0.0, 0.0 ) );
  // cdata initialization

  int NF = static_cast< int >( this->xsecInteg()->coeff()->nf() );

  for( int i = - NF; i <= NF; i++ ) 
    for( int j = - NF; j <= NF; j++ ) if( i == j ) {
      // only i = j elements are needed

      this->vcoeff_[ i + 6 ][ j + 6 ] = charge( i, j ) * getCvalue( i, j );
      // note CKernel calc (complex) is operated WITHOUT the charge 
      // maltiplication!
    }

  if( ! cache_.cached( N_, X_, q2() ) ) cache_.cache( N_, X_, q2(), cdata_ );

  cdata_.clear();
}

double xXsec::charge( const int& i, const int& j ){
  IntegXsec::CKernel* ck_ = dynamic_cast< IntegXsec::CKernel* >
    ( this->xsecInteg()->coeff() );
  return ck_->vchar()[ i + 6 ];
}

complex< double > xXsec::getCvalue( const int& i, const int& j ){

  if( ! this->setActflav( i, j ) ) return complex< double >( 0.0, 0.0 );

  // stored in memory ?
  if( cache_.cached( N_, X_, q2() ) )
    return cache_.cache( N_, X_, q2() )[ actwc1_[ acti_ ] ];

  // not calculated before in the loop ? 
  if( cdata_[ actwc1_[ acti_ ] ] == complex< double >( 0.0, 0.0 ) ){
    this->xsecInteg()->setflavors( acti_, actj_ ); // note not i(j) + 6 
    cdata_[ actwc1_[ acti_ ] ] = this->integration()
      ->cintegral( this->lower(), this->upper(), *( this->xsecInteg() ) );
    ncount_++;
  }

  return cdata_[ actwc1_[ acti_ ] ];

}

bool xXsec::setActflav( const int& i, const int& j ){
  // default : DIS (SIA) conditions
  if( i != j ) return false;   // assume i == j 

  (  i == 0 ? acti_ = actj_ = 0  : acti_ = actj_ = 1 );
  // gg (00) & uu(11) -> gg & qq in wc
  // no difference for q <-> qbar & q <-> q' 

  // cdata_.size() = 2 ! ( from getWCdim1() in XsecBase.hh ) 
  if( actwc1_.size() == 0 ){
    actwc1_[ 0 ] = 0; // gg -> cdata_[ 0 ]
    actwc1_[ 1 ] = 1; // qq -> cdata_[ 1 ]
  }

  return true;
}

void xXsec::initialization( const int& low, const int& high, 
                            const double& precision )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );

  if( low != 0 ){
    dynamic_cast< LegQuadrature* >( GridInt->method1() )->setngrid( low );
  } else{
    GridInt->setmethod1( ( Integration* ) NULL ); 
  }

  if( high != 0 ){
    dynamic_cast< LegQuadrature* >( GridInt->method2() )->setngrid( high );
  } else{
    GridInt->setmethod2( ( Integration* ) NULL ); 
  }

  GridInt->precision( precision );
  GridInt->ngrid( 4 );
}

void xXsec::setloginteg( const bool& logint )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  
  if( GridInt->method1() )
    dynamic_cast< LegQuadrature* >( GridInt->method1() )
      ->setloginteg( logint );
  if( GridInt->method2() )
    dynamic_cast< LegQuadrature* >( GridInt->method2() )
      ->setloginteg( logint );
}

void xXsec::setGngrid( const int& ngrid )
{  
  GridIntegration* GridInt 
    = dynamic_cast< GridIntegration* >( this->integration() );
  GridInt->ngrid( ngrid );
}
