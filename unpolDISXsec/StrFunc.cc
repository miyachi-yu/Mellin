#include "StrFunc.hh"
#include "XsecComp.hh"
#include <Xsec/CKernel.hh>

#define CACHE_CONTROL 0

using namespace std;
using namespace Utility;
using namespace unpolDISXsec;
using namespace Transform;

//! S for HERMES Kinematics e : 27.6 GeV  p : 1.0 GeV ( at rest )
//! SET ONE S VALUE FOR ONE OBJECT !! DO NOT APPLY SEVERAL S
//! <- because of memory manipulation. 
//! ( you should set another valiable for caching, curretly n, m, q2 )
double StrFunc::HERMES_S = 56.2;

StrFunc::StrFunc( Arguments& args, 
		  const Xsection::Charge::TARGET& n ) throw( int ):
  Xsection::Xsec(), coeff_( NULL )
{
  
  try{
    
    this->xsecComp( new XsecComp( args, n ) );
    if( this->xsecComp() == NULL ) throw 1 ;
 
    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );
   
    this->setS( StrFunc::HERMES_S );
   
  }
 
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::StrFunc( Evolution::KernelBase* kernel,
                  const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::Xsec(), coeff_( NULL )
{
  
  try{

    this->xsecComp( new XsecComp( kernel, n ) );
    if( this->xsecComp() == NULL ) throw 1 ;

    coeff_ = dynamic_cast< CoeffFunc* >( XsecComp_->coeff() );
   
    this->setS( StrFunc::HERMES_S );

  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::~StrFunc(){
  delete this->xsecComp();
}

complex< double > StrFunc::operator()( const complex< double >& n ){
  
  if( XsecComp_->vevo().size() != 1 ) throw( 1 );
  
  complex< double > r( 0.0, 0.0 );
  
  // don't use memory cache for direct calc !!!! 
  // cache argument does not include variable y 
  if( CACHE_CONTROL && 
      ! coeff_->getDirect() &&
      this->retrive( n, XsecComp_->q2(), r ) ){
    return r;
  }
  
  // prepare coefficient functions
  XsecComp_->coeff()->setMoment( n );
  
  int NF = static_cast< int >( XsecComp_->coeff()->nf() );
  
  if( coeff_->getDirect() ){
    for( int i = - NF; i <= NF; i++ ){
      
      int idx = i + 6;
      // get coefficient from CKernel
      complex< double >& C1 = XsecComp_->vCoeff()[ idx ][ idx ];
      complex< double >& C2 = XsecComp_->vCoeff2()[ idx ][ idx ];
      complex< double >& C3 = XsecComp_->vCoeff3()[ idx ][ idx ];
      
      // take PDF from 1st evolution kernel
      ComplexFunction& q1 = *( XsecComp_->vpdf()[ 0 ][ idx ] );
      
      r += ( C1 + C2 + C3 ) * q1( n );
    }

  } else {
    
    for( int j = 0; j < 3; j ++ ){        // loop for different moment
      complex< double > n_j = n - 1.0 * j;
      for( int i = - NF; i <= NF; i++ ){  // loop for parton type
	int idx = i + 6;
	complex< double >& C = 
	  ( j == 0 ? XsecComp_->vCoeff2()  :
	    ( j == 1 ? XsecComp_->vCoeff3() : 
	      XsecComp_->vCoeff() ) )[ idx ][ idx ];	
	ComplexFunction& q1 = *( XsecComp_->vpdf()[ 0 ][ idx ] );
	r += C * q1( n_j );
      }
    }
  }
  
  r *= getgscale();
  
  // don't use memory cache for direct calc !!!! 
  // cache argument does not include variable y 
  if( CACHE_CONTROL && 
      ! coeff_->getDirect() ) this->insert( n, XsecComp_->q2(), r );
  
  return r;
}

void StrFunc::update(){
  double scale_ = 
    dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->getXsecfact();
  this->globalScale( scale_ );
}

void StrFunc::setS( const double& s ){
  dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->setS( s );
}

void StrFunc::setX( const double& x ){
  dynamic_cast< XsecComp* >( this->xsecComp() )->setX( x );
}

void StrFunc::setDirect( const bool& dir ){
  dynamic_cast< CoeffFunc* >( XsecComp_->coeff() )->setDirect( dir );  
}
