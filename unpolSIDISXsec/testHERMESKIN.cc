/*!
  \file  testF1.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libunpolDISXsec.so.

  You may get some feeling how to use polDISXsec::StructureFunction class.

*/
#include <Utility/Timer.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <unpolDIS/StrFunc.hh>

#include <iostream>
#include <iomanip>
#include <map>
#include <time.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TLatex.h>

#include "StrFunc.hh"
#include "xzXsec.hh"
#include <unpolDISXsec/StrFunc.hh>
#include <unpolDISXsec/xXsec.hh>

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Utility;

void loadxzdata( vector< double>& z1, vector< double>& z2, 
	       vector< double>& x1, vector< double>& x2 );

void load_PIp_p_data( vector< double>& pipp, vector< double>& pipper );
void load_PIp_d_data( vector< double>& pipd, vector< double>& pipder );
void load_PIm_p_data( vector< double>& pimp, vector< double>& pimper );
void load_PIm_d_data( vector< double>& pimd, vector< double>& pimder );

void loadxzdata_z( vector< double>& z1, vector< double>& z2 );

void load_PIp_p_data_z( vector< double>& pipp, vector< double>& pipper );
void load_PIp_d_data_z( vector< double>& pipd, vector< double>& pipder );
void load_PIm_p_data_z( vector< double>& pimp, vector< double>& pimper );
void load_PIm_d_data_z( vector< double>& pimd, vector< double>& pimder );

TCanvas* rootout( vector< double>& xave, 
		  map< int, map< int, vector< double > > >& expdata_,
		  vector< double>& zzave, 
		  map< int, map< int, vector< double > > >& expdata_z_, 
		  map< int, map< int, vector< double > > >& thedata_,
		  map< int, map< int, vector< double > > >& thedata_z_,
		  double q2 );

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  gROOT->SetStyle("Plain");
  
  TApplication TA( "browser", &argc, argv ); //needed if you use root
  
  try {
    
    Timer timer;
    
    vector< double > q2(0);
    q2.push_back( args.get( "Q2", 2.5 ) );

    // ------------- exp load data ---------------- //
    // --- X expdata ---- //
    vector< double > z1( 0 ), z2( 0 ), x1( 0 ), x2( 0 ), xave( 0 ), zave( 0 );
    loadxzdata( z1, z2, x1, x2 );

    xave.resize( x1.size(), 0.0 );
    for( int i = 0; i < x1.size(); i++ ) 
      xave[ i ] = 0.5 * ( x1[ i ] + x2[ i ] );
    zave.resize( x1.size(), 0.0 );
    for( int i = 0; i < x1.size(); i++ ) 
      zave[ i ] = 0.5 * ( z1[ i ] + z2[ i ] );

    vector< double > pipp( 0 ), pipd( 0 ), pimp( 0 ), pimd( 0 );
    vector< double > pipper( 0 ), pipder( 0 ), pimper( 0 ), pimder( 0 );

    load_PIp_p_data( pipp, pipper );
    load_PIp_d_data( pipd, pipder );
    load_PIm_p_data( pimp, pimper );
    load_PIm_d_data( pimd, pimder );

    map< int, map< int, vector< double > > > expdata_;
    expdata_[ 0 ][  1 ] = pipp;  
    expdata_[ 2 ][  1 ] = pipd;
    expdata_[ 0 ][ -1 ] = pimp;
    expdata_[ 2 ][ -1 ] = pimd;

    // --- Z expdata ---- //
    vector< double > zz1( 0 ), zz2( 0 ), zzave( 0 );
    loadxzdata_z( zz1, zz2 );

    zzave.resize( zz1.size(), 0.0 );
    for( int i = 0; i < zz1.size(); i++ ) 
      zzave[ i ] = 0.5 * ( zz1[ i ] + zz2[ i ] );

    vector< double > pipp_z( 0 ), pipd_z( 0 ), pimp_z( 0 ), pimd_z( 0 );
    vector< double > pipper_z( 0 ), pipder_z( 0 ), pimper_z( 0 ), pimder_z( 0 );

    load_PIp_p_data_z( pipp_z, pipper_z );
    load_PIp_d_data_z( pipd_z, pipder_z );
    load_PIm_p_data_z( pimp_z, pimper_z );
    load_PIm_d_data_z( pimd_z, pimder_z );

    map< int, map< int, vector< double > > > expdata_z_;
    expdata_z_[ 0 ][  1 ] = pipp_z;  
    expdata_z_[ 2 ][  1 ] = pipd_z;
    expdata_z_[ 0 ][ -1 ] = pimp_z;
    expdata_z_[ 2 ][ -1 ] = pimd_z;

    // ---- SET INTEGPARAMS ---- //    
    double FFoffset, FFangle, FFlength;
    double UPDFoffset, UPDFangle, UPDFlength;
    double UPDFoffsetN, UPDFangleN, UPDFlengthN;

    FFoffset = args.get( "FFoffset", 3.0  );  
    FFangle  = args.get( "FFangle",  0.60 );  
    FFlength = args.get( "FFlength", 8.0 );  
    UPDFoffset = args.get( "UPDFoffset", 4.5  );  
    UPDFangle  = args.get( "UPDFangle",  0.6 );  
    UPDFlength = args.get( "UPDFlength", 8.0 );  
    UPDFoffsetN = args.get( "UPDFoffsetN", 4.5  );  
    UPDFangleN  = args.get( "UPDFangleN",  0.6 );  
    UPDFlengthN = args.get( "UPDFlengthN", 8.0 );  

    // ---- the. calc vectors ---- //
    map< int, map< int, vector< double > > > thedata_;
    map< int, map< int, vector< double > > > thedata_z_;

    // ---- xave indication ---- //
    cout << setw(7) << "Q^2" << setw(4) << "TGT" 
	 << setw(4) << "HAD" << setw(4) << "XAV";
    for( int i = 0; i < xave.size() / 4 ; i++ ) {
      cout << setw(13) << xave[ i ] << flush;
    }
    cout << endl; 

    // ---- zzave indication ---- //
    cout << setw(19) << "ZAV";
    //	  for( int i = 0; i < zzave.size() ; i++ ){
    for( int i = 0; i < 9 ; i++ ){
      cout << setw(13) << zzave[ i ] << flush;
    }
    cout << endl; 

    cout << endl;

    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    // p = 0; d = 2; PIp = 1; PIm = -1;

    for( int tk = 0; tk < ( args.hasOpt( "short" ) ? 1 : 3 ); tk++ ){
      if( tk == 1 ) continue;
      for( int hk = 1 ; hk >  ( args.hasOpt( "short" ) ? 0 : -2 ); hk-- ){
	if( hk == 0 ) continue;
	
	unpolDISXsec::StrFunc f1( args, Xsection::Charge::TARGET( tk ) );
	if( args.hasOpt( "Dcalc" ) ) f1.setDirect( true );
	//	f1.setS( args.get( "S", 56.2 ) );

	unpolSIDISXsec::StrFunc f1h( args, Xsection::Charge::TARGET( tk ), 
				     Flavor::HADRON( hk ) );
	if( args.hasOpt( "Dcalc" ) ) f1h.setDirect( true );
	//	f1h.setS( args.get( "S", 56.2 ) );

	Xsection::xXsec* f1x = NULL;
	Xsection::xzXsec* f1hx = NULL;
	//	Xsection::xXsec* f1hx = NULL;
	
	if( args.hasOpt( "laguerre" ) ){
	  throw( 1 );
	}else{
	  
	  // --- denominator --- //
	  f1x = new Xsection::xXsec( &f1, 6, 8, 5.0E-3 );
	  f1x->setGngrid( 2 );
	  
	  f1x->setParameter( UPDFoffsetN, UPDFangleN );
	  f1x->upper( UPDFlengthN ); 
	  
	  // --- numerator --- //
	  //	  /*
	  f1hx = new Xsection::xzXsec( &f1h, 6, 8, 5.0E-3 );
	  f1hx->setGngrid( 2 );
	  
	  vector< double > offset2_( 2, FFoffset ); 
	  vector< double > angle2_( 2, FFangle ); 
	  offset2_[ 0 ] = UPDFoffset;
	  angle2_[ 0 ] = UPDFangle;
	  f1hx->setParameter( offset2_, angle2_ );
	  f1hx->integrand().upper( UPDFlength ); // x
	  f1hx->upper( FFlength ); // z
	  //	  */
	  /*
	  f1hx = new Xsection::xXsec( &f1h, 6, 8, 5.0E-3 );
	  f1hx->setGngrid( 2 );
	  
	  vector< double > offset2_( 2, FFoffset ); 
	  vector< double > angle2_( 2, FFangle ); 
	  vector< double > length2_( 2, FFlength ); 
	  offset2_[ 0 ] = UPDFoffset;
	  angle2_[ 0 ] = UPDFangle;
	  length2_[ 0 ] = UPDFlength;
	  f1hx->setParameter( offset2_, angle2_ );
	  f1hx->upper( length2_ );
	  */
	}

	vector< double > xx_( 2, 0.0 ), zz_( 2, 0.0 ), mom( 2, 0.0 );
	vector< double > xs_( 1, 0.0 ), zs_( 1, 0.0 );
	if( !args.hasOpt( "xzave" ) ) mom[ 0 ] = mom[ 1 ] = 1.0;

	f1hx->setMomParam( mom );

	for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	  
	  f1.setQ2( q2[ iq2 ] );
	  f1h.setQ2( q2[ iq2 ] );

	  // ---- X distribution ---- //

	  // ---- exp data out ---- //
	  cout << "EXP" << setw(4) << f1.q2() << setw( 4 ) 
	       << Xsection::Charge::tname( Xsection::Charge::TARGET( tk ) ) 
	       << setw( 4 ) << Flavor::name( Flavor::HADRON( hk ) )
	       << setw( 4 ) << "XAV";
	  
	  for( int i = 0; i < xave.size() / 4 ; i++ ){
	    cout << setw(13) << expdata_[ tk ][ hk ][ i ] << flush;
	  }
	  cout << endl;
	  
	  // ---- theory out ---- //
	  cout << "THE" << setw(4) << f1.q2() << setw( 4 ) 
	       << Xsection::Charge::tname( Xsection::Charge::TARGET( tk ) ) 
	       << setw( 4 ) << Flavor::name( Flavor::HADRON( hk ) )
	       << setw( 4 ) << "XAV";

	  for( int iii = 0; iii < 1; iii++ ){ // cache check

	  timer.start();

	  for( int i = 0; i < xave.size() / 4 ; i++ ){	    
	    xx_[ 0 ] = x2[ i ]; xx_[ 1 ] = x1[ i ]; xs_[ 0 ] = xave[ i ];
	    zz_[ 0 ] = z2[ i ]; zz_[ 1 ] = z1[ i ]; zs_[ 0 ] = zave[ i ];

	    cout << setw(13) << ( ! args.hasOpt( "xzave" ) ?
				  (*f1hx)( xx_, zz_ ) / (*f1x)( xx_ ) :
				  (*f1hx)( xs_, zs_ ) / (*f1x)( xs_ ) )
		 << flush;
	  }
	  timer.stop();
	  
	  double msec = timer.check( Timer::Msec );
	  cout << setw(10) << msec / 1000.0 << " sec";
	  cout << endl;      

	  }

	  // for plot //
	  thedata_[ tk ][ hk ] = vector< double >( xave.size(), 0.0 );
	  for( int i = 0; i < xave.size(); i++ ){	    
	    xx_[ 0 ] = x2[ i ]; xx_[ 1 ] = x1[ i ]; xs_[ 0 ] = xave[ i ];
	    zz_[ 0 ] = z2[ i ]; zz_[ 1 ] = z1[ i ]; zs_[ 0 ] = zave[ i ];
	    thedata_[ tk ][ hk ][ i ] =
	      ! args.hasOpt( "xzave" ) ? (*f1hx)( xx_, zz_ ) / (*f1x)( xx_ ) 
	      : (*f1hx)( xs_, zs_ ) / (*f1x)( xs_ );
	  }

	  // ---- Z distribution ---- //

	  // ---- exp data out ---- //
	  cout << "EXP" << setw(4) << f1.q2() << setw( 4 ) 
	       << Xsection::Charge::tname( Xsection::Charge::TARGET( tk ) ) 
	       << setw( 4 ) << Flavor::name( Flavor::HADRON( hk ) )
	       << setw( 4 ) << "ZAV";
	  
	  //	  for( int i = 0; i < zzave.size() ; i++ ){
	  for( int i = 0; i < 9 ; i++ ){
	    cout << setw(13) << expdata_z_[ tk ][ hk ][ i ] << flush;
	  }
	  cout << endl;

	  // ---- theory out ---- //
	  cout << "THE" << setw(4) << f1.q2() << setw( 4 ) 
	       << Xsection::Charge::tname( Xsection::Charge::TARGET( tk ) ) 
	       << setw( 4 ) << Flavor::name( Flavor::HADRON( hk ) )
	       << setw( 4 ) << "ZAV";

	  for( int iii = 0; iii < 1; iii++ ){ // cache check

	  timer.start();

	  //	  for( int i = 0; i < zzave.size() ; i++ ){
	  for( int i = 0; i < 9 ; i++ ){
	    zs_[ 0 ] = zzave[ i ];
	    xs_[ 0 ] = 0.1;

	    cout << setw(13) << 
	      (*f1hx)( xs_, zs_ ) / (*f1x)( xs_ ) << flush;
	  }
	  timer.stop();
	  
	  double msec = timer.check( Timer::Msec );
	  cout << setw(10) << msec / 1000.0 << " sec";
	  cout << endl;      

	  }

	  cout << endl;

	  // for plot //
	  thedata_z_[ tk ][ hk ] = vector< double >( zzave.size(), 0.0 );
	  for( int i = 0; i < zzave.size(); i++ ){	    
	    zs_[ 0 ] = zzave[ i ];
	    xs_[ 0 ] = 0.1;
	    thedata_z_[ tk ][ hk ][ i ] 
	      = (*f1hx)( xs_, zs_ ) / (*f1x)( xs_ );
	  }

	}
      }
    }    
    
  TCanvas* c1 
    = rootout( xave, expdata_, zzave, expdata_z_, thedata_, thedata_z_, q2[ 0 ] );

  if( c1 ){
    string output = "MultiPl_";
    output += ( args.hasOpt( "order" ) ? args.getOpt( "order" ) : "NLO" );
    output += "." + args.get( "format", "png" );
    c1->Print( output.c_str() );
  }

  }
  
  catch ( int error ) {
  }
  
  return 0;
}


TCanvas* rootout( vector< double>& xave, 
		  map< int, map< int, vector< double > > >& expdata_,
		  vector< double>& zzave, 
		  map< int, map< int, vector< double > > >& expdata_z_,
		  map< int, map< int, vector< double > > >& thedata_,
		  map< int, map< int, vector< double > > >& thedata_z_,
		  double q2 ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  TCanvas* c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
  c1->Divide( 2, 2 );
  double x[ 1000 ], y1[ 1000 ], y2[ 1000 ], z1[ 1000 ], z2[ 1000 ];
  char q2chr[5];  sprintf(q2chr,"%1.1f",q2);
  string q2tag = "Q^{2} = " + string(q2chr) + " GeV^{2}";
  
  // M_PIp vs x //
  TVirtualPad *pad1 = c1->cd( 1 );
  gPad->SetLogx(); gPad->SetLogy();
  TH1F* frame = gPad->DrawFrame( 0.015, 0.05, 1.0, 5 );
  frame->SetTitle( "; X_BJ ; Multiplicity" );
  
  TLatex* text = new TLatex;
  text->SetTextSize( 0.05 );
  text->DrawLatex( 0.2, 3.0, q2tag.c_str() );
  text->SetTextSize( 0.10 );
  text->DrawLatex( 0.02, 3.0, "#pi+" );

  for( int i = 0; i < xave.size(); i++ ) x[ i ] = xave[ i ]; 

  for( int i = 0; i < xave.size(); i++ ) y1[ i ] = expdata_[ 0 ][ 1 ][ i ]; 
  for( int i = 0; i < xave.size(); i++ ) y2[ i ] = expdata_[ 2 ][ 1 ][ i ]; 
  TGraph* ln1 = new TGraph( xave.size(), x, y1 );
  TGraph* ln2 = new TGraph( xave.size(), x, y2 );
  ln1->SetMarkerStyle( 20 ); ln2->SetMarkerStyle( 24 );
  ln1->SetMarkerSize( 0.6 ); ln2->SetMarkerSize( 0.3 );
  ln1->SetMarkerColor( 2 );  ln2->SetMarkerColor( 4 );
  ln1->Draw("P"); ln2->Draw("P");

  for( int j = 0; j < 4; j++ ){
  for( int i = 0; i < xave.size() / 4; i++ ) 
    z1[ i ] = thedata_[ 0 ][ 1 ][ i + j * 9 ]; 
  for( int i = 0; i < xave.size() / 4; i++ ) 
    z2[ i ] = thedata_[ 2 ][ 1 ][ i + j * 9 ]; 
  TGraph* tln1 = new TGraph( xave.size() / 4, x, z1 );
  TGraph* tln2 = new TGraph( xave.size() / 4, x, z2 );
  tln1->SetLineColor( 2 );  tln2->SetLineColor( 4 );
  tln1->Draw("L"); tln2->Draw("L");
  }
  pad1->Print( string( "pad1." + args.get( "format", "png" ) ).c_str()  );
  
  // M_PIm vs x //
  TVirtualPad *pad2 = c1->cd( 2 );
  gPad->SetLogx(); gPad->SetLogy();
  frame = gPad->DrawFrame( 0.015, 0.05, 1.0, 5 );
  frame->SetTitle( "; X_BJ ; Multiplicity" );

  text = new TLatex;
  text->SetTextSize( 0.05 );
  text->DrawLatex( 0.2, 3.0, q2tag.c_str() );
  text->SetTextSize( 0.10 );
  text->DrawLatex( 0.02, 3.0, "#pi-" );

  for( int i = 0; i < xave.size(); i++ ) x[ i ] = xave[ i ]; 

  for( int i = 0; i < xave.size(); i++ ) y1[ i ] = expdata_[ 0 ][ -1 ][ i ]; 
  for( int i = 0; i < xave.size(); i++ ) y2[ i ] = expdata_[ 2 ][ -1 ][ i ];   
  TGraph* ln3 = new TGraph( xave.size(), x, y1 );
  TGraph* ln4 = new TGraph( xave.size(), x, y2 );
  ln3->SetMarkerStyle( 20 ); ln4->SetMarkerStyle( 24 );
  ln3->SetMarkerSize( 0.6 ); ln4->SetMarkerSize( 0.3 );
  ln3->SetMarkerColor( 2 );  ln4->SetMarkerColor( 4 );
  ln3->Draw("P"); ln4->Draw("P");

  for( int j = 0; j < 4; j++ ){
  for( int i = 0; i < xave.size() / 4; i++ ) 
    z1[ i ] = thedata_[ 0 ][ -1 ][ i + j * 9 ]; 
  for( int i = 0; i < xave.size() / 4; i++ ) 
    z2[ i ] = thedata_[ 2 ][ -1 ][ i + j * 9 ]; 
  TGraph* tln3 = new TGraph( xave.size() / 4, x, z1 );
  TGraph* tln4 = new TGraph( xave.size() / 4, x, z2 );
  tln3->SetLineColor( 2 );  tln4->SetLineColor( 4 );
  tln3->Draw("L"); tln4->Draw("L");
  }
  pad2->Print( string( "pad2." + args.get( "format", "png" ) ).c_str()  );

  // M_PIp vs z //
  TVirtualPad *pad3 = c1->cd( 3 );
  gPad->SetLogy();
  frame = gPad->DrawFrame( 0.0, 0.01, 1.0, 5 );
  frame->SetTitle( "; Z ; Multiplicity" );

  text = new TLatex;
  text->SetTextSize( 0.05 );
  text->DrawLatex( 0.6, 3.0, q2tag.c_str() );
  text->SetTextSize( 0.10 );
  text->DrawLatex( 0.05, 3.0, "#pi+" );

  for( int i = 0; i < zzave.size(); i++ ) x[ i ] = zzave[ i ]; 

  for( int i = 0; i < zzave.size(); i++ ) y1[ i ] = expdata_z_[ 0 ][ 1 ][ i ]; 
  for( int i = 0; i < zzave.size(); i++ ) y2[ i ] = expdata_z_[ 2 ][ 1 ][ i ];  
  TGraph* lnn1 = new TGraph( zzave.size(), x, y1 );
  TGraph* lnn2 = new TGraph( zzave.size(), x, y2 );
  lnn1->SetMarkerStyle( 20 ); lnn2->SetMarkerStyle( 24 );
  lnn1->SetMarkerSize( 0.6 ); lnn2->SetMarkerSize( 0.3 );
  lnn1->SetMarkerColor( 2 );  lnn2->SetMarkerColor( 4 );
  lnn1->Draw("P"); lnn2->Draw("P");

  for( int i = 0; i < zzave.size(); i++ ) z1[ i ] = thedata_z_[ 0 ][ 1 ][ i ]; 
  for( int i = 0; i < zzave.size(); i++ ) z2[ i ] = thedata_z_[ 2 ][ 1 ][ i ]; 
  TGraph* tlnn1 = new TGraph( zzave.size(), x, z1 );
  TGraph* tlnn2 = new TGraph( zzave.size(), x, z2 );
  tlnn1->SetLineColor( 2 );  tlnn2->SetLineColor( 4 );
  tlnn1->Draw("L"); tlnn2->Draw("L");
  pad3->Print( string( "pad3." + args.get( "format", "png" ) ).c_str()  );

  // M_PIm vs z //
  TVirtualPad *pad4 = c1->cd( 4 );
  gPad->SetLogy();
  frame = gPad->DrawFrame( 0.0, 0.01, 1.0, 5 );
  frame->SetTitle( "; Z ; Multiplicity" );

  text = new TLatex;
  text->SetTextSize( 0.05 );
  text->DrawLatex( 0.6, 3.0, q2tag.c_str() );
  text->SetTextSize( 0.10 );
  text->DrawLatex( 0.05, 3.0, "#pi-" );

  for( int i = 0; i < zzave.size(); i++ ) x[ i ] = zzave[ i ]; 

  for( int i = 0; i < zzave.size(); i++ ) y1[ i ] = expdata_z_[ 0 ][ -1 ][ i ]; 
  for( int i = 0; i < zzave.size(); i++ ) y2[ i ] = expdata_z_[ 2 ][ -1 ][ i ]; 
  TGraph* lnn3 = new TGraph( zzave.size(), x, y1 );
  TGraph* lnn4 = new TGraph( zzave.size(), x, y2 );
  lnn3->SetMarkerStyle( 20 ); lnn4->SetMarkerStyle( 24 );
  lnn3->SetMarkerSize( 0.6 ); lnn4->SetMarkerSize( 0.3 );
  lnn3->SetMarkerColor( 2 );  lnn4->SetMarkerColor( 4 );
  lnn3->Draw("P"); lnn4->Draw("P");


  for( int i = 0; i < zzave.size(); i++ ) z1[ i ] = thedata_z_[ 0 ][ -1 ][ i ]; 
  for( int i = 0; i < zzave.size(); i++ ) z2[ i ] = thedata_z_[ 2 ][ -1 ][ i ]; 
  TGraph* tlnn3 = new TGraph( zzave.size(), x, z1 );
  TGraph* tlnn4 = new TGraph( zzave.size(), x, z2 );
  tlnn3->SetLineColor( 2 );  tlnn4->SetLineColor( 4 );
  tlnn3->Draw("L"); tlnn4->Draw("L");
  pad4->Print( string( "pad4." + args.get( "format", "png" ) ).c_str()  );

  return c1;

}


void loadxzdata( vector< double>& z1, vector< double>& z2, 
	       vector< double>& x1, vector< double>& x2 ){
  z1.resize( 36, 0.0 );   z2.resize( 36, 0.0 );
  x1.resize( 36, 0.0 );   x2.resize( 36, 0.0 );

  for( int i = 0; i < 9; i++ ){ z1[ i ] = 0.250;   z2[ i ] = 0.350; }
  for( int i = 9; i < 18; i++ ){ z1[ i ] = 0.350;   z2[ i ] = 0.450; }
  for( int i = 18; i < 27; i++ ){ z1[ i ] = 0.450;   z2[ i ] = 0.600; }
  for( int i = 27; i < 36; i++ ){ z1[ i ] = 0.600;   z2[ i ] = 0.750; }

  for( int i = 0; i < 4; i++ ){
  x1[ 0 + i * 9 ] = 0.023; x2[ 0 + i * 9 ] = 0.040;
  x1[ 1 + i * 9 ] = 0.040; x2[ 1 + i * 9 ] = 0.055;
  x1[ 2 + i * 9 ] = 0.055; x2[ 2 + i * 9 ] = 0.075;
  x1[ 3 + i * 9 ] = 0.075; x2[ 3 + i * 9 ] = 0.100;
  x1[ 4 + i * 9 ] = 0.100; x2[ 4 + i * 9 ] = 0.140;
  x1[ 5 + i * 9 ] = 0.140; x2[ 5 + i * 9 ] = 0.200;
  x1[ 6 + i * 9 ] = 0.200; x2[ 6 + i * 9 ] = 0.300;
  x1[ 7 + i * 9 ] = 0.300; x2[ 7 + i * 9 ] = 0.400;
  x1[ 8 + i * 9 ] = 0.400; x2[ 8 + i * 9 ] = 0.600;
  }

}


void load_PIp_p_data( vector< double>& pipp, vector< double>& pipper ){
  pipp.resize( 36, 0.0 ); pipper.resize( 36, 0.0 );

  pipp[  0 ] = 1.49200; pipper[  0 ] = 0.01348;
  pipp[  1 ] = 1.38732; pipper[  1 ] = 0.01202;
  pipp[  2 ] = 1.32628; pipper[  2 ] = 0.01110;
  pipp[  3 ] = 1.27172; pipper[  3 ] = 0.01156;
  pipp[  4 ] = 1.29881; pipper[  4 ] = 0.01162;
  pipp[  5 ] = 1.35854; pipper[  5 ] = 0.01408;
  pipp[  6 ] = 1.48381; pipper[  6 ] = 0.01925;
  pipp[  7 ] = 1.69224; pipper[  7 ] = 0.04386;
  pipp[  8 ] = 1.88727; pipper[  8 ] = 0.09581;
  pipp[  9 ] = 0.79400; pipper[  9 ] = 0.01071;
  pipp[ 10 ] = 0.74235; pipper[ 10 ] = 0.00877;
  pipp[ 11 ] = 0.71769; pipper[ 11 ] = 0.00803;
  pipp[ 12 ] = 0.70611; pipper[ 12 ] = 0.00827;
  pipp[ 13 ] = 0.72941; pipper[ 13 ] = 0.00816;
  pipp[ 14 ] = 0.75977; pipper[ 14 ] = 0.00977;
  pipp[ 15 ] = 0.82188; pipper[ 15 ] = 0.01338;
  pipp[ 16 ] = 0.87387; pipper[ 16 ] = 0.03037;
  pipp[ 17 ] = 1.00231; pipper[ 17 ] = 0.06820;
  pipp[ 18 ] = 0.40504; pipper[ 18 ] = 0.00651;
  pipp[ 19 ] = 0.37472; pipper[ 19 ] = 0.00518;
  pipp[ 20 ] = 0.36294; pipper[ 20 ] = 0.00452;
  pipp[ 21 ] = 0.35889; pipper[ 21 ] = 0.00465;
  pipp[ 22 ] = 0.37852; pipper[ 22 ] = 0.00458;
  pipp[ 23 ] = 0.41650; pipper[ 23 ] = 0.00555;
  pipp[ 24 ] = 0.41428; pipper[ 24 ] = 0.00735;
  pipp[ 25 ] = 0.43401; pipper[ 25 ] = 0.01692;
  pipp[ 26 ] = 0.47061; pipper[ 26 ] = 0.03771;
  pipp[ 27 ] = 0.17469; pipper[ 27 ] = 0.00562;
  pipp[ 28 ] = 0.16779; pipper[ 28 ] = 0.00395;
  pipp[ 29 ] = 0.15574; pipper[ 29 ] = 0.00328;
  pipp[ 30 ] = 0.16439; pipper[ 30 ] = 0.00364;
  pipp[ 31 ] = 0.17195; pipper[ 31 ] = 0.00350;
  pipp[ 32 ] = 0.17661; pipper[ 32 ] = 0.00421;
  pipp[ 33 ] = 0.19337; pipper[ 33 ] = 0.00596;
  pipp[ 34 ] = 0.19563; pipper[ 34 ] = 0.01315;
  pipp[ 35 ] = 0.16519; pipper[ 35 ] = 0.02711;

}


void load_PIp_d_data( vector< double>& pipd, vector< double>& pipder ){
  pipd.resize( 36, 0.0 ); pipder.resize( 36, 0.0 );
  //error from pip_p
  pipd[  0 ] = 1.46639; pipder[  0 ] = 0.01348;
  pipd[  1 ] = 1.34613; pipder[  1 ] = 0.01202;
  pipd[  2 ] = 1.26116; pipder[  2 ] = 0.01110;
  pipd[  3 ] = 1.24828; pipder[  3 ] = 0.01156;
  pipd[  4 ] = 1.24172; pipder[  4 ] = 0.01162;
  pipd[  5 ] = 1.27296; pipder[  5 ] = 0.01408;
  pipd[  6 ] = 1.41834; pipder[  6 ] = 0.01925;
  pipd[  7 ] = 1.63812; pipder[  7 ] = 0.04386;
  pipd[  8 ] = 1.74244; pipder[  8 ] = 0.09581;
  pipd[  9 ] = 0.76792; pipder[  9 ] = 0.01071;
  pipd[ 10 ] = 0.70789; pipder[ 10 ] = 0.00877;
  pipd[ 11 ] = 0.68443; pipder[ 11 ] = 0.00803;
  pipd[ 12 ] = 0.67076; pipder[ 12 ] = 0.00827;
  pipd[ 13 ] = 0.70165; pipder[ 13 ] = 0.00816;
  pipd[ 14 ] = 0.73460; pipder[ 14 ] = 0.00977;
  pipd[ 15 ] = 0.78706; pipder[ 15 ] = 0.01338;
  pipd[ 16 ] = 0.80499; pipder[ 16 ] = 0.03037;
  pipd[ 17 ] = 0.91165; pipder[ 17 ] = 0.06820;
  pipd[ 18 ] = 0.37931; pipder[ 18 ] = 0.00651;
  pipd[ 19 ] = 0.36108; pipder[ 19 ] = 0.00518;
  pipd[ 20 ] = 0.34793; pipder[ 20 ] = 0.00452;
  pipd[ 21 ] = 0.35717; pipder[ 21 ] = 0.00465;
  pipd[ 22 ] = 0.36312; pipder[ 22 ] = 0.00458;
  pipd[ 23 ] = 0.38719; pipder[ 23 ] = 0.00555;
  pipd[ 24 ] = 0.38193; pipder[ 24 ] = 0.00735;
  pipd[ 25 ] = 0.43965; pipder[ 25 ] = 0.01692;
  pipd[ 26 ] = 0.39588; pipder[ 26 ] = 0.03771;
  pipd[ 27 ] = 0.17396; pipder[ 27 ] = 0.00562;
  pipd[ 28 ] = 0.16629; pipder[ 28 ] = 0.00395;
  pipd[ 29 ] = 0.14592; pipder[ 29 ] = 0.00328;
  pipd[ 30 ] = 0.15693; pipder[ 30 ] = 0.00364;
  pipd[ 31 ] = 0.15793; pipder[ 31 ] = 0.00350;
  pipd[ 32 ] = 0.16665; pipder[ 32 ] = 0.00421;
  pipd[ 33 ] = 0.18309; pipder[ 33 ] = 0.00596;
  pipd[ 34 ] = 0.17843; pipder[ 34 ] = 0.01315;
  pipd[ 35 ] = 0.21586; pipder[ 35 ] = 0.02711;

}


void load_PIm_p_data( vector< double>& pimp, vector< double>& pimper ){
  pimp.resize( 36, 0.0 ); pimper.resize( 36, 0.0 );
  //error from pip_p
  pimp[  0 ] = 1.24092; pimper[  0 ] = 0.01348;
  pimp[  1 ] = 1.08861; pimper[  1 ] = 0.01202;
  pimp[  2 ] = 1.02109; pimper[  2 ] = 0.01110;
  pimp[  3 ] = 0.94685; pimper[  3 ] = 0.01156;
  pimp[  4 ] = 0.90742; pimper[  4 ] = 0.01162;
  pimp[  5 ] = 0.94299; pimper[  5 ] = 0.01408;
  pimp[  6 ] = 1.00350; pimper[  6 ] = 0.01925;
  pimp[  7 ] = 1.11384; pimper[  7 ] = 0.04386;
  pimp[  8 ] = 1.17399; pimper[  8 ] = 0.09581;
  pimp[  9 ] = 0.59964; pimper[  9 ] = 0.01071;
  pimp[ 10 ] = 0.54546; pimper[ 10 ] = 0.00877;
  pimp[ 11 ] = 0.51639; pimper[ 11 ] = 0.00803;
  pimp[ 12 ] = 0.47920; pimper[ 12 ] = 0.00827;
  pimp[ 13 ] = 0.48959; pimper[ 13 ] = 0.00816;
  pimp[ 14 ] = 0.49382; pimper[ 14 ] = 0.00977;
  pimp[ 15 ] = 0.49865; pimper[ 15 ] = 0.01338;
  pimp[ 16 ] = 0.54825; pimper[ 16 ] = 0.03037;
  pimp[ 17 ] = 0.60024; pimper[ 17 ] = 0.06820;
  pimp[ 18 ] = 0.29726; pimper[ 18 ] = 0.00651;
  pimp[ 19 ] = 0.27374; pimper[ 19 ] = 0.00518;
  pimp[ 20 ] = 0.24982; pimper[ 20 ] = 0.00452;
  pimp[ 21 ] = 0.23580; pimper[ 21 ] = 0.00465;
  pimp[ 22 ] = 0.24432; pimper[ 22 ] = 0.00458;
  pimp[ 23 ] = 0.24334; pimper[ 23 ] = 0.00555;
  pimp[ 24 ] = 0.23758; pimper[ 24 ] = 0.00735;
  pimp[ 25 ] = 0.24444; pimper[ 25 ] = 0.01692;
  pimp[ 26 ] = 0.28221; pimper[ 26 ] = 0.03771;
  pimp[ 27 ] = 0.13328; pimper[ 27 ] = 0.00562;
  pimp[ 28 ] = 0.11470; pimper[ 28 ] = 0.00395;
  pimp[ 29 ] = 0.10249; pimper[ 29 ] = 0.00328;
  pimp[ 30 ] = 0.09689; pimper[ 30 ] = 0.00364;
  pimp[ 31 ] = 0.09570; pimper[ 31 ] = 0.00350;
  pimp[ 32 ] = 0.09562; pimper[ 32 ] = 0.00421;
  pimp[ 33 ] = 0.08950; pimper[ 33 ] = 0.00596;
  pimp[ 34 ] = 0.10694; pimper[ 34 ] = 0.01315;
  pimp[ 35 ] = 0.10718; pimper[ 35 ] = 0.02711;

}


void load_PIm_d_data( vector< double>& pimd, vector< double>& pimder ){
  pimd.resize( 36, 0.0 ); pimder.resize( 36, 0.0 );
  //error from pip_p
  pimd[  0 ] = 1.30204; pimder[  0 ] = 0.01348;
  pimd[  1 ] = 1.17789; pimder[  1 ] = 0.01202;
  pimd[  2 ] = 1.07807; pimder[  2 ] = 0.01110;
  pimd[  3 ] = 1.05730; pimder[  3 ] = 0.01156;
  pimd[  4 ] = 1.02306; pimder[  4 ] = 0.01162;
  pimd[  5 ] = 1.03397; pimder[  5 ] = 0.01408;
  pimd[  6 ] = 1.08715; pimder[  6 ] = 0.01925;
  pimd[  7 ] = 1.14986; pimder[  7 ] = 0.04386;
  pimd[  8 ] = 1.39831; pimder[  8 ] = 0.09581;
  pimd[  9 ] = 0.63943; pimder[  9 ] = 0.01071;
  pimd[ 10 ] = 0.58964; pimder[ 10 ] = 0.00877;
  pimd[ 11 ] = 0.56894; pimder[ 11 ] = 0.00803;
  pimd[ 12 ] = 0.53764; pimder[ 12 ] = 0.00827;
  pimd[ 13 ] = 0.54435; pimder[ 13 ] = 0.00816;
  pimd[ 14 ] = 0.55180; pimder[ 14 ] = 0.00977;
  pimd[ 15 ] = 0.56427; pimder[ 15 ] = 0.01338;
  pimd[ 16 ] = 0.58565; pimder[ 16 ] = 0.03037;
  pimd[ 17 ] = 0.57908; pimder[ 17 ] = 0.06820;
  pimd[ 18 ] = 0.32114; pimder[ 18 ] = 0.00651;
  pimd[ 19 ] = 0.28357; pimder[ 19 ] = 0.00518;
  pimd[ 20 ] = 0.27418; pimder[ 20 ] = 0.00452;
  pimd[ 21 ] = 0.27636; pimder[ 21 ] = 0.00465;
  pimd[ 22 ] = 0.27417; pimder[ 22 ] = 0.00458;
  pimd[ 23 ] = 0.26583; pimder[ 23 ] = 0.00555;
  pimd[ 24 ] = 0.27735; pimder[ 24 ] = 0.00735;
  pimd[ 25 ] = 0.29796; pimder[ 25 ] = 0.01692;
  pimd[ 26 ] = 0.32233; pimder[ 26 ] = 0.03771;
  pimd[ 27 ] = 0.14746; pimder[ 27 ] = 0.00562;
  pimd[ 28 ] = 0.12434; pimder[ 28 ] = 0.00395;
  pimd[ 29 ] = 0.11832; pimder[ 29 ] = 0.00328;
  pimd[ 30 ] = 0.11382; pimder[ 30 ] = 0.00364;
  pimd[ 31 ] = 0.11611; pimder[ 31 ] = 0.00350;
  pimd[ 32 ] = 0.12315; pimder[ 32 ] = 0.00421;
  pimd[ 33 ] = 0.11058; pimder[ 33 ] = 0.00596;
  pimd[ 34 ] = 0.11491; pimder[ 34 ] = 0.01315;
  pimd[ 35 ] = 0.10905; pimder[ 35 ] = 0.02711;

}

void loadxzdata_z( vector< double>& z1, vector< double>& z2 ){
  z1.resize( 15, 0.0 );   z2.resize( 15, 0.0 );

  z1[  0 ] = 0.150; z2[  0 ] = 0.200;
  z1[  1 ] = 0.200; z2[  1 ] = 0.250;
  z1[  2 ] = 0.250; z2[  2 ] = 0.300;
  z1[  3 ] = 0.300; z2[  3 ] = 0.350;
  z1[  4 ] = 0.350; z2[  4 ] = 0.400;
  z1[  5 ] = 0.400; z2[  5 ] = 0.450;
  z1[  6 ] = 0.450; z2[  6 ] = 0.500;
  z1[  7 ] = 0.500; z2[  7 ] = 0.550;
  z1[  8 ] = 0.550; z2[  8 ] = 0.600;
  z1[  9 ] = 0.600; z2[  9 ] = 0.650;
  z1[ 10 ] = 0.650; z2[ 10 ] = 0.700;
  z1[ 11 ] = 0.700; z2[ 11 ] = 0.750;
  z1[ 12 ] = 0.750; z2[ 12 ] = 0.800;
  z1[ 13 ] = 0.800; z2[ 13 ] = 0.850;
  z1[ 14 ] = 0.850; z2[ 14 ] = 0.900;

}

void load_PIp_p_data_z( vector< double>& pipp, vector< double>& pipper ){
  pipp.resize( 15, 0.0 ); pipper.resize( 15, 0.0 );

  pipp[  0 ] = 3.02701;  pipper[  0 ] = 0.01222;
  pipp[  1 ] = 2.13312;	 pipper[  1 ] = 0.00892;
  pipp[  2 ] = 1.56293;	 pipper[  2 ] = 0.00736;
  pipp[  3 ] = 1.16725;	 pipper[  3 ] = 0.00646;
  pipp[  4 ] = 0.87276;	 pipper[  4 ] = 0.00584;
  pipp[  5 ] = 0.65989;	 pipper[  5 ] = 0.00541;
  pipp[  6 ] = 0.50582;	 pipper[  6 ] = 0.00513;
  pipp[  7 ] = 0.38966;	 pipper[  7 ] = 0.00503;
  pipp[  8 ] = 0.32043;	 pipper[  8 ] = 0.00508;
  pipp[  9 ] = 0.23887;	 pipper[  9 ] = 0.00524;
  pipp[ 10 ] = 0.21892;	 pipper[ 10 ] = 0.00571;
  pipp[ 11 ] = 0.16280;	 pipper[ 11 ] = 0.00619;
  pipp[ 12 ] = 0.12421;	 pipper[ 12 ] = 0.00674;
  pipp[ 13 ] = 0.07995;	 pipper[ 13 ] = 0.00676;
  pipp[ 14 ] = 0.06576;	 pipper[ 14 ] = 0.00480;

}

void load_PIp_d_data_z( vector< double>& pipd, vector< double>& pipder ){
  pipd.resize( 15, 0.0 ); pipder.resize( 15, 0.0 );
  //error from pip_p
  pipd[  0 ] = 2.94858;  pipder[  0 ] = 0.01222;
  pipd[  1 ] = 2.07807;	 pipder[  1 ] = 0.00892;
  pipd[  2 ] = 1.49781;	 pipder[  2 ] = 0.00736;
  pipd[  3 ] = 1.11943;	 pipder[  3 ] = 0.00646;
  pipd[  4 ] = 0.82861;	 pipder[  4 ] = 0.00584;
  pipd[  5 ] = 0.62918;	 pipder[  5 ] = 0.00541;
  pipd[  6 ] = 0.48236;	 pipder[  6 ] = 0.00513;
  pipd[  7 ] = 0.37607;	 pipder[  7 ] = 0.00503;
  pipd[  8 ] = 0.29300;	 pipder[  8 ] = 0.00508;
  pipd[  9 ] = 0.24857;	 pipder[  9 ] = 0.00524;
  pipd[ 10 ] = 0.19071;	 pipder[ 10 ] = 0.00571;
  pipd[ 11 ] = 0.15151;	 pipder[ 11 ] = 0.00619;
  pipd[ 12 ] = 0.10953;	 pipder[ 12 ] = 0.00674;
  pipd[ 13 ] = 0.06699;	 pipder[ 13 ] = 0.00676;
  pipd[ 14 ] = 0.06443;	 pipder[ 14 ] = 0.00480;

}

void load_PIm_p_data_z( vector< double>& pimp, vector< double>& pimper ){
  pimp.resize( 15, 0.0 ); pimper.resize( 15, 0.0 );
  //error from pip_p
  pimp[  0 ] = 2.42483;  pimper[  0 ] = 0.01222;
  pimp[  1 ] = 1.69087;	 pimper[  1 ] = 0.00892;
  pimp[  2 ] = 1.16385;	 pimper[  2 ] = 0.00736;
  pimp[  3 ] = 0.84561;	 pimper[  3 ] = 0.00646;
  pimp[  4 ] = 0.60182;	 pimper[  4 ] = 0.00584;
  pimp[  5 ] = 0.44347;	 pimper[  5 ] = 0.00541;
  pimp[  6 ] = 0.33370;	 pimper[  6 ] = 0.00513;
  pimp[  7 ] = 0.25635;	 pimper[  7 ] = 0.00503;
  pimp[  8 ] = 0.19677;	 pimper[  8 ] = 0.00508;
  pimp[  9 ] = 0.15299;	 pimper[  9 ] = 0.00524;
  pimp[ 10 ] = 0.11365;	 pimper[ 10 ] = 0.00571;
  pimp[ 11 ] = 0.08634;	 pimper[ 11 ] = 0.00619;
  pimp[ 12 ] = 0.06420;	 pimper[ 12 ] = 0.00674;
  pimp[ 13 ] = 0.04625;	 pimper[ 13 ] = 0.00676;
  pimp[ 14 ] = 0.02808;	 pimper[ 14 ] = 0.00480;

}

void load_PIm_d_data_z( vector< double>& pimd, vector< double>& pimder ){
  pimd.resize( 15, 0.0 ); pimder.resize( 15, 0.0 );
  //error from pip_p
  pimd[  0 ] = 2.66967;  pimder[  0 ] = 0.01222;
  pimd[  1 ] = 1.82696;	 pimder[  1 ] = 0.00892;
  pimd[  2 ] = 1.26842;	 pimder[  2 ] = 0.00736;
  pimd[  3 ] = 0.92162;	 pimder[  3 ] = 0.00646;
  pimd[  4 ] = 0.66081;	 pimder[  4 ] = 0.00584;
  pimd[  5 ] = 0.48943;	 pimder[  5 ] = 0.00541;
  pimd[  6 ] = 0.37761;	 pimder[  6 ] = 0.00513;
  pimd[  7 ] = 0.28032;	 pimder[  7 ] = 0.00503;
  pimd[  8 ] = 0.21618;	 pimder[  8 ] = 0.00508;
  pimd[  9 ] = 0.17805;	 pimder[  9 ] = 0.00524;
  pimd[ 10 ] = 0.13292;	 pimder[ 10 ] = 0.00571;
  pimd[ 11 ] = 0.10240;	 pimder[ 11 ] = 0.00619;
  pimd[ 12 ] = 0.07601;	 pimder[ 12 ] = 0.00674;
  pimd[ 13 ] = 0.05178;	 pimder[ 13 ] = 0.00676;
  pimd[ 14 ] = 0.03510;	 pimder[ 14 ] = 0.00480;

}
