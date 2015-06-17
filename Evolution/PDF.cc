// $Id: PDF.cc,v 1.13 2009/02/19 04:26:51 miyachi Exp $
/*!
  \file    PDF.cc
  \version $Revision: 1.13 $
  \date    $Date: 2009/02/19 04:26:51 $
  \author  $Author: miyachi $
  \brief   Implementation of PDF class ( for moment )
 */
#include "PDF.hh"
#include "KernelBase.hh"
#include "Kernel.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

PDF::PDF(  KernelBase *kernel, const Flavor::PARTON& q ) :
  kernel_( kernel ), type_( PdfBase::parton ), 
  parton_( q ), combo_( Flavor::a0 )
{
}

PDF::PDF(  KernelBase *kernel, const Flavor::TYPE& q ) :
  kernel_( kernel ), type_( PdfBase::combo ), 
  parton_( Flavor::g ), combo_( q )
{
}

PDF::PDF( const PDF& pdf ) : 
  kernel_( pdf.kernel_ ), type_( pdf.type_ ),
  parton_( pdf.parton_ ), combo_( pdf.combo_ )
{
}

PDF::~PDF() {
}

PDF& PDF::operator=( const PDF& pdf ){
  if( this != &pdf ) {
    kernel_ = pdf.kernel_;
    type_   = pdf.type_;
    parton_ = pdf.parton_;
    combo_  = pdf.combo_;
  }
  return *this;
}

complex< double > PDF::operator() ( const complex< double >& n ) {
  kernel_->setN( n );
  return ( type_ == PdfBase::parton ?
	   kernel_->q( parton_ ) : kernel_->q( combo_  ) );
}

string PDF::name() const {
  return ( type_ == PdfBase::parton ?
	   Flavor::name( parton_ ) : Flavor::name( combo_ ) );
}

int PDF::id() const {
  return ( type_ == PdfBase::parton ? (int) parton_  :  (int) combo_ );
}

QCD::PdfBase* PDF::parameters(){
  if( kernel_ == NULL ) return NULL;
  Kernel *k = dynamic_cast< Kernel* >( kernel_ );
  if( k == NULL ) return NULL;
  return ( k->initialDistribution() ? 
	   k->initialDistribution()->find( this->name() ) : NULL );
}

ostream& Evolution::operator<<( ostream& os, PDF& pdf ){
  os << "PDF: " << setw(10) << pdf.name() << endl;
  return os;
}
