#include "DGLAPpdf.hh"
#include <iomanip>

#include <QCD/Alpha.hh>
#include <QCD/PdfParametrization.hh>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

DGLAPpdf::DGLAPpdf() : 
  S_( 2, 1 ),
  NSp_( 6, complex< double >( 0.0 ) ),
  NSm_( 6, complex< double >( 0.0 ) ),
  par_( NULL ),
  nf_( 3 )
{
}

DGLAPpdf::DGLAPpdf( const DGLAPpdf& pdf ) :
  S_( pdf.S_ ), NSp_( pdf.NSp_ ), NSm_( pdf.NSm_ ), par_( pdf.par_ ),
  nf_( pdf.nf_ )
{
}

DGLAPpdf::~DGLAPpdf(){
}

DGLAPpdf& DGLAPpdf::operator=( const DGLAPpdf& pdf ){
  if( this != &pdf ){
    S_   = pdf.S_;
    NSp_ = pdf.NSp_;
    NSm_ = pdf.NSm_;
    par_ = pdf.par_;
    nf_  = pdf.nf_;
  }
  return *this;
}

DGLAPpdf& DGLAPpdf::operator<<( PdfParametrization* par ){
  return this->load( par );
}


DGLAPpdf& DGLAPpdf::load( PdfParametrization* par ){
  
  par_ = par;
  
  this->S()[0][0]   = par->q( Flavor::a0 );
  this->S()[1][0]   = par->q( Flavor::g  );
  
  this->NS( +1 )[ 0 ] = par->q( Flavor::uns );
  this->NS( -1 )[ 0 ] = par->q( Flavor::uv );
  //  if( nf_ == 1 ) return *this;
  
  this->NS( +1 )[ 1 ] = par->q( Flavor::dns );
  this->NS( -1 )[ 1 ] = par->q( Flavor::dv );
  //  if( nf_ == 2 ) return *this;
  
  this->NS( +1 )[ 2 ] = par->q( Flavor::sns );
  this->NS( -1 )[ 2 ] = par->q( Flavor::sv );
  if( nf_ == 3 ) return *this;
  
  this->NS( +1 )[ 3 ] = par->q( Flavor::cns );
  this->NS( -1 )[ 3 ] = par->q( Flavor::cv );
  if( nf_ == 4 ) return *this;
  
  this->NS( +1 )[ 4 ] = par->q( Flavor::bns );
  this->NS( -1 )[ 4 ] = par->q( Flavor::bv );
  if( nf_ == 5 ) return *this;
  
  this->NS( +1 )[ 5 ] = par->q( Flavor::tns );
  this->NS( -1 )[ 5 ] = par->q( Flavor::tv );
  
  return *this;
}

void DGLAPpdf::renormalize( const double& nf ){
  
  if( nf_ == nf ) return;                  // do nothing
  
  int now  = static_cast< int >( nf_ );
  int next = static_cast< int >( nf );
  
  // direction of renormalization
  bool forward = ( now < next );
  
  // calculate average singlet component
  complex< double >& a0 = this->S()[0][0];
  complex< double > a0ave = a0 / nf_;
  
  // remove singlet compnent from non-singlet component
  vector< complex< double > >& NSpl = this->NS( +1 );
  for( int i = 0; i < now; i++ ) NSpl[ i ] += a0ave;
  
  // reference to non-singlet minus
  vector< complex< double > >& NSmi = this->NS( -1 );
  
  complex< double > plus, minus;
  
  // add or remove new flavor to singlet for forward or backward 
  for( int i = now ; i != next ; ( forward ? i++ : i-- ) ){ 
    
    if( i == 0 ) {
      plus  = par_->q( Flavor::u ) + par_->q( Flavor::ubar );
      minus = par_->q( Flavor::u ) - par_->q( Flavor::ubar );
    } else if( i == 1 ) {
      plus  = par_->q( Flavor::d ) + par_->q( Flavor::dbar );
      minus = par_->q( Flavor::d ) - par_->q( Flavor::dbar );
    } else if( i == 2 ) {
      plus  = par_->q( Flavor::s ) + par_->q( Flavor::sbar );
      minus = par_->q( Flavor::s ) - par_->q( Flavor::sbar );
    } else if( i == 3 ) {
      plus  = par_->q( Flavor::c ) + par_->q( Flavor::cbar );
      minus = par_->q( Flavor::c ) - par_->q( Flavor::cbar );
    } else if( i == 4 ) {
      plus  = par_->q( Flavor::b ) + par_->q( Flavor::bbar );
      minus = par_->q( Flavor::b ) - par_->q( Flavor::bbar );
    } else if( i == 5 ) {
      plus  = par_->q( Flavor::t ) + par_->q( Flavor::tbar );
      minus = par_->q( Flavor::t ) - par_->q( Flavor::tbar );
    } 
    
    if( forward ){
      a0 += plus;
      NSpl[ i ] = plus;
      NSmi[ i ] = minus;
    } else {
      a0 -= plus;
      NSpl[ i ] = complex< double >( 0.0, 0.0 );
      NSmi[ i ] = complex< double >( 0.0, 0.0 );
    }
    
  }
  
  // add singlet contribution again
  a0ave = a0 / nf;
  for( int i = 0; i < next; i++ ) NSpl[ i ] -= a0ave;
  
  // reset number of flavor
  nf_ = nf;
  
}

namespace Evolution {
  
  ostream& operator<<( ostream& os, DGLAPpdf& pdf ){
    os << "Nf:       " << setw(6)  << pdf.nf() << endl;
    os << "Singlet:  " << setw(18) << pdf.S()[0][0]    << endl;
    os << "Gluon:    " << setw(18) << pdf.S()[1][0]    << endl;
    for( int i = 0; i < pdf.NS( +1 ).size(); i++ ){
      os << "NS(" << i + 1 << "):    "
	 << setw(18) << pdf.NS( +1 )[ i ] 
	 << setw(18) << pdf.NS( -1 )[ i ] 
	 << endl;
    }
    return os;
  }
}
