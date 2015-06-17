//
// residual.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 15:28:43 2008 Yoshiyuki Miyachi
// Started on  Thu Oct  2 15:28:43 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>

#include <TROOT.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TLatex.h>
#include <TBox.h>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;

class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) :
    TApplication( "myappli", &argc, argv ),
    data_( NULL ), 
    labels_( 1, "zmid" ), xaxis_( "" ), 
    canvas_( NULL ), line_(),
    latex_(), normFlag_( true )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    if( args.hasOpt( "help" ) ) MyApplication::Usage();
    
    normFlag_ = ( args.get( "normalization", "error" ) == "error" );
    
    try {
      
      if( args.hasOpt( "expdata" ) ) data_ = new Exp::ExpData( args );
      labels_ = args.get( "xaxis", ",", labels_ ); 
      
      canvas_ = new TCanvas( "c" );
      if( args.hasOpt( "logx" ) ) canvas_->SetLogx();
      if( args.hasOpt( "logy" ) ) canvas_->SetLogy();
      
      line_.SetLineWidth( 1 );
      latex_.SetTextSize( 0.05 );
      
    } 
    
    catch( int error ){
      ARG_ERR << "catch error " << error << endl;
    }
    
  }
  
  virtual ~MyApplication(){
    if( data_ ) delete data_ ;
    
  }
  
  static void Usage() {
    Utility::Arguments& args = Utility::Arguments::ref();
    args.usage( "expdata",   "", "Experiment Data XML file" );
    args.usage( "xaxis", "zmid", "TAG for x-axis. (2nd candidate can be specified separated by ," );
    args.usage( "boundary",  "", "x boundary position (separated by ,)" );
    args.usage( "xmin",      "", "x-axis lower value" );
    args.usage( "xmax",      "", "x-axis upper value" );
    args.usage( "ymax",     "3", "y-axis upper/lower value" );
    args.usage( "format", "png", "Draing format (png,eps,ps,gif,jpg)" );
    args.usage( "xoff",     "0", "x-axis offset" );
    args.usage( "logx",      "", "set x-axis in log" );
    args.usage( "logy",      "", "set y-axis in log" );
    args.usage( "xtitle",    "", "Overwirte x-axis title" );
    args.usage( "gstyle", "Plain", "gROOT style (Plain|Pub|Default)" );
    args.usage( "xbin_low",  "", "name of variable for xbin lower edge" );
    args.usage( "xbin_high",  "", "name of variable for xbin higher edge" );
    args.usage( "tolerance", "0", "tolerance" );
    args.usage( "color_data", "4,1", "color id for data point" );
    args.usage( "color_fit_error", "2,3", "color id for error band" );
    args.usage( "color_ext_error", "18,17", "color id for error band" );
    args.usage( "normalization", "error", "error|1" );
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    
    Utility::Arguments& args = Utility::Arguments::ref();
    if( data_ == NULL ) return;
    vector< Exp::DataSet >& dset = data_->data();
    for( int i = 0; i < dset.size(); i++ ){
      this->draw( dset[i] );
      ostringstream ostr;
      ostr << "residual-" << setw(3) << setfill( '0' ) << i << "-"
	   << ( normFlag_ ? "ratio" : "direct" )
	   << "." << args.get( "format", "png" );
      canvas_->Print( ostr.str().c_str() );

    }
    
  }
  
private:
  
  Exp::ExpData *data_;
  vector< string > labels_;
  string xaxis_;
  TCanvas *canvas_;
  TLine line_;
  TLatex latex_;
  bool normFlag_;

  void draw( Exp::DataSet& set ){
    
    Utility::Arguments& args = Utility::Arguments::ref();

    list< double > xcache;
    
    string xbin_low  = args.get( "xbin_low", "" );
    string xbin_high = args.get( "xbin_high", "" );
    
    int    fitted    = set.fittedDataPoint();
    double tolerance = args.get( "tolerance", 0.0 );
    
    double ymax = args.get( "ymax", 3.0 );
    double ymin = - ymax;
    
    double xoff = args.get( "xoff", 0.0 );
    
    double scale = set.normalization();
    
    if( set.size() == 0 ) return;
    
    xaxis_ = "";
    for( int i = 0; i < labels_.size(); i++ ){
      if( set[ 0 ].hasKine( labels_[ i ] ) ) {
	xaxis_ = labels_[ i ];
	break;
      }
    }
    if( xaxis_ == "" ) return;
    
    TGraphErrors *edata = new TGraphErrors( fitted );
    TGraphErrors *odata = new TGraphErrors( set.size() - fitted );
    
    vector< TBox* > err_fit;
    vector< TBox* > err_not;
    
    vector< TBox* > err_fit_lm;
    vector< TBox* > err_not_lm;
    
    TGraph *upper = new TGraph( fitted );
    TGraph *lower = new TGraph( fitted );
    
    TGraph *upper_lm = new TGraph( fitted );
    TGraph *lower_lm = new TGraph( fitted );
    
    TGraph *upper_o = new TGraph( set.size() - fitted );
    TGraph *lower_o = new TGraph( set.size() - fitted );
    
    TGraph *upper_o_lm = new TGraph( set.size() - fitted );
    TGraph *lower_o_lm = new TGraph( set.size() - fitted );
    
    double xmin = args.get( "xmin", 0.5 ); 
    double xmax = args.get( "xmax", 0.5 );
    
    int ifit = 0;
    int inot = 0;
    for( int i = 0; i < set.size(); i++ ){
      
      Exp::DataPoint& p = set[ i ];
      
      double errsum = p.error();
      double x      = p.kine( xaxis_ );
      double y      = p.data() - scale * p.model();
      if( normFlag_ ) y /= sqrt( errsum );

      double xb_l = 0.0;
      double xb_h = 0.0;
      if( xbin_low != "" && xbin_high != "" ){
	xb_l = p.kine( xbin_low ) ;
	xb_h = p.kine( xbin_high ) ;
	// if bin size can not be determined, back to default value
	if( xb_l == xb_h ) xb_l = xb_h = 0.0;
      }
      
      if( xmin == xmax ){
	xmin = 0.9 * x;
	xmax = 1.1 * x;
      }
      
      if( ! args.hasOpt( "xmin" ) ) xmin = ( x < xmin ? x : xmin );
      if( ! args.hasOpt( "xmax" ) ) xmax = ( x > xmax ? x : xmax );
      
      if( xoff != 0.0 ){
	list< double >::iterator itr;
	for( itr = xcache.begin(); itr != xcache.end(); itr++ ){
	  if( (*itr) - 0.5 * xoff < x && x < (*itr) + 0.5 * xoff ){
	    x += xoff;
	  }
	}
	xcache.push_back( x );
	xcache.sort();
      }
      
      double band = scale * sqrt( p.modelError() );
      if( normFlag_ ) band /= sqrt( errsum );

      if( p.fitted() ){
	edata->SetPoint( ifit, x, y );
	edata->SetPointError( ifit, 0.0, 
			      ( normFlag_ ? 1.0 : sqrt( errsum ) ) );
	
	upper->SetPoint( ifit, x,   band );
	lower->SetPoint( ifit, x, - band );
	
	upper_lm->SetPoint( ifit, x,   band * tolerance );
	lower_lm->SetPoint( ifit, x, - band * tolerance );
	
	ifit++;
	if( xb_l != 0.0 && xb_h != 0.0 ) {
	  err_fit.push_back( new TBox( xb_l, -band, xb_h, band ) ); 
	  if( tolerance > 0.0 )
	    err_fit_lm.push_back( new TBox( xb_l, -band * tolerance,
					    xb_h,  band * tolerance ) ); 
	}
      } else {
	odata->SetPoint( inot, x, y );
	odata->SetPointError( inot, 0.0, 
			      ( normFlag_ ? 1.0 : sqrt( errsum ) ) );

	upper_o->SetPoint( inot, x,   band );
	lower_o->SetPoint( inot, x, - band );

	upper_o_lm->SetPoint( inot, x,   band * tolerance );
	lower_o_lm->SetPoint( inot, x, - band * tolerance );

	inot++;
	if( xb_l != 0.0 && xb_h != 0.0 ) {
	  err_not.push_back( new TBox( xb_l, -band, xb_h, band ) ); 
	  if( tolerance > 0.0 )
	    err_not_lm.push_back( new TBox( xb_l, -band * tolerance, 
					    xb_h, band * tolerance ) ); 
	}
      }
    }
    
    double xmargin = 0.1 * ( xmax - xmin );
    double xlow;
    double xhigh;
    
    if( ! Utility::Arguments::ref().hasOpt( "logx" ) ){
      xlow  = xmin - ( args.hasOpt( "xmin" ) ? 0.0 : xmargin );
      xhigh = xmax + ( args.hasOpt( "xmax" ) ? 0.0 : xmargin );
    } else { 
      double xlh = log10( xmax );
      double xll = log10( xmin );
      double xlm = 0.1 * ( xlh - xlm );
      xlow  = pow( 10.0, xll - xlm );
      xhigh = pow( 10.0, xlh + xlm );
    }
    
    TH1 *frame = canvas_->DrawFrame( xlow, ymin, xhigh, ymax );
    
    string ytitle = set[0].info();
    //    if( set[0].unit() != "" ) ytitle += " (" + set[0].unit() + ")";
    //    frame->GetYaxis()->SetTitle( ytitle.c_str() );
    frame->GetYaxis()->SetTitle( normFlag_ ?  
				 "( Data - Fit ) / #sigma_{Data}" : 
				 "( Data - Fit )" );
    
    vector< int > col_data( 2 );
    col_data[ 0 ] = 4;
    col_data[ 1 ] = 1;
    col_data = args.get( "color_data", ",", col_data );
    
    vector< int > col_fit_error( 2 );
    col_fit_error[ 0 ] = 2;
    col_fit_error[ 1 ] = 3;
    col_fit_error = args.get( "color_fit_error", ",", col_fit_error );
    
    vector< int > col_ext_error( 2 );
    col_ext_error[ 0 ] = 18;
    col_ext_error[ 1 ] = 17;
    col_ext_error = args.get( "color_ext_error", ",", col_ext_error );
    
    edata->SetMarkerStyle( 20 );
    edata->SetMarkerColor( col_data[ 0 ] );
    edata->SetMarkerSize( 1.2 );
    edata->SetLineColor( col_data[ 0 ] );
    
    odata->SetMarkerStyle( 20 );
    odata->SetMarkerColor( col_data[ 1 ] );
    odata->SetMarkerSize( 1.0 );
    odata->SetLineColor( col_data[ 1 ] );
    
    upper->SetFillColor( col_fit_error[ 0 ] );
    lower->SetFillColor( col_fit_error[ 0 ] );

    upper_lm->SetFillColor( col_fit_error[ 1 ] );
    lower_lm->SetFillColor( col_fit_error[ 1 ] );

    upper_o->SetFillColor( col_ext_error[ 0 ] );
    lower_o->SetFillColor( col_ext_error[ 0 ] );

    upper_o_lm->SetFillColor( col_ext_error[ 1 ] );
    lower_o_lm->SetFillColor( col_ext_error[ 1 ] );

    
    if( err_not.size() > 0 ){

      for( int i = 0; i < err_not_lm.size(); i++ ){

	TBox *box = err_not_lm[ i ];
	double x1 = box->GetX1();
	double x2 = box->GetX2();
	double y1 = box->GetY1();
	double y2 = box->GetY2();
	
	if( xlow < x1 && x2 < xhigh ) {
	  if( y1 < ymin )  box->SetY1( ymin );
	  if( y2 > ymax )  box->SetY2( ymax );
	  box->SetFillStyle( 1001 );
	  box->SetFillColor( col_ext_error[ 1 ] );
	  box->Draw("l");
	}
      }

      for( int i = 0; i < err_not.size(); i++ ){
	TBox *box = err_not[ i ];
	double x1 = box->GetX1();
	double x2 = box->GetX2();
	double y1 = box->GetY1();
	double y2 = box->GetY2();
	
	if( xlow < x1 && x2 < xhigh ) {
	  if( y1 < ymin )  box->SetY1( ymin );
	  if( y2 > ymax )  box->SetY2( ymax );
	  box->SetFillStyle( 1001 );
	  box->SetFillColor( col_ext_error[ 0 ] );
	  box->Draw("l");
	}
      }

      
    } else {
      // theory band
      if( tolerance != 0.0 ){
	upper_o_lm->Draw( "B" );
	lower_o_lm->Draw( "B" );
      }
      upper_o->Draw( "B" );
      lower_o->Draw( "B" );
    }
    
    if( err_fit.size() > 0 ){

      for( int i = 0; i < err_fit_lm.size(); i++ ){
	err_fit_lm[ i ]->SetFillStyle( 1001 );
	err_fit_lm[ i ]->SetFillColor( col_fit_error[ 1 ] );
	err_fit_lm[ i ]->Draw("l");
      }
      
      for( int i = 0; i < err_fit.size(); i++ ){
	err_fit[ i ]->SetFillStyle( 1001 );
	err_fit[ i ]->SetFillColor( col_fit_error[ 0 ] );
	err_fit[ i ]->Draw("l");
      }
      
    } else {
      if( tolerance != 0.0 ){
	upper_lm->Draw( "B" );
	lower_lm->Draw( "B" );
      }
      upper->Draw( "B" );
      lower->Draw( "B" );
    }
    
    
    // grid
    line_.SetLineStyle( 1 );
    line_.DrawLine( xlow, 0.0, xhigh, 0.0 );

    if( normFlag_ ){
      line_.SetLineStyle( 2 );
      line_.DrawLine( xlow, 1.0, xhigh, 1.0 );
      
      line_.SetLineStyle( 2 );
      line_.DrawLine( xlow, -1.0, xhigh, -1.0 );
    }

    if( args.hasOpt( "boundary" ) ){
      line_.SetLineStyle( 3 );
      line_.SetLineColor( 4 );
      vector< double > xb( 0 );
      xb = args.get( "boundary", ",", xb );
      for( int i = 0; i < xb.size(); i++ ){
	line_.DrawLine( xb[ i ], ymin, xb[ i ], ymax );
      }
      line_.SetLineColor( 1 );
    }
    
    // exp data info
    odata->Draw( "P" );
    edata->Draw( "P" );

    //    frame->SetTitle( set.info().c_str() );
    frame->SetTitle( "" );

    double xt = ( xhigh - xlow ) * 0.05 + xlow;
    double yt = ( ymax - ymin ) * 0.95  + ymin;

    latex_.SetTextAlign( 11 );
    //    latex_.DrawLatex( xt, yt, set.info().c_str() );
    latex_.DrawLatex( xt, yt, set.abbreviation().c_str() );


    string xtitle = args.get( "xtitle", "" );
    frame->GetXaxis()->SetTitle( ( xtitle != "" ? xtitle :  xaxis_).c_str() );
    
    canvas_->Update();
  }
  
};

int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  //  gROOT->SetStyle("Plain");
  string gstyle = args.get( "gstyle", "Plain" );
  gROOT->SetStyle( gstyle.c_str() );
  
  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

