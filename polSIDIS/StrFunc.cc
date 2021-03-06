#include "StrFunc.hh"
#include "XsecComp.hh"
#include <Xsec/CKernel.hh>
#include <Xsec/PDF_SW.hh>

//#include "CoeffFunc.hh"

using namespace std;
using namespace Utility;
using namespace polSIDIS;

StrFunc::StrFunc( Arguments& args, 
		  const Xsection::Charge::TARGET& n,
		  const QCD::Flavor::HADRON& h )
  throw( int ):
  Xsection::Xsec()
{
  
  try{    
    this->xsecComp( new XsecComp( args, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    // for deuteron targe, D-wave correction should be applied.
    // g1d = ( 1 - 1.5 * w_D )( g1p + g1n ), where w_D = 0.05
    if( n == Xsection::Charge::d ) this->globalScale( 0.925 );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  this->setPDFswitch( n );  
}

StrFunc::StrFunc( Evolution::Kernel *pdf,
		  Evolution::Kernel *ff,
		  const Xsection::Charge::TARGET& n,
		  const QCD::Flavor::HADRON& h )
  throw( int ):
  Xsection::Xsec()
{
  
  try{    
    this->xsecComp( new XsecComp( pdf, ff, n, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    // for deuteron targe, D-wave correction should be applied.
    // g1d = ( 1 - 1.5 * w_D )( g1p + g1n ), where w_D = 0.05
    if( n == Xsection::Charge::d ) this->globalScale( 0.925 );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->setPDFswitch( n );
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

