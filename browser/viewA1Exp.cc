//
// viewA1Exp.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Wed Feb 18 09:44:17 2009 Yoshiyuki Miyachi
// Started on  Wed Feb 18 09:44:17 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <cmath>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <Fitting/A1Data.hh>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLine.h>

using namespace std;

class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) :
    TApplication( "MyApplication", &argc, argv ),
    c_( NULL )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    c_ = new TCanvas( "A1 expdata" );
    c_->SetLogx();

    expr_ = new Exp::ExpData( args );
    
    expr_->summary( cout );
    
    //  frame size
    xmin_ = args.get( "xmin", 1.0E-3 );
    xmax_ = args.get( "xmin", 1.0 );
    
    ymin_ = args.get( "ymin", -0.3 );
    ymax_ = args.get( "ymax",  1.0 );

    latex_ = new TLatex();
    latex_->SetTextSize( 0.04 );

    line_ = new TLine();
    
  }
  
  virtual ~MyApplication() {
  }
  
  virtual void draw( Exp::DataSet& ds ) {
    
    if( ds.size() == 0 ) return;
    
    if( ds[0].info().substr( 0, 2 ) != "A1" ) return ;
    
    bool dis    = Fit::A1Data::isDIS( ds[ 0 ].info() );
    int  target = Fit::A1Data::nType( dis, ds[ 0 ].info() );
    int  hadron = Fit::A1Data::hType( dis, ds[ 0 ].info() );

    string title = "A_{1, ";
    switch( target ) {
    case 0: title += "p}"; break;
    case 1: title += "n}"; break;
    case 2: title += "d}"; break;
    }
    
    if( ! dis ){
      switch( hadron ){
      case 0: title += "^{#pi^{+}}"; break;
      case 1: title += "^{K^{+}}"; break;
      case 3: title += "^{h^{+}}"; break;
      case 4: title += "^{#pi^{-}}"; break;
      case 5: title += "^{K^{-}}"; break;
      case 7: title += "^{h^{-}}"; break;
      }
    }
    
    ARG_DETAIL 
      << setw(10) << ds[0].info()
      << setw(2) << target 
      << setw(2) << hadron 
      << setw(10) << title
      << endl;
    
    TH1* frame = c_->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    frame->GetXaxis()->SetTitle( "x" );
    frame->GetYaxis()->SetTitle( title.c_str() );
    
    line_->SetLineWidth( 1 );
    line_->SetLineStyle( 2 );
    line_->DrawLine( xmin_, 0.0, xmax_, 0.0 );
    
    double lxmin = log10( xmin_ );
    double lxmax = log10( xmax_ );
    double dlx   = lxmax - lxmin;
    double xtxt  = pow( 10.0, lxmin + 0.1 * dlx ); 
    double dy    = ymax_ - ymin_;
    
    latex_->DrawLatex( xtxt, ymax_ - 0.1 * dy, ds.info().c_str() );
    latex_->DrawLatex( xtxt, ymax_ - 0.18 * dy, ds.ref().c_str() );


    TGraphErrors *gErr = new TGraphErrors( ds.size() );
    gErr->SetLineWidth( 2 );
    gErr->SetMarkerStyle( 20 );
    gErr->SetLineColor( 2 );
    gErr->SetMarkerColor( 2 );

    TGraph       *gFit = new TGraph( ds.size() );
    
    for( int i = 0; i < ds.size(); i++ ){
      
      Exp::DataPoint& p = ds[ i ];

      double x   = p.kine( "x" );
      double a1  = p.data();
      double da1 = p.stat();
      
      double fit = p.model();
      
      //      ARG_DETAIL << setw(2) << i << setw(6) << x << setw(6) << a1 << setw(6) << da1 << setw(12) << fit << endl;
      
      gErr->SetPoint( i, x, a1 );
      gErr->SetPointError( i, 0.0, da1 );
      gFit->SetPoint( i, x, fit );
      
      
    }

    
    gErr->Draw( "P" );
    //    gFit->Draw( "C" );
    gFit->Draw( "L" );
    
    c_->Print( string( ds.info() + "_" + ds[0].info() + ".png" ).c_str() );
    
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    
    vector< Exp::DataSet >& data = expr_->data();
    for( vector< Exp::DataSet >::iterator itr = data.begin();
	 itr != data.end();
	 itr++ ){

      ARG_LOG << "Draw: " << setw(35) << itr->info() << endl;
      this->draw( *itr );
    }

  }

private:
  
  TCanvas *c_;
  TLatex  *latex_;
  TLine   *line_;

  Exp::ExpData *expr_;

  double xmin_;
  double xmax_;

  double ymin_;
  double ymax_;



};

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( ! args.hasOpt( "expdata" ) ){
    ARG_ERR << args.command() << " requires --expdata option" << endl;
    return 1;
  }
  
  MyApplication app( argc, argv );
  app.Run();

  return 0;
}
