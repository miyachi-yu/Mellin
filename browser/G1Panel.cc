#include "G1Panel.hh"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <TAxis.h>
#include <TVirtualPad.h>
#include <TH1.h>
#include <TLine.h>
#include <TLatex.h>

#include <Utility/Arguments.hh>
#include <Tranform/GridIntegration.hh>
#include <ExpData/ExpData.hh>
#include <Xsec/xXsec.hh>
#include <polDIS/StrFunc.hh>

#include "PdfGraph.hh"

using namespace std;

G1Panel::G1Panel() : 
  TCanvas( "G1Panel", "Structure Function, g_{1}", 600, 800 ),
  edata_( NULL ),
  g1_( 3, ( Xsection::xXsec* ) NULL ),  
  gg1_( 3, vector< PdfGraph* >( 0 ) ),
  line_( new TLine() ), latex_( new TLatex() ),
  xrange_( 2, 0.0 ),
  style_(),
  q2_( 0 ),
  weight_( 0.0 ),
  xedge_( ),
  q2p_( xedge_.size(), (TGraph*) NULL ),
  q2d_( xedge_.size(), (TGraph*) NULL ),
  vsX_( true )
{
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // experimental data
  if( args.hasOpt( "expdata" ) ) edata_ = new Exp::ExpData( args );
  
  double precision = args.get( "precision", 5.0E-3 );
  int    leg1      = args.get( "leg1", 6 );
  int    leg2      = args.get( "leg2", 8 );
  double offset    = args.get( "offset", 2.00  );  
  double angle     = args.get( "angle",  0.55  );  
  double length    = args.get( "length", 20.0 );  
  int    ngrid     = args.get( "ngrid",  2 );
  
  // structure functions
  vector< string > opts = polPDF::Evo::options();
  if( args.hasOpt( opts ) != opts.end() ) {
    
    g1_[ 0 ] = 
      new Xsection::xXsec( new polDIS::StrFunc( args, Xsection::Charge::p ),
			   leg1, leg2, precision  );
    
    g1_[ 1 ] =
      new Xsection::xXsec( new polDIS::StrFunc( args, Xsection::Charge::d ),
			   leg1, leg2, precision  );
    
    g1_[ 2 ] = 
      new Xsection::xXsec( new polDIS::StrFunc( args, Xsection::Charge::n ),
			   leg1, leg2, precision  );
    
    for( int i = 0; i < g1_.size(); i++ )
      if( g1_[ i ] )  {
	g1_[ i ]->setParameter( offset, angle );
	g1_[ i ]->upper( length );
	
	dynamic_cast< Transform::GridIntegration* >
	  ( g1_[ i ]->integration() )->ngrid( ngrid );
	
      }
  }
  
  // set Q2 range
  q2_.push_back(   1.0 );
  //  q2_.push_back(   2.5 );
  //  q2_.push_back(   5.0 );
  q2_.push_back(  10.0 );
  //  q2_.push_back(  20.0 );
  q2_.push_back( 100.0 );
  
  // set size of PdfGarph container
  for( int i = 0; i < gg1_.size(); i++ ){
    gg1_[ i ].resize( q2_.size(), ( PdfGraph* ) NULL );
  }
  
  // xrange setting
  xrange_[ 0 ] = 3.0E-3;
  xrange_[ 1 ] = 1.0;
  
  // power weight for g1 graph
  weight_ = ( args.get( "weight" , 0.0 ) != 0.0 ? 1.0 : 0.0 );

  // x axis type
  vsX_ = ( args.get( "plot", "X" ) == string( "X" ) );
  
  // marker 
  this->style();
  this->createPads();
  this->drawExp();
  this->Draw();
  
}

void G1Panel::style() {

  latex_->SetTextSize( 0.024 );
  line_->SetLineStyle( 2 );
  
  // CERN experiments
  style_[ "EMC,1989,g1p" ].color  = 1; 
  style_[ "EMC,1989,g1p" ].marker = 24;
  
  style_[ "SMC,1998,g1p" ].color = 4; 
  style_[ "SMC,1998,g1p" ].marker = 20;

  style_[ "SMC,1998,g1d" ].color = 4; 
  style_[ "SMC,1998,g1d" ].marker = 20;
  
  style_[ "COMPASS,2005,g1d" ].color = 9; 
  style_[ "COMPASS,2005,g1d" ].marker = 24;

  style_[ "COMPASS,2007,g1d" ].color = 9; 
  style_[ "COMPASS,2007,g1d" ].marker = 20;

  // DESY experiments
  style_[ "HERMES,2006,g1p" ].color = 2; 
  style_[ "HERMES,2006,g1p" ].marker = 20;

  style_[ "HERMES,2006,g1d" ].color = 2; 
  style_[ "HERMES,2006,g1d" ].marker = 20;
  
  style_[ "HERMES,1997,g1n" ].color = 2; 
  style_[ "HERMES,1997,g1n" ].marker = 20;
  
  // SLAC
  style_[ "E143,1998,g1p" ].color  = 3; 
  style_[ "E143,1998,g1p" ].marker = 24;

  style_[ "E143,1998,g1d" ].color  = 3; 
  style_[ "E143,1998,g1d" ].marker = 24;
  
  style_[ "E155,2000,g1p" ].color  = 3; 
  style_[ "E155,2000,g1p" ].marker = 20;
  
  style_[ "E155,1999,g1d" ].color  = 3; 
  style_[ "E155,1999,g1d" ].marker = 20;
  
  style_[ "E142,1996,g1n" ].color  = 3; 
  style_[ "E142,1996,g1n" ].marker = 24;
  
  style_[ "E154,1997,g1n" ].color  = 3; 
  style_[ "E154,1997,g1n" ].marker = 20;
  
}

G1Panel::~G1Panel() {
  
  for( int i = 0; i < g1_.size(); i++ )
    if( g1_[ i ] ) { delete g1_[ i ]->moment(); delete g1_[ i ]; }
  
}


Xsection::Xsec* G1Panel::xsec() {
  if( g1_[ 0 ] == NULL )  return NULL;
  return g1_[ 0 ]->moment();
}


void G1Panel::setQ2( const double& q2 ){

  for( int i = 0; i < g1_.size(); i++ )
    if( g1_[ i ] )  g1_[ i ]->moment()->setQ2( q2 );  
  
}

void G1Panel::drawExp(){

  if( edata_ == NULL ) return;
  
  vector< Exp::DataSet >& data = edata_->data();
  int icol = 1;
  TLegend *legend = NULL;
  
  for( int i = 0; i < data.size(); i++ ){
    
    if( data[ i ].size() == 0 ) continue;
    
    Exp::DataSet& set = data[i];
    
    if( set[0].info() != "g1p" &&
	set[0].info() != "g1d" &&
	set[0].info() != "g1n" ) continue;
    
    PdfGraph *gExp = 
      new PdfGraph( set, weight_, 1.0, ( vsX_ ? "x" : "Q^2" ) );
    
    map< std::string, Style >::iterator itr;
    if( ( itr = style_.find( set.info() ) ) != style_.end() ){
      gExp->SetMarkerColor( itr->second.color );
      gExp->SetLineColor( itr->second.color );
      gExp->SetMarkerStyle( itr->second.marker );
    }
    gExp->SetMarkerSize( 0.8 );
    
    if( set[0].info() == "g1p" ) this->cd( 1 );
    if( set[0].info() == "g1d" ) this->cd( 2 );
    if( vsX_ && set[0].info() == "g1n" ) this->cd( 3 );
    gExp->Draw("P");
    
    this->Update();
  }
}

void G1Panel::draw() {
  if( vsX_ ) this->draw_x();
  else       this->draw_q2();
}

void G1Panel::draw_x() {
  
  for( int iq2 = 0; iq2 < q2_.size(); iq2++ ){
    this->setQ2( q2_[ iq2 ]  );
    
    for( int i = 0; i < g1_.size(); i++ ){
      this->cd( i + 1 );
      
      PdfGraph *g = gg1_[ i ][ iq2 ];
      
      if( g == NULL ) {
	
	g = 
	  ( gg1_[ i ][ iq2 ] 
	    = new PdfGraph( g1_[ i ], xrange_[ 0 ], xrange_[ 1 ], 40, 
			    weight_, 0.5 ) );
	
	g->SetLineColor( iq2 + 1 );
	
      } else {
	g->update();
      }
      
      g->Draw();
      this->Update();
    }
    
  }
  
  this->Print( "g1.png" );
  
}

void G1Panel::pads_x(){
  
  this->Clear();
  this->Divide( 1, 3, 5.0, 0.0 );

  for( int iY = 0; iY < 3; iY++ ){
    
    int iPad = iY + 1;
    
    TVirtualPad *pad = this->cd( iPad );
    pad->SetFillStyle( kFALSE );
    pad->SetFillColor( 18 );
    pad->SetFrameLineWidth( 2 );
    pad->SetLogx();
    
    // set margin
    pad->SetTopMargin( iY == 0 ? 0.1 : 0.0 );
    pad->SetBottomMargin( iY == 2 ? 0.1 : 0.0 );
    
    TH1 *frame = NULL;
    
    string ytitle = ( weight_ == 0.0 ? "g_{1}" : "xg_{1}");
    
    switch ( iY ) {
    case 0: 
      ytitle += "^{p}"; 
      
      if( weight_ == 0.0 ){
	frame = pad->DrawFrame( xrange_[0], -0.75, xrange_[1], 1.25 );
      } else {
	frame = pad->DrawFrame( xrange_[0], -0.04, xrange_[1], 0.11 );
      }

      break;
      
    case 1: 
      ytitle += "^{d}"; 

      if( weight_ == 0.0 ){
	frame = pad->DrawFrame( xrange_[0], -1.05, xrange_[1], 1.05 );
      } else {
	frame = pad->DrawFrame( xrange_[0], -0.04, xrange_[1], 0.07 );
      }

      break;

    case 2: 
      ytitle += "^{n}"; 

      if( weight_ == 0.0 ){
	frame = pad->DrawFrame( xrange_[0], -1.55, xrange_[1], 0.45 );
      } else {
	frame = pad->DrawFrame( xrange_[0], -0.04, xrange_[1], 0.03 );
      }

      break;
    }
    

    frame->SetLineWidth( 2 );
    
    TAxis *xAxis = frame->GetXaxis();
    TAxis *yAxis = frame->GetYaxis();
    
    xAxis->SetTitle( "x_{B}" );
    
    yAxis->SetTitle( ytitle.c_str() );
    
    xAxis->SetLabelSize( 0.06 );
    yAxis->SetLabelSize( 0.06 );
    //    xAxis->SetTitleSize( 0.10 );
    //    yAxis->SetTitleSize( 0.10 );
    
    line_->DrawLine( xrange_[0], 0.0, xrange_[1], 0.0 );
    
  }
  this->Update();
}

void G1Panel::pads_q2(){
  
  this->Clear();
  this->Divide( 1, 2, 0.001, 0.001 );
  
  // proton
  for( int i = 0; i < 2; i++ ){
    
    TVirtualPad *pad = this->cd( i + 1 );
    pad->SetFillStyle( kFALSE );
    pad->SetFillColor( 16 );
    pad->SetFrameLineWidth( 2 );
    pad->SetTopMargin( i == 1 ? 0.001 : 0.1 );
    pad->SetBottomMargin( i == 1 ? 0.1 : 0.001 );
    pad->SetLogx();
    TH1* frame = pad->DrawFrame( 0.5, 0.0, 900.00, 16.0 );
    
    frame->GetXaxis()->SetTitle( "Q^{2}   GeV^{2}" );
    
    string ylab = "g_{1}^{";
    ylab += ( i == 0 ? "p" : "d" );
    ylab += "}( x, Q^{2} )";
    
    frame->GetYaxis()->SetTitle( ylab.c_str() );
    
    line_->DrawLine( 1.0, 0.0, 1.0, 16.0 );

    
    for( int j = 0; j < xedge_.size() - 1; j++ ){
      double x  = 0.5 * ( xedge_[ j ] + xedge_[ j + 1 ] );
      double y  = xedge_.offset( x );
      double q2 = pow( 10.0, log10( xedge_.q2max( x ) ) + 0.1 );
      
      ostringstream ostr;
      ostr << "x = " << setw( 8 ) << x 
	   << "( + " << setw( 5 ) << xedge_.offset( x ) << " )";
      
      latex_->DrawLatex( q2, y, ostr.str().c_str() );
    }

    

  }

}



void G1Panel::createPads() {
  if( vsX_ ) this->pads_x(); 
  else       this->pads_q2();
}

/*
 */
void G1Panel::draw_q2() {
  
  int nq2 = 10;
  for( int ix = 0; ix < xedge_.size() - 1; ix++ ){
    
    double x = 0.5 * ( xedge_[ ix ] + xedge_[ ix + 1 ] );
    
    if( q2p_[ ix ] == NULL ) {
      q2p_[ ix ] = new TGraph( nq2 );
      q2p_[ ix ]->SetLineWidth( 2 );
      q2p_[ ix ]->SetLineColor( 2 );
    }
    
    if( q2d_[ ix ] == NULL ) {
      q2d_[ ix ] = new TGraph( nq2 );
      q2d_[ ix ]->SetLineWidth( 2 );
      q2d_[ ix ]->SetLineColor( 2 );
    }
    
    double dq2 = ( xedge_.q2max( x ) - xedge_.q2min( x ) ) / nq2;

    for( int iq2 = 0; iq2 < nq2; iq2++ ){
      
      double q2 = xedge_.q2min( x ) + dq2 * iq2;
      
      g1_[ 0 ]->moment()->setQ2( q2 );
      g1_[ 1 ]->moment()->setQ2( q2 );
      
      double p = 0.5 * (*g1_[ 0 ])( x ) + xedge_.offset( x ) ;
      double d = 0.5 * (*g1_[ 1 ])( x ) + xedge_.offset( x ) ;
      
      q2p_[ ix ]->SetPoint( iq2, q2, p );
      q2d_[ ix ]->SetPoint( iq2, q2, d );
    }

    this->cd( 1 ); 
    q2p_[ ix ]->Draw();
    
    this->cd( 2 ); 
    q2d_[ ix ]->Draw();

    this->Update();
    
  }
  
  this->Print( "g1_Q2.png" );
}
