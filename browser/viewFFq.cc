/*!
  \brief compare FF among different flavors.....

  Available options are listed with --Help option.


 */
// standard C++ and STL
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

// ------- Mellin package
#include "Utility/Arguments.hh"
#include "QCD/Flavor.hh"
#include "QCD/PdfParametrization.hh"
#include "Fragmentation/Evolution.hh"
#include "Evolution/PDFx.hh"

// ------- ROOT package
#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <Rtypes.h>

// ------- local tools
#include "Colors.hh"


// namespace
using namespace std;

// ---------------------------------------------------------------  //
//                         MyGraph                                  //
// ---------------------------------------------------------------  //
class MyGraph : public TGraph {
public:
  MyGraph( Evolution::PDFx *pdf = NULL, const double& w = 1.0 );
  virtual ~MyGraph();
  
  void calc( const int& n, const double& xmin, const double& xmax,
	     const bool log );
  
  double max() { return ymax_; }
  double min() { return ymin_; }
  string name();
  void weight( const double& w ) { weight_ = w; }
  
private:
  int color();
  Evolution::PDFx *pdf_;
  
  double ymin_;
  double ymax_;
  double weight_;
  
};

MyGraph::MyGraph( Evolution::PDFx *pdf, const double& w ) : 
  TGraph(),
  pdf_( pdf ),
  ymin_( 1E+5 ), ymax_( -1E+5 ), weight_( w )
{
  this->SetLineWidth( 2 );
  this->SetLineStyle( 1 );
  this->SetLineColor( this->color() );
}

MyGraph::~MyGraph() {
  if( pdf_ ) delete pdf_;
}

string MyGraph::name() {
  if( pdf_ == NULL ) return "none";
  return pdf_->pdf().name();
}

int MyGraph::color(){
  if( pdf_ == NULL ) return 1;
  return ( pdf_->pdf().type() == QCD::PdfBase::parton ?
	   Colors::id( pdf_->pdf().getPID() ):
	   Colors::id( pdf_->pdf().getTID() ) );
}

void MyGraph::calc( const int& n, const double& xmin, const double& xmax,
		    const bool log ){
  
  if( pdf_ == NULL || n < 1 ) return;
  this->Set( n ); 
  
  double x1 = ( log ? log10( xmin ) : xmin );
  double x2 = ( log ? log10( xmax ) : xmax );
  double dx = ( x2 - x1 ) / n;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  Evolution::Kernel *kernel =
    dynamic_cast< Evolution::Kernel * >( pdf_->pdf().kernel() );
  
  args.log() << kernel->initialDistribution()->path() << endl;
  args.log() << "Parton type: " << pdf_->pdf().name() << endl;
  
  for( int i = 0; i < n; i++ ) args.log() << "-" << flush;
  args.log() << endl;

  
  for( int i = 0; i < n; i++ ){
    double x = x1 + i * dx;
    if( log ) x = pow( 10, x );
    double y = pow( x, weight_ ) * (*pdf_)( x );

    this->SetPoint( i, x, y );
    if( y < ymin_ ) ymin_ = y;
    if( y > ymax_ ) ymax_ = y;
    
    args.log() << "*" << flush;
    
  }
  args.log() << endl;

}


// ---------------------------------------------------------------  //
//                         MyApp                                    //
// ---------------------------------------------------------------  //
class MyApp : public TApplication {
public:
  MyApp() throw( int ) ;
  virtual ~MyApp();
  virtual void Run( Bool_t retrn = kFALSE );
  
private:
  TCanvas *c_;
  
  vector< string > candidates_;
  vector< int >    pid_;                      // parton IDs
  vector< bool >   isParton_;                 // parton id or type id ?
  vector< Fragmentation::Evo* > evo_;         // FF objects
  vector< vector< MyGraph* > > xpdf_;         // PDFx object
  
  double q2_;
  double zmin_;
  double zmax_;
  int    nz_;
  bool   logz_;
  
  TLegend *legFF_;
  TLegend *legQ_;
  
  TLine *line_;
  
  double weight_;
  
  void check_pid();
  void check_candidate();
  QCD::Flavor::HADRON check_hadron( const string& name );
  
  void create_ff() throw( int ) ;
  void create_pdf() throw( int ) ;

  void calculation();

  void frame();
  void legend();

  void usage();
  
  static vector< string > optPI;
  static vector< string > optKA;
  static vector< string > optPR;
  static vector< string > optRE;
  
};

vector< string > MyApp::optPI = Fragmentation::Evo::options( QCD::Flavor::PIp );
vector< string > MyApp::optKA = Fragmentation::Evo::options( QCD::Flavor::KAp );
vector< string > MyApp::optPR = Fragmentation::Evo::options( QCD::Flavor::PRp );
vector< string > MyApp::optRE = Fragmentation::Evo::options( QCD::Flavor::REp );

MyApp::MyApp() throw( int ):
  TApplication( "MyApp", 
		Utility::Arguments::ref().argcPtr(),
		Utility::Arguments::ref().argv() ),
  c_( NULL ),
  candidates_( 0 ),
  pid_( 0 ), isParton_( 0 ),
  evo_( 0 ), xpdf_( 0 ),
  q2_( 1.0 ), zmin_( 0.01 ), zmax_( 0.9 ), nz_( 40 ), logz_( false ),
  weight_( 1.0 )
{
  
  this->check_candidate();
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( args.hasOpt( "help" ) || args.hasOpt( "HELP" ) ||
      args.hasOpt( "Help" ) || args.hasOpt( "?" ) ){
    this->usage();
    throw( 0 );
  }

  // check log or not
  logz_ = args.hasOpt( "logz" );
  
  // check weight power for FF
  weight_ = args.get( "weight", weight_ );
  
  this->check_pid();
  if( pid_.size() == 0 ){
    args.err() << "you have to specify at least one parton type" << endl;
    throw( 1 );
  }
  
  try {
    this->create_ff();
  } 
  catch( int err ){
    throw err;
  }
  
  if( evo_.size() == 0 ) {
    args.log( Utility::Arguments::ERROR ) 
      << "You have to specify at least one FF data." << endl;
    throw( 2 );
  }
  
  try {
    this->create_pdf();
  }
  catch( int err ){
    throw err;
  }
  
  this->calculation();
  
  c_ = new TCanvas( "MyApp", "", 800, 600 );
  if( logz_ ) c_->SetLogx();
  
  line_ = new TLine();
  line_->SetLineWidth( 1 );
  line_->SetLineColor( kGray );


}

MyApp::~MyApp() {
  for( int i = 0; i < evo_.size(); i++ ) if( evo_[ i ] ) delete evo_[ i ];
  for( int i = 0; i < xpdf_.size(); i++ ) 
    for( int j = 0; j < xpdf_[ i ].size(); j++ )     
      if( xpdf_[ i ][ j ] ) delete xpdf_[ i ][ j ];
}

void MyApp::frame() {

  Utility::Arguments& args = Utility::Arguments::ref();

  double ymin = 0.0;
  double ymax = 0.0;
  
  // check y range
  for( int i = 0; i < xpdf_.size(); i++ ) 
    for( int j = 0; j < xpdf_[ i ].size(); j++ ) {
      MyGraph& g = *( xpdf_[i][j] );
      if( i == 0 && j == 0 ){
	ymin = g.min();
	ymax = g.max();
      } else {
	if( ymin > g.min() ) ymin = g.min();
	if( ymax < g.max() ) ymax = g.max();
      }
    }
  
  double dy = 0.05 * ( ymax - ymin );
  ymax += dy;
  ymin -= dy;
  
  TH1 *frame = c_->DrawFrame( zmin_, ymin, zmax_, ymax );
  line_->SetLineStyle( 2 );
  line_->DrawLine( zmin_, 0.0, zmax_, 0.0 );
  
  vector< double > zmarker( 2 );
  zmarker[ 0 ] = 0.05;
  zmarker[ 1 ] = 0.8;
  zmarker = args.get( "marker", ",", zmarker );
  
  line_->SetLineStyle( 3 );
  for( int i = 0; i < zmarker.size(); i++ )
    line_->DrawLine( zmarker[ i ], ymin, zmarker[ i ], ymax );

  // axis title
  frame->GetXaxis()->SetTitle( "z" );
  
  ostringstream ostr;
  if( weight_ != 0.0 ){
    ostr << "z";
    if( weight_ != 1.0 ) ostr << "^{" << weight_ << "} ";
  }
  ostr << "D( z, Q^{2} = " << q2_ << " GeV^{2})" ;
  frame->GetYaxis()->SetTitle( ostr.str().c_str() );
  
}

void MyApp::legend() {
  
  // ---- Legend
  int nevo = xpdf_.size();
  double ybot = 0.89 - 0.04 * xpdf_.size();
  legFF_ = new TLegend( 0.5, ybot, 0.895, 0.895 );
  
  double ytop = ybot - 0.01;
  ybot = ytop - 0.03 * pid_.size();
  legQ_ =  new TLegend( 0.8, ybot, 0.895, ytop );
  
  legFF_->SetFillStyle( 0 );  // set fill style to hollow
  legQ_->SetFillStyle( 0 );   // set fill style to hollow
  
  // FF legend
  for( int i = 0; i < xpdf_.size(); i++ ){
    if( xpdf_[ i ].size() == 0 ) continue;
    legFF_->AddEntry( xpdf_[ i ][ 0 ], 
		      evo_[ i ]->initialDistribution()->path().c_str(),
		      "l" );
  }
  
  // flavor legend
  for( int j = 0; j < xpdf_[ 0 ].size(); j++ ){
    MyGraph *g = xpdf_[ 0 ][ j ];
    legQ_->AddEntry( g, g->name().c_str(), "l" );
  }
  
  legFF_->Draw();
  legQ_->Draw();
  
}


void MyApp::calculation() {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  q2_   = args.get( "Q2",   1.0 );
  zmin_ = args.get( "zmin", 0.01 );
  zmax_ = args.get( "zmax", 0.9 );
  nz_   = args.get( "nz",   40  );
  
  args.log() << "Q2: " << q2_ << endl;
  
  for( int i = 0; i < evo_.size(); i++ ) if( evo_[ i ] ) evo_[ i ]->setQ2( q2_ );
  for( int i = 0; i < xpdf_.size(); i++ ) 
    for( int j = 0; j < xpdf_[ i ].size(); j++ )     
      if( xpdf_[ i ][ j ] ) xpdf_[ i ][ j ]->calc( nz_, zmin_, zmax_, logz_ );
  
}

void MyApp::Run( Bool_t retrn ){
  
  if( xpdf_.size() == 0 ) return ;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  this->frame();   // draw frame
  this->legend();                      // draw legend
  
  // draw FF lines
  for( int i = 0; i < xpdf_.size(); i++ ) 
    for( int j = 0; j < xpdf_[ i ].size(); j++ )     
      if( xpdf_[ i ][ j ] ) xpdf_[ i ][ j ]->Draw( "l" );
  
  
  // print canvas image to a file
  ostringstream ostr;
  ostr << "ffq_" << q2_ << "_" << zmin_ << "_" << zmax_;
  
  string ofile = 
    args.get( "output", ostr.str() ) + "." + args.get( "format", "png" );
  
  c_->Print( ofile.c_str() );
  
  // wait primitive
  if( args.hasOpt( "wait" ) ) c_->WaitPrimitive();

}

void MyApp::check_pid(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > partonStr( 1, "gluon" );
  partonStr = args.get( "parton", ",", partonStr );
  for( int i = 0; i < partonStr.size(); i++ ){
    pid_.push_back( QCD::Flavor::id( partonStr[ i ] ) );
    isParton_.push_back( QCD::Flavor::isParton( partonStr[ i ] ) );
  }
  
}

void MyApp::create_pdf() throw( int ) {
  
  xpdf_.resize( evo_.size(), vector< MyGraph* >( pid_.size(), NULL ) );
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  vector< int > leg( 2 );
  leg[ 0 ] = 6;
  leg[ 1 ] = 8;
  leg      = args.get( "leg", ",", leg );
  
  args.log() 
    << "Legendre dimentions: --leg=" << leg[ 0 ] << "," << leg[ 1 ] << endl;

  
  double precision = args.get( "precision", 5.0E-3 );
  double length = args.get( "length", 15.0 );
  double offset = args.get( "offset", 3.0 );
  double angle  = args.get( "angle", 0.55 );
  
  for( int i = 0; i < evo_.size(); i++ ){
    for( int j = 0; j < pid_.size(); j++ ){
      
      Evolution::PDFx *pdf = NULL;
      bool parton = isParton_[ j ]; 
      int& id     = pid_[ j ];
      try {
	pdf = ( parton ?
		new Evolution::PDFx( evo_[ i ], (QCD::Flavor::PARTON) id,
				     leg[ 0 ], leg[ 1 ], precision,
				     length, offset, angle ) :
		new Evolution::PDFx( evo_[ i ], (QCD::Flavor::TYPE) id,
				     leg[ 0 ], leg[ 1 ], precision,
				     length, offset, angle ) );
      } 
      catch ( int err ){
	throw err;
      }
      if( pdf == NULL ){
	args.err() 
	  << "failing to create xPDF object for "
	  << ( parton ? 
	       QCD::Flavor::name( (QCD::Flavor::PARTON) id ) :
	       QCD::Flavor::name( (QCD::Flavor::TYPE) id ) )
	  << endl;
	throw 1;
      }
      xpdf_[ i ][ j ] = new MyGraph( pdf, weight_ );

    }
  }
  
  vector< int > lstyle( xpdf_.size(), 1 );
  for( int i = 0; i < xpdf_.size(); i++ ) lstyle[ i ] = i + 1;
  lstyle = args.get( "lstyle", ",", lstyle );
  
  vector< int > lcolor( pid_.size(), 1 );
  if( xpdf_.size() > 0 ){
    for( int j = 0; j < xpdf_[ 0 ].size(); j++ ) 
      lcolor[ j ] = xpdf_[ 0 ][ j ]->GetLineColor();
  }
  lcolor = args.get( "lcolor", ",", lcolor );
  
  
  for( int i = 0; i < xpdf_.size(); i++ ){
    for( int j = 0; j < xpdf_[ i ].size(); j++ ){
      xpdf_[ i ][ j ]->SetLineStyle( lstyle[ i ] );
      xpdf_[ i ][ j ]->SetLineColor( lcolor[ j ] );
    }
  }

}

void MyApp::create_ff() throw( int ) {

  Utility::Arguments& args = Utility::Arguments::ref();
  
  for( int i = 0; i < candidates_.size(); i++ ){
    
    string& name = candidates_[ i ];
    
    if( ! args.hasOpt( name ) ) continue;
    
    try {
      
      Fragmentation::Evo *evo = 
	new Fragmentation::Evo( args, this->check_hadron( name ), name );
      
      if( evo == NULL ) {
	args.err() << "failing to create Fragmentation::Evo object" << endl;
	throw( 1 );
      }
      
      evo_.push_back( evo );
      
    }
    catch( int err ) {
      throw( err );
    }
  }
  
}

void MyApp::check_candidate(){
  
  candidates_.insert( candidates_.end(), optPI.begin(), optPI.end() );
  candidates_.insert( candidates_.end(), optKA.begin(), optKA.end() );
  candidates_.insert( candidates_.end(), optPR.begin(), optPR.end() );
  candidates_.insert( candidates_.end(), optRE.begin(), optRE.end() );

}

QCD::Flavor::HADRON MyApp::check_hadron( const string& name ){
  
  if( find( optKA.begin(), optKA.end(), name ) != optKA.end() ) 
    return QCD::Flavor::KAp;
  if( find( optPR.begin(), optPR.end(), name ) != optPR.end() ) 
    return QCD::Flavor::PRp;
  if( find( optRE.begin(), optRE.end(), name ) != optRE.end() ) 
    return QCD::Flavor::REp;
  
  return QCD::Flavor::PIp;
}

void MyApp::usage(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  for( int i = 0; i < candidates_.size(); i++ ){
    args.usage( candidates_[ i ], "", "FF parameter xml file" );
  }
  
  args.usage( "Q2", "1.0", "Q^2 scale value" );
  args.usage( "weight", "1.0", "weight for FF" );

  args.usage( "zmin", "0.01", "minimum z value");
  args.usage( "zmax", "0.9",  "maximum z value");
  args.usage( "nz", "40", "number of z bin" );
  
  args.usage( "leg", "6,8", "Dimention of Lagurange Quadrature" );
  args.usage( "precision", "1.E-3", "numerical calculation precision" );
  args.usage( "length", "15.0", "length of mellin-inversion path" );
  args.usage( "angle", "0.55", "angle of mellin-inversion path" );
  args.usage( "offset", "3.0", "offset of mellin-inversion path" );

  args.usage( "parton", "gluon", "flavors to be drawn. (separated with ,)" );
  
  args.usage( "lstyle", "", "line style separated by ," );
  args.usage( "lcolor", "", "line color separated by ," );

  args.usage( "gstyle", "Plain", "gROOT style" );
  
  args.usage( "output", "", "output filename without suffix" );
  args.usage( "format", "png", "output drawing format" );

  args.usage( "wait", "", "call WaitPrimitive() at the end" );

  args.usage( "marker", "0.05,0.8", "vertical z markers" );
  
  args.usage( "help", "", "Print Help" );
  args.usage( "Help", "", "Print Help" );
  args.usage( "HELP", "", "Print Help" );
  args.usage( "?",    "", "Print Help" );
  
}

// ---------------------------------------------------------------  //
//                         main routine                             //
// ---------------------------------------------------------------  //
int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );

  try {
    MyApp app;
    app.Run();
  }
  catch ( int err ){
  }
  
  return 0;
}

