#include "PdfView.hh"
#include "Evolution/PDFx.hh"
#include "Utility/Timer.hh"

#include <TLine.h>
#include <sstream>

using namespace std;
using namespace QCD;
using namespace Transform;
using namespace Evolution;
using namespace Utility;

PdfView::PdfView( Kernel *k, const double& w ) : 
  TCanvas( "PdfView", "", 800, 600 ),
  k_( k ), w_( w ),
  xmin_( 1.0E-4 ), xmax_( 1.0 ), ymin_( -1.0 ), ymax_( 1.0 ),
  n_( 40 ),
  grsv_( NULL )
{
  
}

PdfView::~PdfView(){
}

void PdfView::draw() {

  this->frame();                           // draw frame at first
  this->legend();                          // create legend
  
  this->pdf();
  if( grsv_ ) this->GRSV();                // GRSV
  legend_->Draw();
  this->Draw();
  
  ostringstream title;
  if( w_ != 0.0 ){
    title << "x" << w_ << "_";
  }
  title << "pdf_" << k_->Q2() << "_" 
	<< ( k_->alpha().scheme().order() == Scheme::LO ? "LO" : "NLO" )
	<< ".png" ;
  this->Print( title.str().c_str() );
  this->WaitPrimitive(); 
  
}

void PdfView::frame() {

  this->SetLogx();                // set log scale

  ymax_ = ( w_ == 1.0 ? 0.6 : 6.0 );
  ymin_ = -1.0 * ymax_;
  
  frame_ = this->DrawFrame( xmin_, ymin_, xmax_, ymax_ );

  ostringstream title;
  if( w_ != 0.0 ){
    title << "x";
    if( w_ != 1.0 ) title << "^{" << w_ << "}";
  }
  title << "PDF(x)";

  frame_->GetXaxis()->SetTitle( "x" );
  frame_->GetYaxis()->SetTitle( title.str().c_str() );

  // center lin4
  TLine *line = new TLine();
  line->SetLineStyle( 2 );
  line->SetLineColor( 1 );
  line->DrawLine( xmin_, 0.0, xmax_, 0.0 );
  
}

void PdfView::legend() {
  
  legend_ =
    ( w_ == 0.0 ?
      new TLegend( 0.60, 0.65, 0.88, 0.88 ) :
      new TLegend( 0.15, 0.65, 0.45, 0.88 ) ) ;
  
  ostringstream title;
  title 
    << ( k_->alpha().scheme().order() == Scheme::LO ? "LO  " : "NLO " )
    << "Q^{2}: "
    << k_->initialDistribution()->initialScale() << " #rightarrow "
    << k_->Q2() << "(GeV^{2})";
  legend_->SetHeader( title.str().c_str() );
  
}

void PdfView::pdf(){

  double x;
  double y;
  double ymax = 0.0; 
  double ymin = 0.0;
  
  PDFx g(  k_, Flavor::g,  4, 6, 1.0E-3, 15.0, 1.6, 0.6  ); 
  PDFx u(  k_, Flavor::u,  4, 6, 1.0E-3, 15.0, 1.6, 0.6  );
  PDFx d(  k_, Flavor::d,  4, 6, 1.0E-3, 15.0, 1.6, 0.6  );
  PDFx s(  k_, Flavor::s,  4, 6, 1.0E-3, 15.0, 1.6, 0.6  );
  PDFx a0( k_, Flavor::a0, 4, 6, 1.0E-3, 15.0, 1.6, 0.6  );
  
  PdfGraph& gG  = this->graph( &g, xmin_, xmax_, n_ );
  frame_->SetMaximum( ymax = ( gG.maxY() > ymax ? gG.maxY() : ymax ) );
  frame_->SetMinimum( ymin = ( gG.minY() < ymin ? gG.minY() : ymin ) );
  gG.SetLineColor( 1 );
  gG.SetLineStyle( 1 );
  gG.Draw("l");
  this->Update();
  
  PdfGraph& gU  = this->graph( &u, xmin_, xmax_, n_  );
  frame_->SetMaximum( ymax = ( gU.maxY() > ymax ? gU.maxY() : ymax ) );
  frame_->SetMinimum( ymin = ( gU.minY() < ymin ? gU.minY() : ymin ) );
  gU.SetLineColor( 2 );
  gU.SetLineStyle( 1 );
  gU.Draw("l");
  this->Update();
  
  PdfGraph& gD  = this->graph( &d, xmin_, xmax_, n_  );
  frame_->SetMaximum( ymax = ( gD.maxY() > ymax ? gD.maxY() : ymax ) );
  frame_->SetMinimum( ymin = ( gD.minY() < ymin ? gD.minY() : ymin ) );
  gD.SetLineColor( 3 );
  gD.SetLineStyle( 1 );
  gD.Draw("l");
  this->Update();
  
  PdfGraph& gS  = this->graph( &s, xmin_, xmax_, n_  );
  frame_->SetMaximum( ymax = ( gS.maxY() > ymax ? gS.maxY() : ymax ) );
  frame_->SetMinimum( ymin = ( gS.minY() < ymin ? gS.minY() : ymin ) );
  gS.SetLineColor( 4 );
  gS.SetLineStyle( 1 );
  gS.Draw("l");
  this->Update();
  
  PdfGraph& gA0 = this->graph( &a0, xmin_, xmax_, n_  ); 
  frame_->SetMaximum( ymax = ( gA0.maxY() > ymax ? gA0.maxY() : ymax ) );
  frame_->SetMinimum( ymin = ( gA0.minY() < ymin ? gA0.minY() : ymin ) );
  gA0.SetLineColor( 1 );
  gA0.SetLineStyle( 2 );
  gA0.Draw("l");
  
  
  frame_->SetMaximum( ymax + 0.05 * ( ymax - ymin ) );
  frame_->SetMinimum( ymin - 0.05 * ( ymax - ymin ) );
  this->Update();
  
  legend_->AddEntry( &gA0, gA0.legend().c_str(), "l" );
  legend_->AddEntry( &gU,   gU.legend().c_str(),  "l" );
  legend_->AddEntry( &gD,   gD.legend().c_str(),  "l" );
  legend_->AddEntry( &gS,   gS.legend().c_str(),  "l" );
  legend_->AddEntry( &gG,   gG.legend().c_str(),  "l" );
  
}


PdfGraph& PdfView::graph( RealFunction* f, 
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

TGraph* PdfView::grsv(  Flavor::PARTON q, double xmin, double xmax, int n ){
  
  grsv_->setPID( q );
  
  TGraph *g = new TGraph( n );
  double step = ( log10( xmax ) - log10( xmin ) ) / n;
  for( int i = 0; i < n; i++ ){
    double x = pow( 10, log10( xmin ) + step * i );
    double y = (*grsv_)( x );
    g->SetPoint( i, x, y );
  }

  g->SetLineStyle( 3 );
  g->SetLineWidth( 1 );
  g->SetLineColor( ( q > 0 ? q : - q ) + 1 );

  return g;
}

void PdfView::GRSV(){
  
  TGraph *grsvG = this->grsv( Flavor::g );
  TGraph *grsvU = this->grsv( Flavor::u );
  TGraph *grsvD = this->grsv( Flavor::d );
  TGraph *grsvS = this->grsv( Flavor::s );
  
  grsvG->Draw("l");
  grsvU->Draw("l");
  grsvD->Draw("l");
  grsvS->Draw("l");
}

