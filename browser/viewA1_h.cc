#include <iostream>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <ExpData/ExpData.hh>
#include <unpolDISXsec/xXsec.hh>
#include <unpolSIDISXsec/xzXsec.hh>
#include <unpolSIDIS/StrFunc.hh>
#include <unpolDIS/StrFunc.hh>
#include <polSIDIS/StrFunc.hh>
#include <polDIS/StrFunc.hh>

#include <unpolPDF/Evolution.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/XsecComp.hh>

#include <TROOT.h>
#include <TApplication.h>
#include <TGraph.h>

#include "ExpDataPanel.hh"

using namespace std;
using namespace Utility;

class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) :
    TApplication( "MyApplication", &argc, argv ),
    ed_( NULL ), panel_( NULL )
  {

    gROOT->SetStyle( "Plain" );
    
    Timer timer;
    
    Utility::Arguments& args = Utility::Arguments::ref();
    if( args.hasOpt( "expdata" ) ) { 
      
      ed_    = new Exp::ExpData( args );
      
      panel1_ = new ExpDataPanel( *ed_, "data", "x" );
      panel2_ = new ExpDataPanel( *ed_, "data", "Q^2" );

      vector< double > x( 0 );
      vector< TGraph* > xTG = panel1_->getTGraph();
      vector< double > Q2( 0 );
      vector< TGraph* > Q2TG = panel2_->getTGraph();
      
      for( int j = 0; j < xTG.size() ; j++ )
	for( int i = 0; i < xTG[ j ]->GetN(); i++ )
	  x.push_back( xTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < Q2TG.size() ; j++ )
	for( int i = 0; i < Q2TG[ j ]->GetN(); i++ )
	  Q2.push_back( Q2TG[ j ]->GetX()[ i ] );

      delete panel1_; delete panel2_;


      panel_ = new ExpDataPanel( *ed_, "data", 
				 args.get( "kine1", "zmid" ),
				 args.get( "kine2", "" ) );
      timer.start();
      panel_->addTGraphs( this->addGraphs( x, Q2 ) );
      timer.stop(); 
      
      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << setw(10) << msec / 1000.0 / x.size() << " sec/pt ";
      cout << endl;

      if( args.hasOpt( "logx" ) ) panel_->SetLogx();
      if( args.hasOpt( "logy" ) ) panel_->SetLogy();

    }
    
  }
  
  virtual ~MyApplication() {
    if( ed_ ) delete ed_;
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    if( panel_ ) panel_->draw();
  }
  
  Exp::ExpData *ed_;
  ExpDataPanel *panel_;

  ExpDataPanel *panel1_;
  ExpDataPanel *panel2_;

  vector< TGraph* > addGraphs( vector< double > x, vector< double > Q2 ){

    Utility::Arguments& args = Utility::Arguments::ref();
    
    vector< TGraph* > tmpgraph( 0 );
    
    double UPDFoffset = args.get( "UPDFoffset", 3.0  );  
    double UPDFangle  = args.get( "UPDFangle",  0.55 );  
    double UPDFlength = args.get( "UPDFlength", 8.0 );
    double FFoffset   = args.get( "FFoffset", 3.0  );  
    double FFangle    = args.get( "FFangle",  0.55 );  
    double FFlength   = args.get( "FFlength", 8.0 );
    double PPDFoffset = args.get( "PPDFoffset", 2.0  );  
    double PPDFangle  = args.get( "PPDFangle",  0.55 );  
    double PPDFlength = args.get( "PPDFlength", 8.0 );

    int leg1 = args.get( "leg1", 6 );
    int leg2 = args.get( "leg2", 8 );
    double prec = args.get( "prec", 5.0E-3 );
    int ngrid = args.get( "ngrid", 2 );
    
    // get target and hadron information from arguments
    QCD::Flavor::HADRON h_ = QCD::Flavor::HADRON( args.get( "had", 1 ) );
    Xsection::Charge::TARGET t_ = 
      Xsection::Charge::TARGET( args.get( "tar", 0 ) );
    
    // create unpol and pol pdfs
    unpolPDF::Evo* UPDFevo_    = new unpolPDF::Evo( args );
    polPDF::Evo* PPDFevo_      = new polPDF::Evo( args );

    // create fragmentation funcitons
    Fragmentation::Evo* FFevo_ = new Fragmentation::Evo( args, h_ );
    
    UPDFevo_->constNf( false );
    FFevo_->constNf( false );
    PPDFevo_->constNf( true );
    
    // for A1
    unpolDIS::StrFunc f1( UPDFevo_, t_ );
    polDIS::StrFunc   g1( PPDFevo_, t_ );

    // for A1h
    unpolSIDIS::StrFunc f1h( UPDFevo_, FFevo_, t_, h_ );
    polSIDIS::StrFunc g1h( PPDFevo_, FFevo_, t_, h_ );
    
    // --- denominator --- //
    // f1 for A1
    Xsection::xXsec f1x( &f1, leg1, leg2, prec );
    f1x.setGngrid( ngrid );
    f1x.setParameter( UPDFoffset, UPDFangle );
    f1x.upper( UPDFlength ); 
    // f1h for A1h
    Xsection::xzXsec f1hx( &f1h, leg1, leg2, prec );
    f1hx.setGngrid( ngrid );    
    vector< double > offset2_( 2, FFoffset ); 
    vector< double > phi2_( 2, FFangle ); 
    offset2_[ 0 ] = UPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = UPDFangle; // 1st element for xx integ
    f1hx.setParameter( offset2_, phi2_ );
    f1hx.integrand().upper( UPDFlength ); // x
    f1hx.upper( FFlength ); // z
    
    // --- numerator --- //
    // g1 for A1
    Xsection::xXsec g1x( &g1, leg1, leg2, prec );
    g1x.setGngrid( ngrid );
    g1x.setParameter( PPDFoffset, PPDFangle );
    g1x.upper( PPDFlength ); 
    // g1h for A1h
    Xsection::xzXsec g1hx( &g1h, leg1, leg2, prec );
    g1hx.setGngrid( ngrid );    
    offset2_[ 0 ] = PPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = PPDFangle; // 1st element for xx integ
    g1hx.setParameter( offset2_, phi2_ );
    g1hx.integrand().upper( PPDFlength ); // x
    g1hx.upper( FFlength ); // z


    vector< double > xx_( 1, 0.0 ), zz_( 2, 0.0 ), mom( 2, 0.0 );
    mom[ 0 ] = 0.0; mom[ 1 ] = 1.0;
    
    zz_[ 0 ] = args.get( "zmax", 0.8 );
    zz_[ 1 ] = args.get( "zmin", 0.2 );

    f1hx.setMomParam( mom );
    g1hx.setMomParam( mom );
    
    cout << " start theory calc " << endl;

    vector< double > X( 0 );  
    vector< double > Y( 0 );  

    for( int i = 0; i < x.size(); i++ ){

      if( Q2[ i ] < args.get( "q2limit", 1.0 ) ) continue;
      
      xx_[ 0 ] = x[ i ];

      f1.setQ2( Q2[ i ] );
      f1h.setQ2( Q2[ i ] );
      g1.setQ2( Q2[ i ] );
      g1h.setQ2( Q2[ i ] );
      
      double deno = args.hasOpt( "A1h" ) ? f1hx( xx_, zz_ ) : f1x( xx_ );
      cout << " deno " << deno << flush;
      double nume = args.hasOpt( "A1h" ) ? g1hx( xx_, zz_ ) : g1x( xx_ );
      cout << " nume " << nume << endl;

      X.push_back( x[ i ] );
      Y.push_back( nume / deno ); // <- z averaged

      cout << "x = " << x[ i ] << " ||  " 
	   << ( args.hasOpt( "A1h" ) ? "A1h" : "A1" ) << "  = " 
	   << nume / deno << endl;      
    }

    double X1_[ X.size() ], Y1_[ Y.size() ];
    for( int i = 0; i < X.size(); i++ ){
      X1_[ i ] = X[ i ];
      Y1_[ i ] = Y[ i ];
    }

    tmpgraph.push_back( new TGraph( X.size(), X1_, Y1_ ) );
 
   return tmpgraph;

  }

};
  
int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

