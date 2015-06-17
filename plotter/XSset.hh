#ifndef _plotter_XSset_hh_
#define _plotter_XSset_hh_

#include "XS.hh"
#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <vector>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TSystem.h>

#include <StrFncParam/F1Param.hh>
#include <StrFncParam/F2Param.hh>
#include <StrFncParam/R1990.hh>
#include <StrFncParam/R1998.hh>
#include <StrFncParam/F2pSMC.hh>

using namespace std;

class XSset {
  string canvastitle_;
  string xval_ ;
  string xsname_;
  string order_;
  string outfilename_;

  double Q2_;

  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;

  vector< string > vtitlename; // names for each data file
  vector< XS* > vmycalc; // each data file

  vector< string > vvtitlename; // names for each TGraph
  vector< vector< TGraph*> > vvmycalc; // TGraphs in each file

public:
  XSset() : vtitlename( 0 ), vmycalc( 0 ), vvtitlename( 0 ), vvmycalc( 0 ){};

  virtual ~XSset(){ 
    for( int i = 0; i < vmycalc.size(); i++ ) delete vmycalc[ i ];
  };

  void insertdata( string mycalc_dir, string titlename = "" ){
    vtitlename.push_back( titlename == "" ? mycalc_dir : titlename );
    vmycalc.push_back( new XS( mycalc_dir ) );
  }
  
  void setTGraph( double Q2, double scale = 1.0, int style = 0 ){
    // TGraph handling //
    for( int i = 0; i < vmycalc.size(); i++ ){
      vector< TGraph* > vmycalcc = vmycalc[ i ]->getData( Q2, scale );
      if( vmycalcc.size() == 0 )  break; // return; // throw( 1 );

      for( int j = 0; j < vmycalcc.size(); j++ ){

	vmycalcc[ j ]->SetLineColor( 1 + i );
	vmycalcc[ j ]->SetLineStyle( ( j == 0 ? 1 : 3 ) + style );
	vmycalcc[ j ]->SetLineWidth( 2 );

      }
      vvmycalc.push_back( vmycalcc );
      ostringstream titlename;
      titlename << vtitlename[ i ];
      if( Utility::Arguments::instance()->hasOpt( "sum" ) )
	  titlename << " - Q2 = " << Q2;
      vvtitlename.push_back( titlename.str() );
    }
  }

  void sumplot( vector< double > vQ2, bool display = true ) throw( int ) {

    for( int i = 0; i < vQ2.size(); i++ ){
      // Set TGraphs //
      this->setTGraph( vQ2[ i ], 1.0, i );
      // Additional Graphs //
      this->AddGraph( vQ2[ i ] );
    }
    if( vvmycalc.size() == 0 ) return;

    this->setstrings();

    ostringstream output;
    output << canvastitle_ << "-sumplot-" << order_ << ".png"; 
    outfilename_ = output.str();

    double xmin = Utility::Arguments::instance()->get( "xmin", 0.001 );
    double xmax = Utility::Arguments::instance()->get( "xmax", 1.0 );
    //    plotprocess( vvmycalc[ vvmycalc.size() - 1 ][ 0 ], display, 0.001 );
    plotprocess( vvmycalc[ 0 ][ 0 ], display, xmin, xmax );
  }

  void plot( double Q2, bool display = true ) throw( int ) {
    Q2_ = Q2;
    // Set TGraphs //
    this->setTGraph( Q2 );
    // Additional Graphs //
    this->AddGraph( Q2 );  
    if( vvmycalc.size() == 0 ) return;

    // for frame and outputname //
    this->setstrings();

    // outputname //
    ostringstream output;
    output << canvastitle_ << "-" << Q2 
	   << "-" << order_ << ".png"; 
    outfilename_ = output.str();

    double xmin = Utility::Arguments::instance()->get( "xmin", 0.01 );
    double xmax = Utility::Arguments::instance()->get( "xmax", 1.0 );
    plotprocess( vvmycalc[ 0 ][ 0 ], display, xmin, xmax );
  }

  void plotprocess( TGraph* gc, bool display, double xmin, double xmax ) 
  {

    Utility::Arguments& args = Utility::Arguments::ref();

    // TCanvas handling //
    TCanvas* c = new TCanvas( canvastitle_.c_str() );
    if( args.hasOpt( "logx" ) ) c->SetLogx();
    if( args.hasOpt( "logy" ) ) c->SetLogy();

    // Frame handling //
    this->setframeparam( gc, xmin, xmax );
    if( args.hasOpt( "logy" ) ){ 
      ymin_ = args.get( "ymin", 1.0E-2 );
      ymax_ = args.get( "ymax", 1.0E+5 );
    }
    TH1F* frame = c->DrawFrame( xmin_, ymin_, xmax_, ymax_ );

    frame->GetXaxis()->SetTitle( xval_.c_str() );      
    frame->GetYaxis()->SetTitle( xsname_.c_str() );

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

    // Tgraph Draw process //
    for( int i = 0; i < vvmycalc.size(); i++ )
      for( int j = 0; j < vvmycalc[ i ].size(); j++ ) 
	vvmycalc[ i ][ j ]->Draw(); 
      
      // Output process //
    if( c ) {
      if( display ) c->Draw();
      
      string dirname_ = "png_" + canvastitle_;
      gSystem->mkdir( dirname_.c_str() );    
      string filename = dirname_ + "/" + outfilename_;

      c->Print( filename.c_str() );
    }
    
  }

private:
  
  void setstrings() throw( int ){

    if( ! Utility::Arguments::instance() ) throw( 1 );
    Utility::Arguments& args = Utility::Arguments::ref();
    
    order_ = args.get( "order", "NLO" );

    if( args.hasOpt( "FF" ) ){
      
      canvastitle_ = "XS";
      xval_ = "z";
      xsname_ = "#frac{ d#sigma/dz }{ #sigma_{ tot } } ( e+ + e- -> " ;
      string hname_ = gethadron( QCD::Flavor::HADRON( args.get( "HAD", 1 ) ) );
      xsname_ += hname_ + " + X )";
      
    } else if( args.hasOpt( "unpolPDF" ) ) {
      
      canvastitle_ = args.hasOpt( "F2" ) ? "F2" : "F1";
      xval_ = "x";
      xsname_ = ( args.hasOpt( "F2" ) ? " F2( x )/2" : " x F1( x )" );

    } else if( args.hasOpt( "MRST" ) ) {

      canvastitle_ = args.hasOpt( "F2" ) ? "F2_MRST" : "F1_MRST";
      xval_ = "x";
      xsname_ = ( args.hasOpt( "F2" ) ? " F2( x )/2" : " x F1( x )" );
      
    } else if( args.hasOpt( "polPDF" ) ) {
      
      canvastitle_ = "G1";
      xval_ = "x";
      xsname_ = "x G1( x )";

    } else {

      canvastitle_ = "XS";
      xval_ = "z";
      xsname_ = "#frac{ d #sigma / d z }{ #sigma } ( e+ + e- -> " ;
      string hname_ = gethadron( QCD::Flavor::HADRON( args.get( "HAD", 1 ) ) );
      xsname_ += hname_ + " + X )";

    }

  }

  string gethadron( const QCD::Flavor::HADRON& had ){
    switch( had ){
    case  QCD::Flavor::PIp : 
    case  QCD::Flavor::PIm : return "#pi #pm";
    case  QCD::Flavor::KAp : 
    case  QCD::Flavor::KAm : return "K #pm";
    }
    return "";
  }

  void setframeparam( TGraph* gc, double xmin, double xmax ){

    xmax_ = xmax;
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
    ymax_ = ymax_ + 0.05;

    // Find y min //    
    for( int idat = 0; idat < gc->GetN(); idat++ )
      if( xmin_ < xd[ idat ] && ymin_ > yd[ idat ] 
	  && yd[ idat ] > yminlimit_ ) ymin_ = yd[ idat ];
    
    if( ymin_ < yminlimit_ + 2.0 ) ymin_ = yminlimit_ / 2.0 ;
    ymin_ = ymin_ - 0.05;

  }

  void AddGraph( double Q2, double scale = 1.0 ){
    
    StrFncParam::R1990   r1990;
    StrFncParam::R1998   r1998;
    StrFncParam::F2pSMC  f2p;
    StrFncParam::F1Param f1pb_1( f2p, r1990 );   // reference 1
    StrFncParam::F1Param f1pb_2( f2p, r1998 );   // reference 2
    
    double xmin_ = 1.0E-3;
    double xmax_ = 1.0;
    int dn_ = 100;
    double dx_ = log( xmin_ ) / dn_;
    
    double xx_[ 1000 ], yy_[ 1000 ];

    if( ! Utility::Arguments::ref().hasOpt( "F2" ) ){
    
    for( int j = 0; j < 2; j++ ){
      
      vector< TGraph* > tmpvtg( 0 );
      if( Utility::Arguments::ref().hasOpt( "MRST" ) ){
	for( int i = 0; i <= dn_ ; i++ ){
	  xx_[ i ] = xmin_ * exp( - dx_ * i );
	  yy_[ i ] = scale * xx_[ i ] *
	    ( j == 0 ? f1pb_1( xx_[ i ], Q2 ) : f1pb_2( xx_[ i ], Q2 ) );
	}
	tmpvtg.push_back( new TGraph( dn_, xx_, yy_ ) );
      }
      
      if( tmpvtg.size() == 0 ) return;
      
      vvmycalc.push_back( tmpvtg );
      string f1name = ( j == 0 ? "F1par_SMC+r1990" : "F1par_SMC+r1998" );
      ostringstream titlename;
      titlename << f1name << " - Q2 = " << Q2;
      vvtitlename.push_back( titlename.str() );
      
    }

    } else {

      vector< TGraph* > tmpvtg( 0 );
      if( Utility::Arguments::ref().hasOpt( "MRST" ) ){
	for( int i = 0; i <= dn_ ; i++ ){
	  xx_[ i ] = xmin_ * exp( - dx_ * i );
	  yy_[ i ] = scale * f2p( xx_[ i ], Q2 ) / 2.0;
	}
	tmpvtg.push_back( new TGraph( dn_, xx_, yy_ ) );
      }
      
      if( tmpvtg.size() == 0 ) return;

      vvmycalc.push_back( tmpvtg );
      string f1name = "F2pSMC";
      ostringstream titlename;
      titlename << f1name << " - Q2 = " << Q2;
      vvtitlename.push_back( titlename.str() );

    }
    
  }
  
};

#endif // _plotter_XSset_hh_
