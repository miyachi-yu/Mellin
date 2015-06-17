//
// viewPDF.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <yoshiyuki@yoshiyuki-laptop>
// 
// Started on  Wed Oct  8 00:09:08 2008 Yoshiyuki Miyachi
// Started on  Wed Oct  8 00:09:08 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TLine.h>
#include <TLatex.h>


// -------------------------------------------------------------------
//                         PDF parameterizations
// -------------------------------------------------------------------
#include <Utility/Arguments.hh>

// -------------------------------------------------------------------
//                         FF parameterizations
// -------------------------------------------------------------------
#include <PDFLIB/FF.hh>

// -------------------------------------------------------------------
//                         PDF parameterizations
// -------------------------------------------------------------------
#include <Evolution/PDFx.hh>
#include <Fragmentation/Evolution.hh>

class MyApp : public TApplication {
public:
  
  MyApp( int& argc, char* argv[] ) : 
    TApplication( "MyApp", &argc, argv ),
    args_( Utility::Arguments::ref() ),
    packs_( 0 ),
    q2_( args_.get( "Q2", 2.5 ) ),
    xmin_( args_.get( "zmin", 0.01 ) ),
    xmax_( args_.get( "zmax", 1.00 ) ),
    nx_( args_.get( "nz", 40 ) ),
    parton_( QCD::Flavor::g ),
    type_( QCD::Flavor::a0 ),
    isParton_( true ),
    gPacks_( 0 ),
    weight_( args_.get( "weight", 1.0 ) ),
    ymin_( 0.0 ), ymax_( 0.01 ),
    c_( NULL ),
    frame_( NULL ),
    output_( args_.get( "output", "" ) ),
    legend_( NULL ), line_( NULL ), latex_( NULL ),
    pPDF_( 0 ), pdf_( 0 ), gPDF_( 0 ),
    datafile_( "" )
  {
    
    c_ = new TCanvas( "mycanvas" );
    if( args_.hasOpt( "logx" ) ) c_->SetLogx( kTRUE );
    if( args_.hasOpt( "logy" ) ) c_->SetLogx( kFALSE );
    
    legend_ = new TLegend( 0.6, 0.6, 0.88, 0.88 );
    
    line_ = new TLine();
    
    latex_ = new TLatex();
    latex_->SetTextSize( 0.04 );
    
    vector< string > optPI =
      Fragmentation::Evo::options( QCD::Flavor::PIp );

    vector< string > optKA =
      Fragmentation::Evo::options( QCD::Flavor::KAp );

    vector< string > optPR =
      Fragmentation::Evo::options( QCD::Flavor::PRp );

    vector< string > optRE =
      Fragmentation::Evo::options( QCD::Flavor::REp );

    vector< string > candidate( 0 );
    candidate.insert( candidate.end(), optPI.begin(), optPI.end() );
    candidate.insert( candidate.end(), optKA.begin(), optKA.end() );
    candidate.insert( candidate.end(), optPR.begin(), optPR.end() );
    candidate.insert( candidate.end(), optRE.begin(), optRE.end() );
    
    string ptype = args_.get( "parton", "gluon" );
    if( isParton_ = QCD::Flavor::isParton( ptype ) ){
      parton_ = (QCD::Flavor::PARTON) QCD::Flavor::id( ptype );
    } else {
      type_   = (QCD::Flavor::TYPE ) QCD::Flavor::id( ptype );
    }
    
    for( int i = 0; i < candidate.size(); i++ ){
      
      if( ! args_.hasOpt( candidate[i] ) ) continue;
      
      QCD::Flavor::HADRON h = QCD::Flavor::PIp;
      
      if( find( optKA.begin(), optKA.end(), candidate[ i ] ) != 
	  optKA.end() ) h = QCD::Flavor::KAp;
      
      if( find( optPR.begin(), optPR.end(), candidate[ i ] ) != 
	  optPR.end() ) h = QCD::Flavor::PRp;
      
      if( find( optRE.begin(), optRE.end(), candidate[ i ] ) != 
	  optRE.end() ) h = QCD::Flavor::REp;
      
      Fragmentation::Evo *evo = 
	new Fragmentation::Evo( args_, h, candidate[i] );
      
      TGraph *g = new TGraph( nx_ );
      g->SetLineWidth( 2 );
      g->SetLineColor( i + 1 );
      g->SetLineStyle( 1 );
      legend_->AddEntry( g, candidate[i].c_str(), "l" );
      
      
      Evolution::PDFx *pdf;
      if( isParton_ ){
	pdf = new Evolution::PDFx( evo, parton_,
				   args_.get( "leg1", 6 ),
				   args_.get( "leg2", 8 ),
				   args_.get( "precision", 5.0E-3 ),
				   args_.get( "length", 15.0 ),
				   args_.get( "offset", 3.0 ),
				   args_.get( "angle",  0.55 )
				   );
      } else {
	pdf = new Evolution::PDFx( evo, type_,
				   args_.get( "leg1", 6 ),
				   args_.get( "leg2", 8 ),
				   args_.get( "precision", 5.0E-3 ),
				   args_.get( "length", 15.0 ),
				   args_.get( "offset", 3.0 ),
				   args_.get( "angle",  0.55 )
				   );
      }
      
      pPDF_.push_back( evo );
      gPDF_.push_back( g );
      pdf_.push_back( pdf );
      
    }
    
    int color = 2;
    if( args_.hasOpt( "FFLIB" ) ){
      string path = args_.get( "FFLIB", "ff_tables" );
      cout << path << endl;
      packs_.push_back( new PDFLIB::FF( 1, path ) ) ;
      gPacks_.push_back( this->newGraph( color++ ) );      
    }
    
    ostringstream ostr;
    if( weight_ != 0.0 ){
      ostr << "z";
      if( weight_ != 1.0 ) ostr << "^{" << weight_ << "}" ;
      ostr << " ";
    }
    ostr 
      << "D_{"
      << ( isParton_ ? 
	   QCD::Flavor::name( parton_ ).c_str() :
	   QCD::Flavor::name( type_ ).c_str() )
      << "}^{"
      << "#pi"
      << "}(x, Q^{2}=" << q2_ << " GeV^{2})";
    legend_->SetHeader( ostr.str().c_str() );
    
    args_.log() << "#---- set parton type and Q2" << endl;
    for( int i = 0; i < packs_.size(); i++ ){
      packs_[ i ]->setQ2( q2_ );
      if( isParton_ ) packs_[ i ]->setPID( parton_ );
      else packs_[ i ]->setPID( type_ );
      packs_[ i ]->xWeight( weight_ );
    }
    
    for( int i = 0; i < pPDF_.size(); i++ ){
      pPDF_[ i ]->setQ2( q2_ );
    }
    
    // calculate PDFLIBs
    double lxmin = log10( xmin_ );
    double lxmax = log10( xmax_ );
    double dlx   = ( lxmax - lxmin ) / nx_;
    
    args_.log() << "#---- calculate with PDFLIB" << endl;
    for( int ix = 0; ix < nx_; ix++ ){
      double x = pow( 10.0, lxmin + ix * dlx );
      
      for( int ip = 0; ip < packs_.size(); ip++ ){
	double y = (*packs_[ip])( x );
	this->checkY( y );
	gPacks_[ ip ]->SetPoint( ix, x, y );
      }
      
      for( int i = 0; i < pPDF_.size(); i++ ){
	double y = pow( x, weight_ ) * (*pdf_[i])( x );
	this->checkY( y );
	gPDF_[i]->SetPoint( ix, x, y );
      }
      
    }
    
    frame_ = c_->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    frame_->GetXaxis()->SetTitle( "z" );
    
    line_->DrawLine( xmin_, 0.0, xmax_, 0.0 ); 
    legend_->Draw();
    
    for( int i = 0; i < gPacks_.size(); i++ ){
      gPacks_[ i ]->Draw( "l" );
      legend_->AddEntry( gPacks_[ i ], 
			 string( packs_[ i ]->name() + " grid" ).c_str(), "l" );
    }
    
    for( int i = 0; i < gPDF_.size(); i++ ) gPDF_[i]->Draw();
    
    c_->Draw();
  }
  
  virtual ~MyApp() {
    for( int i = 0; i < packs_.size(); i++ ) delete packs_[ i ];
    for( int i = 0; i < pdf_.size(); i++ ) {
      delete pdf_[i];
      delete pPDF_[i];
    }
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    c_->Update();
    
    string filename;
    if( output_ != "" ){
      filename = output_ ;
    } else {
      ostringstream ostr;
      ostr << "viewFF_"
	   << ( isParton_ ? 
		QCD::Flavor::name( parton_ ).c_str() :
		QCD::Flavor::name( type_ ).c_str() )
	   << "_" << q2_ << "_" << weight_ << "."
	   << args_.get( "format", "png" );
      filename = ostr.str();
    }
    
    if( gPDF_.size() > 0 ){
      
      if( datafile_ == "" ){
	ostringstream ostr;
	ostr << "viewFF_"
	     << ( isParton_ ? 
		  QCD::Flavor::name( parton_ ).c_str() :
		  QCD::Flavor::name( type_ ).c_str() )
	     << "_" << q2_ << "_" << weight_ << ".dat";
	datafile_ = ostr.str();
      }
      
      ofstream ofs( datafile_.c_str() );
      
      ofs << setw(14) << "x" << flush;
      for( int i = 0; i < pdf_.size(); i++ ){
	ostringstream ostr;
	ostr << "FF" << i ;
	ofs << setw(14) << ostr.str() << flush;
      }
      
      for( int ip = 0; ip < packs_.size(); ip++ ){
	ofs << setw(14) << packs_[ ip ]->name()  << flush;
      }
      ofs << endl;
      
      for( int i = 0; i < nx_; i++ ){
	double x, y;
	
	for( int ip = 0; ip < gPDF_.size(); ip++ ){
	  gPDF_[ ip ]->GetPoint( i, x, y );
	  if( ip == 0 ) ofs << setw(14) << x << flush;
	  ofs << setw(14) << y << flush;
	}
	
	for( int ip = 0; ip < gPacks_.size(); ip++ ){
	  gPacks_[ ip ]->GetPoint( i, x, y );
	  ofs << setw(14) << y << flush;
	}
	
	ofs << endl;
	
      }
      
      
    }
    
    c_->Print( filename.c_str() );
    c_->WaitPrimitive();
  }
  
  TGraph* newGraph( const int& color ){
    TGraph *g = new TGraph( nx_ );
    g->SetMarkerColor( color );
    g->SetLineColor( color );
    g->SetLineWidth( 2 );
    g->SetLineStyle( 1 );
    return g;
  }
  
  void checkY( const double& y ){
    ymin_ = ( y < ymin_ ? y : ymin_ );
    ymax_ = ( y > ymax_ ? y : ymax_ );
  }

  Utility::Arguments& args_;
  vector< PDFLIB::Package* > packs_;
  double q2_;
  double xmin_;
  double xmax_;
  int    nx_;
  QCD::Flavor::PARTON parton_;
  QCD::Flavor::TYPE   type_;
  bool isParton_;
  vector< TGraph* > gPacks_;
  double weight_;
  double ymin_;
  double ymax_;
  TCanvas *c_;
  TH1 *frame_;
  string output_;
  TLegend *legend_;
  TLine *line_;
  TLatex *latex_;
  
  vector< Fragmentation::Evo* > pPDF_;
  vector< Evolution::PDFx* > pdf_;
  vector< TGraph* > gPDF_;

  string datafile_;
};


using namespace std;

int main( int argc, char* argv[] ){

  //  gROOT->SetStyle( "Plain" );
  gROOT->SetStyle( "Pub" );
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  MyApp app( argc, argv );;
  
  
  app.Run();

  return 0;
}
