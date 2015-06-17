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
#include <sstream>
#include <string>

#include <TROOT.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TLatex.h>

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
    latex_()
  {
    
    Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
    
    if( args.hasOpt( "help" ) ) MyApplication::Usage();
    
    try {
      
      if( args.hasOpt( "expdata" ) ) data_ = new Exp::ExpData( args );
      labels_ = args.get( "xaxis", ",", labels_ ); 
      
      canvas_ = new TCanvas( "c" );
      if( args.hasOpt( "logx" ) ) canvas_->SetLogx();
      if( args.hasOpt( "logy" ) ) canvas_->SetLogy();
      
      line_.SetLineWidth( 1 );
      latex_.SetTextSize( 0.04 );
      
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
    args.usage( "ymax",     "2", "y-axis upper/lower value" );
    args.usage( "format", "png", "Draing format (png,eps,ps,gif,jpg)" );
  }

  virtual void Run( Bool_t retrn = kFALSE ){
    
    Utility::Arguments& args = Utility::Arguments::ref();
    if( data_ == NULL ) return;
    vector< Exp::DataSet >& dset = data_->data();
    for( int i = 0; i < dset.size(); i++ ){
      this->draw( dset[i] );
      ostringstream ostr;
      ostr << "residual_scale-" << i << "." << args.get( "format", "png" );
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
  
  void draw( Exp::DataSet& set ){
    
    int fitted = 0;
    for( int i = 0; i < set.size(); i++ ) if( set[ i ].fitted() ) fitted++;
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    double ymax = args.get( "ymax", 3.0 );
    
    double ymin = - ymax;
    
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
    
    TGraph *upper = new TGraph( set.size() );
    TGraph *lower = new TGraph( set.size() );
    
    double xmin = args.get( "xmin", 0.5 ); 
    double xmax = args.get( "xmax", 0.5 );
    
    int ifit = 0;
    int inot = 0;
    for( int i = 0; i < set.size(); i++ ){
      
      Exp::DataPoint& p = set[ i ];

      double errsum = p.stat() * p.stat();
      for( int j = 0; j < p.syst().size(); j++ )
	errsum += p.syst()[ j ] * p.syst()[ j ];
      
      double x = p.kine( xaxis_ );
      
      if( xmin == xmax ){
	xmin = 0.9 * x;
	xmax = 1.1 * x;
      }
      
      if( ! args.hasOpt( "xmin" ) ) xmin = ( x < xmin ? x : xmin );
      if( ! args.hasOpt( "xmax" ) ) xmax = ( x > xmax ? x : xmax );

      // scale1 (upper) = 0.5, scale2 (lower) = 2.0
      
      double upp = ( p.scale1() - p.model() ) / sqrt( errsum );
      double dpp = ( p.scale2() - p.model() ) / sqrt( errsum );

      upper->SetPoint( i, x, abs( upp ) > ymax ? 
		       upp / abs( upp ) * ymax : upp );
      lower->SetPoint( i, x, abs( dpp ) > ymax ? 
		       dpp / abs( dpp ) * ymax : dpp );

    }
    
    double xmargin = 0.1 * ( xmax - xmin );
    double xlow;
    double xhigh;
    
    if( ! Utility::Arguments::ref().hasOpt( "logx" ) ){
      xlow  = xmin - ( args.hasOpt( "xmin" ) ? 0.0 : xmargin );
      xhigh = xmax + ( args.hasOpt( "xmax" ) ? 0.0 : xmargin );
    } else { 
      xlow  = 0.04;
      xhigh = xmax;
    }
    
    TH1 *frame = canvas_->DrawFrame( xlow, ymin, xhigh, ymax );
    
    string ytitle = set[0].info();
    //    if( set[0].unit() != "" ) ytitle += " (" + set[0].unit() + ")";
    //    frame->GetYaxis()->SetTitle( ytitle.c_str() );
    frame->GetYaxis()->SetTitle( "( Scale - Fit ) / d(Data)" );
    
    //    upper->SetFillColor( 3 );    
    //    lower->SetFillColor( 4 );
    upper->SetFillColor( 3 );    
    lower->SetFillColor( 4 );

    // theory band
    upper->Draw( "B" );
    lower->Draw( "B" );

    // grid
    line_.SetLineStyle( 1 );
    line_.DrawLine( xlow, 0.0, xhigh, 0.0 );
    
    line_.SetLineStyle( 2 );
    line_.DrawLine( xlow, 1.0, xhigh, 1.0 );
    
    line_.SetLineStyle( 2 );
    line_.DrawLine( xlow, -1.0, xhigh, -1.0 );
    
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
    
    frame->SetTitle( set.info().c_str() );
    frame->GetXaxis()->SetTitle( xaxis_.c_str() );
    
    canvas_->Update();
  }
  
};

int main( int argc, char* argv[] ){

  gROOT->SetStyle("Plain");

  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

