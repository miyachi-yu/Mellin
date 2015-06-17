#include "StrFunc.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"

#include <Xsec/PDF_SW.hh>
#include <Xsec/CKernel.hh>
#include <unpolDISXsec/StrFunc.hh>

using namespace std;
using namespace Utility;
using namespace unpolSIDISXsec;

#define CACHE_CONTROL 0

StrFunc::StrFunc( Arguments& args, 
		  const Xsection::Charge::TARGET& n, 
		  const QCD::Flavor::HADRON& h ) throw( int ):
  Xsection::Xsec(), coeff_( NULL )
{
  
  try{
    
    this->xsecComp( new XsecComp( args, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    
    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );
    
    this->setS( unpolDISXsec::StrFunc::HERMES_S );

    this->setPDFswitch( n );

  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
}

StrFunc::StrFunc( Evolution::KernelBase *pdf,
                  Evolution::Kernel *ff,
                  const Xsection::Charge::TARGET& n,
                  const QCD::Flavor::HADRON& h )
  throw( int ):
  Xsection::Xsec(), coeff_( NULL )
{
  
  try{    
    this->xsecComp( new XsecComp( pdf, ff, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;

    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );

    //! S for HERMES Kinematics e : 27.6 GeV  p : 1.0 GeV ( at rest )
    //! SET ONE S VALUE FOR ONE OBJECT !! DO NOT APPLY SEVERAL S
    //! <- because of memory manipulation. 
    //! ( you should set another valiable for caching, curretly n, m, q2 )
    this->setS( unpolDISXsec::StrFunc::HERMES_S );

    this->setPDFswitch( n );

  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::~StrFunc(){
  delete this->xsecComp() ;
}

complex< double > StrFunc::operator()( const complex< double >& n, 
				       const complex< double >& m ) {
  
  //   if( XsecComp_->vevo().size() == 1 ) return (*this)( n );
  if( XsecComp_->vevo().size() == 1 ) throw( 1 );
  //    return  (*dynamic_cast< Xsection::Xsec* >(this))( n );
  //    charge != Xsection's

  complex< double > r( 0.0, 0.0 );
  
  // don't use memory cache for direct calc !!!! 
  // cache argument does not include variable y 
  if( CACHE_CONTROL &&
      ( ! coeff_->getDirect() ) && this->retrive( n, m, XsecComp_->q2(), r ) ){
    return r;
  }
  
  // prepare coefficient functions
  XsecComp_->coeff()->setMoment( n, m );
  
  int NF = static_cast< int >( XsecComp_->coeff()->nf() );
  
  for( int j = - NF; j <= NF; j++ ){
    
    int jdx = j + 6;
    // take PDF from 2nd evolution kernel
    Transform::ComplexFunction&  pdf2 = *( XsecComp_->vpdf()[ 1 ][ jdx ] );
    complex< double > rtmp( 0.0, 0.0 );
    
    if( coeff_->getDirect() ){
      
      for( int i = - NF; i <= NF; i++ ) {
	int idx = i + 6;
	// get coefficient from CKernel
	complex< double >& C1 = XsecComp_->vCoeff()[ idx ][ jdx ];
	complex< double >& C2 = XsecComp_->vCoeff2()[ idx ][ jdx ];
	complex< double >& C3 = XsecComp_->vCoeff3()[ idx ][ jdx ];
	rtmp += ( C1 + C2 + C3 ) * (*PDF_)( i, n ) ;
      }
      
    } else {
      
      for( int k = 0; k < 3; k++ ){
	complex< double > n_k = n - 1.0 * k ;
	for( int i = - NF; i <= NF; i++ ) {
	  int idx = i + 6;
	  complex< double >& C = 
	    ( k == 0 ? XsecComp_->vCoeff2() :
	      ( k == 1 ? XsecComp_->vCoeff3() : 
		XsecComp_->vCoeff() ) )[ idx ][ jdx ];
	  
	  rtmp += C * (*PDF_)( i, n_k ) ;
	}
      }
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

void StrFunc::update(){
  double scale_ = 
    dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->getXsecfact();
  this->globalScale( scale_ );
}

void StrFunc::setS( const double& s ){
  dynamic_cast< XsecComp* >( this->xsecComp() )->setS( s );
}

void StrFunc::setX( const double& x ){
  dynamic_cast< XsecComp* >( this->xsecComp() )->setX( x );
}

void StrFunc::setDirect( const bool& dir ){
  dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->setDirect( dir );  
}

