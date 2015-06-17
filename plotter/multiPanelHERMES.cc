// $Id: multiPanelHERMES.cc,v 1.7 2010/05/17 01:57:10 miyachi Exp $
/* 
   Residual plot for HERMES multiplicity: based on multiPanel.cc

   Display residual in 4by4 layout?


 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
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
#include <ExpData/ExpData.hh>

// -------------------------------------------------------------------- //
using namespace std;

// -------------------------------------------------------------------- //
class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) throw( int ) : 
    TApplication( "myApp", &argc, argv ),
    c_( NULL ), panelSizeX_( 200 ), panelSizeY_( 100 ),
    xmin_( 0.9 ), xmax_( 18.0 ), ymin_( -3.2 ), ymax_( 3.2 ),
    latex_(),
    edata_( NULL ),
    column_( 4 ), row_( 4 ), suffix_( "png" ), ps_( NULL ), 
    ofname_( "mp_HERMES" ),
    dataName_( "Q^2 ave" ), fNorm_( false ), fLogx_( true ),
    binLowEdge_( "Q^2 min" ), binUpEdge_( "Q^2 max" ),
    fShowUnFitData_( false ),
    tolerance_( 2.89 ),
    cid_fitted_( kBlue ), cid_fitted_error_( kRed ), 
    cid_fitted_toler_( kSpring ),
    cid_unused_( kGray + 3 ), cid_unused_error_( kGray + 1 ), 
    cid_unused_toler_( kGray ),
    mid_fitted_( 20 ), mid_unused_( 24 ), marker_size_( 0.6 ),
    fid_fitted_error_( 3002 ), fid_fitted_toler_( 3002 ),
    fid_unused_error_( 3003 ), fid_unused_toler_( 3003 ),
    labelX_( 0.03 ), labelY_( 0.97 ), labelSize_( 0.08 ), labelAlign_( 13 ),
    xtitle_( "" ), ytitle_( "" ),
    xtitle_offset_( 0.9 ), ytitle_offset_( 0.6 ),
    xtitle_size_( 0.07 ), ytitle_size_( 0.07 ),
    xlabel_offset_( 0.1 ), ylabel_offset_( 0.6 ),
    xlabel_size_( 0.07 ), ylabel_size_( 0.07 ),
    zbin_( 0 ),
    zbinLabelAlign_( 33 ), zbinLabelX_( 0.98 ), zbinLabelY_( 0.97 ),
    marginBottom_( 0.14 ), marginLeft_( 0.1 ),
    selColumn_( -1 )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    if( args.hasOpt( "help" ) ||
	args.hasOpt( "Help" ) ||
	args.hasOpt( "HELP" ) ||
	args.hasOpt( "?" ) ) {
      this->Usage();
      throw( 0 );
    }
    
    // load data
    string path = args.get( "expdata", "" );
    if( path == "" ){
      args.err() << "please specify ExpData file with --expdata option"
		   << endl;
      this->Usage();
      throw( 1 );
    }
    
    try {
      edata_ = new Exp::ExpData( path );
    }
    catch( int e ){
      args.err() << "catch error (" << e << ")" << endl;
      throw( e );
    }
    
    // column select
    selColumn_ = args.get( "select_column", selColumn_ );
    if( selColumn_ > -1 && selColumn_ < 4 ){
      column_ = 1;
      panelSizeX_    = 400;
      panelSizeY_    = 200;
  } else {
      selColumn_ = -1;
    }
    
    // label for xaxis
    dataName_ = args.get( "dataName", dataName_ );
    if( dataName_ == "" ){
      args.err() << "please specify data name for x axis with --dataName" 
		 << endl;
      this->Usage();
      throw( 2 );
    }
    
    // normalization
    fNorm_ = ( args.get( "normalization", "error" ) == "error" );
    if( ! fNorm_ ){
      ymax_ = 0.045;
      ymin_ = -0.045;
      marginLeft_    = 0.14;
      ytitle_offset_ = 0.9;
    }
    
    // logx (force to true )
    //    fLogx_ = args.hasOpt( "logx" );
    
    // size
    xmin_ = args.get( "xmin", xmin_ );
    if( ! ( xmin_ > 0.0 ) ) xmin_ = 1.0;
    xmax_ = args.get( "xmax", xmax_ );
    ymin_ = args.get( "ymin", ymin_ );
    ymax_ = args.get( "ymax", ymax_ );
    
    // bin edge
    binLowEdge_ = args.get( "binLowEdge", binLowEdge_  );
    binUpEdge_  = args.get( "binUpEdge",  binUpEdge_   );

    // un-fitted data status
    fShowUnFitData_ = args.hasOpt( "showUnFitData" );      

    // tolerance
    tolerance_ = args.get( "tolerance", tolerance_ );

    // style
    this->style();

    // resize paper size, default is x = 20 cm 
    suffix_ = args.get( "suffix", suffix_ );
    if( suffix_ == "eps" || suffix_ == "ps" ){
      args.log() << "create: " << this->ofilename() << endl;
      ps_ = new TPostScript( this->ofilename().c_str(), 
			     ( suffix_ == "ps" ? 111 : 113 ) );
    }
    
    // label setting
    //    xtitle_ = dataName_;
    xtitle_ = "Q^{2}  (GeV^{2})";
    ytitle_ = ( fNorm_ ? 
		"(Data - Fit) / #sigma(Data)" :
		"Data - Fit" ); 
    
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

    
    // zbin configuration
    for( int i = 0; i < edata_->data().size(); i++ ){
      Exp::DataSet& dset = edata_->data()[ i ];
      for( int j = 0; j < dset.size(); j++ ){
	Exp::DataPoint& p = dset[ j ];
	if( p.hasKine( "z min" ) == true ) zbin_.push_back( p.kine( "z min" ) );
	if( p.hasKine( "z max" ) == true ) zbin_.push_back( p.kine( "z max" ) );
      }
    }
    if( zbin_.size() == 0 ){
      args.err() << "can not find kinematic variable \"z min\" or \"z max\" " 
		 << endl;
      throw(4);
    }
    
    // sort zbin_ data 
    sort( zbin_.begin(), zbin_.end() ); 
    vector< double >::iterator itr_end = unique( zbin_.begin(), zbin_.end() );
    vector< double > ztmp;
    for( vector< double >::iterator itr = zbin_.begin(); itr < itr_end; itr++ ){
      ztmp.push_back( *itr );
    }
    zbin_ = ztmp;
    args.log() << "zbin configuration:" << endl;
    for( int i = 0; i < zbin_.size(); i++ ) {
	args.log() << setw(4) << i << setw(10) << zbin_[ i ] << endl;
    }
    
    // create canvas
    c_ = this->createCanvas();
    c_->Update();
    
  }
  
  virtual ~MyApplication() {
    if( edata_ ) delete edata_;
    if( ps_ ){
      ps_->Close();
    }
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    if( c_ ) {
      c_->Update();
      if( suffix_ == "eps" || suffix_ == "ps" ){
      } else {
	c_->Print( this->ofilename().c_str() );
      }
    }
  }
  
private:
  
  // -----------------------------------------------
  TCanvas *c_;
  int panelSizeX_;
  int panelSizeY_;
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;

  TLatex latex_;
  TLine  line_;

  TCanvas* createCanvas(){

    this->setRow();

    if( row_ == 0 || column_ == 0 ) return NULL;
    
    if( ps_ ) {
      ps_->Range( 20.0 * column_ , 10.0 * row_  );
    }
    
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
	frame->GetYaxis()->SetLabelSize( ylabel_size_ );
	frame->GetXaxis()->SetLabelSize( xlabel_size_ );

	frame->GetXaxis()->SetTitle( xtitle_.c_str() );
	frame->GetXaxis()->SetTitleOffset( ir == row_ - 1 ? 
					   xtitle_offset_ : 200.0 );
	frame->GetXaxis()->SetTitleSize( xtitle_size_ );
	
	frame->GetYaxis()->SetTitle( ytitle_.c_str() );
	frame->GetYaxis()->SetTitleOffset( ic == 0        ? 
					   ytitle_offset_ : 200.0 );
	frame->GetYaxis()->SetTitleSize( ytitle_size_ );
	
	
	// display zbin range for the last column
	if( ic == column_ - 1 ){
	  ostringstream ostr;
	  ostr << zbin_[ ir ] << " < z < " << zbin_[ ir + 1 ] << endl;
	  latex_.SetTextAlign( zbinLabelAlign_ );
	  latex_.DrawLatex( this->zbinLabelPosX(), this->zbinLabelPosY(),
			    ostr.str().c_str() );
	}
	
	if( ic > edata_->data().size() ) continue;
	Exp::DataSet &dset = edata_->data()[ selColumn_ < 0 ? ic : selColumn_ ];
	
	this->draw( dset, ir );
	
	//  center line
	line_.SetLineWidth( 1 );
	line_.SetLineStyle( 2 );
	line_.DrawLine( xmin_, 0.0, xmax_, 0.0 );
	
	//  1 sigma line for nomalization plots
	if( fNorm_ ){
	  line_.SetLineStyle( 3 );
	  line_.DrawLine( xmin_,  1.0, xmax_,  1.0 );
	  line_.DrawLine( xmin_, -1.0, xmax_, -1.0 );
	}

	if( ir == 0 ){
	  latex_.SetTextAlign( labelAlign_ );
	  latex_.DrawLatex( this->labelPosX(), this->labelPosY(),
			    dset.abbreviation().c_str() );
	}
	
      }
    }
    
    return c;
  }
  
  // ----------------------------------------------- 
  Exp::ExpData *edata_;
  
  // -----------------------------------------------
  int column_;
  int row_;
  void setRow() {
    if( edata_ == NULL ) return;
    Utility::Arguments& args = Utility::Arguments::ref();

    /*
    // set number of column
    column_ = args.get( "column", 1 );
    if( column_ < 1 ) column_ = 1;
    row_    = edata_->data().size() / column_;
    if( column_ > 1 ) row_ += 1;
    */
    row_ = zbin_.size() - 1;
    args.log()
      << "Number of column (--column):    " << column_ << "\n"
      << "Number of row   (from data):    " << row_ 
      << endl;
    
  }
  
  string suffix_;
  TPostScript *ps_;
  string ofname_;

  string ofilename() {
    
    ostringstream ostr;
    
    Utility::Arguments& args = Utility::Arguments::ref();
    ofname_ = args.get( "output", ofname_ );
    ostr << ofname_;
    
    if( fNorm_ )            ostr << "_norm";
    if( tolerance_ != 0.0 ) ostr << "_tole" << tolerance_;
    
    ostr << "." << suffix_;

    return ostr.str();
  }
  
  void fitted_( Exp::DataSet& dset, const int& iz ){

    TGraphErrors *fit  = new TGraphErrors();
    TGraphErrors *nfit = new TGraphErrors();
    double scale = dset.normalization();
    
    for( int ip = 0; ip < dset.size(); ip++ ){

      Exp::DataPoint& p = dset[ ip ];
      
      double z = p.kine( "z ave" );
      int icheck = 0;
      for( icheck = 0; icheck < zbin_.size() - 1; icheck++ ){
	if( zbin_[ icheck ] < z && zbin_[ icheck + 1 ] > z ) break;
      }
      if( icheck != iz ) continue;
      
      double e = p.error();
      double x = p.kine( dataName_ );
      double y = p.data() - scale * p.model();

      if( fNorm_ && e > 0.0 ) y /= e;
      
      TGraphErrors *g = ( p.fitted() ? fit : nfit );
      int n = g->GetN();
      g->Set( n + 1 );
      g->SetPoint( n, x, y );
      g->SetPointError( n, 0.0, ( fNorm_ ? 1.0 : e ) );
      
    }
    
    // line style
    fit->SetLineColor( cid_fitted_ );
    fit->SetLineWidth( 2 );
    fit->SetLineStyle( 1 );

    nfit->SetLineColor( cid_unused_ );
    nfit->SetLineWidth( 2 );
    nfit->SetLineStyle( 1 );
    
    // marker style
    fit->SetMarkerColor( cid_fitted_ );
    fit->SetMarkerStyle( mid_fitted_ );
    fit->SetMarkerSize( marker_size_ );

    nfit->SetMarkerColor( cid_unused_ );
    nfit->SetMarkerStyle( mid_unused_ );
    nfit->SetMarkerSize( marker_size_ );
    
    if( fShowUnFitData_ && nfit->GetN() > 0 ) nfit->Draw("P");
    if( fit->GetN() > 0 )  fit->Draw("P");

  }
  
  TBox* theoryError( Exp::DataPoint& p, const double& torr = 1.0,
		     const int& color = kGray, const int& fill = 1001 ){
    
    double xlower = p.kine( binLowEdge_ );
    double xupper = p.kine( binUpEdge_  );
    double scale  = p.dataSet()->normalization();
    double band   = scale * p.modelError() * torr;
    double e      = p.error();
    if( fNorm_ && e != 0.0 ) band /= e;

    if( xlower > xmax_ || xupper < xmin_ ) return NULL;
    
    if( xlower < xmin_ ) xlower = xmin_ * 1.012;
    if( xupper > xmax_ ) xupper = xmax_ * 0.988;
    if( band > ymax_ )   band   = ymax_ * 0.988;
    
    TBox *b = new TBox( xlower, - band, xupper, band );
    
    b->SetLineColor( color );
    b->SetFillColor( color );
    b->SetFillStyle( fill );

    return b;
  }
  
  void error_( Exp::DataSet& dset, const int& iz ){
    
    if( dset.size() == 0 ) return;
    
    if( binLowEdge_ == "" || binUpEdge_ == "" ) return;
    
    if( ! dset[ 0 ].hasKine( binLowEdge_ ) ||
	! dset[ 0 ].hasKine( binUpEdge_ ) )  return;
    
    for( int ip = 0; ip < dset.size(); ip++ ){
      Exp::DataPoint& p = dset[ ip ];
      if( fShowUnFitData_ == false && p.fitted() == false ) continue; 
      
      double z = p.kine( "z ave" );
      int icheck = 0;
      for( icheck = 0; icheck < zbin_.size() - 1; icheck++ ){
	if( zbin_[ icheck ] < z && zbin_[ icheck + 1 ] > z ) break;
      }
      if( icheck != iz ) continue;
      
      if( tolerance_ != 0.0 ){
	TBox *box = 
	  this->theoryError( p, tolerance_, 
			     p.fitted() ? cid_fitted_toler_ : cid_unused_toler_,
			     p.fitted() ? fid_fitted_toler_ : fid_unused_toler_ );
	if( box ) box->Draw("l");
      }

      TBox *box = 
	this->theoryError( p, 1.0, 
			   p.fitted() ? cid_fitted_error_ : cid_unused_error_,
			   p.fitted() ? fid_fitted_error_ : fid_unused_error_ );


      if( box ) box->Draw("l");
      
    }
  }
  
  void draw( Exp::DataSet& dset, const int& iz ){
    
    Utility::Arguments& args = Utility::Arguments::ref();
    if( dset.size() == 0 ) {
      args.log() << "no data found in " <<  dset.abbreviation() << endl;
      return;
    }
    
    if( ! dset[ 0 ].hasKine( dataName_ ) ){
      args.log() << "can not found kinematic variable " << dataName_ << endl;
      return;
    }
    
    this->error_(  dset, iz );
    this->fitted_( dset, iz );
    
  }
  
  string dataName_;
  bool   fNorm_;
  bool   fLogx_;
  string binLowEdge_;
  string binUpEdge_;
  
  bool   fShowUnFitData_;

  double tolerance_;

  
  // --------------------------------------------------------------
  //    style
  // --------------------------------------------------------------
  
  // color code
  int cid_fitted_;       // color code for fitted data point
  int cid_unused_;       // color code for unused data point
  int cid_fitted_error_;  // color code for theory error band
  int cid_fitted_toler_; // color code for tolerance
  int cid_unused_error_;  // color code for theory error ( unused point )
  int cid_unused_toler_; // color code for tolerance  ( unused point )
  
  // marker type
  int mid_fitted_;  // marker style for fitted data point
  int mid_unused_;  // marker style for unused data point
  double marker_size_; // marker size
  
  // fill style
  int fid_fitted_error_;  // error band fill style
  int fid_fitted_toler_;  // tolerance band fill style
  int fid_unused_error_;  // error band fill style
  int fid_unused_toler_;  // tolerance band fill style

  void style(){

    Utility::Arguments& args = Utility::Arguments::ref();
    
    cid_fitted_       = args.get( "color_fitted",           cid_fitted_  );
    cid_fitted_error_ = args.get( "color_fitted_error",     cid_fitted_error_ );
    cid_fitted_toler_ = args.get( "color_fitted_tolerance", cid_fitted_toler_ );
    
    cid_unused_       = args.get( "color_unused",           cid_unused_ );
    cid_unused_error_ = args.get( "color_unused_error",     cid_unused_error_ );
    cid_unused_toler_ = args.get( "color_unused_tolerance", cid_unused_toler_ );
    
    mid_fitted_ = args.get( "marker_fitted", mid_fitted_);
    mid_unused_ = args.get( "marker_unused", mid_unused_);
    marker_size_ = args.get( "marker_size",  marker_size_ );

    fid_fitted_error_ = args.get( "fill_fitted_error",      fid_fitted_error_ );
    fid_fitted_toler_ = args.get( "fill_fitted_tolerance",  fid_fitted_toler_ );

    fid_unused_error_ = args.get( "fill_unused_error",      fid_unused_error_ );
    fid_unused_toler_ = args.get( "fill_unused_tolerance",  fid_unused_toler_ );

    // label style
    labelSize_  = args.get( "label_size", labelSize_ );
    labelX_     = args.get( "label_x_pos", labelX_ );
    labelY_     = args.get( "label_y_pos", labelY_ );
    labelAlign_ = args.get( "label_align", labelAlign_ );
    latex_.SetTextSize( labelSize_ );
    
    // panel size
    panelSizeX_ = args.get( "panel_width", panelSizeX_ );
    panelSizeY_ = args.get( "panel_heigth", panelSizeY_ );

    // zbin label position
    zbinLabelAlign_ = args.get( "zbin_label_align", zbinLabelAlign_ );
    zbinLabelX_ = args.get( "zbin_label_x_pos", zbinLabelX_ );
    zbinLabelY_ = args.get( "zbin_label_y_pos", zbinLabelY_ );

    marginBottom_ = args.get( "margin_bottom", marginBottom_ );
    marginLeft_ = args.get( "margin_bottom", marginLeft_ );

  }

  
  void Usage() {
    Utility::Arguments& args = Utility::Arguments::ref();
    
    args.usage( "--------", "", "------------------------------" );
    args.usage( "--------", "", "-------  REQUIRED  -----------" );
    args.usage( "--------", "", "------------------------------" );
    args.usage( "expdata", "", "path for input experimen data XML file" );
    args.usage( "dataName", dataName_, "kinematic variable to be drawn (fixed)" );
    args.usage( "--------", "", "------------------------------" );
    args.usage( "--------", "", "--------  OPTIONAL  ----------" );
    args.usage( "--------", "", "------------------------------" );
    args.usage( "normalization", ( fNorm_ ? "error" : "1" ), "normalization methdo" );
    args.usage( "suffix", suffix_, "suffix for output file name" );
    args.usage( "column", column_, "number of column (fixed)" );
    args.usage( "gstyle", "Plain", "ROOT document style" );
    args.usage( "logx", "", "Draw x-axis in Log" );
    args.usage( "xmin", xmin_, "lower edge of x-axis" );
    args.usage( "xmax", xmax_, "upper edge of x-axis" );
    args.usage( "ymin", ymin_, "lower edge of y-axis" );
    args.usage( "ymax", ymax_, "upper edge of y-axis" );
    args.usage( "binLowEdge", binLowEdge_, "label for x-bin lower bound" );
    args.usage( "binUpEdge",  binUpEdge_,  "label for x-bin upper bound" );
    args.usage( "showUnFitData", "",  "show un-fitted data point" );
    args.usage( "tolerance", tolerance_,  "tolerance value" );
    args.usage( "--------", "", "------------------------------" );
    args.usage( "--------", "", "----------  STYLE  -----------" );
    args.usage( "--------", "", "------------------------------" );
    args.usage( "panel_width", panelSizeX_, "panel width" );
    args.usage( "panel_height", panelSizeY_, "panel height" );
    args.usage( "color_fitted", cid_fitted_, "color code for fitted data point" );
    args.usage( "color_fitted_error", cid_fitted_error_, "color code for error band for fit data" );
    args.usage( "color_fitted_tolerance", cid_fitted_toler_, "color code for toleranec band for fit data" );
    args.usage( "color_unused", cid_unused_, "color code for unused data point" );
    args.usage( "color_unused_error", cid_unused_error_, "color code for error band for unused data" );
    args.usage( "color_unused_tolerance", cid_unused_toler_, "color code for toleranec band for unused data" );
    args.usage( "marker_fitted", mid_fitted_, "marker style for unused data point" );
    args.usage( "marker_unused", mid_unused_, "marker style for unused data point" );
    args.usage( "marker_size",  marker_size_, "marker size for data points" );
    args.usage( "fill_fitted_error", fid_fitted_error_, "fill style for error band" );
    args.usage( "fill_fitted_tolerance", fid_fitted_toler_, "fill style for tolerance band" );
    args.usage( "fill_unused_error", fid_unused_error_, "fill style for error band" );
    args.usage( "fill_unused_tolerance", fid_unused_toler_, "fill style for tolerance band" );

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

    args.usage( "zbin_label_align", zbinLabelAlign_, "zbin label text align" );
    args.usage( "zbin_label_x_pos", zbinLabelX_, "zbin label x position" );
    args.usage( "zbin_label_y_pos", zbinLabelY_, "zbin label y position" );

    args.usage( "margin_bottom", marginBottom_, "bottom margin size" );
    args.usage( "margin_left", marginLeft_, "left margin size" );


    args.usage( "select_column", selColumn_, "only display selected column data (0-3)" );

    args.usage( "--------", "", "------------------------------" );
    
    
  }
  
  double labelX_;
  double labelY_;
  double labelSize_;
  double labelAlign_;

  double posX_( const double& x ){
    return fLogx_ ?
      pow( 10.0, 
	   x * ( log10( xmax_ ) - log10( xmin_ ) ) + log10( xmin_ ) ) :
      x * ( xmax_ - xmin_ ) + xmin_;
  }

  double posY_( const double& y ){ return y * ( ymax_ - ymin_ ) + ymin_; }
  
  double labelPosX(){ return this->posX_( labelX_ ); }
  double labelPosY(){ return this->posY_( labelY_ ); }

  string xtitle_;
  string ytitle_;

  double xtitle_offset_;
  double ytitle_offset_;

  double xtitle_size_;
  double ytitle_size_;

  double xlabel_offset_;
  double ylabel_offset_;

  double xlabel_size_;
  double ylabel_size_;

  // zbin definition
  vector< double > zbin_;
  int zbinLabelAlign_;

  double zbinLabelX_;
  double zbinLabelY_;
  
  double zbinLabelPosX(){ return this->posX_( zbinLabelX_ ); }
  double zbinLabelPosY(){ return this->posY_( zbinLabelY_ ); }

  double marginBottom_;
  double marginLeft_;

  int selColumn_;
  
};

// -------------------------------------------------------------------- //
int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );

  // set global style
  gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );

  // create application
  try {
    MyApplication app( argc, argv );
    app.Run();
  }
  catch( int e ){
  }

  return 0;
}
