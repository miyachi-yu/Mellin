#include <iostream> // cout(cin)

#include <TGraph.h>
#include <TF1.h>
#include <Utility/Arguments.hh>

#include "TGraVmk.hh"

namespace forFit{
  Double_t forfit( const Double_t* x, const Double_t* par ); // for TF1
  std::vector< double > fparv_( 6, 0.0 ); // parameters of a polynominal
  //  std::vector< double > fparv_( 5, 0.0 ); // parameters of a polynominal
  int parsize_;
  std::vector< double > fparv_act_( 0 );
}

using namespace std;

TGraVmk::TGraVmk() :
  vector< TGraph* >( 0 ), 
  xlimit_( 1000 ), ylimit_( 0.0 ), yfrag_( false ),
  xupperlimit_( xlimit_ ), xlowerlimit_( - xlimit_ ), 
  yfrag2_( false ),
  tminmax_( vector< double >( 4, 0.0 ) ),
  //  tminmax_( 4, 0.0 )
  vminmax_( 0 ), sampleTG_( 0 ), vinfo_( 0 ),
  vinfo2_( 0 ), baseinfo_( "" ),
  funcparv_( forFit::fparv_act_ ) // reference to forFit::fparv_act_
{}

TGraVmk::~TGraVmk(){
  for( int i = 0; i < this->size(); i++ )
    delete (*this)[ i ];

  for( int i = 0; i < sampleTG_.size(); i++ )
    delete sampleTG_[ i ];
}

void TGraVmk::set_min_max( const double& x, const double& y ){
  if( tminmax_[ 0 ] > x ) tminmax_[ 0 ] = x; // xmin
  if( tminmax_[ 1 ] < x ) tminmax_[ 1 ] = x; // xmax
  if( tminmax_[ 2 ] > y ) tminmax_[ 2 ] = y; // ymin
  if( tminmax_[ 3 ] < y ) tminmax_[ 3 ] = y; // ymax
}

bool TGraVmk::setvec( const vector< double >& x,
		      const vector< double >& y ){

  if( x.size() != y.size() ){ 
    cout << "vector size inconsistent" << endl;
    return false; 
  }

  vector< double > x_( 0 ), y_( 0 );
  tminmax_.clear(); tminmax_.resize( 4, 0.0 ); // tminmax_ reset 

  for( int i = 0; i < x.size(); i++ )
    if( abs( x[ i ] ) * ( 1 + 1.0 / 100.0 ) < xlimit_ )
      if( x[ i ] < xupperlimit_ && x[ i ] > xlowerlimit_ )
	if( ! yfrag_ || ( y[ i ] < getylimit() && abs( y[ i ] ) > 1.0e-10 ) )
	  if( ! yfrag2_ || y[ i ] < 4.0 * getylimit() ){
	    x_.push_back( x[ i ] );
	    y_.push_back( y[ i ] );
	    this->set_min_max( x[ i ], y[ i ] );
	  }

  TGraph* tg 
    = new TGraph( x_.size(), &(*x_.begin()), &(*y_.begin()) );

  if( ! tg ) return false;

  this->push_back( tg );
  vminmax_.push_back( tminmax_ );

  return true;
}

void TGraVmk::setsampleTG(){

  vector< double > x_( 0 ), y_( 0 );
  int disc_( 100 ); 
  double dwidth_( 2.0 * xlimit_ / disc_ );
  
  for( int i = 0; i <= disc_; i++ ){
    x_.push_back( - xlimit_ + dwidth_ * i );
    y_.push_back( (*this)( x_[ x_.size() - 1 ] ) );
  }
  
  sampleTG_.push_back( new TGraph( x_.size(), 
				   &(*x_.begin()), 
				   &(*y_.begin()) ) );

  // param initialization after tg creation
  for( int i = 0; i < funcparv_.size(); i++ )
    funcparv_[ i ] = 0.0;
}

void TGraVmk::setinfo( const std::string& info ){
  if( info.size() ) vinfo_.push_back( info );
}

void TGraVmk::setinfo2( const std::string& info2 ){
  if( info2.size() ) vinfo2_.push_back( info2 );
}

void TGraVmk::polyfitToTG( const int& itg ){

  Utility::Arguments& args = Utility::Arguments::ref();

  forFit::parsize_ = args.get( "paramsize", int( forFit::fparv_.size() ) );
  forFit::fparv_act_.resize( forFit::parsize_, 0.0 );

  Double_t (*pfit_)( const Double_t* x, const Double_t* par );
  pfit_ = &forFit::forfit;

  TF1* fitfunc_ = new TF1( "fitf", pfit_, - xlimit_, xlimit_, forFit::parsize_ );

  for( int i = 0; i < forFit::parsize_; i++ )
    fitfunc_->SetParameters( i, 0.0 ); // set initial values 

  (*this)[ itg ]->Fit( "fitf", "QWRN0" );
  // set fitted params to funcparav_ through forfit process in Func

  delete fitfunc_;
}

double TGraVmk::operator()( const double& x ){ 
  return forFit::forfit( &x, &(*forFit::fparv_act_.begin() ) );
}

double TGraVmk::operator()( const double& x ) const { 
  return forFit::forfit( &x, &(*forFit::fparv_act_.begin() ) );
}

Double_t forFit::forfit( const Double_t* x, const Double_t* par ){

  using namespace forFit;
  Utility::Arguments& args = Utility::Arguments::ref();

  if( ! fparv_act_.size() ){
    parsize_ = args.get( "paramsize", int( fparv_.size() ) );
    fparv_act_.resize( parsize_, 0.0 );
  }

  // mirroring of fit param (par) to forFit::fparv_act_
  for( int i = 0; i < forFit::fparv_act_.size(); i++ )
    fparv_act_[ i ] = par[ i ];

  int j = 0;
  for( int i = 0; i < fparv_.size(); i++ ){
    ostringstream ost; ost << "fixparam" << i;
    if( ! args.hasOpt( ost.str() ) ){
      if( j < forFit::parsize_ ){
	fparv_[ i ] = par[ j ]; j++;
	//	cout << "param" << i << "=" << fparv_[ i ] << "(" << j - 1 << ")" << endl;
      } else {
	//	cout << "param" << i << "=" << fparv_[ i ] << "(default)" << endl;
      }
    } else {
      fparv_[ i ] = args.get( ost.str(), 0.0 );
      //      cout << "param" << i << "=" << fparv_[ i ] << "(fixed)"<< endl;
    }
    ost.clear(); ost.str( "" );
  }
  if( j != forFit::parsize_ ) throw( 1 );

  double dvx_ = x[ 0 ] - fparv_[ 1 ];
  return fparv_[ 0 ] 
    + fparv_[ 2 ] * pow( dvx_, 1 ) 
    + fparv_[ 3 ] * pow( dvx_, 2 )
    + fparv_[ 4 ] * pow( dvx_, 3 );
    + fparv_[ 5 ] * pow( dvx_, 4 );
}
