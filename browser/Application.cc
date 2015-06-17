#include "Application.hh"

#include <Utility/Timer.hh>
#include <Tranform/InverseMellin.hh>

#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include <Xsec/Charge.hh>
#include <Xsec/xXsec.hh>

#include <polDIS/StrFunc.hh>

#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>

#include "PdfGraph.hh"
#include "PdfView.hh"
#include "MomentView.hh"
#include "FragXsecPanel.hh"

#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>
#include <TLine.h>

#include <iomanip>
#include <sstream>
#include <cmath>

#include <Utility/Arguments.hh>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Transform;
using namespace Evolution;
using namespace Exp;

Application::Application( int argc, char* argv[] ) throw( int ) : 
  TApplication( "browser", &argc, argv ),
  evo_( NULL ),
  evoFF_( NULL ),
  xsec_( NULL ),
  w_( 0.0 ),
  c_( NULL ), grsv_( NULL ), grsvPath_(""),
  exp_( NULL ),
  mellinType_( "real" ),
  viewPDF_( false ), viewMoment_( false ), viewMellin_( false ), 
  viewG1_( false ),
  viewFF_( false ),
  viewFragXsec_( false ),
  viewFragXsecMom_( false ),
  nucleon_( 1 ),
  q2_( 1.0 ),
  isParton_( true ), q_( Flavor::g ), type_( Flavor::a0 ),
  xsecPanel_( NULL ),
  helper_( NULL )
{

  Arguments *args = Arguments::instance( argc, argv );

  if( args->hasOpt( "Help" ) || args->hasOpt( "help" ) || 
      args->hasOpt( "HELP" ) || args->hasOpt( "?" ) ){
    args->usage( "view", "", "Plot data type (PDF|Moment|g1p|g1n|g1d|FF|Mellin|FragXsec|FragXsecMom)" );
    args->usage( "parton", "", "parton type  (g|u|d|s|ubar|dbar|sbar|..|a0|..)" );
    args->usage( "complex", "", "real|imag|abs|arg" );
    args->usage( "weight", "", "Weight factor " );
  }
  
  try {
    this->configure();
    c_ = new TCanvas( "browser", "browser", 800, 600 );
  }
  
  catch ( int err ) {
  }
  
}

Application::~Application(){
  if( evo_ )   delete evo_;
  if( evoFF_ ) delete evoFF_;
  if( grsv_ )  delete grsv_;
  if( xsec_ )  delete xsec_;
  //  if( xsecPanel_ ) delete xsecPanel_;
}

void Application::configure() throw ( int ) {
  
  Arguments *args = Arguments::instance();
  
  // preparation for the invese Mellin Transform
  // configuration of QCD evolution
  // get weight
  if( args->hasOpt( "weight" ) )
    w_ = atof( args->getOpt( "weight" ).c_str() );
  
  // GRSV2000
  if( args->hasOpt( "GRSV" ) ){
    grsvPath_ = args->getOpt( "GRSV" );
    grsv_ = 
      new PDFLIB::GRSV2000( ( evo_->alpha().scheme().order() == Scheme::NLO ? 
			      PDFLIB::GRSV2000::STANDARD_NLO :
			      PDFLIB::GRSV2000::STANDARD_LO ),
			    grsvPath_.c_str() );
    grsv_->setQ2( evo_->Q2() );
    grsv_->xWeight( w_ );
  } else grsvPath_ = "";
  
  
  if( args->hasOpt( "polPDF" ) ){
    evo_ = new polPDF::Evo( *args );
  }
  
  if( args->hasOpt( "Q2" ) ){
    q2_ = atof( args->getOpt( "Q2" ).c_str() );
  }
  
  if( args->hasOpt( "FF" ) ){
    evoFF_ = new Fragmentation::Evo( *args );
    xsec_  = new FragXsec_T::FragXsec( *args );
    xsec_->setQ2( q2_ );
  }
  
  
  if( args->hasOpt( "parton" ) ){
    string name = args->getOpt( "parton" );
    isParton_ = Flavor::isParton( name );
    if( isParton_ ) q_ = (Flavor::PARTON) Flavor::id( name );
    else type_ = (Flavor::TYPE) Flavor::id( name );
  }

  
  
  if(  args->hasOpt( "view" ) ){
    string option = args->getOpt( "view" );
    
    if( ( option.find( "PDF" ) != option.npos ) && evo_ ){
      viewPDF_    = true;
    }
    
    if( ( option.find( "Moment" ) != option.npos ) && evo_ ) {
      viewMoment_ = true;
    }
    
    if( ( option.find( "Mellin" ) != option.npos ) && evo_ ) {
      viewMellin_ = true;
    }
    
    if( ( option.find( "g1" ) != option.npos ) && evo_ ) {
      viewG1_     = true;
      if( option.find( "g1p" ) != option.npos ) nucleon_ =  1;
      if( option.find( "g1n" ) != option.npos ) nucleon_ = -1;
      if( option.find( "g1d" ) != option.npos ) nucleon_ =  0;
      
    }
    
    
    if( ( option.find( "FF" ) != option.npos ) && evoFF_ ){
      viewFF_ = true;
    }
    
    if( ( option.find( "FragXsec" ) != option.npos ) && xsec_ ){
      viewFragXsec_ = true;
    }
    
    if( ( option.find( "FragXsecMom" ) != option.npos ) && xsec_ ){
      viewFragXsecMom_ = true;
    }
    
  }
  
  if( args->hasOpt( "complex" ) ){
    mellinType_ = args->getOpt( "complex" ) ;
  }
  
  
  if( viewG1_ && args->hasOpt( "expdata" ) ){
    try {
      exp_ = new ExpData( *args );
    } 
    
    catch( int err ) {
    }
    
  }
  
  if( args->hasOpt( "Helper" ) ){
    helper_ = new InverseMellinHelper();
  }
  
}

PdfGraph& Application::createGraph( RealFunction* f, 
				    double xmin, double xmax, int n ){
  cout << "Start Mellin Inversion for " << setw(6) << f->name() << "  "
       << flush;
  Timer timer;
  PdfGraph *g = new PdfGraph( f, xmin, xmax, n, w_ );
  
  double time  = timer.check();
  double ptime = time / g->GetN();
  cout << setw(10) << time  << " s used \t" 
       << setw(10) << ptime << " (s/point) "
       << endl;
  return *g;
}

void Application::drawFF( TH1 *frame, TLegend *legend ){

  double x;
  double y;
  double ymax = 0.0; 
  double ymin = 0.0;
  
  PDFx g(  evoFF_, Flavor::g  ); 
  g.setParameter( 1.8, 0.5 * M_PI );
  g.upper( 20.0 );
  
  PDFx u(  evoFF_, Flavor::u  );
  u.setParameter( 1.8, 0.5 * M_PI );
  u.upper( 20.0 );

  PDFx d(  evoFF_, Flavor::d  );
  d.setParameter( 1.8, 0.5 * M_PI );
  d.upper( 20.0 );

  PDFx s(  evoFF_, Flavor::s  );
  s.setParameter( 1.8, 0.5 * M_PI );
  s.upper( 20.0 );
  
  
  PdfGraph& gG  = this->createGraph( &g, 0.01, 1.0, 40 );
  frame->SetMaximum( ymax = ( gG.maxY() > ymax ? gG.maxY() : ymax ) );
  frame->SetMinimum( ymin = ( gG.minY() < ymin ? gG.minY() : ymin ) );
  gG.SetLineColor( 1 );
  gG.SetLineStyle( 1 );
  gG.Draw("l");
  c_->Update();
  
  PdfGraph& gU  = this->createGraph( &u, 0.01, 1.0, 40  );
  frame->SetMaximum( ymax = ( gU.maxY() > ymax ? gU.maxY() : ymax ) );
  frame->SetMinimum( ymin = ( gU.minY() < ymin ? gU.minY() : ymin ) );
  gU.SetLineColor( 2 );
  gU.SetLineStyle( 1 );
  gU.Draw("l");
  c_->Update();
  
  PdfGraph& gD  = this->createGraph( &d, 0.01, 1.0, 40  );
  frame->SetMaximum( ymax = ( gD.maxY() > ymax ? gD.maxY() : ymax ) );
  frame->SetMinimum( ymin = ( gD.minY() < ymin ? gD.minY() : ymin ) );
  gD.SetLineColor( 3 );
  gD.SetLineStyle( 1 );
  gD.Draw("l");
  c_->Update();
  
  PdfGraph& gS  = this->createGraph( &s, 0.01, 1.0, 40  );
  frame->SetMaximum( ymax = ( gS.maxY() > ymax ? gS.maxY() : ymax ) );
  frame->SetMinimum( ymin = ( gS.minY() < ymin ? gS.minY() : ymin ) );
  gS.SetLineColor( 4 );
  gS.SetLineStyle( 1 );
  gS.Draw("l");
  c_->Update();
  
  frame->SetMaximum( ymax + 0.05 * ( ymax - ymin ) );
  frame->SetMinimum( ymin - 0.05 * ( ymax - ymin ) );
  c_->Update();
  
  legend->AddEntry( &gU, gU.legend( "FF" ).c_str(),  "l" );
  legend->AddEntry( &gD, gD.legend( "FF" ).c_str(),  "l" );
  legend->AddEntry( &gS, gS.legend( "FF" ).c_str(),  "l" );
  legend->AddEntry( &gG, gG.legend( "FF" ).c_str(),  "l" );
  
}

void Application::createFF(){
  
  c_->SetLogx();
  double ymax = ( w_ == 1.0 ? 0.6 : 6.0 );

  TH1* frame = c_->DrawFrame( 0.01, - ymax, 1.0, ymax );
  frame->GetXaxis()->SetTitle( "z" );
  ostringstream title;
  if( w_ != 0.0 ){
    title << "z";
    if( w_ != 1.0 ) title << "^{" << w_ << "}";
  }
  title << "FF(x)";
  frame->GetYaxis()->SetTitle( title.str().c_str() );
  
  // center lin4
  TLine *line = new TLine();
  line->SetLineStyle( 2 );
  line->SetLineColor( 1 );
  line->DrawLine( 0.01, 0.0, 1.0E+0, 0.0 );
  
  TLegend *legend =
    ( w_ == 0.0 ?
      new TLegend( 0.60, 0.65, 0.88, 0.88 ) :
      new TLegend( 0.15, 0.65, 0.45, 0.88 ) ) ;
  
  title.str("");
  title 
    << ( evoFF_->alpha().scheme().order() == Scheme::LO ? "LO  " : "NLO " )
    << "Q^{2}: "
    << evoFF_->initialDistribution()->initialScale() << " #rightarrow "
    << evoFF_->Q2() << "(GeV^{2})";
  legend->SetHeader( title.str().c_str() );
  
  this->drawFF( frame, legend );
  legend->Draw();
  c_->Draw();
  
  title.str("");
  if( w_ != 0.0 ){
    title << "x" << w_ << "_";
  }
  title << "ff_" << evoFF_->Q2() << "_" 
	<< ( evoFF_->alpha().scheme().order() == Scheme::LO ? "LO" : "NLO" )
	<< ".png" ;
  c_->Print( title.str().c_str() );
  c_->WaitPrimitive(); 
}

void Application::createPDF(){
  
  PdfView *c = new PdfView( evo_, w_ );
  if( grsv_ ) c->GRSV( grsv_ );
  c->draw();
  
}


void Application::Run( Bool_t retrn ){
  
  if( viewPDF_    ) this->createPDF();
  if( viewMoment_ ) this->createMoment();
  if( viewG1_ )     this->createG1();
  if( viewFF_ )     this->createFF();
  if( viewFragXsec_ ) {
    xsecPanel_ = new FragXsecPanel( Arguments::ref() );
    
    if( helper_ ){
      //      xsecPanel_->xsec()->setQ2( 100.0 );
      //      helper_->set( xsecPanel_->g() );
      //      helper_->offset( 2.5, 3.5, 10 );
      //      helper_->angle( 0.5, 0.6, 10 );
      //      helper_->length( 5.0, 15.0, 10 );
    }
    
  }
  
  if( viewMellin_ || viewFragXsecMom_ ){
    
    MomentView *c = NULL;
    
    if( viewFragXsecMom_ ) c = new MomentView( xsec_ );
    else c = ( isParton_ ? 
	       new MomentView( evo_, q_ ) :
	       new MomentView( evo_, type_ )  );
    
    c->draw( mellinType_ );
  }
  
  return;
}

void Application::createG1(){
  
  
  ostringstream ost;
  
  c_->SetLogx();

  double xmin = 1.0E-3;
  double xmax = 1.0;
  double ymax =  0.1;
  double ymin = -0.1;
  
  TH1 *frame = c_->DrawFrame( xmin, ymin, xmax, ymax );
  frame->GetXaxis()->SetTitle( "x" );
  frame->GetYaxis()->SetTitle( "g_{1}" );
  
  TLine *line = new TLine();
  line->SetLineStyle( 2 );
  line->DrawLine( xmin, 0.0, xmax, 0.0 );
  
  string strType =
    ( nucleon_ == 1 ? "g1p" : ( nucleon_ == -1 ? "g1n" : "g1d" ) );
  
  if( exp_ ) {

    vector< DataSet* >& data = exp_->data();
    int icol = 1;
    TLegend *legend = NULL;
    
    for( int i = 0; i < data.size(); i++ ){
      if( data[ i ]->size() == 0 ) continue;
      DataSet& set = *data[i];


      if( set[0].info().substr( 0, 3 ) != strType ) continue;
      
      if( legend == NULL ) {
	legend = new TLegend( 0.6, 0.31, 0.89, 0.45 );
	legend->SetHeader( "Exp Data" );
      }
      PdfGraph *gExp = new PdfGraph( set );
      gExp->SetMarkerColor( icol++ );
      legend->AddEntry( gExp, set.info().c_str(), "p" );
      
      ymax = gExp->maxY();
      ymin = gExp->minY();
      double margin = 0.05 * ( ymax - ymin );
      frame->SetMaximum( ymax + margin );
      frame->SetMinimum( ymin - margin );
      gExp->Draw("P");
      
      c_->Update();
     
   }
    
    if( legend ) {
      legend->Draw();
      c_->Update();
    }

  }
  
  
  /*
    polDIS::G1_x g1( evo_,
    ( nucleon_ == 1 ? polDIS::G1_n::proton : 
    ( nucleon_ == -1 ? polDIS::G1_n::neutron : 
    polDIS::G1_n::deuteron ) ) );
  */
  Xsection::Charge::TARGET N = 
    ( nucleon_ == 1 ? Xsection::Charge::p : 
      ( nucleon_ == -1 ? Xsection::Charge::n : Xsection::Charge::d ) );
  
  polDIS::StrFunc g1n(  Arguments::ref(), N );
  Xsection::xXsec g1( &g1n );
  g1.setParameter( 1.2, 0.55 );
  g1.upper( 10.0 );
  
  vector< double > vq2;
  vq2.push_back(   1.0 );
  vq2.push_back(   2.5 );
  vq2.push_back(  10.0 );
  vq2.push_back( 100.0 );
  
  TLegend *legend = new TLegend( 0.6, 0.13, 0.89, 0.3 );
  ost.str("");
  ost << ( nucleon_ == 1 ? "proton" : 
	   ( nucleon_ == -1 ? "neutron" : "deuteron" ) ) 
      << "  "
      << ( evo_->alpha().scheme().order() == Scheme::LO ? "LO" : "NLO" );
  
  
  legend->SetHeader( ost.str().c_str() );
  
  for( int i = 0; i < vq2.size(); i++ ){
    g1n.setQ2( vq2[i] );
    //    evo_->setQ2( vq2[i] );
    
    PdfGraph& gg1 = this->createGraph( &g1, xmin, xmax );
    gg1.SetLineColor( i + 1 );
    gg1.SetLineWidth( 2 );
    gg1.SetLineStyle( 1 );
    
    ost.str("");
    ost << vq2[i] << " GeV^{2}" ;
    
    legend->AddEntry( &gg1, ost.str().c_str(), "l" );
    legend->Draw();

    ymax = ( gg1.maxY() > ymax ? gg1.maxY() : ymax );
    ymin = ( gg1.minY() < ymin ? gg1.minY() : ymin );
    
    double margin = 0.1 * ( ymax - ymin );
    frame->SetMaximum( ymax + margin );
    frame->SetMinimum( ymin - margin );
    
    gg1.Draw("l");
    c_->Update();
  }

  c_->Draw();
  
  string ofname =
    string( nucleon_ == 1 ? "g1p" : ( nucleon_ == -1 ? "g1n" : "g1d" ) ) +
    "_" +
    string( evo_->alpha().scheme().order() == Scheme::LO ? "LO" : "NLO" ) +
    ".png";
  
  c_->Print( ofname.c_str() );
  c_->WaitPrimitive();   
}

void Application::createMoment(){
  c_->SetLogx( 0 ); // back to linear
  double q20  =   1.0;
  double q2   = 100.0;
  
  PDF g(  evo_, Flavor::g  ); 
  PDF u(  evo_, Flavor::u  ); 
  PDF d(  evo_, Flavor::d  ); 
  PDF s(  evo_, Flavor::s  ); 
  PDF a0( evo_, Flavor::a0  ); 
 
  TGraph *gG  = new TGraph( 100 );
  TGraph *gU  = new TGraph( 100 );
  TGraph *gD  = new TGraph( 100 );
  TGraph *gS  = new TGraph( 100 );
  TGraph *gA0 = new TGraph( 100 );
  
  TGraph *gAlpha = new TGraph( 100 );
  
  double ymax =  1.0;

  for( int i = 0; i < 100; i++ ){
    double q2 = i + 1.0;
    evo_->setQ2( q2 );
    complex< double > n = w_ + 1.0;
    gG->SetPoint(  i,  q2,  g( n ).real() );
    gU->SetPoint(  i,  q2,  u( n ).real() );
    gD->SetPoint(  i,  q2,  d( n ).real() );
    gS->SetPoint(  i,  q2,  s( n ).real() );
    gA0->SetPoint( i,  q2, a0( n ).real() );
    
    gAlpha->SetPoint( i, q2, evo_->alpha()( q2 ) );
    
    if( g( n ).real() > ymax ) ymax =  g( n ).real() + 0.2 ;
    
  }
  
  gG->SetLineWidth( 2 );
  gU->SetLineWidth( 2 );
  gD->SetLineWidth( 2 );
  gS->SetLineWidth( 2 );
  gA0->SetLineWidth( 2 );
  gA0->SetLineStyle( 3 );

  gAlpha->SetLineStyle( 1 );
  gAlpha->SetLineColor( 6 );
  
  gG->SetLineColor( 1 );
  gU->SetLineColor( 2 );
  gD->SetLineColor( 3 );
  gS->SetLineColor( 4 );

  double ymin = -0.5;
  
  TH1 *frame = c_->DrawFrame( q20, ymin, q2, ymax );
  frame->GetXaxis()->SetTitle( "Q^{2} (GeV^{2})" );
  frame->GetYaxis()->SetTitle( "Moment" );

  TLine *line = new TLine();
  line->SetLineStyle( 2 );
  line->DrawLine( q20, 0.0, q2, 0.0 );

  gG->Draw( "l" );
  gU->Draw( "l" );
  gD->Draw( "l" );
  gS->Draw( "l" );
  gA0->Draw( "l" );
  gAlpha->Draw( "l" );

  c_->Draw();
  c_->WaitPrimitive(); 

}
