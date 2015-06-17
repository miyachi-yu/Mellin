// $Id: contour_plot.cc,v 1.5 2009/01/16 04:24:11 miyachi Exp $
/*!
  \brief   example ROOT macro for making contour plots from Fitting output
  
  Fittint::contour() method create ROOT file which contains 2-D histograms
  for contour plots for the fitted parameters. One can initiate the method
  via testFragXsecFit, testG1fit with --contour=[filename] option.

  \par
  This is an example ROOT macro to create one PDF file where the created
  contour plots are drawn.

  \par
  $ root -l 'contour_plot("filename")'

 */
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

#include <TObjArray.h>
#include <TPDF.h>
#include <TH2.h>

string hname( int par1, int par2 ){
  ostringstream ostr;
  ostr << "corr" << par1 << "-" << par2 ;
  return ostr.str();
}


int contour_plot( string path = "contour.root", int nstart = 0, int nend = 0,
		  string format = "png" ){
  
  gROOT->SetStyle("Plain");
  
  TCanvas *c = new TCanvas( "contour" );
  TFile tfile( path.c_str() );
  
  TPDF pdf( "contour.pdf" );
  
  int max = 20;
  
  TObjArray coll;
  
  vector< string > xtitle;
  vector< string > ytitle;
  
  for( int i = 0; i < max; i++ ){
    for( int j = i + 1; j < max; j++ ){
      
      string name = hname( i, j );
      TH2* h2 = (TH2*) tfile.Get( name.c_str() ); 
      
      if( h2 ){
	
	if( xtitle.size() == 0 ||
	    xtitle[ xtitle.size() - 1 ] != h2->GetXaxis()->GetTitle() ){
	  xtitle.push_back( h2->GetXaxis()->GetTitle() );
	  cout << "X title: " <<  h2->GetXaxis()->GetTitle() << endl;
	}
	
	if( ytitle.size() == 0 ||
	    ytitle[ ytitle.size() - 1 ] != h2->GetYaxis()->GetTitle() ){
	  ytitle.push_back( h2->GetYaxis()->GetTitle() );
	  cout << "Y title: " <<  h2->GetYaxis()->GetTitle() << endl;
	}
	
	coll.Add( h2 );
	
	h2->SetStats( kFALSE );
	double minimum = h2->GetMinimum();
	h2->SetContour( 20 );
	double lstep = 0.5; 
	for( int il = 0; il < 20; il++ ) {
	  h2->SetContourLevel( il, minimum + pow( lstep * il, 2 ) );
	}
	
	h2->Draw("CONT1");
	//      	h2->Draw("CONT");
	c->Update();
	
	/*
	  ostringstream ostr;
	  ostr << "c_" << i << "_" << j << ".png";
	  c->Print( ostr.str().c_str() );
	*/
      }
    }
  }
  
  int n = 
    static_cast< int >( 0.5 * ( sqrt( 8.0 * coll.GetEntries() + 1 ) - 1.0 ) );
  
  if( nend == 0 ) nend = n - nstart;
  
  cout << "NUmber of histograms:       " << coll.GetEntries() << endl;
  cout << "Number of free parameter is " << n << endl;
  cout << "colleration will be plotted from parameter " << nstart 
       << " for " << nend << " parameters" << endl;
  
  TCanvas *ct = new TCanvas( "all" );
  
  ct->Divide( nend + 1, nend + 1, 0.0, 0.0 );

  TLatex latex;
  latex.SetTextSize( 0.2 );
  latex.SetTextAlign( 22 );
  
  for( int i = nstart; i < nend + 1; i++ ){
    TVirtualPad *pad = ct->cd( ( i - nstart ) * ( nend + 1 ) + i + 1 - nstart );
    TH1 *frame = pad->DrawFrame( 0, 0, 10, 10 );
    
    //    frame->GetXaxis()->SetAxisColor( 10 );
    //    frame->GetYaxis()->SetAxisColor( 10 );

    frame->GetXaxis()->SetLabelColor( 10 );
    frame->GetYaxis()->SetLabelColor( 10 );

    frame->GetXaxis()->SetTitleColor( 10 );
    frame->GetYaxis()->SetTitleColor( 10 );

    /*
      ostringstream ostr;
      ostr << "P" << i + 1 ;
      latex.DrawLatex( 5.0, 7.0, ostr.str().c_str() );
    */
    
    if( i < nend ){
      latex.DrawLatex( 5.0, 1.5, xtitle[ i ].c_str() );
    }
    
    if( i > 0 && i - 1 < ytitle.size() ){
      latex.SetTextAngle( 90.0 );
      latex.DrawLatex( 1.0, 5.0, ytitle[ i - 1 ].c_str() );
      latex.SetTextAngle( 0.0 );
    }
  }
  
  int index = 0;
  for( int i = nstart; i < nend; i++ ){
    for( int j = i; j < nend; j++ ){
      int ipad =  ( j + 1 ) * ( nend + 1 ) + ( i - nstart ) + 1;
      TVirtualPad *pad = ct->cd( ipad );
      //      pad->DrawFrame( 0.0, 0.0, 1.0, 1.0 );
      if( coll[ index ] ) {

	TH2* h2 = dynamic_cast< TH2* >( coll[ index] );
	
	h2->SetTitle( "" );

	h2->GetXaxis()->SetLabelColor( 10 );
	h2->GetYaxis()->SetLabelColor( 10 );

	h2->GetXaxis()->SetTitleColor( 10 );
	h2->GetYaxis()->SetTitleColor( 10 );

	h2->Draw( "CONT1" );
      }
      index++;
    }
  }
  
  ct->Draw();

  ct->Print( string( "contour_all." + format ).c_str() );
  
  pdf.Close();
  return 0;
}
