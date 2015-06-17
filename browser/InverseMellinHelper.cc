#include "InverseMellinHelper.hh"

#include <Utility/Arguments.hh>

#include <FragXsec_T/FragXsec.hh>
#include <unpolDIS/StrFunc.hh>
#include <unpolSIDIS/StrFunc.hh>
#include <polDIS/StrFunc.hh>
#include <polSIDIS/StrFunc.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFuncSub.hh>

#include <TLine.h>
#include <TH1.h>

#include <iostream>
#include <iomanip>

#include <sstream>
#include <cmath>

using namespace std;

InverseMellinHelper::InverseMellinHelper() :
  TCanvas( "InvMellinHeler" ),
  x_( 0 ), f_( 0 ), 
  fsum_( 0.0 ), msec_( 0.0 ),
  c_( 0.0 ), phi_( 0.0 ), l_( 0.0 ),
  timer_(),
  line_( NULL ),
  latex_( NULL ),
  q2_( 1.0 )
{
  
  line_  = new TLine();
  latex_ = new TLatex();

  this->SetFillColor( 14 );
  this->Draw();
}

InverseMellinHelper::~InverseMellinHelper() {
  
}

void InverseMellinHelper::add( const vector< double >& x ){
  this->add( x, vector< double >( 0 ) );
}

void InverseMellinHelper::add( const vector< double >& x, 
			       const vector< double >& z ){
  x_.push_back( x );
  z_.push_back( z );
  f_.resize( x_.size() );
}

void InverseMellinHelper::draw( const double& min, const double& max, 
				const int& n ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  this->cd();
  this->SetLogx( args.hasOpt( "logx" ) ? kTRUE : kFALSE );
  this->SetLogy( args.hasOpt( "logy" ) ? kTRUE : kFALSE );
  
  int mode = args.get( "mode", 1 );
  if( mode < 0 ) mode = 1;
  if( mode > 6 ) mode = 6;
  func_.set( ( RealFunctionWrapper::MODE ) mode );
  
  double dl = ( max - min ) / n;
  double ymin = 0.0; double ymax = 0.0;
  
  vector< TGraph* > vg( 0 );
  
  TLegend *legend = new TLegend( 0.82, 0.120, 0.98, 0.88 );
  legend->SetFillColor( 16 );
  
  int icolor = 1;
  
  Xsection::xXsec* xsec = func_.xsec();
  bool negative = ( dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec->moment()  ) != NULL &&
		    dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec->moment()  )->sub() == 1 );
  
  
  ARG_LOG << setw(10) << "x" << flush;
  ARG_LOG << setw(10) << "z" << flush;
  ARG_LOG << "  " << flush;
  for( int il = 0; il < n; il++ ) ARG_LOG << "-" ;
  ARG_LOG << endl;

  for( int ix = 0; ix < x_.size(); ix++ ){
    
    TGraph *g = new TGraph( 0 );
    g->SetLineWidth( 2 );
    
    timer_.start();
    
    if( x_[ ix ].size() > 0 ){
      for( int i = 0; i < x_[ ix ].size(); i++ ) 
	ARG_LOG << setw(10) << x_[ ix ][ i ] << flush ;
      ARG_LOG << flush;
    }
    
    if( z_[ ix ].size() > 0 ){
      for( int i = 0; i < z_[ ix ].size(); i++ ) 
	ARG_LOG << setw(10) << z_[ ix ][ i ] << flush ;
      ARG_LOG << flush;
    }
    
    // set Q2
    func_.Q2( q2_ );

    // set x and z to be calculated
    func_.set( x_[ ix ], z_[ ix ] );
    
    ARG_LOG << "  " << flush;
    
    for( int il = 0; il < n; il++ ){
      
      double length = min + il * dl ;
      double y = ( negative ? -1.0 : 1.0 ) * func_( length );
      
      if( y < 0.0 ) {
	ARG_LOG << "X" << flush;
	continue;
      }
      
      if( ymin == ymax ) {
	ymin = ( y > 0 ? 0.9 : 1.1 ) * y;
	ymax = ( y > 0 ? 1.1 : 0.9 ) * y;
      } else {
	ymin = ( y < ymin ? y : ymin );
	ymax = ( y > ymax ? y : ymax );
      }
      
      g->Set( g->GetN() + 1 );
      g->SetPoint( g->GetN() - 1, length, y );
      
      ARG_LOG << "O" << flush;
      
    }
    
    timer_.stop();
    
    ARG_LOG
      << "  :"
      << setw(14) << timer_.check( Utility::Timer::Msec )
      << " ms" 
      << endl;
    
    if( g->GetN() > 0 ) {
      g->SetLineColor( icolor++ );
      vg.push_back( g ); 
      ostringstream ost;
      
      if( x_[ ix ].size() > 0 ){
	ost << "x:" << flush; 
	if( x_[ ix ].size() > 1 ) ost << "(" ;
	for( int i = 0; i < x_[ ix ].size(); i++ ){
	  ost << x_[ix][i] << endl;
	  if( i != x_[ix].size() - 1 ) ost << ",";
	}
	if( x_[ ix ].size() > 1 ) ost << ")" ;
      }
      
      if( z_[ ix ].size() > 0 ){
	ost << " z:" << flush; 
	if( z_[ ix ].size() > 1 ) ost << "(" ;
	for( int i = 0; i < z_[ ix ].size(); i++ ){
	  ost << z_[ix][i] << endl;
	  if( i != z_[ix].size() - 1 ) ost << ",";
	}
	if( z_[ ix ].size() > 1 ) ost << ")" ;
      }

      legend->AddEntry( g, ost.str().c_str(), "l" );
    }
    
  }
  
  double xmargin = 0.05 * ( max - min );
  double ymargin = 0.05 * ( ymax - ymin );
  
  double xAxisMax = max + 2.0 * xmargin;
  double xAxisMin = min - xmargin;
  double yAxisMax = ymax + ymargin;
  double yAxisMin = ymin - ymargin;
  
  if( args.hasOpt( "logx" ) && xAxisMin < 0.0 ) xAxisMin = 1.0E-3;
  if( args.hasOpt( "logy" ) && yAxisMin < 0.0 ) yAxisMin = 1.0E-3;
  
  TH1 *frame = this->DrawFrame( xAxisMin, yAxisMin, xAxisMax, yAxisMax );
  switch( mode ){
  case 1: 
  case 2:
    frame->GetXaxis()->SetTitle( "integration path length" );
    break;
  case 3: 
  case 4:
    frame->GetXaxis()->SetTitle( "integration path angle" );
    break;
  case 5: 
  case 6:
    frame->GetXaxis()->SetTitle( "integration path offset" );
    break;
  }
  
  for( int ig = 0; ig < vg.size(); ig++ ) vg[ ig ]->Draw();
  legend->Draw();
  
  this->Update();
  
  ostringstream ostr;
  ostr << "helper_" ;

  if( dynamic_cast< FragXsec_T::FragXsec* >( xsec->moment() ) ){
    ostr << "xsec_";
  } else if( dynamic_cast< unpolDIS::StrFunc* >(  xsec->moment() ) ){
    ostr << "unpolDIS_";
  } else if( dynamic_cast< polDIS::StrFunc*   >(  xsec->moment() ) ){
    ostr << "polDIS_";
  } else if( dynamic_cast< unpolSIDIS::StrFunc* >( xsec->moment() ) ){
    ostr << "unpolSIDIS_";
  } else if( dynamic_cast< polSIDIS::StrFunc*   >( xsec->moment() ) ){
    ostr << "polDIS_";
  } else if( dynamic_cast< unpolDISXsec::StrFunc* >( xsec->moment() ) ){
    ostr << "unpolDISXsec_";
  } else if( dynamic_cast< unpolSIDISXsec::StrFunc* >( xsec->moment()  ) ){
    ostr << "unpolSIDISXsec_";
  } else if( dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec->moment()  ) ){
    ostr << "unpolSIDISXsecSub" 
	 << dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec->moment()  )->sub()
	 << "_";
  }
  
  ostr << mode ;  
  ostr << "_" << min << "_" << max << "_" << q2_ << ".png";
  
  this->Print( args.get( "output", ostr.str() ).c_str() ); 
  
  this->WaitPrimitive();
}
