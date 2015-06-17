#include <iostream>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <ExpData/ExpData.hh>

#include <Xsec/xXsec.hh>
#include <FragXsec_T/FragXsec.hh>
#include <Fragmentation/Evolution.hh>
#include <Evolution/KernelSum.hh>

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

      panel1_ = new ExpDataPanel( *ed_, "data", "zmin" );
      panel2_ = new ExpDataPanel( *ed_, "data", "zmax" );
      panel3_ = new ExpDataPanel( *ed_, "data", "Q^2" );

      vector< double > zmin( 0 );
      vector< TGraph* > zminTG = panel1_->getTGraph();
      vector< double > zmax( 0 );
      vector< TGraph* > zmaxTG = panel2_->getTGraph();
      vector< double > Q2( 0 );
      vector< TGraph* > Q2TG = panel3_->getTGraph();

      for( int j = 0; j < zminTG.size() ; j++ )
	for( int i = 0; i < zminTG[ j ]->GetN(); i++ )
	  zmin.push_back( zminTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < zmaxTG.size() ; j++ )
	for( int i = 0; i < zmaxTG[ j ]->GetN(); i++ )
	  zmax.push_back( zmaxTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < Q2TG.size() ; j++ )
	for( int i = 0; i < Q2TG[ j ]->GetN(); i++ )
	  Q2.push_back( Q2TG[ j ]->GetX()[ i ] );

      delete panel1_; delete panel2_; delete panel3_;

      panel_ = new ExpDataPanel( *ed_, "data", 
				 args.get( "kine1", "zmid" ),
				 args.get( "kine2", "" ) );
      timer.start();
      panel_->addTGraphs( this->addGraphs( zmin, zmax, Q2 ) );
      timer.stop(); 

      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << setw(10) << msec / 1000.0 / zmin.size() << " sec/pt ";
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
  ExpDataPanel *panel3_;

  vector< TGraph* > addGraphs( vector< double > zmin, 
			       vector< double > zmax, 
			       vector< double > Q2 ){

    Utility::Arguments& args = Utility::Arguments::ref();

    vector< TGraph* > tmpgraph( 0 );

    double FFoffset = args.get( "FFoffset", 3.0  );  
    double FFangle  = args.get( "FFangle",  0.55 );  
    double FFlength = args.get( "FFlength", 8.0 );

    int leg1 = args.get( "leg1", 6 );
    int leg2 = args.get( "leg2", 8 );
    double prec = args.get( "prec", 5.0E-2 );
    int ngrid = args.get( "ngrid", 2 );

    int hh_ = QCD::Flavor::HADRON( args.get( "had", 100 ) );

    Fragmentation::Evo* FFevo1_ = new Fragmentation::Evo( args, 
							  QCD::Flavor::PIp );
    Fragmentation::Evo* FFevo2_ = new Fragmentation::Evo( args,
							  QCD::Flavor::KAp );
    Fragmentation::Evo* FFevo3_ = new Fragmentation::Evo( args,
							  QCD::Flavor::REp );

    FFevo1_->constNf( false );
    FFevo2_->constNf( false );
    FFevo3_->constNf( false );
    //    FFevo3_->alpha().atMz( 0.114 ); // don't forget

    Evolution::KernelSum* hadron_ = new Evolution::KernelSum();
    hadron_->add( FFevo1_ );
    hadron_->add( FFevo2_ );
    hadron_->add( FFevo3_ );
    hadron_->constNf( false );

    //    /*
    cout << FFevo1_->initialDistribution()->initialScale() << " " 
	 << FFevo1_->alpha().atMz() << endl;
    cout << FFevo2_->initialDistribution()->initialScale() << " " 
	 << FFevo2_->alpha().atMz() << endl;
    cout << FFevo3_->initialDistribution()->initialScale() << " " 
	 << FFevo3_->alpha().atMz() << endl;
    cout << hadron_->initialDistribution()->initialScale() << " " 
	 << hadron_->alpha().atMz() << endl;
    cout << (*hadron_)[ 0 ]->initialDistribution()->initialScale() << " " 
	 << (*hadron_)[ 0 ]->alpha().atMz() << endl;
    cout << (*hadron_)[ 1 ]->initialDistribution()->initialScale() << " " 
	 << (*hadron_)[ 1 ]->alpha().atMz() << endl;
    cout << (*hadron_)[ 2 ]->initialDistribution()->initialScale() << " " 
	 << (*hadron_)[ 2 ]->alpha().atMz() << endl;
    //    */

    FragXsec_T::FragXsec xsec( hadron_, QCD::Flavor::HAp );
    FragXsec_T::FragXsec xsec1( FFevo1_, QCD::Flavor::PIp );
    FragXsec_T::FragXsec xsec2( FFevo2_, QCD::Flavor::KAp );
    FragXsec_T::FragXsec xsec3( FFevo3_, QCD::Flavor::REp );
    
    Xsection::xXsec xxsec( &xsec, leg1, leg2, prec );
    xxsec.setGngrid( ngrid );
    xxsec.setParameter( FFoffset, FFangle );
    xxsec.upper( FFlength ); 

    Xsection::xXsec xxsec1( &xsec1, leg1, leg2, prec );
    xxsec1.setGngrid( ngrid );
    xxsec1.setParameter( FFoffset, FFangle );
    xxsec1.upper( FFlength ); 

    Xsection::xXsec xxsec2( &xsec2, leg1, leg2, prec );
    xxsec2.setGngrid( ngrid );
    xxsec2.setParameter( FFoffset, FFangle );
    xxsec2.upper( FFlength ); 

    Xsection::xXsec xxsec3( &xsec3, leg1, leg2, prec );
    xxsec3.setGngrid( ngrid );
    xxsec3.setParameter( FFoffset, FFangle );
    xxsec3.upper( FFlength ); 

    vector< double > zz_( 2, 0.0 );
    
    cout << " start theory calc " << endl;

    vector< double > X( 0 );  
    vector< double > Y( 0 );  

    for( int i = 0; i < zmin.size(); i++ ){

      if( Q2[ i ] < args.get( "q2limit", 1.0 ) ) continue;
      
      zz_[ 0 ] = zmin[ i ];
      zz_[ 1 ] = zmax[ i ];

      xsec.setQ2( Q2[ i ] );
      xsec1.setQ2( Q2[ i ] );
      xsec2.setQ2( Q2[ i ] );
      xsec3.setQ2( Q2[ i ] );

      double deno = 0.0;

      if( hh_ == 100 || hh_ == -100 ) deno = 2.0 * xxsec( zz_ );
      if( args.hasOpt( "decomp" ) )
	if( hh_ == 100 || hh_ == -100 ) 
	  deno = 2.0 * ( xxsec1( zz_ ) + xxsec2( zz_ ) + xxsec3( zz_ ) );
      if( hh_ == 1 || hh_ == -1 ) deno = 2.0 * xxsec1( zz_ );
      if( hh_ == 2 || hh_ == -2 ) deno = 2.0 * xxsec2( zz_ );
      if( hh_ == 3 || hh_ == -3 ) deno = 2.0 * xxsec3( zz_ );

      X.push_back( 0.5 * ( zz_[ 0 ] + zz_[ 1 ] ) );
      Y.push_back( deno * args.get( "factor", 1.0 ) );

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

