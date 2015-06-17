#include <iostream>
#include <sstream>
#include <string>

#include <TROOT.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
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
    xmin_( 0.0 ), xmax_( 1.0 ),
    ymin_( pow( 10.0, -4 ) ), ymax_( pow( 10.0, 3 ) ),
    canvas_( NULL ),
    vtgraph_( 0 ), vtgrapherr_( 0 ),
    vexpname_( 0 ),
    lege_( NULL ), legt_( NULL ),
    vreqds_( 0 )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
    
    if( args.hasOpt( "help" ) ) MyApplication::Usage();
    
    try {
      
      if( args.hasOpt( "expdata" ) ) data_ = new Exp::ExpData( args );
      
      if( !args.hasOpt( "sumplot" ) ){ canvas_ = new TCanvas( "c" ); }
      else { canvas_ = new TCanvas( "c", "mycanv", 10, 10, 800, 1000 ); }

      if( args.hasOpt( "logx" ) ) canvas_->SetLogx();
      if( args.hasOpt( "logy" ) ) canvas_->SetLogy();

      if( args.hasOpt( "xmin" ) ) xmin_ = args.get( "xmin", xmin_ );
      if( args.hasOpt( "xmax" ) ) xmax_ = args.get( "xmax", xmax_ );
      if( args.hasOpt( "ymin" ) ) ymin_ = args.get( "ymin", ymin_ );
      if( args.hasOpt( "ymax" ) ) ymax_ = args.get( "ymax", ymax_ );

      if( args.hasOpt( "sumplot" ) ){ 
	vreqds_ = args.get( "sumplot", ",", vreqds_ );
      }

    }
    
    catch( int error ){
      ARG_ERR << "catch error " << error << endl;
    }
    
  }
  
  virtual ~MyApplication(){
    for( int i = 0; i < vtgraph_.size(); i++ ) 
      delete vtgraph_[ i ];
    for( int i = 0; i < vtgrapherr_.size(); i++ ) 
      delete vtgrapherr_[ i ];
    if( data_ ) delete data_ ;
    if( legt_ ) delete legt_ ;
    if( lege_ ) delete lege_ ;
  }
  
  static void Usage() {
    Utility::Arguments& args = Utility::Arguments::ref();
    args.usage( "expdata",   "", "Experiment Data XML file" );
    args.usage( "xmin",      "", "x-axis lower value" );
    args.usage( "xmax",      "", "x-axis upper value" );
    args.usage( "ymin",      "", "y-axis lower value" );
    args.usage( "ymax",      "", "y-axis upper value" );
    args.usage( "format", "png", "Draing format (png,eps,ps,gif,jpg)" );
  }

  virtual void Run( Bool_t retrn = kFALSE ){

    Utility::Arguments& args = Utility::Arguments::ref();
    if( data_ == NULL ) return;

    vector< Exp::DataSet >& dset = data_->data();

    if( !args.hasOpt( "sumplot" ) ){

      for( int i = 0; i < dset.size(); i++ ){
	this->draw( dset[i] );
	ostringstream ostr;
	ostr << "xsec-" << i << "." << args.get( "format", "png" );
	canvas_->Print( ostr.str().c_str() );      
      }

    } else {

      /*
      for( int i = 0; i < dset.size(); i++ ){
	vector< int >::iterator itr = 
	  find( vreqds_.begin(), vreqds_.end(), i );
	if( itr == vreqds_.end() ) continue;
	this->add( dset[ *itr ] );
      }
      */
      for( int i = 0; i < vreqds_.size(); i++ ){
	this->add( dset[ vreqds_[ i ] ] );
      }
      this->sumdraw( dset[ 0 ] );
 
      ostringstream ostr;
      ostr << "xsec-sumplot-" << getname( dset[ 0 ] ) 
	   << "." << args.get( "format", "png" );
      canvas_->Print( ostr.str().c_str() );

    }

  }
  
private:
  
  Exp::ExpData *data_;
  TCanvas *canvas_;
  double xmin_;  double xmax_;
  double ymin_;  double ymax_;
  vector< TGraph* > vtgraph_;
  vector< TGraphErrors* > vtgrapherr_;
  vector< string > vexpname_;
  TLegend* lege_;
  TLegend* legt_;
  vector< int > vreqds_;

  string getname( Exp::DataSet& set ){
    string sinfo = set.info();
    if( sinfo.find( "PIpm" ) != -1 ) return "#pi^{#pm}";
    if( sinfo.find( "KApm" ) != -1 ) return "K^{#pm}";
    if( sinfo.find( "PRpm" ) != -1 ) return "p^{#pm}";
    if( sinfo.find( "Hpm" ) != -1 ) return "h^{#pm}";
    if( sinfo.find( "PI0" ) != -1 ) return "#pi^{0}";
    return "";
  }

  void draw( Exp::DataSet& set ){
    
//    int fitted = 0;
//    for( int i = 0; i < set.size(); i++ ) if( set[ i ].fitted() ) fitted++;
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    double scale = set.normalization();
    string sinfo = set.info();
    string xname = getname( set );
    
    if( set.size() == 0 ) return;
    
    TGraph* theory = new TGraph( set.size() );
    TGraphErrors* expdt = new TGraphErrors( set.size() );
    
    for( int i = 0; i < set.size(); i++ ){
      
      Exp::DataPoint& p = set[ i ];
      
      double errsum = p.stat() * p.stat();
      for( int j = 0; j < p.syst().size(); j++ )
	errsum += p.syst()[ j ] * p.syst()[ j ];
      
      double zmin = p.kine( "zmin" );
      double zmax = p.kine( "zmax" );
      double zmid = ( p.kine( "zmid" ) != 0.0 ? p.kine( "zmid" ) : 
		      0.5 * ( zmin + zmax ) );

      double data = p.data();
      
      double theo = p.model();
      double merr = p.modelError();
      
      theory->SetPoint( i, zmid, scale * theo );
      expdt->SetPoint( i, zmid, data );
      expdt->SetPointError( i, 0.5 * ( zmax - zmin ), sqrt( errsum ) );
      
    }
    
    TH1 *frame = canvas_->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    
    frame->SetTitle( sinfo.c_str() );
    string xsecname = "#frac{1}{#sigma_{tot}}#frac{d#sigma}{dz}";
    xsecname += "( e^{+} e^{-} -> " + xname + " + X )";
    frame->GetYaxis()->SetTitle( xsecname.c_str() );
    frame->GetXaxis()->SetTitle( "z" );
    
    theory->SetLineStyle( 1 );   
    theory->SetLineColor( 1 );
    
    expdt->SetMarkerSize( 1.5 );   
    expdt->SetMarkerStyle( 1 );   
    expdt->SetMarkerColor( 1 );
    expdt->SetLineStyle( 1 );   
    expdt->SetLineColor( 1 );
    
    theory->Draw( "C" );
    expdt->Draw( "P" );
    
    canvas_->Update();
  }
  
  void add( Exp::DataSet& set ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  double scale = set.normalization();
  int tscale = 
    static_cast< int >( pow( 10.0, static_cast< int >( vtgrapherr_.size() )));
  
  if( set.size() == 0 ) return;
  
  TGraph* theory = new TGraph( set.size() );
  TGraphErrors* expdt = new TGraphErrors( set.size() );
  
  for( int i = 0; i < set.size(); i++ ){
      
    Exp::DataPoint& p = set[ i ];
    
    double errsum = p.stat() * p.stat();
    for( int j = 0; j < p.syst().size(); j++ )
      errsum += p.syst()[ j ] * p.syst()[ j ];
    
    double zmin = p.kine( "zmin" );
    double zmax = p.kine( "zmax" );
    double zmid = ( p.kine( "zmid" ) != 0.0 ? p.kine( "zmid" ) : 
		    0.5 * ( zmin + zmax ) );
    
    double data = p.data();
    
    double theo = p.model();
    double merr = p.modelError();
    
    theory->SetPoint( i, zmid, tscale * scale * theo );
    expdt->SetPoint( i, zmid, tscale * data );
    expdt->SetPointError( i, 0.5 * ( zmax - zmin ), sqrt( errsum ) );
    
  }
  
  theory->SetLineStyle( 1 );   
  theory->SetLineColor( 1 );
  
  expdt->SetMarkerSize( 1.0 );   
  expdt->SetMarkerStyle( vtgrapherr_.size() + 2 );   
  expdt->SetMarkerColor( 1 );
  expdt->SetLineStyle( 1 );   
  expdt->SetLineColor( 1 );

  ostringstream ostr;
  ostr << set.info() << "   " << sqrt( set[ 0 ].kine( "Q^2" ) ) << " GeV ";
  ostr << " ( #times" << tscale << " )";
  vexpname_.push_back( ostr.str() );
  vtgraph_.push_back( theory );
  vtgrapherr_.push_back( expdt );
  
  }
  
  void sumdraw( Exp::DataSet& repset ){

    string xname = getname( repset );
    
    TH1 *frame = canvas_->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    
    string xsecname = "#frac{1}{#sigma_{tot}}#frac{d#sigma}{dz}";
    xsecname += "( e^{+} e^{-} -> " + xname + " + X )";
    frame->GetYaxis()->SetTitle( xsecname.c_str() );
    frame->GetYaxis()->SetLabelSize( 0.03 );
    frame->GetYaxis()->SetTitleSize( 0.03 );
    frame->GetYaxis()->SetTitleOffset( 1.4 );
    frame->GetYaxis()->CenterTitle();

    frame->GetXaxis()->SetTitle( "z" );
    frame->GetXaxis()->SetLabelSize( 0.03 );
    frame->GetXaxis()->SetTitleSize( 0.03 );
    frame->GetXaxis()->CenterTitle();

    for( int i = 0; i < vtgraph_.size(); i++ ) 
      vtgraph_[ i ]->Draw( "C" );
    for( int i = 0; i < vtgrapherr_.size(); i++ ) 
      vtgrapherr_[ i ]->Draw( "P" );

    // exp legend
    lege_ = new TLegend( 0.12, 0.18, 0.4, 0.28 );
    for( int i = 0; i < vtgrapherr_.size(); i++ ){
      lege_->AddEntry( vtgrapherr_[ i ], vexpname_[ i ].c_str(), "p" );
    }
    lege_->SetTextSize( 0.023 );
    lege_->SetLineColor( 0 );
    lege_->SetFillColor( 0 );
    lege_->Draw();

    // theory legend
    legt_ = new TLegend( 0.65, 0.85, 0.85, 0.88 );
    legt_->AddEntry( vtgrapherr_[ 0 ], "  Fit Results", "l" );
    legt_->SetTextSize( 0.025 );
    legt_->SetLineColor( 0 );
    legt_->SetFillColor( 0 );
    legt_->Draw();

    canvas_->Update();

  }
  
};


int main( int argc, char* argv[] ){

  gROOT->SetStyle("Plain");

  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

