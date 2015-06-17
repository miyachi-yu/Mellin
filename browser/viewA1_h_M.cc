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
    panel_( NULL ), tgraphv_( 0 )
  {
    gROOT->SetStyle( "Plain" );

    Timer timer;
    
    vector< string > dataname_( 0 ), had_( 0 ), tar_( 0 );
    dataname_.push_back( "../ExpData/A1h_p_pip.xml" );
    dataname_.push_back( "../ExpData/A1h_d_pip.xml" );
    dataname_.push_back( "../ExpData/A1h_p_pim.xml" );
    dataname_.push_back( "../ExpData/A1h_d_pim.xml" );
    had_.push_back( "1" );
    had_.push_back( "1" );
    had_.push_back( "-1" );
    had_.push_back( "-1" );
    tar_.push_back( "0" );
    tar_.push_back( "2" );
    tar_.push_back( "0" );
    tar_.push_back( "2" );
    char tmp1[ 100 ], tmp2[ 100 ], tmp3[ 100 ];

    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    
    vector< Exp::ExpData* > vedtmp_( 0 );

    for( int jj = 0; jj < 2; jj++){
      
      args.set( "expdata", dataname_[ jj ] );
      args.set( "had",     had_[ jj ] );
      args.set( "tar",     tar_[ jj ] );

      cout << setw( 10 ) << args.getOpt( "expdata" ) << endl;
      
      //      vedtmp_.push_back( new Exp::ExpData( args ) );
      vedtmp_.push_back( new Exp::ExpData( dataname_[ jj ] ) );
      
      vector< double > x  = vedtmp_[ vedtmp_.size() - 1 ]->findkin( "x" );
      vector< double > Q2 = vedtmp_[ vedtmp_.size() - 1 ]->findkin( "Q^2" );
      
      timer.start();
      vector< TGraph* > tmp = this->addGraphs( args, x, Q2 );
      timer.stop(); 
      
      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << setw(10) << msec / 1000.0 / x.size() << " sec/pt ";
      cout << endl;

      for( int i = 0; i < tmp.size(); i++ ) tgraphv_.push_back( tmp[ i ] );

    }

    panel_ = new ExpDataPanel( *vedtmp_[ 0 ], "data", 
			       Arguments::ref().get( "kine1", "zmid" ),
			       Arguments::ref().get( "kine2", "" ) );

    for( int i = 1; i < vedtmp_.size(); i++ )
      panel_->addEgraphs( *vedtmp_[ i ] );

    panel_->addTGraphs( tgraphv_ );

    if( Arguments::ref().hasOpt( "logx" ) ) panel_->SetLogx();
    if( Arguments::ref().hasOpt( "logy" ) ) panel_->SetLogy();

    for( int i = 0; i < vedtmp_.size(); i++ ) if( vedtmp_[ i ] )
      delete vedtmp_[ i ];

  }
  
  virtual ~MyApplication() {
    for( int i = 0; i < tgraphv_.size(); i++ ) if( tgraphv_[ i ] ) 
      delete tgraphv_[ i ];
    if( panel_ ) delete panel_;
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    if( panel_ ) panel_->draw();
  }

  vector< TGraph* > tgraphv_;
  ExpDataPanel *panel_;

  vector< TGraph* > addGraphs( Arguments& args, 
			       vector< double > x, vector< double > Q2 ){

    vector< TGraph* > tmpgraph( 0 );

    double UPDFoffset = args.get( "UPDFoffset", 3.0  );  
    double UPDFangle  = args.get( "UPDFangle",  0.55 );  
    double UPDFlength = args.get( "UPDFlength", 8.0 );
    double FFoffset = args.get( "FFoffset", 3.0  );  
    double FFangle  = args.get( "FFangle",  0.55 );  
    double FFlength = args.get( "FFlength", 8.0 );
    double PPDFoffset = args.get( "PPDFoffset", 2.0  );  
    double PPDFangle  = args.get( "PPDFangle",  0.55 );  
    double PPDFlength = args.get( "PPDFlength", 8.0 );

    int leg1 = args.get( "leg1", 6 );
    int leg2 = args.get( "leg2", 8 );
    double prec = args.get( "prec", 5.0E-2 );
    int ngrid = args.get( "ngrid", 2 );

    QCD::Flavor::HADRON h_ = QCD::Flavor::HADRON( args.get( "had", 1 ) );
    Xsection::Charge::TARGET t_ = 
      Xsection::Charge::TARGET( args.get( "tar", 0 ) );

    unpolPDF::Evo* UPDFevo_ = new unpolPDF::Evo( args );
    polPDF::Evo* PPDFevo_= new polPDF::Evo( args );
    Fragmentation::Evo* FFevo_ = new Fragmentation::Evo( args, h_ );

    UPDFevo_->constNf( false );
    FFevo_->constNf( false );
    PPDFevo_->constNf( true );

    // for A1
    unpolDIS::StrFunc f1( UPDFevo_, t_ );
    polDIS::StrFunc g1( PPDFevo_, t_ );

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

