//
// mrst2001.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Wed Feb  4 16:17:29 2009 Yoshiyuki Miyachi
// Started on  Wed Feb  4 16:17:29 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <sstream>

#include <Utility/Arguments.hh>
#include <PDFLIB/MRST2001.hh>
#include <unpolPDF/Evolution.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TLegend.h>

using namespace std;

void Usage() {

  Utility::Arguments& args = Utility::Arguments::ref();
  
  ARG_LOG << args << endl;

  args.usage( "parton", "gluon", "name of parton to be calculated.");
  args.usage( "Q2", "10.0", "Q^2 (multiple definition with \",\")" );
  args.usage( "x",   "0.1", "x   (multiple definition with \",\")" );

}


int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( args.hasOpt( "help" ) ){
    Usage();
    return 0;
  }
  
  if( ! args.hasOpt( "MRST" ) ){
    
    Usage();

    ARG_ERR << args.command() 
	    << " requires --MRST=MRST2001.xml"
	    << endl;
    
    return 0;
  }
  
  vector< string > parton( 1, "g" );
  parton = args.get( "parton", ",", parton );

  // --- ROOT 
  TApplication app( "MRST2001", &argc, argv );
  //  gROOT->SetStyle( "Plain" );
  gROOT->SetStyle( "Pub" );
  
  PDFLIB::MRST2001 grid;
  unpolPDF::Evo pdf( args );
  
  vector< double > q2( 1, 10.0 );
  q2 = args.get( "Q2", ",", q2 );
  
  vector< double > x( 1, 0.1 );
  x = args.get( "x", ",", x );
  
  if( x.size() == 1 ){
    double lxmin = -2.0;
    double lxmax =  0.0;
    x.clear();
    for( double lx = lxmin; lx < lxmax; lx += 0.1 ){
      x.push_back( pow( 10.0, lx ) );
    }
  }
  
  ARG_LOG << setw(6) << "Q2" << flush;
  for( int ix = 0; ix < x.size(); ix++ ){
    ARG_LOG << setw(12) << x[ ix ] << flush;
  }
  ARG_LOG << endl;
  
  ARG_LOG << setw(6) << "-----" << flush;
  for( int ix = 0; ix < x.size(); ix++ ){
    ARG_LOG << setw(12) << "-----------" << flush;
  }
  ARG_LOG << endl;
  
  TCanvas c( "xPDF" );
  c.SetLogx();
  
  ostringstream ost;
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    ARG_LOG << setw(6) << q2[ iq2 ] << flush;
    
    // -----------------------------------------------------------
    //       Mellin library based calculation
    // -----------------------------------------------------------
    TH1* frame = c.DrawFrame( 0.01, -0.01, 1.0, 1.0 );
    frame->GetXaxis()->SetTitle( "x" );
    ost.str( "" );
    ost << "x q(x, " << q2[ iq2 ] << " GeV^{2})" ;
    frame->GetYaxis()->SetTitle( ost.str().c_str() );
    
    pdf.setQ2( q2[ iq2 ] );
    
    TLegend *legEvo = new TLegend( 0.7, 0.6, 0.88, 0.85 );
    legEvo->SetHeader( "MRST2001 EVO" );
    
    for( int ip = 0; ip < parton.size(); ip++ ){
      
      QCD::Flavor::PARTON pid = 
	(QCD::Flavor::PARTON) QCD::Flavor::id( parton[ ip ] );
      Evolution::PDFx xq( &pdf, pid );
      
      TGraph *gevo = new TGraph( x.size() );
      gevo->SetLineWidth( 2 );
      gevo->SetLineStyle( ( pid < 0 ? 2 : 1 ) );
      gevo->SetLineColor( ( pid < 0 ? pid + 10 : pid ) + 1 );
      
      legEvo->AddEntry( gevo, 
			( pid == 0 ? "g * 0.1" : parton[ip].c_str() ),
			"l" );
      
      for( int ix = 0; ix < x.size(); ix++ ){
	double value =  xq( x[ ix ] );
	gevo->SetPoint( ix, 
			x[ ix ],
			x[ ix ] * value * ( pid == 0 ? 0.1 : 1.0 ) );
	ARG_LOG << setw(12) << value << flush;
      }
      gevo->Draw();
      
    }
    legEvo->Draw();
    ARG_LOG << endl;

    ost.str("");
    ost << "mrst_evo_" <<  q2[ iq2 ] << ".png";
    c.Print( ost.str().c_str() );
    
    
    // ------------------------------------------------------------------
    //                     grid data based calculation
    // ------------------------------------------------------------------
    ARG_LOG << setw(6) << "grid" << flush;
    grid.setQ2( q2[ iq2 ] );

    frame = c.DrawFrame( 0.01, -0.01, 1.0, 1.0 );
    frame->GetXaxis()->SetTitle( "x" );
    ost.str( "" );
    ost << "x q(x, " << q2[ iq2 ] << " GeV^{2}) " ;
    frame->GetYaxis()->SetTitle( ost.str().c_str() );
    
    TLegend *legGri = new TLegend( 0.7, 0.6, 0.88, 0.85 );
    legGri->SetHeader( "MRST2001 Grid" );
    
    for( int ip = 0; ip < parton.size(); ip++ ){
      
      QCD::Flavor::PARTON pid = 
	(QCD::Flavor::PARTON) QCD::Flavor::id( parton[ ip ] );
      grid.setPID( pid );
      
      TGraph *ggri = new TGraph( x.size() );
      ggri->SetLineWidth( 2 );
      ggri->SetLineStyle( ( pid < 0 ? 2 : 1 ) );
      ggri->SetLineColor( ( pid < 0 ? pid + 10 : pid ) + 1 );
      
      legGri->AddEntry( ggri, 
			( pid == 0 ? "g * 0.1" : parton[ip].c_str() ),
			"l" );

      for( int ix = 0; ix < x.size(); ix++ ){
	double value = grid( x[ ix ] );
	ggri->SetPoint( ix,
			x[ ix ], 
			x[ ix ] * value * ( pid == 0 ? 0.1 : 1.0 ) );
	ARG_LOG << setw(12) << value << flush;
      }
      ARG_LOG << endl;
      ARG_LOG << endl;
      
      ggri->Draw();
    }      
    legGri->Draw();
    
    ost.str("");
    ost << "mrst_grid_" <<  q2[ iq2 ] << ".png";
    c.Print( ost.str().c_str() );
    
  }
  
  
  return 0;
}
