#include "KretzerFF.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace PDFSET;

KretzerFF::KretzerFF() : 
  QCD::FFSimple()
{
}

KretzerFF::~KretzerFF() {
}

void KretzerFF::update() {
  
  // copy to member variable
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  b1_.set( a_, b_ );
  
  // force to desable ( 1 - c *x^d ) term.
  c_ = 0.0;
  
  // second moment
  static complex< double > norm( 2.0, 0.0 );
  b00_ = b1_( norm );
  
  if( b00_ == complex< double >( 0.0, 0.0 ) ) {
    A_ = complex< double >( 0.0, 0.0 );
  } else if( !favored_ || !disfavo_ ) {
    A_ = complex< double >( 0.0, 0.0 );
  } else {
    
    favored_->update();
    disfavo_->update();
    
    // update Parameter data
    (*this)[ 0 ].state( "fixed" );
    (*this)[ 0 ].error() = 0.0;
    (*this)[ 0 ].value() 
      = 0.5 * ( (*favored_)( norm ) + (*disfavo_)( norm ) ).real();
    
    A_ =  (*this)[ 0 ].value() / b00_;
    
  }
  
}
