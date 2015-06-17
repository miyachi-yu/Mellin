#include "MomentView.hh"

#include <iomanip>
#include <iostream>
#include <TH2.h>
#include <TAxis.h>

#include <Utility/Timer.hh>

using namespace std;
using namespace QCD;
using namespace Transform;
using namespace Evolution;
using namespace Utility;

MomentView::MomentView( Kernel *kernel, const Flavor::TYPE& q ) : 
  TCanvas( "Moment", "Mellin Space", 800, 600 ),
  k_( kernel ), fcn_( NULL ), q_( NULL ), xsec_( NULL ), nspace_( NULL ) {
  q_ = new PDF( kernel, q );
}

MomentView::MomentView( Kernel *kernel, const Flavor::PARTON& q ) : 
  TCanvas( "Moment", "Mellin Space", 800, 600 ),
  k_( kernel ), fcn_( NULL ), q_( NULL ), xsec_( NULL ), nspace_( NULL ) {
  q_ = new PDF( kernel, q );
}

MomentView::MomentView(  Xsection::Xsec* fcn ) :
  TCanvas( "Moment", "Mellin Space", 800, 600 ),
  k_( NULL ), fcn_( NULL ), q_( NULL ), xsec_( fcn ), nspace_( NULL ) {
}

MomentView::~MomentView() {
  if( q_ ) delete q_;
}

void MomentView::draw( string type ){
  
  nspace_ = new TH2F( "nspace", "", 150, -6.0, 6.0, 100, -4.0, 4.0 );
  nspace_->SetStats( kFALSE );
  this->Draw();
  
  Timer timer;
  double spend = 0.0;
  
  for( int ix = 1; ix <= nspace_->GetXaxis()->GetNbins(); ix++ ){
    
    timer.start();

    for( int iy = 1; iy <= nspace_->GetYaxis()->GetNbins(); iy++ ){
      
      double real = nspace_->GetXaxis()->GetBinCenter( ix );
      double imag = nspace_->GetYaxis()->GetBinCenter( iy );
      
      complex< double > n( real, imag );
      complex< double > cv( 0.0, 0.0 );
      
      if( imag == 0.0 && floor( real ) == real ){
      } else {
	if( q_ ) cv = (*q_)( n );
	else if( xsec_ ) cv = (*xsec_)( n );
      }
      
      //      cout << setw(18) << n << setw(18) << cv << endl;
      double z = 0.0;
      
      if( type == "real" ) z = cv.real() ;
      else if( type == "imag" ) z = cv.imag() ;
      else if( type == "abs" ) z = abs( cv ); 
      else if( type == "arg" ) z = arg( cv );

      nspace_->Fill( real, imag, z );
      
    }
    nspace_->Draw("CONTZ");
    this->Update();
    
    timer.stop();
    spend += timer.check( Timer::Sec );
    double ratio = 1.0 * ix / nspace_->GetXaxis()->GetNbins();
    
    cout << setw(12) << 100.0 * ratio << " %: "
	 << "spend ( " << setw(10) << spend << " s)"
	 << " left ( " << setw(10) << spend / ratio - spend << " s)" << endl;
  }
  
  this->WaitPrimitive(); 
}

