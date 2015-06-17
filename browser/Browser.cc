#include "Browser.hh"

#include <Utility/Arguments.hh>
#include <FragXsec_T/FragXsec.hh>
#include <ExpData/ExpData.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/XsecComp.hh>

using namespace std;
using namespace Utility;
using namespace Exp;

#include "G1Panel.hh"
#include "FragXsecPanel.hh"

#include <TROOT.h>

Browser*  Browser::instance_ = NULL;

Browser* Browser::instance( int argc, char* argv[] ) {
  if( Browser::instance_ == NULL ) 
    Browser::instance_ = new Browser( argc, argv );
  return Browser::instance_;
}

Browser* Browser::instance() { return Browser::instance_; }


Browser::Browser( int argc, char* argv[] ) :
  TRint( "Mellin Browser", &argc, argv ),
  ff_( NULL ),
  panel_( NULL ),
  view_( NULL ),
  expr_( NULL )
{
  
  this->SetPrompt( "Mellin > " );
  // Arguments object will be used sometime later.....
  // so it should be kept somewhat in the memory...
  Arguments& args = Arguments::ref( argc, argv );

  gROOT->SetStyle( args.get( "gStyle", "Plain" ).c_str() );

  // ------------------  polPDF and g1  ------------------------ //
  vector< string > polPdfOpts = polPDF::Evo::options();
  if( args.hasOpt( polPdfOpts ) != polPdfOpts.end() ) {
    try {
      g1view_ = new G1Panel();
    } 
    catch( int e ){
      ARG_ERR << " catch error " << e << endl;
    }
  }
  
  // ------------------  FF and FragXsec --------------------- //
  vector< string > ffPi = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string > ffKa = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string > ffRe = Fragmentation::Evo::options( QCD::Flavor::REp );
  vector< string > ffPr = Fragmentation::Evo::options( QCD::Flavor::PRp );
  if( args.hasOpt( ffPi ) != ffPi.end() || 
      args.hasOpt( ffKa ) != ffKa.end() || 
      args.hasOpt( ffRe ) != ffRe.end() || 
      args.hasOpt( ffPr ) != ffPr.end()   ) {
    
    try {
      view_ = new FragXsecPanel( args );
    }
    catch( int e ){
      ARG_ERR << " catch error " << e << endl;
    }
    
  }
  
}

Browser::~Browser(){
}

void Browser::pdfPanel() {
  
  if( panel_ == NULL && ff_ ) {
    panel_ = new PdfPanel( gClient->GetRoot() );
    panel_->set( view_->ff() );
  }
  
  if( panel_ == NULL && g1view_ ){
    Evolution::Kernel *k = 
      dynamic_cast< Evolution::Kernel* >( g1view_->xsec()->xsecComp()->vevo()[0] );
    if( k != NULL ) {
      panel_ = new PdfPanel( gClient->GetRoot() );
      panel_->set( k );
    }
  }
  
}

void Browser::draw() {
  if( view_ ) view_->draw();
  if( g1view_ ) g1view_->draw();
}

void Browser::update(){
  if( view_ ) view_->update();

  if( g1view_ ) { 
    g1view_->xsec()->clearCache();
  }
}

ClassImp( Browser )

