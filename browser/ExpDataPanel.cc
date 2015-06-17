//
// ExpDataPanel.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Sep 26 15:50:12 2008 Yoshiyuki Miyachi
// Started on  Fri Sep 26 15:50:12 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <TH1.h>
#include <TGraphErrors.h>

#include <ExpData/DataSet.hh>

#include "ExpDataPanel.hh"

using namespace std;

ExpDataPanel::ExpDataPanel( const Exp::ExpData& ed, 
			    const string& name, 
			    const string& xkey,
			    const string& xkey2 ) : 
  TCanvas( ( name + "_" + xkey ).c_str(),
	   ( name + " vs " + xkey ).c_str()  ),
  xkey_( xkey ), xkey2_( xkey2 ), graphs_( 0 ), addgraphs_( 0 ),
  xmin_( 0.0 ), xmax_( 0.0 ), ymin_( 0.0 ), ymax_( 0.0 ),
  frame_( NULL )
{
  this->load( ed );
}

ExpDataPanel::~ExpDataPanel(){
  for( int i = 0; i < graphs_.size(); i++ ) if( graphs_[ i ] ) delete graphs_[ i ];
}

void ExpDataPanel::load( const Exp::ExpData& ed ){
  vector< Exp::DataSet > dset = ed.data();
  for( int i = 0; i < dset.size(); i++ ){ 
    this->load( dset[ i ] );
  }
}

void ExpDataPanel::load( const Exp::DataSet& ds ){
  if( ds.size() == 0 ) return;

  TGraphErrors *graph = new TGraphErrors( 0 ); 
  Exp::Kinematic ref( xkey_, 0.0, "" );
  Exp::Kinematic ref2( xkey2_, 0.0, "" );

  for( int i = 0; i < ds.size(); i++ ){
    
    list< Exp::Kinematic > kine = ds[ i ].kinematic();
    list< Exp::Kinematic >::iterator itr = find( kine.begin(), kine.end(), 
						 ref );
    list< Exp::Kinematic >::iterator itr2;
    if( xkey2_ != "" ) itr2 = find( kine.begin(), kine.end(), ref2 );
    
    if( itr == kine.end() ) continue;
    if( xkey2_ != "" ) if( itr2 == kine.end() ) continue;
    int n = graph->GetN();
    
    double x  = itr->data();
    double x2 = xkey2_ != "" ? itr2->data() : x;
    if( x > x2 ){ double tmp( x ); x = x2; x2 = tmp; }
    double y  = ds[ i ].data();
    double dy = ds[ i ].stat();

    if( xmin_ == xmax_ ){
      xmin_ = ( x > 0.0 ? 0.9 : 1.1 ) * x ;
      xmax_ = ( x > 0.0 ? 1.1 : 0.9 ) * x2;
    } else {
      xmin_ = ( x < xmin_ ? x : xmin_ );
      xmax_ = ( xmax_ < x2 ? x2 : xmax_ );
    }
    
    if( ymin_ == ymax_ ) {
	ymin_ = y - dy;
	ymax_ = y + dy;
    } else {
      ymin_ = ( y - dy < ymin_ ? y - dy : ymin_ );
      ymax_ = ( ymax_ < y + dy ? y + dy : ymax_ );
    }

    graph->Set( n + 1 );
    graph->SetPoint( n, 0.5 * ( x + x2 ) , y );
    graph->SetPointError( n, 0.0, dy );
    
  }

  // ---  set style 
  graph->SetMarkerStyle( 20 + graphs_.size() );
  graph->SetMarkerSize( 1.0 );
  graph->SetMarkerColor( 1 + graphs_.size() );
  graph->SetLineColor( 1 + graphs_.size() );
  graph->SetLineWidth( 1 );
  graphs_.push_back( graph );

}

void ExpDataPanel::draw( const std::string& filename ) {
  // --- resize to have margin
  double xmargin, ymargin;
  double fxmin, fxmax, fymin, fymax;
  Utility::Arguments& args = Utility::Arguments::ref();

  if( ! args.hasOpt( "logx" ) ){
    xmargin = 0.05 * ( xmax_ - xmin_ );
    fxmin = xmin_ - xmargin;
    fxmax = xmax_ - xmargin;
  } else {
    xmargin = exp( 0.05 * ( log( xmax_ ) - log( xmin_ ) ) );
    fxmin = xmin_ / xmargin;
    fxmax = xmax_ * xmargin;
  }
  
  if( ! args.hasOpt( "logy" ) ){
    ymargin = 0.05 * ( ymax_ - ymin_ );
    fymin = ymin_ - ymargin;
    fymax = ymax_ - ymargin;
  } else {
    ymargin = exp( 0.05 * ( log( ymax_ ) - log( ymin_ ) ) );
    fymin = ymin_ / ymargin;
    fymax = ymax_ * ymargin;
  }
  
  fxmin = args.get( "fxmin", fxmin );
  fxmax = args.get( "fxmax", fxmax );
  fymin = args.get( "fymin", fymin );
  fymax = args.get( "fymax", fymax );
  
  frame_ = this->DrawFrame( fxmin, fymin, fxmax, fymax );
  
  for( int i = 0; i < graphs_.size(); i++ ){
    graphs_[ i ]->Draw( "P" );
  }
  
  for( int i = 0; i < addgraphs_.size(); i++ ){
    addgraphs_[ i ]->SetLineColor( 1 + i );
    addgraphs_[ i ]->SetLineWidth( 1 );
    addgraphs_[ i ]->Draw( "L" );
  }
  
  this->Update();
  this->Print( filename.c_str() );

}

void ExpDataPanel::addEgraphs( const Exp::ExpData& ed,
			       const string& xkey, const string& xkey2 ){
  if( xkey != "" ){ xkey_ = xkey; xkey2_ = xkey2; }
  this->load( ed );
}

void ExpDataPanel::setXmin( const double& xmin ){
  xmin_ = xmin;
  if( frame_ ) frame_->GetXaxis()->SetRangeUser( xmin_, xmax_ );
}

void ExpDataPanel::setXmax( const double& xmax ){
  xmax_ = xmax;
  if( frame_ ) frame_->GetXaxis()->SetRangeUser( xmin_, xmax_ );
}

void ExpDataPanel::setYmin( const double& ymin ){
  ymin_ = ymin;
  if( frame_ ) frame_->GetYaxis()->SetRangeUser( ymin_, ymax_ );
}

void ExpDataPanel::setYmax( const double& ymax ){
  ymax_ = ymax;
  if( frame_ ) frame_->GetYaxis()->SetRangeUser( ymin_, ymax_ );
}


