#include "Xsec.hh"
#include "XsecComp.hh"
#include "CKernel.hh"
#include "PDF_SW.hh"

#include <iostream>
#include <iomanip>
#include <Evolution/Kernel.hh>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace Xsection;

Xsec::Xsec() throw( int ) :
  XsecComp_( NULL ),
  gscale_( 1.0 ),
  cache_(), 
  cache2_(), 
  cdata_( 1, complex< double >( 0.0 ) ),
  indiv_( false ), qtype_( Flavor::PARTON( 0 ) ),
  PDF_( NULL ),
  cacheDisable_( false )
{
  cache_.clear();
  cache2_.clear();
}

Xsec::~Xsec(){
  if( PDF_ ) delete PDF_;
}

void Xsec::setQ2( const double& q2 ){
  XsecComp_->setQ2( q2 );
  this->update();
}

double Xsec::q2(){
  return XsecComp_->q2();
}

vector< vector< ComplexFunction* > >& Xsec::vpdf(){
  return XsecComp_->vpdf();
}

void Xsec::update(){
  XsecComp_->update();
}

void Xsec::setOrder( const Scheme::ORDER& order ){
  XsecComp_->setOrder( order );
}

Scheme::ORDER Xsec::order() {
  return  XsecComp_->order();
}

complex< double > Xsec::operator()( const complex< double >& n, 
				    const complex< double >& m ) {

  //  if( XsecComp_->vevo().size() == 1 ) return (*this)( n );
  if( XsecComp_->vevo().size() == 1 ) throw( 1 );

  complex< double > r( 0.0, 0.0 );
  if( ! Utility::Arguments::ref().hasOpt( "noCache" ) )  
    if( this->retrive( n, m, XsecComp_->q2(), r ) ){
      return r;
    }

  // prepare coefficient functions
  XsecComp_->coeff()->setMoment( n, m );
  
  int NF = static_cast< int >( XsecComp_->coeff()->nf() );
  
  for( int i = - NF; i <= NF; i++ ) 
    for( int j = - NF; j <= NF; j++ ){
      
      // get coefficient from CKernel
      complex< double >& C = XsecComp_->vCoeff()[ i + 6 ][ j + 6 ];

//      cout << " i = " << i << " j = " << j << " C = " << C << endl;
      
      // take PDF from 1st evolution kernel
      ComplexFunction& q1 = *( XsecComp_->vpdf()[ 0 ][ i + 6 ] );
      
      // take PDF from 2nd evolution kernel
      ComplexFunction& q2 = *( XsecComp_->vpdf()[ 1 ][ j + 6 ] );
      
      r += C * q1( n ) * q2( m );
    }
  
  // apply global scale factor
  r *= gscale_;
  
  if( ! Utility::Arguments::ref().hasOpt( "noCache" ) )  
    this->insert( n, m, XsecComp_->q2(), r );

  return r;
}


complex< double > Xsec::operator()( const complex< double >& n ){
  
  if ( indiv_ ) return (*this)( n, qtype_ );
  
  complex< double > r( 0.0, 0.0 );
  if( ! Utility::Arguments::ref().hasOpt( "noCache" ) )  
    if( this->retrive( n, XsecComp_->q2(), r ) ){
      return r;
    }
  
  XsecComp_->coeff()->setMoment( n );
  
  int NF = static_cast< int >( XsecComp_->coeff()->nf() );
  for( int j = - NF; j <= NF; j++ ){
    complex< double >& C = XsecComp_->vCoeff()[ j + 6 ][ j + 6 ];
    ComplexFunction& q   = *( XsecComp_->vpdf()[ 0 ][ j + 6 ] );
    //    cout << "NF: " << NF << " j: "<< j << endl;
    //    cout << "n: "<< n << " C: " << C << " q(n): "<< q(n) << endl;
    r += C * q( n );
  }
  
  // apply global scale factor
  r *= gscale_;

  if( ! Utility::Arguments::ref().hasOpt( "noCache" ) ) 
    this->insert( n, XsecComp_->q2(), r );

  return r;
}

// for individual contribution  //
complex< double > Xsec::operator()( const complex< double >& n, 
				    const Flavor::PARTON& q ){
  
  if( q == Flavor::g ) return complex< double >( 0.0, 0.0 );
  complex< double > r( 0.0, 0.0 );
  
  XsecComp_->coeff()->setMoment( n );
  
  // quark part // 
  complex< double >& Cq = XsecComp_->vCoeff()[ q + 6 ][ q + 6 ];
  ComplexFunction& qq               = *( XsecComp_->vpdf()[ 0 ][ q + 6 ] );
  ChargeBase& e = *( XsecComp_->coeff()->charge() );
  double charr = e.bare( Flavor::PARTON( 0 ) ) / e.bare( q );
  
  r += charr * Cq * qq( n );
  
  // gluon part //
  // note that gluon contribution to individual quark is 1/2 of Cg
  complex< double >& Cg = XsecComp_->vCoeff()[ 0 + 6 ][ 0 + 6 ];
  ComplexFunction&               gg = *( XsecComp_->vpdf()[ 0 ][ 0 + 6 ] );
  //   charr = 0.5 * e.bare( Flavor::PARTON( 0 ) ) 
  //	/ e.bare( Flavor::PARTON( 0 ) );
  
  r += 0.5 * Cg * gg( n );
  
  return r;
}

bool Xsec::retrive( const complex< double >& n, const double& q2, 
		    complex< double >& val ) {
  
  if( cacheDisable_ ) return false;

  if( ! cache_.cached( n, q2 ) ) return false;
  val = cache_.cache( n, q2 ) [ 0 ];
  return true;
}

bool Xsec::retrive( const complex< double >& n, 
		    const complex< double >& m, 
		    const double& q2, 
		    complex< double >& val ) {
  
  if( cacheDisable_ ) return false;
  if( ! cache2_.cached( n, m, q2 ) ) return false;
  val = cache2_.cache( n, m, q2 ) [ 0 ];
  return true;
}

void Xsec::insert( const complex< double >& n, 
		   const double& q2, 
		   complex< double >& val ) {

  if( cacheDisable_ ) return;

  if( cache_.cached( n, q2 ) ) {
    cerr 
      << __FILE__ << ":" << __LINE__ 
      << "(" << n << ", " << q2 << ") has been cached already."
      << endl;
    return;
  }
  cdata_[ 0 ] = val;
  cache_.cache( n, q2, cdata_ );
}

void Xsec::insert( const complex< double >& n, 
		   const complex< double >& m, 
		   const double& q2, 
		   complex< double >& val ) {
  
  if( cacheDisable_ ) return;

  if( cache2_.cached( n, m, q2 ) ) {
    cerr 
      << __FILE__ << ":" << __LINE__ 
      << "(" << n << ", " << m << ", " 
      << q2 << ") has been cached already."
      << endl;
    return;
  }
  cdata_[ 0 ] = val;
  cache2_.cache( n, m, q2, cdata_ );
}

void Xsec::clearCache() {
  cache_.clear();
  cache2_.clear();
}

void Xsec::setPDFswitch( const Xsection::Charge::TARGET& n ){
  switch( n ){
  case Xsection::Charge::p : PDF_ = new Xsection::PDF_P( this ); break;
  case Xsection::Charge::n : PDF_ = new Xsection::PDF_N( this ); break;
  case Xsection::Charge::d : PDF_ = new Xsection::PDF_D( this ); break;
  }
}

namespace Xsection {
  ostream& operator<<( ostream& os, Xsec& xs ){
    
    os << "#----------------------------------------------------#" << endl;
    os << " Xsection::Xsec Information " << endl;
    os << "   Q2 = " << setw(6) << xs.q2() << endl;
    
    
    if( xs.xsecComp() ) os << *(  xs.xsecComp() ) << endl;
    
    os << "#----------------------------------------------------#" << endl;
    
    return os;
  }
}
