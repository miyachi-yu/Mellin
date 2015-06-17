#include "XsecIntegrand.hh"
#include "CKernel.hh"

using namespace std;
using namespace IntegXsec;
using namespace Transform;
using namespace QCD;

XsecIntegrand::XsecIntegrand():
  IntegXsecComp(), gscale_( 1.0 ), 
  indiv_( false ), qtype_( QCD::Flavor::PARTON( 0 ) ),
  i_( 0 ), j_( 0 )
{}

XsecIntegrand::XsecIntegrand( Xsection::XsecCompBase* xc ):
  IntegXsecComp( xc ), gscale_( 1.0 ), 
  indiv_( false ), qtype_( QCD::Flavor::PARTON( 0 ) ),
  i_( 0 ), j_( 0 )
{}

XsecIntegrand::~XsecIntegrand(){}

double XsecIntegrand::operator()( const double& x ){

  if ( indiv_ ) return (*this)( x, qtype_ );

  double r( 0.0 );
  dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
    ->setMoment( x ); // cast necessary : setmoment( double )

  int NF = static_cast< int >( this->coeff()->nf() );
  
  for( int j = - NF; j <= NF; j++ ){
    double& C = this->vCoeffr()[ j + 6 ][ j + 6 ];
    r += C;
  }
  
  r *= gscale_;
  return r;

}

double XsecIntegrand::operator()( const double& x, const double& z ){

  if( this->vevo().size() == 1 ) return (*this)( x );

  double r( 0.0 );
  dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
    ->setMoment( x, z ); // cast necessary : setmoment( double )
  
  int NF = static_cast< int >( this->coeff()->nf() );
  
  for( int i = - NF; i <= NF; i++ ) 
    for( int j = - NF; j <= NF; j++ ){      
      double& C = this->vCoeffr()[ i + 6 ][ j + 6 ];
      //      cout << " i = " << i << " j = " << j << " C = " << C << endl;
      r += C;
    }
  
  r *= gscale_;
  return r;
}

double XsecIntegrand::operator()
  ( const double& x, const QCD::Flavor::PARTON& q ){

  if( q == Flavor::g ) return 0.0;
  double r( 0.0 );

  dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
    ->setMoment( x ); // cast necessary : setmoment( double )  

  // quark part // 
  double& Cq = this->vCoeffr()[ q + 6 ][ q + 6 ];
  ChargeBase& e = *( this->coeff()->charge() );
  double charr = e.bare( Flavor::PARTON( 0 ) ) / e.bare( q );
  
  r += charr * Cq;
  
  // gluon part //
  // note that gluon contribution to individual quark is 1/2 of Cg
  // ( not quark + anti-quark )
  double& Cg = this->vCoeffr()[ 0 + 6 ][ 0 + 6 ];
  //   charr = 0.5 * e.bare( Flavor::PARTON( 0 ) ) 
  //    / e.bare( Flavor::PARTON( 0 ) );
  r += 0.5 * Cg;
  
  return r;
}

complex< double > XsecIntegrand::operator()( const complex< double >& n ){

  if( i_ != j_ ) throw( 1 );
  if( n.imag() != 0.0 ) throw( 1 );

  this->coeff()->setMoment( n );
  // cast unnecessary : setmoment( complex )  

  return gscale_ * this->coeff()->vCoeff()[ i_ + 6 ][ j_ + 6 ];
}

complex< double > XsecIntegrand::operator()( const complex< double >& n, 
					     const complex< double >& m ){ 

  if( n.imag() != 0.0 || m.imag() != 0.0 ) throw( 1 );

  this->coeff()->setMoment( n, m );
  // cast unnecessary : setmoment( complex )  

  return gscale_ * this->coeff()->vCoeff()[ i_ + 6 ][ j_ + 6 ];
}

void XsecIntegrand::setflavors( const int& i, const int& j ){ 
  i_ = i; j_ = j; 
  ( dynamic_cast< IntegXsec::CKernel* >( this->coeff() ) )
    ->setflavors( i_, j_ );
} 
