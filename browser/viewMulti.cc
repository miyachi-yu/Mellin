/*
  display SIDIS multiplicity data
 */

#include <iostream>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <ExpData/ExpData.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/xzXsec.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolDISXsec/xXsec.hh>
#include <unpolPDF/Evolution.hh>
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

      panel1_ = new ExpDataPanel( *ed_, "data", "z min" );
      panel2_ = new ExpDataPanel( *ed_, "data", "z max" );
      panel3_ = new ExpDataPanel( *ed_, "data", "Q^2 min" );
      panel4_ = new ExpDataPanel( *ed_, "data", "Q^2 max" );
      panel5_ = new ExpDataPanel( *ed_, "data", "z ave" );
      panel6_ = new ExpDataPanel( *ed_, "data", "Q^2 ave" );
      panel7_ = new ExpDataPanel( *ed_, "data", "x ave" );

      vector< double > zmin( 0 );
      vector< TGraph* > zminTG = panel1_->getTGraph();
      vector< double > zmax( 0 );
      vector< TGraph* > zmaxTG = panel2_->getTGraph();
      vector< double > Q2min( 0 );
      vector< TGraph* > Q2minTG = panel3_->getTGraph();
      vector< double > Q2max( 0 );
      vector< TGraph* > Q2maxTG = panel4_->getTGraph();
      vector< double > Q2ave( 0 );      

      vector< double > zave( 0 );
      vector< TGraph* > zaveTG = panel5_->getTGraph();
      vector< double > Q2A( 0 );
      vector< TGraph* > Q2ATG = panel6_->getTGraph();
      vector< double > xave( 0 );
      vector< TGraph* > xaveTG = panel7_->getTGraph();

      for( int j = 0; j < zminTG.size() ; j++ )
	for( int i = 0; i < zminTG[ j ]->GetN(); i++ )
	  zmin.push_back( zminTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < zmaxTG.size() ; j++ )
	for( int i = 0; i < zmaxTG[ j ]->GetN(); i++ )
	  zmax.push_back( zmaxTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < Q2minTG.size() ; j++ )
	for( int i = 0; i < Q2minTG[ j ]->GetN(); i++ )
	  Q2min.push_back( Q2minTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < Q2maxTG.size() ; j++ )
	for( int i = 0; i < Q2maxTG[ j ]->GetN(); i++ )
	  Q2max.push_back( Q2maxTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < zaveTG.size() ; j++ )
	for( int i = 0; i < zaveTG[ j ]->GetN(); i++ )
	  zave.push_back( zaveTG[ j ]->GetX()[ i ] );

      for( int j = 0; j < Q2ATG.size() ; j++ )
	for( int i = 0; i < Q2ATG[ j ]->GetN(); i++ )
	  Q2A.push_back( Q2ATG[ j ]->GetX()[ i ] );

      for( int j = 0; j < xaveTG.size() ; j++ )
	for( int i = 0; i < xaveTG[ j ]->GetN(); i++ )
	  xave.push_back( xaveTG[ j ]->GetX()[ i ] );

      for( int i = 0; i < zmin.size(); i++ )
	Q2ave.push_back( args.hasOpt( "Ave" ) ? Q2A[ i ] :
			  0.5 * ( Q2min[ i ] + Q2max[ i ] ) );
      
      delete panel1_; delete panel2_; delete panel3_; delete panel4_;
      delete panel5_; delete panel6_; delete panel7_;

      panel_ = new ExpDataPanel( *ed_, "data", 
				 args.get( "kine1", "zmid" ),
				 args.get( "kine2", "" ) );

      timer.start();
      if( ! args.hasOpt( "Zmom" ) ){
	panel_->addTGraphs( args.hasOpt( "Ave" ) ? // x z avarage
			    this->addGraphs( zave, xave, Q2ave ) :
			    this->addGraphs( zmin, zmax, Q2ave ) );
      } else {
	panel_->addTGraphs( args.hasOpt( "Ave" ) ? // x z average
			    this->addGraphsZ( zave, xave, Q2ave ) :
			    this->addGraphsZ( zmin, zmax, Q2ave ) );
      }
      timer.stop();
      
      double msec = timer.check( Timer::Msec );
      cout << setw(10) << msec / 1000.0 << " sec";
      cout << setw(10) << msec / 1000.0 / zmin.size() << " sec/pt";
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
  ExpDataPanel *panel4_;
  ExpDataPanel *panel5_;
  ExpDataPanel *panel6_;
  ExpDataPanel *panel7_;

  vector< TGraph* > addGraphs( vector< double > zmin, vector< double > zmax,
			       vector< double > Q2ave ){
    Utility::Arguments& args = Utility::Arguments::ref();

    vector< TGraph* > tmpgraph( 0 );

    double UPDFoffsetN = args.get( "UPDFoffsetN", 4.5  );  
    double UPDFangleN  = args.get( "UPDFangleN",  0.60 );  
    double UPDFlengthN = args.get( "UPDFlengthN", 8.0 );
    double UPDFoffset = args.get( "UPDFoffset", 4.5 );  
    double UPDFangle  = args.get( "UPDFangle",  0.60 );  
    double UPDFlength = args.get( "UPDFlength", 8.0 );
    double FFoffset = args.get( "FFoffset", 3.0  );  
    double FFangle  = args.get( "FFangle",  0.60 );  
    double FFlength = args.get( "FFlength", 8.0 );

    int leg1 = args.get( "leg1", 6 );
    int leg2 = args.get( "leg2", 8 );
    double prec = args.get( "prec", 5.0E-3 );
    int ngrid = args.get( "ngrid", 2 );

    QCD::Flavor::HADRON h_ = QCD::Flavor::HADRON( args.get( "had", 1 ) );
    Xsection::Charge::TARGET t_ = 
      Xsection::Charge::TARGET( args.get( "tar", 0 ) );

    unpolPDF::Evo* UPDFevo_= new unpolPDF::Evo( args );
    Fragmentation::Evo* FFevo_ = new Fragmentation::Evo( args, h_ );

    UPDFevo_->constNf( false );
    FFevo_->constNf( false );

    //    unpolDISXsec::StrFunc f1( args, t_ );
    //    if( args.hasOpt( "Dcalc" ) ) f1.setDirect( true );
    unpolDISXsec::StrFunc f1( UPDFevo_, t_ );
    //        f1.setS( args.get( "S", 56.2 ) );

    //    unpolSIDISXsec::StrFunc f1h( args, t_, h_ );    
    //    if( args.hasOpt( "Dcalc" ) ) f1h.setDirect( true );
    unpolSIDISXsec::StrFunc f1h( UPDFevo_, FFevo_, t_, h_ );
    //        f1h.setS( args.get( "S", 56.2 ) );

    //    f1.xsecComp()->coeff()->constNf( true );
    //    f1h.xsecComp()->coeff()->constNf( true );
    
    // --- denominator --- //
    //    unpolDISXsec::xXsec f1x( &f1, 6, 8, 1.0E-3 );
    Xsection::xXsec f1x( &f1, leg1, leg2, prec );
    f1x.setGngrid( ngrid );
    f1x.setParameter( UPDFoffsetN, UPDFangleN );
    f1x.upper( UPDFlengthN ); 
    
    // --- numerator --- //
    //    /*
    //    unpolSIDISXsec::xzXsec f1hx( &f1h, 6, 8, 1.0E-3 );
    Xsection::xzXsec f1hx( &f1h, leg1, leg2, prec );
    f1hx.setGngrid( ngrid );    
    vector< double > offset2_( 2, FFoffset ); 
    vector< double > phi2_( 2, FFangle ); 
    offset2_[ 0 ] = UPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = UPDFangle; // 1st element for xx integ
    f1hx.setParameter( offset2_, phi2_ );
    f1hx.integrand().upper( UPDFlength ); // x
    f1hx.upper( FFlength ); // z
    //    */
    /*
    Xsection::xXsec f1hx( &f1h, leg1, leg2, prec );
    f1hx.setGngrid( ngrid );    
    vector< double > offset2_( 2, FFoffset ); 
    vector< double > phi2_( 2, FFangle ); 
    vector< double > length2_( 2, FFlength ); 
    offset2_[ 0 ] = UPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = UPDFangle; // 1st element for xx integ
    length2_[ 0 ] = UPDFlength; // 1st element for xx integ
    f1hx.setParameter( offset2_, phi2_ );
    f1hx.upper( length2_ );
    */

    vector< double > xx_( 2, 0.0 ), zz_( 2, 0.0 ), mom( 2, 0.0 );
    vector< double > xa_( 1, 0.0 ), za_( 1, 0.0 );
    if( !args.hasOpt( "Ave" ) ) mom[ 0 ] = 1.0; mom[ 1 ] = 1.0;

    f1hx.setMomParam( mom );
    
    cout << " start theory calc " << endl;

    for( int j = 0; j < 4; j++ ){

    vector< double > X( 0 );  
    vector< double > Y( 0 );  

    for( int i = 0; i < zmin.size() / 4; i++ ){
      
      zz_[ 0 ] = zmax[ i + 5 * j ]; zz_[ 1 ] = zmin[ i + 5 * j ];
      xa_[ 0 ] = zmax[ i + 5 * j ]; za_[ 0 ] = zmin[ i + 5 * j ]; 

      f1.setQ2( Q2ave[ i + 5 * j ] );
      f1h.setQ2( Q2ave[ i + 5 * j ] );

      xx_ = ( args.hasOpt( "Zmom" ) ? getx( Q2ave[ i + 5 * j ] + 0.1 ) :
	      getx( Q2ave[ i + 5 * j ] ) );

      double deno = args.hasOpt( "Ave" ) ? f1x( xa_ ) : f1x( xx_ );
      cout << " deno " << deno << flush;
      double nume = args.hasOpt( "Ave" ) ? f1hx( xa_, za_ ) : f1hx( xx_, zz_ );
      cout << " nume " << nume << endl;

      X.push_back( Q2ave[ i + 5 * j ] );
      Y.push_back( nume / deno ); // <- z x averaged

      cout << "Q2 = " << X[ i ] << " ||  Multi = " << Y[ i ] << endl; 
      if( args.hasOpt( "Ave" ) ){
	cout << "Zave = " << za_[ 0 ] << " ||  Xave = " << xa_[ 0 ] << endl;
      } else {
	cout << "Xmin = " << xx_[ 1 ] << " ||  Xmax = " << xx_[ 0 ] << endl;
      }

    }

    double X1_[ X.size() ], Y1_[ Y.size() ];
    for( int i = 0; i < X.size(); i++ ){
      X1_[ i ] = X[ i ];
      Y1_[ i ] = Y[ i ];
    }

    tmpgraph.push_back( new TGraph( X.size(), X1_, Y1_ ) );

    }

    return tmpgraph;

  }


  vector< TGraph* > addGraphsZ( vector< double > zmin, vector< double > zmax,
				vector< double > Q2ave ){
    Utility::Arguments& args = Utility::Arguments::ref();

    vector< TGraph* > tmpgraph( 0 );
    
    double UPDFoffsetN = args.get( "UPDFoffsetN", 4.5  );  
    double UPDFangleN  = args.get( "UPDFangleN",  0.60 );  
    double UPDFlengthN = args.get( "UPDFlengthN", 8.0 );
    double UPDFoffset = args.get( "UPDFoffset", 4.5 );  
    double UPDFangle  = args.get( "UPDFangle",  0.60 );  
    double UPDFlength = args.get( "UPDFlength", 8.0 );
    double FFoffset = args.get( "FFoffset", 3.0  );  
    double FFangle  = args.get( "FFangle",  0.60 );  
    double FFlength = args.get( "FFlength", 8.0 );
    
    int leg1 = args.get( "leg1", 6 );
    int leg2 = args.get( "leg2", 8 );
    double prec = args.get( "prec", 5.0E-3 );
    int ngrid = args.get( "ngrid", 2 );
    
    QCD::Flavor::HADRON h_ = QCD::Flavor::HADRON( args.get( "had", 1 ) );
    Xsection::Charge::TARGET t_ = 
      Xsection::Charge::TARGET( args.get( "tar", 0 ) );

    unpolPDF::Evo* UPDFevo_= new unpolPDF::Evo( args );
    Fragmentation::Evo* FFevo_ = new Fragmentation::Evo( args, h_ );

    UPDFevo_->constNf( false );
    FFevo_->constNf( false );
    
    //    unpolDISXsec::StrFunc f1( args, t_ );
    //    if( args.hasOpt( "Dcalc" ) ) f1.setDirect( true );
    unpolDISXsec::StrFunc f1( UPDFevo_, t_ );
    //        f1.setS( args.get( "S", 56.2 ) );
    
    //    unpolSIDISXsec::StrFunc f1h( args, t_, h_ );    
    //    if( args.hasOpt( "Dcalc" ) ) f1h.setDirect( true );
    unpolSIDISXsec::StrFunc f1h( UPDFevo_, FFevo_, t_, h_ );
    //        f1h.setS( args.get( "S", 56.2 ) );
    
    //    f1.xsecComp()->coeff()->constNf( true );
    //    f1h.xsecComp()->coeff()->constNf( true );
    
    // --- denominator --- //
    //    unpolDISXsec::xXsec f1x( &f1, 6, 8, 1.0E-3 );
    Xsection::xXsec f1x( &f1, leg1, leg2, prec );
    f1x.setGngrid( ngrid );
    f1x.setParameter( UPDFoffsetN, UPDFangleN );
    f1x.upper( UPDFlengthN ); 
    
    // --- numerator --- //
    //    /*
    //    unpolSIDISXsec::xzXsec f1hx( &f1h, 6, 8, 1.0E-3 );
    Xsection::xzXsec f1hx( &f1h, leg1, leg2, prec );
    f1hx.setGngrid( ngrid );    
    vector< double > offset2_( 2, FFoffset ); 
    vector< double > phi2_( 2, FFangle ); 
    offset2_[ 0 ] = UPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = UPDFangle; // 1st element for xx integ
    f1hx.setParameter( offset2_, phi2_ );
    f1hx.integrand().upper( UPDFlength ); // x
    f1hx.upper( FFlength ); // z
    //    */
    /*
    Xsection::xXsec f1hx( &f1h, leg1, leg2, prec );
    f1hx.setGngrid( ngrid );    
    vector< double > offset2_( 2, FFoffset ); 
    vector< double > phi2_( 2, FFangle ); 
    vector< double > length2_( 2, FFlength ); 
    offset2_[ 0 ] = UPDFoffset; // 1st element for xx integ
    phi2_[ 0 ] = UPDFangle; // 1st element for xx integ
    length2_[ 0 ] = UPDFlength; // 1st element for xx integ
    f1hx.setParameter( offset2_, phi2_ );
    f1hx.upper( length2_ );
    */
    
    vector< double > xx_( 2, 0.0 ), zz_( 2, 0.0 ), mom( 2, 0.0 );
    vector< double > xa_( 1, 0.0 ), za_( 1, 0.0 );
    if( !args.hasOpt( "Ave" ) ) mom[ 0 ] = 1.0; mom[ 1 ] = 1.0;
    
    f1hx.setMomParam( mom );
    
    cout << " start theory calc " << endl;
    
    //    for( int j = 0; j < 4; j++ ){
    
    vector< double > X( 0 );  
    vector< double > Y( 0 );  
    
    for( int i = 0; i < zmin.size(); i++ ){
      
      zz_[ 0 ] = zmax[ i ]; zz_[ 1 ] = zmin[ i ];
      xa_[ 0 ] = zmax[ i ]; za_[ 0 ] = zmin[ i ]; 
      
      f1.setQ2( Q2ave[ i ] );
      f1h.setQ2( Q2ave[ i ] );
      
      xx_ = ( args.hasOpt( "Zmom" ) ? getx( Q2ave[ i ] + 0.1 ) :
	      getx( Q2ave[ i ] ) );
      
      double deno = args.hasOpt( "Ave" ) ? f1x( xa_ ) : f1x( xx_ );
      cout << " deno " << deno << flush;
      double nume = args.hasOpt( "Ave" ) ? f1hx( xa_, za_ ) : f1hx( xx_, zz_ );
      cout << " nume " << nume << endl;
      
      X.push_back( 0.5 * ( zz_[ 0 ] + zz_[ 1 ] ) );
      Y.push_back( nume / deno ); // <- z x averaged
      
      cout << "Q2 = " << Q2ave[ i ] << " ||  Multi = " << Y[ i ] << endl; 
      if( args.hasOpt( "Ave" ) ){
	cout << "Zave = " << za_[ 0 ] << " ||  Xave = " << xa_[ 0 ] << endl;
      } else {
	cout << "Xmin = " << xx_[ 1 ] << " ||  Xmax = " << xx_[ 0 ] << endl;
	cout << "Zmin = " << zz_[ 1 ] << " ||  Zmax = " << zz_[ 0 ] << endl;
      }
      
    }
    
    double X1_[ X.size() ], Y1_[ Y.size() ];
    for( int i = 0; i < X.size(); i++ ){
      X1_[ i ] = X[ i ];
      Y1_[ i ] = Y[ i ];
    }
    
    tmpgraph.push_back( new TGraph( X.size(), X1_, Y1_ ) );
    
    cout << tmpgraph.size() << endl;
    return tmpgraph;

  }


  vector< double > getx( const double& q2 ){

    vector< double > tmpv( 0 );

    map< double, map< double, double > > tmp;
    tmp[ 0.023 ][ 0.040 ] = 1.21;
    tmp[ 0.040 ][ 0.055 ] = 1.44;
    tmp[ 0.055 ][ 0.075 ] = 1.72;
    tmp[ 0.075 ][ 0.100 ] = 2.06;
    tmp[ 0.100 ][ 0.140 ] = 2.58;
    tmp[ 0.140 ][ 0.200 ] = 3.52;
    tmp[ 0.200 ][ 0.300 ] = 5.03;
    tmp[ 0.300 ][ 0.400 ] = 7.09;
    tmp[ 0.400 ][ 0.600 ] = 9.76;


    for( map< double, map< double, double > >::iterator itr = tmp.begin();
	 itr != tmp.end(); itr++ ) 
      for( map< double, double >::iterator itr2 = itr->second.begin();
	   itr2 != itr->second.end(); itr2++ ) 
	if( itr2->second > q2 ){
	   --itr;
	   tmpv.push_back( itr->second.begin()->first ); // max
	   tmpv.push_back( itr->first );  // min
	   return tmpv;
	}
    
    tmpv.push_back( 0.600 ); // max
    tmpv.push_back( 0.400 ); // min
    return tmpv;
  }

};
  
int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

