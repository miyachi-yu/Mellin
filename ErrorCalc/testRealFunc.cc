#include "Utility/Arguments.hh"  // for arguments handling
#include "unpolPDF/Evolution.hh" // for PDF evolution
#include "Evolution/PDFx.hh"     // for pdf in x-spaace
#include "ExpData/ExpData.hh"    // for experimental data handling
#include "QCD/PdfErrMatrix.hh"
#include "ErrCalc.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class UDratio : public Transform::RealFunction {
public: 
  
  UDratio( unpolPDF::Evo *pdf ) : 
    RealFunction(), pdf_( pdf ), ubar_( NULL ), dbar_( NULL )
  {
  }
  
  virtual ~UDratio() {
  }
  
  //! calculate f(x) at the given x
  virtual double operator() ( const double& x ) {
    return ( ubar_ && dbar_ ? (*dbar_)( x )/(*ubar_)(x) : 1.0 );
  }
  
  void ubar( Evolution::PDFx *pdfx ) { ubar_ = pdfx; }
  void dbar( Evolution::PDFx *pdfx ) { dbar_ = pdfx; }

  Evolution::PDFx* ubar(){ return ubar_; }
  Evolution::PDFx* dbar(){ return dbar_; }

  void pdf( unpolPDF::Evo *p ) { pdf_ = p; }
  unpolPDF::Evo* pdf() { return pdf_; }
  
  void setQ2( const double& q2 ) { pdf_->setQ2( q2 ); }
  
private:
  
  unpolPDF::Evo *pdf_;
  Evolution::PDFx* ubar_;
  Evolution::PDFx* dbar_;

};

int main( int argc, char* argv[] ){

  // initiate argument list handling object
  // This is required to use Mellin package
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  unpolPDF::Evo *pdf = NULL;
  try{
    // initiate unpolPDF objects
    // it requires one of the following options
    //   --unpolPDF=[standard unpol PDF xml file]
    //   --CTEQ=CTEQ6.xml
    //   --MSTW=MSTW2008.xml
    //   --MRST=MRST2001.xml
    //   see unpolPDF/Evolution.cc for details
    //   possible options are specified in Evo::options() function
    //
    //  *** this object will be destroyed automaticall at "exit".
    //  *** NEVER delete this object
    pdf = unpolPDF::Evo::instance();
    
  } catch ( int error ) {
    cerr << "One of the following options is required." << endl;
    vector< string > opts = unpolPDF::Evo::options();
    for( int i = 0; i < opts.size(); i++ ) 
      cerr << "--" << opts[ i ] << "=[corresponding xml file]" << endl;
    return 1;
  }
  
  // The above PDF returns value in the Mellin space.
  // Converte is required to obtain value in the x-Bjorken space.
  // Mellin integral parameters
  double precision = args.get( "precision", 1.0E-4 );
  double length    = args.get( "length",    18.0 );
  double offset    = args.get( "offset",    2.6 );
  double angle     = args.get( "angle",     0.55 );
  
  // pdfs in x-Bjorken space
  Evolution::PDFx ubar( pdf, QCD::Flavor::ubar, 6, 8, 
			precision, length, offset, angle );

  Evolution::PDFx dbar( pdf, QCD::Flavor::dbar, 6, 8, 
			precision, length, offset, angle );
  
  UDratio ratio( pdf );
  ratio.ubar( &ubar );
  ratio.dbar( &dbar );

  ErrorCalc::ErrCalc eCalc( pdf );
  eCalc.setRealFunc( &ratio );
  eCalc.rfunccalc( true );
  eCalc.setPDFcalc( false ); 
 
  // setup x range
  // -- get number of x point from arguments with --xn option.
  vector< double > x( args.get( "xn", 100 ), 0.0 );
  double xmin = args.get( "xmin", 0.01 ); // get xmin value from arguments
  double xmax = args.get( "xmax", 0.9  ); // get xmax value from arguments
  double dx = ( xmax - xmin ) / x.size();
  
  for( int i = 0; i < x.size(); i++ ) x[ i ] = xmin + i * dx;
  
  // set Q^2 with value specified with --Q2 option from argumnts list
  vector< double > q2( 0 );
  q2.push_back(   2.0 );
  q2.push_back(   5.0 );
  q2.push_back(  10.0 );
  q2.push_back(  20.0 );
  q2.push_back(  50.0 );
  q2.push_back( 100.0 );
  
  Exp::ExpData expdata( "E866_PRD64_052002_pdf_ratio.xml" );
  Exp::DataSet& dset = expdata.data()[ 0 ];
  
  for( int i = 0; i < dset.size(); i++ ){
    Exp::DataPoint& p = dset[ i ];
    
    double x   = p.kine( "x" );
    double q2  = pow( p.kine("M"), 2 );

    ratio.setQ2( q2 );
    cout 
      << setw(12) << x
      << setw(12) << q2
      << setw(16) << ratio( x )
      << setw(16) << sqrt( eCalc( x ) )
      << endl;

  }

  return 0;
}
