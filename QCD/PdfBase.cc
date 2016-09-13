// $Id: PdfBase.cc,v 1.31 2010/03/03 03:04:38 miyachi Exp $
/*!
  \file     PdfBase.cc
  \version  $Revision: 1.31 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization base class
 */
#include "PdfBase.hh"
#include "Flavor.hh"

#include <Tranform/Mellin.hh>
#include <Tranform/IntegFunc.hh>

using namespace std;
using namespace Transform;
using namespace QCD;

const double PdfBase::upperlimit =  1.0E2;
const double PdfBase::lowerlimit = -1.0E2;

PdfBase::PdfBase( const TYPE& type, const int& id, const int& n ) :
  map< int, Parameter >(),
  type_( type ), id_( id ), 
  corr_( n, vector< double >( n, 0.0 ) ), 
  pid_( "" ), src_( "" ),
  wrapper_( this ), mellin_( NULL ), integ_( NULL ), toBeDel_( false ),
  dfav_(""), fav_(""), 
  reference_( NULL ),
  parametrization_( NULL )
{
  integ_ = new IntegFunc();
  if( integ_  && 
      ( mellin_ = new Mellin( integ_->integration() ) ) ){ 
    mellin_->upper( 1.0 );
    mellin_->lower( 0.0 );
    toBeDel_ = true;
  }
}

PdfBase::PdfBase( const PdfBase& pdf ) : 
  map< int, Parameter >( pdf ),
  type_( pdf.type_ ), id_( pdf.id_ ),
  corr_( pdf.corr_ ),
  pid_( pdf.pid_ ), src_( pdf.src_ ),
  wrapper_( pdf.wrapper_ ), mellin_( pdf.mellin_ ), integ_( pdf.integ_ ),
  toBeDel_( false ),
  dfav_( pdf.dfav_ ), fav_( pdf.fav_ ),
  reference_( pdf.reference_ ),
  parametrization_( pdf.parametrization_ )
{
}

PdfBase::~PdfBase() {
  if( toBeDel_ && mellin_ ){
    delete mellin_->integration();
    delete mellin_;
  }
}

PdfBase::RealFunctionWrapper::RealFunctionWrapper( PdfBase *pdf ) :
  pdf_( pdf ) {
}

PdfBase::RealFunctionWrapper::RealFunctionWrapper( const RealFunctionWrapper& wrap ) :
  pdf_( wrap.pdf_ ) {
}

PdfBase::RealFunctionWrapper::~RealFunctionWrapper() {
}

double PdfBase::RealFunctionWrapper::operator()( const double& x ){ 
  return pdf_->x( x ); 
}

PdfBase::RealFunctionWrapper& 
PdfBase::RealFunctionWrapper::operator=( const RealFunctionWrapper& wrap ){
  if( this != &wrap ) pdf_ = wrap.pdf_;
  return *this;
}

PdfBase& PdfBase::operator=( const PdfBase& pdf ){
  if( this != &pdf ) {
    type_    = pdf.type_;
    id_      = pdf.id_;
    corr_    = pdf.corr_;
    pid_     = pdf.pid_;
    src_     = pdf.src_;
    wrapper_ = wrapper_;
    if( mellin_ && toBeDel_ )  delete mellin_; 
    mellin_ = pdf.mellin_;
    toBeDel_ = false;
    dfav_    = pdf.dfav_;
    fav_     = pdf.fav_;
  }
  return *this;
}

string PdfBase::name() const {
  return ( type_ == parton ?
	   Flavor::name( (Flavor::PARTON) id_ ) :
	   Flavor::name( (Flavor::TYPE) id_ ) );
}

void PdfBase::resize( const int& n ){
}

complex< double > 
PdfBase::operator() ( const complex< double >& z ){
  if( mellin_ == 0 ) return complex< double >( 0.0, 0.0 );
  return (*mellin_)( z );
}

namespace QCD {
  ostream& operator<<( ostream& os, PdfBase& pdf ){
    
    os << "<pdf name=\"" << pdf.name() << "\""<< flush;
    
    if( pdf.pid_ != "" ) os << " pid=\"" << pdf.pid_ << "\"" << flush;
    if( pdf.src_ != "" ) os << " src=\"" << pdf.src_ << "\"" << flush;
    if( pdf.dfav_ != "" ) os << " dfav=\"" << pdf.dfav_ << "\"" << flush;
    if( pdf.fav_ != "" ) os << " fav=\"" << pdf.fav_ << "\"" << flush;
    
    os  << ">" << flush; 
    
    if( pdf.dfav_ != "" && pdf.fav_ != "" ){ // KretzerFF or FFSimple(FavDFav)
      os << "<!--  This is a Kretzer type fragmentation function"
	 << " 0th parameter will be ignored. -->" << flush;
    } else if( pdf.dfav_ != "" ) { // dis-favored ff
      os << "<!--  This is a dis-favored fragmentation function:"
	 << " N = " << pdf.getValue( 0 ) 
	 << ", a = " << pdf.getValue( 1 ) 
	 << ", b = " << pdf.getValue( 2 ) 
	 << " -->" << flush;
    }
    
    if( pdf.size() > 0 ) {
      os << endl;
      for( PdfBase::iterator itr = pdf.begin(); itr != pdf.end(); itr++ ) {
	if( itr->second.checkSrcid() || itr->second.add() || itr->second.multi() ) {
	  os << "<!-- "
	     << "index=\"" << itr->second.index() 
	     << "\" name=\"" << itr->second.name() 
	     << "\" value=\"" << pdf.getValue( itr->second.index() ) << "\""
	     << " -->" << endl;
	}
	os << itr->second << endl;
      }
    }
  
    os << "</pdf>" << flush;
    return os;
  }
}

double& PdfBase::value( const int& i ){
  static double def = 0.0;
  PdfBase::iterator itr;
  if( ( itr = this->find( i ) ) != this->end() ) return itr->second.value();
  return ( reference_ ? reference_->value( i ) : def );
}



double PdfBase::getValue( const int& i ){
  
  PdfBase::iterator itr = this->find( i );
  
  double ref( 0.0 );
  
  // if there is reference, check the reference value at first.
  if( reference_ ) ref = reference_->getValue( i );

  // if PdfBase has no corresponding entry, just return reference value
  if( itr == this->end() ) return ref;
  
  // if this entry referes the another entry, overwrite reference value
  bool hasSrcid( false );
  if( ( hasSrcid = itr->second.checkSrcid() ) && 
      itr->second.index() != itr->second.srcid() ){
    ref = this->getValue( itr->second.srcid() );
  }
  
  // get value assigned this entry
  double val = itr->second.value();
  if( itr->second.add() )   return ref + val;
  if( itr->second.multi() ) return ref * val;
  
  return ( hasSrcid ? ref : val );
}
