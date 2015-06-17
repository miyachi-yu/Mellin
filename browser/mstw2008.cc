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
#include <PDFLIB/MSTW2008.hh>
#include <unpolPDF/Evolution.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TLegend.h>

#include "Colors.hh"

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
  
  if( ! args.hasOpt( "MSTW" ) ){
    
    Usage();

    ARG_ERR << args.command() 
	    << " requires --MSTW=MSTW2008.xml"
	    << endl;
    
    return 0;
  }
  
  vector< string > parton( 1, "g" );
  parton = args.get( "parton", ",", parton );
  
  // --- ROOT 
  TApplication app( "MSTW2008", &argc, argv );
  //  gROOT->SetStyle( "Plain" );
  gROOT->SetStyle( "Pub" );
  
  PDFLIB::MSTW2008 grid;
  unpolPDF::Evo pdf( args );
  
  vector< double > q2( 1, 10.0 );
  q2 = args.get( "Q2", ",", q2 );
  
  vector< double > x( 1, 0.1 );
  x = args.get( "x", ",", x );

  

  if( x.size() == 1 ){
    double lxmin = args.get( "lxmin", -2.0 );
    double lxmax = args.get( "lxmax",  0.0 );
    x.clear();
    for( double lx = lxmin; lx < lxmax; lx += 0.1 ){
      x.push_back( pow( 10.0, lx ) );
    }
  }

  vector< double > y( 2, 0.0 );
  y[0] = args.get( "ymin", 0.0 ); 
  y[1] = args.get( "ymax", 1.0 ); 
  
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
    TH1* frame = c.DrawFrame( x[0], y[0], x.back(), y[1] );
    frame->GetXaxis()->SetTitle( "x" );
    ost.str( "" );
    ost << "x q(x, " << q2[ iq2 ] << " GeV^{2})" ;
    frame->GetYaxis()->SetTitle( ost.str().c_str() );
    
    pdf.setQ2( q2[ iq2 ] );
    
    cout << pdf << endl;

    TLegend *legEvo = new TLegend( 0.6, 0.6, 0.88, 0.85 );
    legEvo->SetHeader( "MSTW2008 EVO" );
    
    for( int ip = 0; ip < parton.size(); ip++ ){

      int pid = 
	QCD::Flavor::id( parton[ ip ] );
      
      Evolution::PDFx *ptrQ = 
	( QCD::Flavor::isParton( parton[ ip ] ) ?
	  new Evolution::PDFx( &pdf, (QCD::Flavor::PARTON) pid, 6, 8, 0.001, 15, 3.0, 0.55 ) :
	  new Evolution::PDFx( &pdf, (QCD::Flavor::TYPE)   pid, 6, 8, 0.001, 15, 3.0, 0.55 ) );
      
      Evolution::PDFx& xq = *ptrQ;
      
      TGraph *gevo = new TGraph( x.size() );
      gevo->SetLineWidth( 2 );
      gevo->SetLineStyle( ( pid < 0 ? 2 : 1 ) );
      gevo->SetLineColor( Colors::id( (QCD::Flavor::PARTON) pid ) );
      
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

      delete ptrQ;
    }
    legEvo->Draw();
    ARG_LOG << endl;

    ost.str("");
    ost << "mstw_evo_" <<  q2[ iq2 ] << ".png";
    c.Print( ost.str().c_str() );
    
    
    // ------------------------------------------------------------------
    //                     grid data based calculation
    // ------------------------------------------------------------------
    ARG_LOG << setw(6) << "grid" << flush;
    grid.setQ2( q2[ iq2 ] );

    frame = c.DrawFrame( x[0], y[0], x.back(), y[1] );
    frame->GetXaxis()->SetTitle( "x" );
    ost.str( "" );
    ost << "x q(x, " << q2[ iq2 ] << " GeV^{2}) " ;
    frame->GetYaxis()->SetTitle( ost.str().c_str() );
    
    TLegend *legGri = new TLegend( 0.6, 0.6, 0.88, 0.85 );
    legGri->SetHeader( "MSTW2008 Grid" );
    
    for( int ip = 0; ip < parton.size(); ip++ ){
      
      int pid = QCD::Flavor::id( parton[ ip ] );
      if( QCD::Flavor::isParton( parton[ ip ] ) ){
	grid.setPID( (QCD::Flavor::PARTON) pid );
      } else {
	grid.setPID( (QCD::Flavor::TYPE) pid );
      }

      TGraph *ggri = new TGraph( x.size() );
      ggri->SetLineWidth( 2 );
      ggri->SetLineStyle( ( pid < 0 ? 2 : 1 ) );
      ggri->SetLineColor( Colors::id( (QCD::Flavor::PARTON) pid ) );

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
    ost << "mstw_grid_" <<  q2[ iq2 ] << ".png";
    c.Print( ost.str().c_str() );
    
  }
  
  
  return 0;
}
