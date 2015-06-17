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
//                         PDF parameterizations
// -------------------------------------------------------------------
#include <PDFLIB/GRSV2000.hh>
#include <PDFLIB/LSS.hh>
#include <PDFLIB/AAC.hh>
#include <PDFLIB/BB.hh>

// -------------------------------------------------------------------
//                         PDF parameterizations
// -------------------------------------------------------------------
#include <Evolution/PDFx.hh>
#include <polPDF/Evolution.hh>
#include <QCD/PdfBase.hh>

class MyApp : public TApplication {
public:
  
  MyApp( int& argc, char* argv[] ) : 
    TApplication( "MyApp", &argc, argv ),
    args_( Utility::Arguments::ref() ),
    packs_( 0 ),
    q2_( args_.get( "Q2", 2.5 ) ),
    xmin_( args_.get( "xmin", 0.001 ) ),
    xmax_( args_.get( "xmax", 1.000 ) ),
    nx_( args_.get( "nx", 40 ) ),
    parton_( QCD::Flavor::g ),
    type_( QCD::Flavor::a0 ),
    isParton_( true ),
    gPacks_( 0 ),
    weight_( args_.get( "weight", 0.0 ) ),
    ymin_( 0.0 ), ymax_( 0.01 ),
    c_( NULL ),
    frame_( NULL ),
    output_( args_.get( "output", "" ) ),
    legend_( NULL ), line_( NULL ), latex_( NULL ),
    pPDF_( 0 ), pdf_( 0 ), gPDF_( 0 ),
    datafile_( "" )
  {
    
    c_ = new TCanvas( "mycanvas" );
    c_->SetLogx( kTRUE );
    
    legend_ = new TLegend( 0.12, 0.6, 0.6, 0.88 );
    
    line_ = new TLine();
    
    latex_ = new TLatex();
    latex_->SetTextSize( 0.04 );
    
    string ptype = args_.get( "parton", "gluon" );
    isParton_ = QCD::Flavor::isParton( ptype );
    if( isParton_ ){
      parton_ = (QCD::Flavor::PARTON) QCD::Flavor::id( ptype );
    } else {
      type_   = (QCD::Flavor::TYPE ) QCD::Flavor::id( ptype );
    }

    this->createPDF( "polPDF" );
    this->createPDF( "DSSV" );
    this->createPDF( "BB" );
    
    int color = 1;
    
    if( args_.hasOpt( "GRSV2000" ) ){
      packs_.push_back( new PDFLIB::GRSV2000( PDFLIB::GRSV2000::STANDARD_NLO,
					      args_.getOpt( "GRSV2000" ) ) ) ;
      gPacks_.push_back( this->newGraph( color++ ) );      
    }
    
    if( args_.hasOpt( "AAC" ) ){
      packs_.push_back( new PDFLIB::AAC( 1, args_.getOpt( "AAC" ) ) );
      gPacks_.push_back( this->newGraph( color++ ) );      
      args_.log() << "AAC:  " <<  args_.getOpt( "AAC" ) << endl;
    }
    
    if( args_.hasOpt( "LSS" ) ){
      packs_.push_back( new PDFLIB::LSS( 1, args_.getOpt( "LSS" ) ) );
      gPacks_.push_back( this->newGraph( color++ ) );      
    }
    
    if( args_.hasOpt( "BB_TAB" ) ){
      packs_.push_back( new PDFLIB::BB( 3, args_.getOpt( "BB_TAB" ) ) );
      gPacks_.push_back( this->newGraph( color++ ) );      
      args_.log() << "BB_TAB:   " <<  args_.getOpt( "BB_TAB" ) << endl;
    }
    
    ostringstream ostr;
    if( weight_ != 0.0 ){
      ostr << "x";
      if( weight_ != 1.0 ) ostr << "^{" << weight_ << "}" ;
      ostr << " ";
    }
    
    if( isParton_ ){
      ostr << "#Delta" << QCD::Flavor::name( parton_ );
    } else {
      switch( type_ ){
      case QCD::Flavor::a0 : ostr << "#Delta #Sigma"; break;
      case QCD::Flavor::a3 : ostr << "(#Delta u - #Delta d)"; break;
      case QCD::Flavor::a8 : ostr << "(#Delta u + #Delta d - 2 #Delta s)"; break;
      defalut: ostr << QCD::Flavor::name( type_ ); break;
      }
    }
    ostr << "(x, Q^{2}=" << q2_ << " GeV^{2})";
    legend_->SetHeader( ostr.str().c_str() );
    
    args_.log() << "#---- set parton type and Q2" << endl;
    for( int i = 0; i < packs_.size(); i++ ){
      packs_[ i ]->setQ2( q2_ );
      if( isParton_ ) packs_[ i ]->setPID( parton_ );
      else packs_[ i ]->setPID( type_ );
      packs_[ i ]->xWeight( weight_ );
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
      
      for( int ip = 0; ip < pdf_.size(); ip++ ){
	double y = pow( x, weight_ ) * (*pdf_[ip])( x );
	this->checkY( y );
	gPDF_[ip]->SetPoint( ix, x, y );
      }
      
    }
    
    frame_ = c_->DrawFrame( xmin_, ymin_, xmax_, ymax_ );
    frame_->GetXaxis()->SetTitle( "x" );
    
    line_->DrawLine( xmin_, 0.0, xmax_, 0.0 ); 
    legend_->Draw();
    
    for( int i = 0; i < gPacks_.size(); i++ ){
      gPacks_[ i ]->Draw( "l" );
      legend_->AddEntry( gPacks_[ i ], packs_[ i ]->name().c_str(), "l" );
    }
    
    for( int i = 0; i < gPDF_.size(); i++ ){
      gPDF_[ i ]->Draw();
    }
    
    c_->Draw();
  }
  
  virtual ~MyApp() {
    for( int i = 0; i < packs_.size(); i++ ) delete packs_[ i ];
    for( int i = 0; i < pdf_.size();   i++ ) delete pdf_[ i ];
    for( int i = 0; i < pPDF_.size();  i++ ) delete pPDF_[ i ];
  }
  
  void Run( Bool_t retrn = kFALSE ){
    c_->Update();
    
    string filename;
    if( output_ != "" ){
      filename = output_ ;
    } else {
      ostringstream ostr;
      ostr << "viewPDF_"
	   << ( isParton_ ? 
		QCD::Flavor::name( parton_ ).c_str() :
		QCD::Flavor::name( type_ ).c_str() )
	   << "_" << q2_ << "_" << weight_ << "." 
	   << args_.get( "format", "png" );
      filename = ostr.str();
    }
    c_->Print( filename.c_str() );
    
    if( gPDF_.size() > 0 ){
      
      if( datafile_ == "" ){
	ostringstream ostr;
	ostr << "viewPDF_"
	     << ( isParton_ ? 
		  QCD::Flavor::name( parton_ ).c_str() :
		  QCD::Flavor::name( type_ ).c_str() )
	     << "_" << q2_ << "_" << weight_ << ".dat";
	datafile_ = ostr.str();
      }
      
      ofstream ofs( datafile_.c_str() );
      
      ofs << setw(14) << "x" << flush;
      for( int ip = 0; ip < pdf_.size(); ip++ ){
	ofs << setw(14) << "PDF" << ip + 1 << flush;
      }

      for( int ip = 0; ip < packs_.size(); ip++ ){
	ofs << setw(14) << packs_[ ip ]->name()  << flush;
      }
      ofs << endl;
      
      for( int i = 0; i < nx_; i++ ){
	double x, y;
	
	for( int ip = 0; ip < gPDF_.size(); ip++ ){
	  gPDF_[ip]->GetPoint( i, x, y );
	  ofs << setw(14) << x << setw(14) << y << flush;
	}
	
	for( int ip = 0; ip < gPacks_.size(); ip++ ){
	  gPacks_[ ip ]->GetPoint( i, x, y );
	  ofs << setw(14) << y << flush;
	}
	
	ofs << endl;
	
      }
    }
    c_->WaitPrimitive();
  }
  
  TGraph* newGraph( const int& color ){
    TGraph *g = new TGraph( nx_ );
    g->SetMarkerColor( color );
    g->SetLineColor( color );
    g->SetLineWidth( 1 );
    g->SetLineStyle( 2 );
    return g;
  }
  
  void checkY( const double& y ){
    ymin_ = ( y < ymin_ ? y : ymin_ );
    ymax_ = ( y > ymax_ ? y : ymax_ );
  }
  
  void createPDF( const string& opt ){
    if( ! args_.hasOpt( opt ) ) return;
    
    polPDF::Evo *evo = new polPDF::Evo( args_, opt );
    
    TGraph *g   = new TGraph( nx_ );
    g->SetLineWidth( 2 );
    g->SetLineStyle( 1 );
    legend_->AddEntry( g, opt.c_str(), "l" );
    
    pPDF_.push_back( evo );
    g->SetLineColor( pPDF_.size() + 1 );
    gPDF_.push_back( g );
    
    if( isParton_ ){
      pdf_.push_back( new Evolution::PDFx( evo, parton_,
					   args_.get( "leg1", 6 ),
					   args_.get( "leg2", 8 ),
					   args_.get( "precision", 5.0E-3 ),
					   args_.get( "length", 15.0 ),
					   args_.get( "offset", 2.5 ),
					   args_.get( "angle",  0.6 )
					   ) );
    } else {
      pdf_.push_back( new Evolution::PDFx( evo, type_,
					   args_.get( "leg1", 6 ),
					   args_.get( "leg2", 8 ),
					   args_.get( "precision", 5.0E-3 ),
					   args_.get( "length", 15.0 ),
					   args_.get( "offset", 2.5 ),
					   args_.get( "angle",  0.6 )
					   ) );
    }

    Evolution::PDFx *pdfx = pdf_.back();
    QCD::PdfBase *p = pdfx->pdf().parameters();
    if( p != NULL ){
      for( QCD::PdfBase::iterator itr = p->begin();
	   itr != p->end(); itr++ ){
	
	ARG_DETAIL << setw(2)  << itr->first 
		   << setw(12) << itr->second.value()
		   << endl;

      }
      /*
      for( double x = 0.1; x < 1.0; x += 0.1 ){
	ARG_LOG << setw(6) << x
		<< setw(12) << pow( x, weight_ ) *  p->x( x ) 
		<< endl;
      }
      */
    }
    
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
  vector< polPDF::Evo* > pPDF_;
  vector< Evolution::PDFx* > pdf_;
  vector< TGraph* > gPDF_;
  string datafile_;
  
};


using namespace std;

int main( int argc, char* argv[] ){

  gROOT->SetStyle( "Plain" );
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  MyApp app( argc, argv );;
  
  
  app.Run();

  return 0;
}
