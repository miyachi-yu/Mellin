#ifndef _plotter_FFset_hh_
#define _plotter_FFset_hh_

#include "FF.hh"

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <vector>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TSystem.h>
#include <cmath>

using namespace std;
using namespace Utility;
using namespace QCD;

class FFset {
  string canvastitle_;
  string xval_ ;
  string pdfname_;
  string hadronname_;
  string order_;

  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;

  vector< string > vtitlename; // names for each data file
  vector< FF* > vmycalc; // each data file

  vector< vector< TGraph* > > vvmycalc; // TGraphs in each file
  vector< int > vflv;

public:
 FFset() : vtitlename( 0 ), vmycalc( 0 ), vvmycalc( 0 ), vflv( 0 ) {};

  virtual ~FFset(){ 
    for( int i = 0; i < vmycalc.size(); i++ ) delete vmycalc[ i ];
  };

  void insertdata( string mycalc_dir, string titlename = "" ){
    vtitlename.push_back( titlename == "" ? mycalc_dir : titlename );
    vmycalc.push_back( new FF( mycalc_dir ) );
  }
  
  void setTGraph_ratio( const double& Q2, const int& rflav, const int& bflav ){

    Utility::Arguments& args = Utility::Arguments::ref();

    if( vmycalc.size() != 1 ) return;

    vector< TGraph* > vmycalcc_base = vmycalc[ 0 ]->getData( Q2, bflav, 1.0 );
    if( vmycalcc_base.size() == 0 )  return;

    vector< TGraph* > vmycalcc_nume = vmycalc[ 0 ]->getData( Q2, rflav, 1.0 );
    if( vmycalcc_nume.size() == 0 )  return;

    double* xx_base( vmycalcc_base[ 0 ]->GetX() );
    double* yy_base( vmycalcc_base[ 0 ]->GetY() );
    double* xx_nume( vmycalcc_nume[ 0 ]->GetX() );
    double* yy_nume( vmycalcc_nume[ 0 ]->GetY() );

    vector< double > vx_( vmycalcc_base[ 0 ]->GetN(), 0.0 ); 
    vector< double > vy_( vx_.size(), 0.0 );
    
    for( int i = 0; i < vmycalcc_base[ 0 ]->GetN(); i++ ){

      if( *xx_base == *xx_nume ){
	vx_[ i ] = *xx_base;
	vy_[ i ] = *yy_nume / *yy_base;
      }

      xx_base++; yy_base++;
      xx_nume++; yy_nume++;
    }

    vector< TGraph* > vmycalcc( 1, new TGraph( vmycalcc_base[ 0 ]->GetN(), &*vx_.begin(),
					       &*vy_.begin() ) );

    for( int i = 0; i < vmycalcc_base.size(); i++ )
      if( vmycalcc_base[ i ] ) delete vmycalcc_base[ i ];
    for( int i = 0; i < vmycalcc_nume.size(); i++ )
      if( vmycalcc_nume[ i ] ) delete vmycalcc_nume[ i ];

    for( int j = 0; j < vmycalcc.size(); j++ ){
      vmycalcc[ j ]
	->SetLineColor( Flavor::color( Flavor::PARTON( rflav ) ) );
      vmycalcc[ j ]
	->SetLineStyle( Flavor::style( Flavor::PARTON( rflav ) ) );

      vmycalcc[ j ]->SetLineWidth( 2 );

      ostringstream oss_cl_; oss_cl_ << "setLW" << 0 + 1;// vmycalc id: i = 0 in i + 1
      if( args.hasOpt( oss_cl_.str().c_str() ) )
	vmycalcc[ j ]->SetLineWidth( args.get( oss_cl_.str().c_str(), 2 ) );
    }

    vvmycalc.push_back( vmycalcc );
    vflv.push_back( rflav );

  }

  void setTGraph( double Q2, int iFlav, bool sum = false, double scale = 1.0 ){

    Utility::Arguments& args = Utility::Arguments::ref();

    // TGraph handling //
    for( int i = 0; i < vmycalc.size(); i++ ){
      vector< TGraph* > vmycalcc = vmycalc[ i ]->getData( Q2, iFlav, scale );
      if( vmycalcc.size() == 0 )  break; // return; // throw( 1 );

      for( int j = 0; j < vmycalcc.size(); j++ ){

	if( ! sum ){
	  if( args.hasOpt( "wyellow" ) ){
	    vmycalcc[ j ]->SetLineColor( 2 + i );
	  } else {
	    vmycalcc[ j ]->SetLineColor( 2 + i != 5 ? 2 + i : 1 );

	    ostringstream oss_cl_; oss_cl_ << "setLC" << i + 1;
	    if( args.hasOpt( oss_cl_.str().c_str() ) )
	      vmycalcc[ j ]
		->SetLineColor( args.get( oss_cl_.str().c_str(), 0 ) );
	  }

	  vmycalcc[ j ]->SetLineStyle( j == 0 ? 1 : 3 );

	  ostringstream oss_cl_; oss_cl_ << "setLS" << i + 1;
	  if( args.hasOpt( oss_cl_.str().c_str() ) )
	    vmycalcc[ j ]
	      ->SetLineStyle( args.get( oss_cl_.str().c_str(), 1 ) );

	} else {
	  /*
	  vmycalcc[ j ]->SetLineColor( iFlav + 3 != 5 ? iFlav + 3 : 1 );
	  */
	  vmycalcc[ j ]
	    ->SetLineColor( Flavor::color( Flavor::PARTON( iFlav ) ) );
	  vmycalcc[ j ]
	    ->SetLineStyle( Flavor::style( Flavor::PARTON( iFlav ) ) );
	  /*
	  ( iFlav == 0 ? vmycalcc[ j ]->SetLineColor( 1 ): 
	    ( iFlav + 3 < 5 ? vmycalcc[ j ]->SetLineColor( iFlav + 3 ):
	      vmycalcc[ j ]->SetLineColor( iFlav + 4 ) ) );
	  */
	  //	  vmycalcc[ j ]->SetLineStyle( j == 0 ? 1 : 3 );
	}

	vmycalcc[ j ]->SetLineWidth( 2 );

	ostringstream oss_cl_; oss_cl_ << "setLW" << i + 1;
	if( args.hasOpt( oss_cl_.str().c_str() ) )
	  vmycalcc[ j ]->SetLineWidth( args.get( oss_cl_.str().c_str(), 2 ) );
      }

      vvmycalc.push_back( vmycalcc );
      vflv.push_back( iFlav );
    }
  }

  void ratio( const double& Q2, const vector< int >& rflav, const int& bflav ) 
    throw( int ){

    if( ! Utility::Arguments::instance() ) throw( 1 );
    Utility::Arguments& args = Utility::Arguments::ref();

    for( int i = 0; i < rflav.size(); i++ ){
      this->setTGraph_ratio( Q2, rflav[ i ], bflav );
    }

    if( vvmycalc.size() == 0 ) throw 100;

    this->setstrings();

    // TCanvas handling //
    TCanvas* c = new TCanvas( canvastitle_.c_str() );
    if( ! args.hasOpt( "linear" ) ) c->SetLogx();
    if( args.hasOpt( "leftmarg" ) )
      c->SetLeftMargin( args.get( "leftmarg", 0.20 ) );
    if( args.hasOpt( "frameWidth") )
      c->SetFrameLineWidth( args.get( "frameWidth", 1 ) );

    // Frame handling //
    for( int j = 0; j < vvmycalc.size(); j++ ){
      //      if( vflv[ j ] == ( Q2 >= 10.0 ? 0 : 1 ) ){
      this->setframeparam( vvmycalc[ j ][ 0 ], args.get( "xmin", 0.001 ) );
      break;
    }
    TH1F* frame = c->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    
    if( ! args.hasOpt( "noxtitle" ) )
      frame->GetXaxis()->SetTitle( xval_.c_str() );
    if( args.hasOpt( "xlabelsize" ) )
      frame->GetXaxis()->SetLabelSize( args.get( "xlabelsize", 0.04 ) );
    if( args.hasOpt( "noxlabel" ) )
      frame->GetXaxis()->SetLabelColor( 0 );

    if( args.hasOpt( "setxTick" ) )
      frame->GetXaxis()->SetTickLength( args.get( "setxTick", 0.03 ) );
      
    ostringstream ost;
    ost << pdfname_ << "^{" << hadronname_ << "}/" 
	<< pdfname_ << "^{" << hadronname_ << "}_{" 
	<< Flavor::name( Flavor::PARTON( bflav ) ) 
	<< "}( " << xval_ << ", #mu_{F}^{2}=" << Q2 << " GeV^{2} )" 
	<< endl;

    if( ! args.hasOpt( "noytitle" ) ){
      frame->GetYaxis()->SetTitle( ost.str().c_str() );
      if( args.hasOpt( "leftmarg" ) )
	frame->GetYaxis()->SetTitleOffset( args.get( "leftmarg", 0.50 ) * 10 );
    }
    if( args.hasOpt( "ylabelsize" ) )
      frame->GetYaxis()->SetLabelSize( args.get( "ylabelsize", 0.04 ) );
    if( args.hasOpt( "noylabel" ) )
      frame->GetYaxis()->SetLabelColor( 0 );
    if( args.hasOpt( "setyTick" ) )
      frame->GetYaxis()->SetTickLength( args.get( "setyTick", 0.03 ) );

    if( args.hasOpt( "zeroline" ) ){
      TLine* zero = new TLine( xmin_, 0.0, xmax_, 0.0 );
      zero->Draw();
    }

    // TLegend handling //
    TLegend* l = new TLegend( 0.65, 0.65, 0.86, 0.86 );
    l->SetFillColor( 0 );
    l->SetTextSize( 0.03 );
    for( int i = -6; i <= 6; i++ )
      for( int j = 0; j < vvmycalc.size(); j++ )
	if( vflv[ j ] == i ){
	  ostringstream legendn;
	  legendn << QCD::Flavor::name( QCD::Flavor::PARTON( i ) );
	  l->AddEntry( vvmycalc[ j ][ 0 ], legendn.str().c_str(), "l" );
	}
    if( l && !args.hasOpt( "nolegend" ) ) l->Draw();

    // Tgraph Draw process //
    for( int i = 0; i < vvmycalc.size(); i++ )
      vvmycalc[ i ][ 0 ]->Draw(); 

    if( c ) {
      ostringstream output;
      string outsuffix_ = Utility::Arguments::ref().get( "suffix", "png" );
      string dirname_ = "png_" + canvastitle_;
      gSystem->mkdir( dirname_.c_str() );    
      output << dirname_ << "/" << canvastitle_ << "-" << Q2 
	     << "-ratio-" << order_ << "." << outsuffix_; 
      c->Print( output.str().c_str() );
    }

  }

  void sumplot( const double& Q2 ) throw( int ){

    if( ! Utility::Arguments::instance() ) throw( 1 );
    Utility::Arguments& args = Utility::Arguments::ref();

    // Set Gscale //
    vector< double > gscale( 13, 1.0 );
    for( int i = -6; i <= 6; i++ ){
      string pname = QCD::Flavor::name( QCD::Flavor::PARTON( i ) );
      if( args.hasOpt( pname ) ) gscale[ i + 6 ] = args.get( pname, 1.0 );
    }

    // Skip Flags //
    vector< bool > skfl( 13, false );
    for( int i = -6; i <= 6; i++ ){
      ostringstream ost; ost << "skip" 
			     << QCD::Flavor::name( QCD::Flavor::PARTON( i ) );
      skfl[ i + 6 ] = args.hasOpt( ost.str().c_str() );
    }

    // Set TGraphs //
    for( int i = -3; i <= 5; i++ ){
    //    for( int i = 0; i <= 2; i++ ){

      if( skfl[ i + 6 ] ) continue;
      this->setTGraph( Q2, i, true, gscale[ i + 6 ] );
      this->AddGraph( Q2, i, gscale[ i + 6 ] );
    }

    if( vvmycalc.size() == 0 ) return;

    this->setstrings();

    // TCanvas handling //
    TCanvas* c = new TCanvas( canvastitle_.c_str() );
    if( ! args.hasOpt( "linear" ) ) c->SetLogx();
    if( args.hasOpt( "frameWidth") )
      c->SetFrameLineWidth( args.get( "frameWidth", 1 ) );

    // Frame handling //
    for( int j = 0; j < vvmycalc.size(); j++ )
      if( vflv[ j ] == ( Q2 >= 10.0 ? 0 : 1 ) ){
	this->setframeparam( vvmycalc[ j ][ 0 ], args.get( "xmin", 0.001 ) );
	break;
      }
    TH1F* frame = c->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    
    if( ! args.hasOpt( "noxtitle" ) )
      frame->GetXaxis()->SetTitle( xval_.c_str() );
    if( args.hasOpt( "xlabelsize" ) )
      frame->GetXaxis()->SetLabelSize( args.get( "xlabelsize", 0.04 ) );
    if( args.hasOpt( "noxlabel" ) )
      frame->GetXaxis()->SetLabelColor( 0 );
    if( args.hasOpt( "setxTick" ) )
      frame->GetXaxis()->SetTickLength( args.get( "setxTick", 0.03 ) );      

    ostringstream ost;
    ost << pdfname_ << "^{" << hadronname_ << "}( " 
	<< xval_ << ", #mu_{F}^{2}=" << Q2 << " GeV^{2} )" << endl;

    if( ! args.hasOpt( "noytitle" ) )
      frame->GetYaxis()->SetTitle( ost.str().c_str() );
    if( args.hasOpt( "ylabelsize" ) )
      frame->GetYaxis()->SetLabelSize( args.get( "ylabelsize", 0.04 ) );
    if( args.hasOpt( "noylabel" ) )
      frame->GetYaxis()->SetLabelColor( 0 );
    if( args.hasOpt( "setyTick" ) )
      frame->GetYaxis()->SetTickLength( args.get( "setyTick", 0.03 ) );

    if( args.hasOpt( "zeroline" ) ){
      TLine* zero = new TLine( xmin_, 0.0, xmax_, 0.0 );
      zero->Draw();
    }

    // TLegend handling //
    TLegend* l = new TLegend( 0.65, 0.65, 0.86, 0.86 );
    l->SetFillColor( 0 );
    l->SetTextSize( 0.03 );
    for( int i = -6; i <= 6; i++ )
      for( int j = 0; j < vvmycalc.size(); j++ )
	if( vflv[ j ] == i ){
	  ostringstream legendn;
	  legendn << QCD::Flavor::name( QCD::Flavor::PARTON( i ) );
	  //	  if( legendn.str() == "gluon" ) legendn.str( "g" );
	  double isc = 1.0 / gscale[ i + 6 ];
	  if( gscale[ i + 6 ] != 1.0 ) legendn << " / " << isc;
	  l->AddEntry( vvmycalc[ j ][ 0 ], legendn.str().c_str(), "l" );
	  break;
	}
    if( l && !args.hasOpt( "nolegend" ) ) l->Draw();

    // Tgraph Draw process //
    for( int i = 0; i < vvmycalc.size(); i++ )
      for( int j = 0; j < vvmycalc[ i ].size(); j++ ){ 
	if( args.hasOpt( "woerror" ) && j != 0 ) continue;
	  vvmycalc[ i ][ j ]->Draw(); 
      }

    if( c ) {
      ostringstream output;
      string outsuffix_ = Utility::Arguments::ref().get( "suffix", "png" );
      string dirname_ = "png_" + canvastitle_;
      gSystem->mkdir( dirname_.c_str() );    
      output << dirname_ << "/" << canvastitle_ << "-" << Q2 
	     << "-sumplot-" << order_ << "." << outsuffix_; 
      c->Print( output.str().c_str() );
    }

  }

  void plot( double Q2, int iFlav, bool display = true ) throw( int ) {

    Utility::Arguments& args = Utility::Arguments::ref();

    // Set TGraphs //
    this->setTGraph( Q2, iFlav );
    // Additional Graphs //
    this->AddGraph( Q2, iFlav );  

    if( vvmycalc.size() == 0 ) return;

    this->setstrings();

    // TCanvas handling //
    TCanvas* c = new TCanvas( canvastitle_.c_str() );
    if( ! args.hasOpt( "linear" ) ) c->SetLogx();
    if( args.hasOpt( "frameWidth") )
      c->SetFrameLineWidth( args.get( "frameWidth", 1 ) );

    // Frame handling //
    this->setframeparam( vvmycalc[ 0 ][ 0 ] );
    TH1F* frame = c->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    
    if( ! args.hasOpt( "noxtitle" ) )
      frame->GetXaxis()->SetTitle( xval_.c_str() );
    if( args.hasOpt( "xlabelsize" ) )
      frame->GetXaxis()->SetLabelSize( args.get( "xlabelsize", 0.04 ) );
    if( args.hasOpt( "noxlabel" ) )
      frame->GetXaxis()->SetLabelColor( 0 );
    if( args.hasOpt( "setxTick" ) )
      frame->GetXaxis()->SetTickLength( args.get( "setxTick", 0.03 ) );
      
    ostringstream ost;
    ost << pdfname_ << "_{" 
	<< QCD::Flavor::name( QCD::Flavor::PARTON( iFlav ) )
	<< "}^{" << hadronname_ << "}( " 
	<< xval_ << ", #mu_{F}^{2}=" << Q2 << " GeV^{2} )" << endl;
    
    if( ! args.hasOpt( "noytitle" ) )
      frame->GetYaxis()->SetTitle( ost.str().c_str() );
    if( args.hasOpt( "ylabelsize" ) )
      frame->GetYaxis()->SetLabelSize( args.get( "ylabelsize", 0.04 ) );
    if( args.hasOpt( "noylabel" ) )
      frame->GetYaxis()->SetLabelColor( 0 );
    if( args.hasOpt( "setyTick" ) )
      frame->GetYaxis()->SetTickLength( args.get( "setyTick", 0.03 ) );

    if( args.hasOpt( "zeroline" ) ){
      TLine* zero = new TLine( xmin_, 0.0, xmax_, 0.0 );
      zero->Draw();
    }

    int color1 = args.get( "color1", 8 );
    int color2 = args.get( "color2", 48 );
    int shade1 = args.get( "shade1", 3001 );
    int shade2 = args.get( "shade2", 3013 );
    int shade3 = args.get( "shade3", 3013 );

    // Tgraph Draw process //
    if( args.hasOpt( "fill" ) ){

      for( int i = 0; i < vvmycalc.size(); i++ )
	if( vvmycalc[ i ].size() != 1 ){

	  /*
	  int cl_;
	  if( args.hasOpt( "wyellow" ) ){
	    cl_ = 2 + i;
	  } else {
	    cl_ = ( 2 + i != 5 ? 2 + i : 1 );
	    ostringstream oss_cl_; oss_cl_ << "setLC" << i + 1;
	    if( args.hasOpt( oss_cl_.str().c_str() ) )
	      cl_ = args.get( oss_cl_.str().c_str(), 0 );
	  }
	  */
	  int cl_ = vvmycalc[ i ][ 0 ]->GetLineColor();

	  int sd_ = ( i == 0 ? shade1 : shade3 );

	  int cll_ = ( args.hasOpt( "color1" ) ? color1 : cl_ );
	  this->Fill( vvmycalc[ i ][ 1 ],vvmycalc[ i ][ 2 ], sd_, cll_ );
	  // for legend
	  vvmycalc[ i ][ 0 ]->SetFillStyle( sd_ );
	  vvmycalc[ i ][ 0 ]->SetFillColor( cll_ );

	  ostringstream woc_; woc_ << "woCenter" << i + 1;
	  if( ! args.hasOpt( woc_.str().c_str() ) ) 
	    vvmycalc[ i ][ 0 ]->Draw();

	} else {

	  if( ! args.hasOpt( "scale" ) ){
	    vvmycalc[ i ][ 0 ]->Draw();
	  }else{
	    if( i != 1 && i != 2 ) vvmycalc[ i ][ 0 ]->Draw();
	  }

	}

      if( args.hasOpt( "scale" ) ){
	if( ! args.hasOpt( "scaleabs" ) ){

	  this->Fill( vvmycalc[ 1 ][ 0 ],vvmycalc[ 2 ][ 0 ], shade2, color2 );
	  // for legend
	  vvmycalc[ 1 ][ 0 ]->SetFillStyle( shade2 );
	  vvmycalc[ 1 ][ 0 ]->SetFillColor( color2 );
	  vvmycalc[ 2 ][ 0 ]->SetFillStyle( shade2 );
	  vvmycalc[ 2 ][ 0 ]->SetFillColor( color2 );

	} else {

	  this->takescaleabs( vvmycalc[ 0 ][ 0 ], vvmycalc[ 1 ][ 0 ], 
			      vvmycalc[ 2 ][ 0 ] );
	  this->Fill( vvmycalc[ 1 ][ 0 ],vvmycalc[ 2 ][ 0 ], shade2, color2 );
	  // for legend
	  vvmycalc[ 1 ][ 0 ]->SetFillStyle( shade2 );
	  vvmycalc[ 1 ][ 0 ]->SetFillColor( color2 );
	  vvmycalc[ 2 ][ 0 ]->SetFillStyle( shade2 );
	  vvmycalc[ 2 ][ 0 ]->SetFillColor( color2 );

	}
      }

    } else {
      
      for( int i = 0; i < vvmycalc.size(); i++ )
	for( int j = 0; j < vvmycalc[ i ].size(); j++ )
	  vvmycalc[ i ][ j ]->Draw();

    }

    //    /*
    // TLegend handling //
    TLegend* l = new TLegend( 0.58, 0.58, 0.89, 0.86 );
    l->SetFillColor( 0 );
    l->SetTextSize( 0.04 );
    l->SetHeader( ost.str().c_str() );
    for( int i = 0; i < vvmycalc.size(); i++ ){
      if( args.hasOpt( "scale" ) 
	  && ( ( i == 1 && ! args.hasOpt( "scaleLG1" ) ) || 
	       ( i == 2 && ! args.hasOpt( "scaleLG2" ) ) ) )  continue;

      ostringstream ost; ost << "flegend" << i + 1;
      ostringstream ostt; ostt << "noautolegend" << i + 1;

      string pf_ = ( args.hasOpt( ost.str().c_str() ) ? "f" : "l" );
      if( ! args.hasOpt( ostt.str().c_str() ) )
	l->AddEntry( vvmycalc[ i ][ 0 ], vtitlename[ i ].c_str(), 
		     pf_.c_str() );
      ostringstream adost; adost << "addflegend" << i + 1;

      if( args.hasOpt( adost.str().c_str() ) )
	l->AddEntry( vvmycalc[ i ][ 0 ], 
		     args.get( adost.str().c_str(), "" ).c_str(), "f" );
    }
    if( l && ! args.hasOpt( "nolegend" ) ) l->Draw();
    //    */

    // Output process //
    if( c ) {
      if( display ) c->Draw();
      
      ostringstream output;
      string outsuffix_ = Utility::Arguments::ref().get( "suffix", "png" );
      string dirname_ = "png_" + canvastitle_;
      gSystem->mkdir( dirname_.c_str() );    
      output << dirname_ << "/" << canvastitle_ << "-" << Q2 
	     << "-" << QCD::Flavor::name( QCD::Flavor::PARTON( iFlav ) )
	     << "-" << order_ << "." << outsuffix_; 
      
      c->Print( output.str().c_str() );
    }
    
  }

private:
  
  void setstrings() throw( int ){

    if( ! Utility::Arguments::instance() ) throw( 1 );
    Utility::Arguments& args = Utility::Arguments::ref();
    
    order_ = args.get( "order", "NLO" );

    if( args.hasOpt( "FF" ) ){
      
      canvastitle_ = "FF";
      switch( args.get( "HAD", 1 ) ){
      case   1: break;
      case   2: canvastitle_ += "K"; break;
      case   3: canvastitle_ += "P"; break;
      case   4: canvastitle_ += "H"; break;
      case  10: canvastitle_ += "R"; break;
      case 100: canvastitle_ += "H"; break;
      }
      xval_ = "z";
      pdfname_ = xval_ + "D";
      hadronname_ = gethadron( QCD::Flavor::HADRON( args.get( "HAD", 1 ) ) );
      
    } else if( args.hasOpt( "unpolPDF" ) ) {
      
      canvastitle_ = "UnpolPDF";
      xval_ = "x";
      pdfname_ = xval_ + "q";
      hadronname_ = "";

    } else if( args.hasOpt( "MRST" ) ) {
      
      canvastitle_ = "MRST";
      xval_ = "x";
      pdfname_ = xval_ + "q";
      hadronname_ = "";
      
    } else if( args.hasOpt( "polPDF" ) ) {
      
      canvastitle_ = "PolPDF";
      xval_ = "x";
      string tmp( "q" );
      pdfname_ = "#Delta " + tmp;
      hadronname_ = "";

    } else if( args.hasOpt( "DSSV" ) ) {
      
      canvastitle_ = "DSSV";
      xval_ = "x";
      string tmp( "q" );
      pdfname_ = "#Delta " + tmp;
      hadronname_ = "";

    } else if( args.hasOpt( "Kretzer" ) ) {
      
      canvastitle_ = "Kretzer";
      switch( args.get( "HAD", 1 ) ){
      case   1: break;
      case   2: canvastitle_ += "K"; break;
      case   3: canvastitle_ += "P"; break;
      case   4: canvastitle_ += "H"; break;
      case  10: canvastitle_ += "R"; break;
      case 100: canvastitle_ += "H"; break;
      }
      xval_ = "z";
      pdfname_ = xval_ + "D";
      hadronname_ = gethadron( QCD::Flavor::HADRON( args.get( "HAD", 1 ) ) );

    } else {

      canvastitle_ = "FF";
      switch( args.get( "HAD", 1 ) ){
      case   1: break;
      case   2: canvastitle_ += "K"; break;
      case   3: canvastitle_ += "P"; break;
      case   4: canvastitle_ += "H"; break;
      case  10: canvastitle_ += "R"; break;
      case 100: canvastitle_ += "H"; break;
      }
      xval_ = "z";
      pdfname_ = xval_ + "D";
      hadronname_ = gethadron( QCD::Flavor::HADRON( args.get( "HAD", 1 ) ) );

    }

  }

  string gethadron( const QCD::Flavor::HADRON& had ){
    switch( had ){
    case  QCD::Flavor::PIp : return "#pi+";
    case  QCD::Flavor::PIm : return "#pi-";
    case  QCD::Flavor::KAp : return "K+";
    case  QCD::Flavor::KAm : return "K-";
    case  QCD::Flavor::PRp : return "P+";
    case  QCD::Flavor::PRm : return "P-";
    case  QCD::Flavor::CHp : return "H+";
    case  QCD::Flavor::CHm : return "H-";
    case  QCD::Flavor::REp : return "R+";
    case  QCD::Flavor::REm : return "R-";
    case  QCD::Flavor::HAp : return "H+";
    case  QCD::Flavor::HAm : return "H-";
    }
    return "";
  }

  void setframeparam( TGraph* gc, double xmin = 0.01 ){

    xmax_ = 1.0;
    xmin_ = xmin;

    ymax_ = 0.0;
    ymin_ = 0.0;
    double ymaxlimit_ = 10.0;
    double yminlimit_ = -10.0;

    double* xd = gc->GetX();
    double* yd = gc->GetY();

    // Find y max //    
    for( int idat = 0; idat < gc->GetN(); idat++ )
      if( xmin_ < xd[ idat ] && ymax_ < yd[ idat ] 
	  && yd[ idat ] < ymaxlimit_ ) ymax_ = yd[ idat ];

    if( ymax_ > ymaxlimit_ - 2.0 ) ymax_ = ymaxlimit_ / 2.0 ;
    //    ymax_ = ymax_ + 0.05;
    ymax_ = ymax_ + 0.10;
    
    // Find y min //    
    for( int idat = 0; idat < gc->GetN(); idat++ )
      if( xmin_ < xd[ idat ] && ymin_ > yd[ idat ] 
	  && yd[ idat ] > yminlimit_ ) ymin_ = yd[ idat ];
    
    if( ymin_ < yminlimit_ + 2.0 ) ymin_ = yminlimit_ / 2.0 ;
    //    ymin_ = ymin_ - 0.05;
    ymin_ = ymin_ - 0.10;
    
    Utility::Arguments& args = Utility::Arguments::ref();

    if( args.hasOpt( "ymax" ) ) ymax_ = args.get( "ymax", 1.0 );
    if( args.hasOpt( "xmax" ) ) xmax_ = args.get( "xmax", 1.0 );
    if( args.hasOpt( "ymin" ) ) ymin_ = args.get( "ymin", 1.0 );
    if( args.hasOpt( "xmin" ) ) xmin_ = args.get( "xmin", 1.0 );

  }

  void AddGraph( Float_t Q2, int iFlav, double scale = 1.0 ){

    double xmin_ = 1.0E-3;
    double xmax_ = 1.0;
    int dn_ = 100;
    double dx_ = log( xmin_ ) / dn_;
    
    double xx_[ 1000 ], yy_[ 1000 ];

    vector< TGraph* > tmpvtg( 0 );
    if( Q2 == 1.0 ) 
      if( Utility::Arguments::ref().hasOpt( "MRST" ) ){
	for( int i = 0; i <= dn_ ; i++ ){
	  xx_[ i ] = xmin_ * exp( - dx_ * i );
	  
	  if( iFlav == 0 ) yy_[ i ] = mrstglu( xx_[ i ] );
	  if( iFlav == 1 ) yy_[ i ] = mrstuv( xx_[ i ] ) 
			     + 0.2 * mrstS( xx_[ i ] ) 
			     - 0.5 * mrstD( xx_[ i ] );
	  if( iFlav == 2 ) yy_[ i ] = mrstdv( xx_[ i ] ) 
			     + 0.2 * mrstS( xx_[ i ] ) 
			     + 0.5 * mrstD( xx_[ i ] );
	  if( iFlav == 3 ) yy_[ i ] = 0.1 * mrstS( xx_[ i ] );
	  if( iFlav == -3 ) yy_[ i ] = 0.1 * mrstS( xx_[ i ] );
	  if( iFlav == -2 ) yy_[ i ] = 0.2 * mrstS( xx_[ i ] ) 
			      + 0.5 * mrstD( xx_[ i ] );
	  if( iFlav == -1 ) yy_[ i ] = 0.2 * mrstS( xx_[ i ] ) 
			      - 0.5 * mrstD( xx_[ i ] );
	  yy_[ i ] *= scale;
	}
	tmpvtg.push_back( new TGraph( dn_, xx_, yy_ ) );
      }
    if( tmpvtg.size() == 0 ) return;
    vvmycalc.push_back( tmpvtg );
    vflv.push_back( iFlav );

  }
  
  double mrstglu( const double& x ){
    double Ag = 64.57;
    double lambdag = 0.9171; // note - sign in lambda
    double etag = 6.587;
    double epsig = -3.168;
    double gammag = 3.251;
    
    return Ag * pow( x, lambdag ) * pow( 1.0 - x, etag ) * 
      ( 1.0 + epsig * pow( x, 0.5 ) + gammag * x );
  }
  
  double mrstuv( const double& x ){
    double Auv = 0.6051;
    double lambdauv = 0.4089;
    double etauv = 3.395;
    double epsiuv = 2.078;
    double gammauv = 14.56;
    
    return Auv * pow( x, lambdauv ) * pow( 1.0 - x, etauv ) * 
      ( 1.0 + epsiuv * pow( x, 0.5 ) + gammauv * x );
  }
  
  double mrstdv( const double& x ){
    double Adv = 0.05811;
    double lambdadv = 0.2882;
    double etadv = 3.874;
    double epsidv = 34.69;
    double gammadv = 28.96;
    
    return Adv * pow( x, lambdadv ) * pow( 1.0 - x, etadv ) * 
      ( 1.0 + epsidv * pow( x, 0.5 ) + gammadv * x );
  }
  
  double mrstS( const double& x ){
    double AS = 0.2004;
    double lambdaS = -0.2712; // note - sign in lambda
    double etaS = 7.808;
    double epsiS = 2.283;
    double gammaS = 20.69;
    
    return AS * pow( x, lambdaS ) * pow( 1.0 - x, etaS ) * 
      ( 1.0 + epsiS * pow( x, 0.5 ) + gammaS * x );
  }
  
  double mrstD( const double& x ){
    double AD = 1.290;
    double lambdaD = 1.183; 
    double etaD = 7.808; // from mrstS
    double gammaD = 9.987;
    double deltaD = -33.34;
    
    return AD * pow( x, lambdaD ) * pow( 1.0 - x, etaD + 2 ) * 
      ( 1.0 + gammaD * x + deltaD * x * x );
  }


  void Fill( TGraph* tg1, TGraph* tg2, int fillstyle, int color ){
    TGraph *g = new TGraph();
    
    double x,y;
    
    for( int i = 0; i < tg1->GetN(); i++ ){
      tg1->GetPoint( i, x, y );
      g->SetPoint( i, x, y );
    }
    for( int i = 0; i < tg2->GetN(); i++ ){
      tg2->GetPoint( tg2->GetN()-1-i, x, y );
      g->SetPoint( tg1->GetN()+i, x, y );
    }       
    
    g->SetFillColor( color );
    g->SetFillStyle( fillstyle );
    g->Draw( "FL" );
    
  }

  void takescaleabs( TGraph* tg0, TGraph* tg1, TGraph* tg2 ){

    double x0,y0,x1,y1,x2,y2;

    for( int i = 0; i < tg0->GetN(); i++ ){
      tg0->GetPoint( i, x0, y0 );
      tg1->GetPoint( i, x1, y1 );
      tg2->GetPoint( i, x2, y2 );

      if( x0 != x1 || x0 != x2 ) throw( 1 );
      double yy_ = ( fabs( y1 - y0 ) > fabs( y2 - y0 ) ? 
		     fabs( y1 - y0 ) : fabs( y2 - y0 ) );
      tg1->SetPoint( i, x0, y0 + yy_ );
      tg2->SetPoint( i, x0, y0 - yy_ );
    }
  }
  
};

#endif // _plotter_FFset_hh_
