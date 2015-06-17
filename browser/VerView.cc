#include "VerView.hh"

#include <iostream>

#include <Evolution/Kernel.hh>

#include "PdfPanel.hh"
#include "Browser.hh"

using namespace std;
using namespace QCD;

VerView::VerView( const TGWindow *p, double& v, 
		  Evolution::Kernel *k, PdfBase* pdf, 
		  PdfPanel *panel ) :
  TGNumberEntry( p, v ),
  ver_( v ),
  kernel_( k ),
  pdf_( pdf ),
  panel_( panel )
{
  this->SetWidth( 100 );
  this->Connect( "ValueSet(Long_t)", "VerView", this, "update()" );
}

VerView::~VerView(){
}

void VerView::update(){
  ver_ = this->GetNumber();
  pdf_->update();
  kernel_->clearCache();
  Browser::instance()->update();
}

ClassImp( VerView )
