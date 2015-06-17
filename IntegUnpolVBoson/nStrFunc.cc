#include "nStrFunc.hh"
#include "nXsecComp.hh"
#include <Xsec/CKernel.hh>
#include <Xsec/PDF_SW.hh>

using namespace std;
using namespace Utility;
using namespace IntegUnpolVBoson;

nStrFunc::nStrFunc( Arguments& args, 
		    const IntegXsec::Charge::TARGET& n,
		    const QCD::Flavor::HADRON& h  ) throw( int ):
  Xsection::Xsec()
{
  
  try{
    this->xsecComp( new nXsecComp( args, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  int i = n;
  this->setPDFswitch( Xsection::Charge::TARGET( i ) );
  
}

nStrFunc::nStrFunc( Evolution::KernelBase *pdf,
		    const IntegXsec::Charge::TARGET& n,
		    const QCD::Flavor::HADRON& h )
  throw( int ):
  Xsection::Xsec()
{
  
  try{    
    this->xsecComp( new nXsecComp( pdf, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

  int i = n;  
  this->setPDFswitch( Xsection::Charge::TARGET( i ) );
}

nStrFunc::~nStrFunc(){
  delete this->xsecComp() ;
}

complex< double > nStrFunc::operator()( const complex< double >& n, 
				       const complex< double >& m ) {
  
  //   if( XsecComp_->vevo().size() == 1 ) return (*this)( n );
  if( XsecComp_->vevo().size() == 1 ) throw( 1 );
  //    return  (*dynamic_cast< Xsection::Xsec* >(this))( n );
  //    charge != Xsection's
  
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
      
      // take PDF from 1st evolution kernel
      //      PDF& q1 = *( XsecComp_->vpdf()[ 0 ][ i + 6 ] );
      
      // take PDF from 2nd evolution kernel
      Transform::ComplexFunction& q2 = *( XsecComp_->vpdf()[ 1 ][ j + 6 ] );
      
      //      r += C * q1( n ) * q2( m );
      r += C * (*PDF_)( i, n ) * q2( m );
    }

  r *= getgscale(); 

  if( ! Utility::Arguments::ref().hasOpt( "noCache" ) )  
    this->insert( n, m, XsecComp_->q2(), r );
  return r;
}
