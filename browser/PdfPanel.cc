#include "PdfPanel.hh"

#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGButton.h>

#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Evolution/KernelBase.hh>
#include <Evolution/Kernel.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/Parameter.hh>

#include "Browser.hh"
#include "VerView.hh"

using namespace std;
using namespace QCD;
using namespace Evolution;

PdfPanel::PdfPanel( const TGWindow *p ) :
  TGVerticalFrame( p, 400, 100 ),
  kernel_( NULL ), path_( NULL ),
  pdfPanel_( NULL )
{
  this->menu();
  this->MapSubwindows();
  this->MapWindow();
}

PdfPanel::~PdfPanel() {
}

void PdfPanel::menu(){
  
  TGHorizontalFrame *mFrame =  new TGHorizontalFrame( this );
  
  // Xsection Draw Buttom
  TGTextButton *draw = new TGTextButton( mFrame, "Draw" );
  draw->Connect( "Clicked()", "Browser", Browser::instance(), "draw()" );

  // PDF Save Button
  TGLabel      *label    = new TGLabel( mFrame, "File Name:" );
  path_ = new TGTextEntry( mFrame, "" );
  path_->SetWidth( 150 );
  TGTextButton *reload   = new TGTextButton( mFrame, "Reload" );
  TGTextButton *save     = new TGTextButton( mFrame, "Save" );
  
  mFrame->AddFrame( draw );
  mFrame->AddFrame( label );
  mFrame->AddFrame( path_ );
  mFrame->AddFrame( reload );
  mFrame->AddFrame( save );
  
  this->AddFrame( mFrame );
}

void PdfPanel::set( Evolution::Kernel *kernel ){
  if( kernel == NULL || kernel_ == kernel ) return;
  kernel_ = kernel;
  this->insert( this,  kernel_->initialDistribution() );
  this->MapSubwindows();
  this->MapWindow();
}

void PdfPanel::insert( TGCompositeFrame *p, PdfParametrization* param ){
  
  // create main frame
  TGVerticalFrame *mFrame = new TGVerticalFrame( p );
  
  // set configuration file name
  path_->SetText( param->path().c_str() );
  
  vector< PdfBase* > pdfs = param->pdfs();
  for( int i = 0; i < pdfs.size(); i++ ) this->insert( mFrame, pdfs[ i ] );
  
  // insert main frame to the parent
  p->AddFrame( mFrame );
  p->Resize();
}

void PdfPanel::insert( TGCompositeFrame *p, PdfBase* pdf ){
  
  // if the pdf is copy pdf, do nothing.
  if( pdf->src() != "" ) return ;
  
  // main frame for this PdfBaes object
  TGGroupFrame *mFrame = 
    new TGGroupFrame( p, pdf->name().c_str(), kVerticalFrame );
  
  for( PdfBase::iterator i = pdf->begin(); i != pdf->end(); i++ ){
    
    Parameter& par = i->second;

    // create horizontal frame to store parameter information
    TGHorizontalFrame *hframe = new TGHorizontalFrame( mFrame );
    
    // create widget elements
    TGLabel *name = new TGLabel( hframe, par.name().c_str() );
    name->SetWidth( 100 );
    
    VerView *para = new VerView( hframe, par.value(), kernel_, pdf, this );
    VerView *erro = new VerView( hframe, par.error(), kernel_, pdf, this );
    VerView *uppe = new VerView( hframe, par.upper(), kernel_, pdf, this );
    VerView *lowe = new VerView( hframe, par.lower(), kernel_, pdf, this );
    TGCheckButton *fixe = new TGCheckButton( hframe );
    fixe->SetState( par.fixed() ? kButtonDown : kButtonUp );
    
    // insert created object to horizontal frame
    hframe->AddFrame( name );
    hframe->AddFrame( para );
    hframe->AddFrame( erro );
    hframe->AddFrame( uppe );
    hframe->AddFrame( lowe );
    hframe->AddFrame( fixe );
    
    hframe->Resize();
    
    // insert horizontal frame to main frame
    mFrame->AddFrame( hframe );
  }
  
  mFrame->Resize();
  p->AddFrame( mFrame );
}

void PdfPanel::drawPdf(){
  if( pdfPanel_ == NULL ) pdfPanel_ = new TCanvas( "pdf" );
  pdfPanel_->cd();
  
  pdfPanel_->Draw();
}

ClassImp( PdfPanel )
