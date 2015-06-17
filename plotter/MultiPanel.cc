// $Id: MultiPanel.cc,v 1.1 2010/05/07 14:45:32 miyachi Exp $
#include "MultiPanel.hh"
/* 
   Residual plot for HERMES multiplicity: based on multiPanel.cc

   Display residual in 4by4 layout?


 */
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

// -------------------------------------------------------------------- //
#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLine.h>
#include <TPostScript.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TBox.h>

// -------------------------------------------------------------------- //
#include <Utility/Arguments.hh>

// -------------------------------------------------------------------- //
using namespace std;

// -------------------------------------------------------------------- //

MultiPanel::MultiPanel( int argc, char* argv[] ) throw( int ) : 
  TApplication( "myApp", &argc, argv ),
  latex_(), line_(),
  suffix_( "png" ), ps_( NULL ), ofname_( "mp" ),
  c_( NULL ), 
  panelSizeX_( 400 ), panelSizeY_( 200 ),
  marginBottom_( 0.12 ), marginLeft_( 0.1 ),
  column_( 1 ), row_( 1 ), 
  xmin_( 0.0 ), xmax_( 10.6 ), ymin_( -3.2 ), ymax_( 3.2 ),
  fLogx_( false ), fLogy_( false ),
  labelX_( 0.03 ), labelY_( 0.97 ), labelSize_( 0.08 ), labelAlign_( 13 ),
  xlabel_offset_( 0.4 ), ylabel_offset_( 0.6 ),
  xlabel_size_( 0.07 ), ylabel_size_( 0.07 ),
  xtitle_( "" ), ytitle_( "" ),
  xtitle_offset_( 0.8 ), ytitle_offset_( 0.6 ),
  xtitle_size_( 0.07 ), ytitle_size_( 0.07 )
{
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( args.hasOpt( "help" ) ||
      args.hasOpt( "Help" ) ||
      args.hasOpt( "HELP" ) ||
      args.hasOpt( "?" ) ) {
    this->Usage();
    throw( 0 );
  }
  
  // logx
  fLogx_ = args.hasOpt( "logx" );
  fLogy_ = args.hasOpt( "logy" );
  
  // size
  xmin_ = args.get( "xmin", fLogx_ && ( xmin_ <= 0.0 ) ? 0.03 : xmin_ );
  xmax_ = args.get( "xmax", xmax_ );
  
  ymin_ = args.get( "ymin", fLogy_ && ( ymin_ <= 0.0 ) ? 0.01 : ymin_ );
  ymax_ = args.get( "ymax", ymax_ );
  
  // resize paper size, default is x = 20 cm 
  suffix_ = args.get( "suffix", suffix_ );
  if( suffix_ == "eps" || suffix_ == "ps" ){
    ps_ = 
      new TPostScript( this->ofilename().c_str(), ( suffix_ == "ps" ? 111 : 113 ) );
  }
  
}

MultiPanel::~MultiPanel() {
  if( c_ ) {
    c_->Update();
    if( suffix_ == "eps" || suffix_ == "ps" ){
      if( ps_ ) ps_->Close();
    } else {
      c_->Print( this->ofilename().c_str() );
    }
  }
}

void MultiPanel::Run( Bool_t retrn ){
  Utility::Arguments& args = Utility::Arguments::ref();
  // create canvas
  c_ = this->createCanvas();
  c_->Update();
}

TCanvas* MultiPanel::createCanvas(){
  
  this->setRow();
  this->setColumn();
  
  if( ps_ ) {
    ps_->Range( 20.0 * ( column_ + 1 ), 10.0 * ( row_ + 1 ) );
  }
  
  if( row_ == 0 || column_ == 0 ) return NULL;
  
  TCanvas *c = new TCanvas( "mp", "mp",
			    column_ * panelSizeX_,
			    row_    * panelSizeY_ );
  
  if( c == NULL ) return c;
  
  //  The 3rd and 4th arguments for Divide define margin between
  //  sub-Pads. Set margin to 0.
  c->Divide( column_, row_, 0, 0 );
  
  for( int ir = 0; ir < row_; ir++ ){
    for( int ic = 0; ic < column_; ic++ ){
      
      int ip = ir * column_ + ic + 1;
      TVirtualPad *pad = c->cd( ip );
      
      // margin for axis label should be adjusted for each case
      if( ic == 0        ) pad->SetLeftMargin( marginLeft_ );
      if( ir == row_ - 1 ) pad->SetBottomMargin( marginBottom_ );
      
      if( fLogx_ ) pad->SetLogx();
      TH1 *frame = pad->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
      
      //  Divide method only set margin size. Axis labels will be 
      //  drawn over the left side plot. The offset for labels must 
      //  be properly set.
      frame->GetYaxis()->SetLabelOffset( ic == 0        ? 0.005 : 20.0 );
      frame->GetXaxis()->SetLabelOffset( ir == row_ - 1 ? 0.005 : 20.0 );

      frame->GetXaxis()->SetLabelSize( xlabel_size_ );
      frame->GetYaxis()->SetLabelSize( ylabel_size_ );
      
      frame->GetXaxis()->SetTitle( xtitle_.c_str() );
      frame->GetXaxis()->SetTitleOffset( ir == row_ - 1 ? 
					 xtitle_offset_ : 200.0 );
      frame->GetXaxis()->SetTitleSize( xtitle_size_ );
      
      frame->GetYaxis()->SetTitle( ytitle_.c_str() );
      frame->GetYaxis()->SetTitleOffset( ic == 0        ? 
					 ytitle_offset_ : 200.0 );
      frame->GetYaxis()->SetTitleSize( ytitle_size_ );
      
      //  center line
      line_.SetLineWidth( 1 );
      line_.SetLineStyle( 2 );
      line_.DrawLine( xmin_, 0.0, xmax_, 0.0 );

      this->drawPad( ic, ir, frame );
      
    }
  }
  return c;
}


void MultiPanel::setColumn() {
  Utility::Arguments& args = Utility::Arguments::ref();
  column_ = args.get( "column", column_ );
}

void MultiPanel::setRow() {
  Utility::Arguments& args = Utility::Arguments::ref();
  row_ = args.get( "row", row_ );
}

string MultiPanel::ofilename() {
  
  ostringstream ostr;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  ofname_ = args.get( "output", ofname_ );
  ostr << ofname_;
  ostr << "." << suffix_;
  
  return ostr.str();
}


void MultiPanel::style(){
  
  Utility::Arguments& args = Utility::Arguments::ref();

  this->privateStyle();
  
  // label style
  labelSize_  = args.get( "label_size", labelSize_ );
  labelX_     = args.get( "label_x_pos", labelX_ );
  labelY_     = args.get( "label_y_pos", labelY_ );
  labelAlign_ = args.get( "label_align", labelAlign_ );
  latex_.SetTextSize( labelSize_ );
  
  // panel size
  panelSizeX_ = args.get( "panel_width", panelSizeX_ );
  panelSizeY_ = args.get( "panel_heigth", panelSizeY_ );
  
  // axis title and label
  xtitle_ = args.get( "xaxis_title", xtitle_ );
  ytitle_ = args.get( "yaxis_title", ytitle_ );
  
  xtitle_offset_ = args.get( "xaxis_title_offset", xtitle_offset_ );
  ytitle_offset_ = args.get( "yaxis_title_offset", ytitle_offset_ );
  
  xtitle_size_ = args.get( "xaxis_title_size", xtitle_size_ );
  ytitle_size_ = args.get( "yaxis_title_size", ytitle_size_ );
  
  xlabel_offset_ = args.get( "xaxis_label_offset", xlabel_offset_ );
  ylabel_offset_ = args.get( "yaxis_label_offset", ylabel_offset_ );
  
  xlabel_size_ = args.get( "xaxis_label_size", xlabel_size_ );
  ylabel_size_ = args.get( "yaxis_label_size", ylabel_size_ );
  
}


void MultiPanel::Usage() {
  Utility::Arguments& args = Utility::Arguments::ref();
  
  args.usage( "--------", "", "------------------------------" );
  args.usage( "--------", "", "-------  REQUIRED  -----------" );
  args.usage( "--------", "", "------------------------------" );


  args.usage( "--------", "", "------------------------------" );
  args.usage( "--------", "", "--------  OPTIONAL  ----------" );
  args.usage( "--------", "", "------------------------------" );

  args.usage( "output", ofname_, "prefix for output file name" );
  args.usage( "suffix", suffix_, "suffix for output file name" );
  args.usage( "column", column_, "number of column (fixed)" );
  args.usage( "row", row_, "number of column (fixed)" );
  args.usage( "gstyle", "Plain", "ROOT document style" );
  args.usage( "logx", "", "Draw x-axis in Log" );
  args.usage( "logy", "", "Draw x-axis in Log" );
  args.usage( "xmin", xmin_, "lower edge of x-axis" );
  args.usage( "xmax", xmax_, "upper edge of x-axis" );
  args.usage( "ymin", ymin_, "lower edge of y-axis" );
  args.usage( "ymax", ymax_, "upper edge of y-axis" );
  args.usage( "--------", "", "------------------------------" );
  args.usage( "--------", "", "----------  STYLE  -----------" );
  args.usage( "--------", "", "------------------------------" );
  args.usage( "panel_width", panelSizeX_, "panel width" );
  args.usage( "panel_height", panelSizeY_, "panel height" );
  
  args.usage( "xaxis_title",  xtitle_,     "x-axis title" );
  args.usage( "xaxis_title_size",   xtitle_size_, "x-axis title size" );
  args.usage( "xaxis_title_offset", xtitle_offset_,  "x-axis title offset" );

  args.usage( "yaxis_title", ytitle_,     "y-axis title" );
  args.usage( "yaxis_title_size",   ytitle_size_, "y-axis title size" );
  args.usage( "yaxis_title_offset", ylabel_offset_,  "y-axis title offset" );
  
  args.usage( "xaxis_label_size", xlabel_size_, "x-axis label size" );
  args.usage( "xaxis_label_offset", xlabel_offset_, "x-axis label offset" );
  
  args.usage( "yaxis_label_size", ylabel_size_, "y-axis label size" );
  args.usage( "yaxis_label_offset", xlabel_offset_, "y-axis label offset" );
  
  args.usage( "margin_bottom", marginBottom_, "bottom margin size" );
  args.usage( "margin_left", marginLeft_, "left margin size" );
  
  args.usage( "--------", "", "------------------------------" );

}

double MultiPanel::posX_( const double& x ){
  return fLogx_ ?
    pow( 10.0, 
	 x * ( log10( xmax_ ) - log10( xmin_ ) ) + log10( xmin_ ) ) :
    x * ( xmax_ - xmin_ ) + xmin_;
}

double MultiPanel::posY_( const double& y ){ 
  return y * ( ymax_ - ymin_ ) + ymin_; 
}

double MultiPanel::labelPosX(){ return this->posX_( labelX_ ); }
double MultiPanel::labelPosY(){ return this->posY_( labelY_ ); }
  
void MultiPanel::drawPad( const int& co, const int& ro, TH1* frame ){
}
