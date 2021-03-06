#include "StrFunc.hh"
#include "StrFuncSub.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"
#include <Xsec/CKernel.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <Xsec/PDF_SW.hh>

#define CACHE_CONTROL 0

using namespace std;
using namespace Utility;
using namespace unpolSIDISXsec;

StrFuncSub::StrFuncSub( Arguments& args, 
			const int& sub,
			const Xsection::Charge::TARGET& n, 
			const QCD::Flavor::HADRON& h ) throw( int ):
  Xsection::Xsec(),
  coeff_( NULL ), sub_( sub )
{
  
  if( sub_ < 0 || sub_ > 2 ){
    ARG_ERR << "sub must be 0, 1, or 2." << endl;
    throw( 1 );
  }

  try{
    
    this->xsecComp( new XsecComp( args, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    
    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );
    coeff_->usecc( sub_ == 0 ? 1 : ( sub_ == 1 ? 2 : 0 ) );

    this->setS( unpolDISXsec::StrFunc::HERMES_S );
    
    this->setPDFswitch( n );
    
  }
  
  catch( int error ) {
    ARG_ERR << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	    << error << ") !!" << endl;
    throw error;
  }
  
}

StrFuncSub::StrFuncSub( Evolution::KernelBase *pdf,
			Evolution::Kernel *ff,
			const int& sub,
			const Xsection::Charge::TARGET& n,
			const QCD::Flavor::HADRON& h )
  throw( int ):
  Xsection::Xsec(), coeff_( NULL ), sub_( sub )
{
  
  if( sub_ < 0 || sub_ > 2 ){
    ARG_ERR << "sub must be 0, 1, or 2." << endl;
    throw( 1 );
  }

  try{    

    this->xsecComp( new XsecComp( pdf, ff, n, h ) );

    if( this->xsecComp() == NULL ) throw 1 ;
    
    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );
    coeff_->usecc( sub_ == 0 ? 1 : ( sub_ == 1 ? 2 : 0 ) );

    //! S for HERMES Kinematics e : 27.6 GeV  p : 1.0 GeV ( at rest )
    //! SET ONE S VALUE FOR ONE OBJECT !! DO NOT APPLY SEVERAL S
    //! <- because of memory manipulation. 
    //! ( you should set another valiable for caching, curretly n, m, q2 )
    this->setS( unpolDISXsec::StrFunc::HERMES_S );
    
    this->setPDFswitch( n );
    
  }
  
  catch( int error ) {
    ARG_ERR << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	    << error << ") !!" << endl;
    throw error;
  }
  
}

StrFuncSub::~StrFuncSub(){
  delete this->xsecComp() ;
}

complex< double > StrFuncSub::operator()( const complex< double >& n, 
					  const complex< double >& m ) {
  
  if( XsecComp_->vevo().size() == 1 ) throw( 1 );
  
  complex< double > r( 0.0, 0.0 );
  
  // use cached data if existed
  if( CACHE_CONTROL &&
      ! coeff_->getDirect() &&
      this->retrive( n, m, XsecComp_->q2(), r ) ){
    return r; 
  }

  // prepare coefficient functions
  XsecComp_->coeff()->setMoment( n, m );
  
  int NF = static_cast< int >( XsecComp_->coeff()->nf() );
  
  vector< vector< complex< double > > >& C 
    = ( sub_ == 0 ? XsecComp_->vCoeff2() :
	( sub_ == 1 ? XsecComp_->vCoeff3() : 
	  XsecComp_->vCoeff() ) );
  
  for( int j = - NF; j <= NF; j++ ){
    int jdx = j + 6;
    // take PDF from 2nd evolution kernel
    Transform::ComplexFunction&  pdf2 = *( XsecComp_->vpdf()[ 1 ][ jdx ] );
    complex< double > rtmp( 0.0, 0.0 );
    for( int i = - NF; i <= NF; i++ ) {
      int idx = i + 6;
      rtmp +=  C[ idx ][ jdx ] * 
	( coeff_->getDirect() ? (*PDF_)( i, n ) 
	  : (*PDF_)( i, n - 1.0 * sub_ ) );
    }
    r += rtmp * pdf2( m );
  }
  
  r *= getgscale();
  
  // don't use memory cache for direct calc !!!! 
  // cache argument does not include variable y   
  if( CACHE_CONTROL && 
      ! coeff_->getDirect() ) this->insert( n, m, XsecComp_->q2(), r );
  
  return r;
}

void StrFuncSub::update(){
  double scale_ = 
    dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->getXsecfact();
  this->globalScale( scale_ );
}

void StrFuncSub::setS( const double& s ){
  dynamic_cast< XsecComp* >( this->xsecComp() )->setS( s );
}

void StrFuncSub::setX( const double& x ){
  dynamic_cast< XsecComp* >( this->xsecComp() )->setX( x );
}

void StrFuncSub::setDirect( const bool& dir ){
  dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->setDirect( dir );  
}

