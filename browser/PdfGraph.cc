#include "PdfGraph.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include <Utility/Timer.hh>
#include <Evolution/PDFx.hh>

#include "ScaleFactor.hh"

using namespace std;
using namespace Transform;
using namespace Exp;
using namespace Utility;
using namespace Evolution;
using namespace QCD;

PdfGraph::PdfGraph( RealFunction *f, 
		    double xmin, double xmax, int n, 
		    const double& weight,
		    const double& scale ) : 
  TGraphErrors( n ),
  f_( f ), w_( weight ), 
  minX_( xmin ), maxX_( xmax ), maxY_( 0.0 ), minY_( 0.0 ),
  sigmaT_(), f1_( sigmaT_ ), scale_( scale )
{
  this->update();
  this->SetLineWidth( 2 );
}

PdfGraph::PdfGraph( DataSet& eset, 
		    const double& weight, 
		    const double& scale,
		    const string& xlabel ) : 
  TGraphErrors( eset.size() ),
  f_( NULL ), w_( weight ), maxY_( 0.0 ), minY_( 0.0 ),
  sigmaT_(), f1_( sigmaT_ )
{
  
  bool useF1 = false; 
  string nucleon = eset.info().substr( 2, 1 );
  
  if( eset[0].info().size() > 6 && eset[0].info().substr( 4, 2 ) == "F1" ) {
    useF1 = true;
  }
  
  for( int i = 0; i < eset.size(); i++ ){
    
    double x  = 0.0;
    
    if( eset[i].info().substr( 0, 4 ) == "e+e-" ){
      x = 0.5 * ( eset[i].kine("zmin") + eset[i].kine("zmax") );
    } else {
      x = eset[i].kine("x");
    }
    
    double q2 = eset[i].kine("Q^2");
    double y  = scale * pow( x, w_ ) * eset[i].data() ;
    //    double dy = scale * pow( x, w_ ) * eset[i].stat() ;
    double dy = scale * pow( x, w_ ) * eset[i].error() ; // use full error
    
    if( useF1 ){
      y  *= f1_( x, q2 );
      dy *= f1_( x, q2 );
    }
    
    if( xlabel == "Q^2" ){
      ScaleFactor sf;
      y  += sf.offset( x );
      x   = q2;
    }
    
    this->SetPoint( i, x, y );
    this->SetPointError( i, 0.0, dy );
    if( i == 0 ) { maxY_ = y + dy; minY_ = y - dy ;}
    else {
      maxY_ = ( y + dy > maxY_ ? y + dy : maxY_ );
      minY_ = ( y - dy < minY_ ? y - dy : minY_ );
    } 
  }
  
  this->SetMarkerStyle( 20 );
  this->SetMarkerSize( 1.0 );
  this->SetMarkerColor( 2 );
}

PdfGraph::~PdfGraph( ){
}

string PdfGraph::legend( const std::string& type ) const {
  ostringstream ost;
  if( w_ != 0.0 ){
    ost << "x";
    if( w_ != 1.0 ) ost << "^{" << w_ << "}";
  }
  
  PDFx *pdf;
  //  polDIS::G1_x *g1;
  
  if( ( pdf = dynamic_cast< PDFx* >( f_ ) ) ){
    if( type == "polPDF" ) {
      if( pdf->type() == PdfBase::PdfBase::parton ) {
	ost << "#Delta ";
      }
      ost << pdf->name() << "(x)";
    } else if( type == "FF" ) {
      ost << "D_{" << pdf->name() << "}(z)";
    }
    //  } else if ( ( g1 = dynamic_cast< polDIS::G1_x* >( f_ ) ) ){
    //    ost << "g1(x)" << endl;
  }
  
  return ost.str();
}

void PdfGraph::update(){
  
  if( f_ == NULL ) return;
  
  double step = ( log10( maxX_ ) - log10( minX_ ) ) / this->GetN();
  Timer timer;
  timer.start();
  for( int i = 0; i < this->GetN(); i++ ){
    double x = pow( 10.0, log10( minX_ ) + step * i );
    double y = scale_ * pow( x, w_ ) * (*f_)( x );
    this->SetPoint( i, x, y );
    if( i == 0 ) {
      maxY_ = y; minY_ = y ;
    }
    else {
      maxY_ = ( y > maxY_ ? y : maxY_ );
      minY_ = ( y < minY_ ? y : minY_ );
    }
    cout << "*" << flush;
  }
  timer.stop();
  cout << "\tElasped time:  " << setw(10) << timer.check( Timer::Sec ) << " s" << endl;
}
