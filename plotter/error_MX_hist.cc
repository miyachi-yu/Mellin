#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <Utility/Arguments.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Fragmentation/Evolution.hh>
#include <QCD/ConfigPdfBase.hh>
#include <QCD/PdfParametrization.hh>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TLatex.h>
#include <TPDF.h>
#include <TAxis.h>
#include <TH2.h>
#include <TPostScript.h>
#include <TROOT.h>
#include <TStyle.h>
#include <Rtypes.h>
#include <TLine.h>

using namespace std;

string getname( const string& pname );

int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  TApplication TA( "browser", &argc, argv ); // needed if you use root
  
  // -------------------------------------------   gROOT style setting
  string gstyle = args.get( "gstyle", "Plain" );
  args.log() << "Set gROOT sytle to " << gstyle << endl;
  gROOT->SetStyle( gstyle.c_str() );
  gROOT->SetBatch( Utility::Arguments::ref().hasOpt( "nodisplay" ) );
  
  gStyle->SetOptStat( kFALSE );           //
  gStyle->SetPalette( 1 );                //
  gStyle->SetFrameFillColor( 0 );
  //  gStyle->SetCanvasColor( 5 );        //
  
  // colleration matrix
  vector< vector< double > > x( 0 );      // matrix element
  vector< string > id( 2, "" );           // matrix id
  vector< string > para( 0 );             // label information
  
  // -----------------------------------------------------------------
  //    An examplel; how to get error matrix from xml file
  // -----------------------------------------------------------------
  if( args.hasOpt( "pdf" ) ){
    
    // create config file handling object
    QCD::ConfigPdfBase config;
    
    // load pdf/ff XML file
    config.parse( args.getOpt( "pdf" ) );
    config.loadRootNode();
    
    // create pdf/ff object
    QCD::PdfParametrization pdf( config );
    
    // create error matrix object
    QCD::PdfErrMatrix em( &pdf );
    
    // if there is no error matrix, do something
    if( em.size() == 0 ){
      args.err() << "Number of error matrix element is 0." << endl;
    }
    
    // normalize error matrix
    args.log() << "get normalized error matrix" << endl;
    x = em.normalized();
    
    // get labels
    para = em.info();

  }
  
  // -----------------------------------  read from plain ascii file
  if( args.hasOpt( "errorMX" ) ){
    
    //  name of input errorMX
    string errorMX = args.get( "errorMX", "" );
    if( errorMX.size() == 0 ){ 
      cerr << "errorMX must be set" << endl; 
      return 1; 
    }
    
    //  number of parameter
    int n_p = 0; 
    if( ! n_p ){
      ifstream tmpf_( errorMX.c_str() );
      string str( "" );
      getline( tmpf_, str ); getline( tmpf_, str );
      istringstream ist( str );
      while( ist >> str ) ++n_p; --n_p;
    }
    
    // resize to the number of parameter
    x.resize( n_p, vector< double >( n_p, 0.0 ) );
    para.resize( n_p, "" );

    // input data
    ifstream data( errorMX.c_str() );
    //  ifstream data("/home/ykobayashi/QCD/error/histogram_of_ERRMX/data.dat");
    
    for( int i = 0; i < 2; i++ )  data >> id[ i ];         // read id
    cout << id[ 0 ] << endl << id[ 1 ] << endl;

    string str( "" );
    getline( data, str ); getline( data, str ); //shift to matrix row
    
    for( int i = 0; i < n_p; i++ ){
      getline( data, str ); istringstream ist( str );
      ist >> para[ i ];
      for( int j = 0; j < n_p; j++ ) ist >> x[ i ][ j ];
    }
    
  }
  
  int dim = x.size();
  if( dim == 0 ){
    args.err() << "No error matrix found" << endl;
    return 1;
  }
  
  // -----------------------------------------------  main job
  args.log() 
    << "normalized error matrix M(" 
    << x.size()  << ", " << x[0].size() << ") is ready. \nCreate histogram." << endl;
  
  // ------------------------------------- color of matrix elements
  int posiFC = args.get( "posiFC", kRed );     // positive correlation
  int negaFC = args.get( "negaFC", kBlue );    // negative correlation
  int diagFC = args.get( "diagFC", kBlack );    // diagonal element

  int posiFS = args.get( "posiFS", 1 );     // positive correlation
  int negaFS = args.get( "negaFS", 1 );    // negative correlation
  int diagFS = args.get( "diagFS", 0 );    // diagonal element

  int posiLW = args.get( "posiLW", 1 );     // positive correlation
  int negaLW = args.get( "negaLW", 1 );    // negative correlation
  int diagLW = args.get( "diagLW", 1 );    // diagonal element
  
  // --------------------------------------------- canvas preparation
  // set Canvas
  TCanvas* c1_ = new TCanvas( "c1", "My Canvas", 10, 10, 800, 1000 );
  if( c1_ == NULL ){
    args.err() << "fail on creating TCanvas object" << endl;
    return 1;
  }
  c1_->SetGrid();
  
  // --------------------------------------------- histograms
  // for positive entry
  TH2I* h2_0_1_ = new TH2I( "h201", "", dim, 0, dim, dim, 0, dim );
  TH2I* h2_0_2_ = new TH2I( "h202", "", dim, 0, dim, dim, 0, dim );
  if( ! h2_0_1_ || ! h2_0_2_ ){ 
    args.err() << "fail on creating histogram for positive elements" << endl;
    return 1;
  }
  h2_0_1_->SetFillStyle( posiFS );
  h2_0_1_->SetFillColor( posiFC );
  h2_0_2_->SetFillStyle( 0 );
  h2_0_2_->SetLineColor( posiFC );
  h2_0_2_->SetLineWidth( posiLW );
  
  // for negative entry
  TH2I* h2_1_1_ = new TH2I( "h211", "", dim, 0, dim, dim, 0, dim );
  TH2I* h2_1_2_ = new TH2I( "h212", "", dim, 0, dim, dim, 0, dim );
  if( ! h2_1_1_ || ! h2_1_2_ ){ 
    args.err() << "fail on creating histogram for natative elements" << endl;
    return 1;
  }
  h2_1_1_->SetFillStyle( negaFS );
  h2_1_1_->SetFillColor( negaFC );
  h2_1_2_->SetFillStyle( 0 );
  h2_1_2_->SetLineColor( negaFC );  
  h2_1_2_->SetLineWidth( negaLW );  

  // diagonal
  TH2I* h2_2_ = new TH2I( "h22", "", dim, 0, dim, dim, 0, dim );
  if( ! h2_2_ ){ 
    args.err() << "fail on creating histogram for diagonal elements" << endl;
    return 1;
  }
  h2_2_->SetFillColor( diagFC );
  h2_2_->SetFillStyle( diagFS );
  h2_2_->SetLineColor( diagFC );
  h2_2_->SetLineWidth( diagLW );

  //  -------------------------------------------- fill histograms
  args.log() << "fill histograms with normalized error matrix" << endl;
  
  for( int i = 0; i < dim; i++ ){
    for(int j = i ; j < dim; j++ ){
      double w = 10000 * x[ i ][ j ];
      if( i == j )  h2_2_->Fill( i, dim - 1 - i, w );     // diagonal
      else {
	if( w < 0 ){ 
	  h2_1_1_->Fill( i, dim - 1 - j, - w );   // negative
	  h2_1_2_->Fill( i, dim - 1 - j, - w );   // negative
	} else { 
	  h2_0_1_->Fill( i, dim - 1 - j,   w );   // positive
	  h2_0_2_->Fill( i, dim - 1 - j,   w );   // positive
	}
      }
    }
  }
  
  // --------------------------------------------- draw histogram
  h2_0_1_->Draw( "Box" );
  h2_0_2_->Draw( "BoxSame" );
  h2_1_1_->Draw( "BoxSame" );
  h2_1_2_->Draw( "BoxSame" );
  h2_2_->Draw( "BoxSame" );
  
  // --------------------------------------------- grid
  vector< double > grid( 0 );
  grid = args.get( "grid", ",", grid );
  if( grid.size() > 0 ){
       TLine line;
       line.SetLineWidth( 3 );
       line.SetLineStyle( 3 );
       
    for( int i = 0; i < grid.size(); i++ ){
	 line.DrawLine( 0, grid[ i ], dim, grid[ i ] );
	 line.DrawLine( grid[ i ], 0, grid[ i ], dim );
    }    
  }

  //  -------------------------------------------- configure labels
  vector< TLine* > vl_( 0 ); // border lines
  vector< int > fline_ = args.get( "fline", ",", vector< int >( 0 ) );

  //  x axis
  TAxis* tx = h2_0_1_->GetXaxis();
  tx->SetLabelColor( 1 );
  tx->SetTickLength( 0.0 );
  tx->SetLabelSize( args.get( "xlabsize", 0.025 ) );

  for( int i = 0; i < dim ; i++ ) {
    tx->SetBinLabel( i + 1, getname( para[ i ] ).c_str() );
    if( para[ i ].find( "N" ) != para[ i ].npos && i )
      vl_.push_back( new TLine( i, 0, i, dim + 1 - i ) );
    for( int j = 0; j < fline_.size(); j++ ) if( i + 1 == fline_[ j ] )
      vl_.push_back( new TLine( i, 0, i, dim + 1 - i ) );
  }

  //  y axis
  TAxis* ty = h2_0_1_->GetYaxis();
  ty->SetLabelColor( 1 );
  ty->SetTickLength( 0.0 );
  ty->SetLabelSize( args.get( "ylabsize", 0.025 ) );

  for( int i = 0; i < dim ; i++ ){
    ty->SetBinLabel( dim - i, getname( para[ i ] ).c_str() );
    if( para[ i ].find( "N" ) != para[ i ].npos && i )
      vl_.push_back( new TLine( 0, dim - i, 1 + i, dim - i ) );
    for( int j = 0; j < fline_.size(); j++ ) if( i + 1 == fline_[ j ] )
      vl_.push_back( new TLine( 0, dim - i, 1 + i, dim - i ) );
  }

  for( int i = 0; i < vl_.size(); i++ ){
    vl_[ i ]->SetLineColor( args.get( "borderLC", 1 ) );
    vl_[ i ]->SetLineStyle( args.get( "borderLS", 1 ) );
    vl_[ i ]->SetLineWidth( args.get( "borderLW", 3 ) );
    vl_[ i ]->Draw();
  }

   // --------------------------------------------- output
  string tmp_out = args.get( "errorMX", "" );
  string out = args.get( "ofile", ( args.hasOpt( "errorMX" ) ? 
				    ( tmp_out.substr( 0, tmp_out.size() - 4 ) + "_out." ) 
				    : "correlation_hist." ) );
  c1_->Update();
  vector< string > suff_ = args.get( "format", ",", vector< string >( 1, "png")  );
  for( int i = 0; i < suff_.size(); i++ )
    c1_->Print( ( out + suff_[ i ] ).c_str() );

  if( args.hasOpt( "wait" ) ) c1_->WaitPrimitive();

  for( int i = 0; i < vl_.size(); i++ )
    if( vl_[ i ] ) delete vl_[ i ];

  return 0;
}

string getname( const string& name ){

  map< string, string > parmap_;
  parmap_[ "N" ] = "eta";
  parmap_[ "a" ] = "alpha";
  parmap_[ "b" ] = "beta";
  parmap_[ "c" ] = "gamma";
  parmap_[ "d" ] = "delta";
  parmap_[ "e" ] = "epsilon";
  parmap_[ "f" ] = "kappa";

  int posi( 0 );

  for( map< string, string >::iterator itr = parmap_.begin(); 
       itr != parmap_.end(); itr++ )
    if( ( posi = name.find( "-" + itr->first ) ) != name.npos ){
      ostringstream ost; 
      ost << "#" << parmap_[ itr->first ]
	  << "_{" << name.substr( 0, posi ) << "}";
      return ost.str(); 
    }

  return name;
}
