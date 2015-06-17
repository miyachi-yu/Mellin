#include <Utility/Arguments.hh>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#include <TGraph.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TSystem.h>

using namespace std;

string VBP[ 3 ] = {
  "WP", "WM", "Z0"
};

int main( int argc, char* argv[] ){

  Utility::Arguments& args = *( Utility::Arguments::instance( argc, argv ) );
  TApplication TA( "browser", &argc, argv ); //needed if you use root

  try {

    gROOT->SetStyle( "Plain" );

    for( int i = 0; i < 3; i++ ){

      string file = args.get( "dir", "./") + VBP[ i ] + ".dat";
	
      ifstream ifs( file.c_str() );
      string str;

      vector< double > x_( 0 );
      vector< vector< double > > y_( 0 );
    
      while( getline( ifs, str ) ){

	double tmpvalue;
	vector< double > tmpvec( 0 );
	istringstream iss( str );

      	while( iss >> tmpvalue ){
	    tmpvec.push_back( tmpvalue );
	}
	
	if( ! y_.size() ) y_.resize( tmpvec.size() - 1, x_ );

	/*	
	for( int j = 0; j < tmpvec.size(); j++ )
	  cout << tmpvec[ j ] << flush;
	cout << endl;
	*/

	x_.push_back( tmpvec[ 0 ] );
	for( int ii = 0; ii < y_.size(); ii++ )
	  y_[ ii ].push_back( tmpvec[ ii + 1 ] );
	
      }

      // plot x_ & y_
      double xx_[ x_.size() ], yy_[ x_.size() ];

      // y upol DSSV BB d u g ubar dbar
      //      for( int j = 0; j < y_.size(); j++ ){
      for( int j = 1; j < 2; j++ ){
      
	for( int jj = 0; jj < x_.size() ; jj++ ){
	  xx_[ jj ] = x_[ jj ];
	  yy_[ jj ] = y_[ j ][ jj ] / y_[ 0 ][ jj ]; //Asymmetry
	}

	// TCanvas handling //
	TCanvas* c = new TCanvas( "TMP" );

	// Frame handling //
	TH1F* frame = c->DrawFrame( -1.5, -0.1, 1.5, 0.1 );

	frame->GetXaxis()->SetTitle( "y_{W}" );      
	frame->GetYaxis()->SetTitle( "#frac{d #Delta #sigma}{d y_w}/#frac{d #sigma}{d y_w}" );

	/*
	// TLegend handling //
	TLegend* l = new TLegend( 0.6, 0.6, 0.86, 0.86 );
	l->SetTextSize( 0.03 );
	if( ! args.hasOpt( "sum" ) ){
	  ostringstream ost;
	  ost << " Q^{2} = " << Q2_ << " GeV^{2}";
	  l->SetHeader( ost.str().c_str() );
	}
	for( int i = 0; i < vvmycalc.size(); i++ )
	  l->AddEntry( vvmycalc[ i ][ 0 ], vvtitlename[ i ].c_str(), "l" );
	if( l ) l->Draw();
	*/
	
	TGraph* gh = new TGraph( x_.size(), xx_, yy_ );
	gh->Draw();
	
	// Output process //
	if( c ) {
	  c->Draw();
	  
	  string dirname_ = "png";
	  gSystem->mkdir( dirname_.c_str() );
	  ostringstream oss;
	  oss << dirname_ << "/" << "y_" << j << ".png";
	  c->Print( oss.str().c_str() );
	}

      }

    }

  }

  catch( int err ){
  }

  return 0;
}
