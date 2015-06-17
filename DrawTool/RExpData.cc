#include "RExpData.hh"

RExpData::RExpData( Exp::DataSet& dset,
		    const std::string& key ) :
  TGraphErrors(), model_( NULL ), fit_( NULL )
{

  this->Set( dset.size() );
  model_ = new TGraphErrors( dset.size() );
  fit_   = new TGraphErrors( dset.size() );

  for( int i = 0; i < dset.size(); i++ ){
    
    Exp::DataPoint& p = dset[ i ];
    double x = p.kine( key );
    
    this->SetPoint( i, x, p.data() );
    this->SetPointError( i, 0.0, p.stat() );
    
    model_->SetPoint( i, x, p.model() );
    model_->SetPointError( i, 0.0, p.modelError() );
    
    fit_->SetPoint( i, x, p.fitted() ? p.model() : 0.0 );
    fit_->SetPointError( i, 0.0, p.fitted() ? p.modelError() : 0.0 );
    
  }
}

RExpData::RExpData( Exp::DataSet& dset,
		    const std::string& min, const std::string& max ) :
  TGraphErrors(), model_( NULL ), fit_( NULL )
{

  this->Set( dset.size() );
  model_ = new TGraphErrors( dset.size() );
  fit_   = new TGraphErrors( dset.size() );

  for( int i = 0; i < dset.size(); i++ ){
    
    Exp::DataPoint& p = dset[ i ];
    double x1 = p.kine( min );
    double x2 = p.kine( max );
    double x  = 0.5 * ( x1 + x2 );
    double dx = 0.5 * ( x2 - x1 );

    this->SetPoint( i, x, p.data() );
    this->SetPointError( i, dx, p.stat() );
    
    model_->SetPoint( i, x, p.model() );
    model_->SetPointError( i, 0.0, p.modelError() );
    
    fit_->SetPoint( i, x, p.fitted() ? p.model() : 0.0 );
    fit_->SetPointError( i, 0.0, p.fitted() ? p.modelError() : 0.0 );
    
  }
}

RExpData::~RExpData()
{
}


void RExpData::style( const Style_t& type, Width_t& size, 
		      const Color_t& color ){

  this->SetMarkerStyle( type );
  this->SetMarkerSize( size );
  this->SetMarkerColor( color );
  this->SetLineColor( color );
  this->SetLineWidth( 2 );

  this->model_->SetLineColor( color + 1 );
  this->model_->SetLineWidth( 2 );
  
  this->fit_->SetLineColor( color + 1 );
  this->fit_->SetLineWidth( 2 );

}

void RExpData::rescale( const double& factor ){
  
  double *x  = this->GetX();
  double *y  = this->GetY();
  double *dx = this->GetEX();
  double *dy = this->GetEY();
  double *m  = this->model_->GetY();
  double *dm = this->model_->GetEY();
  double *f  = this->fit_->GetY();
  double *df = this->fit_->GetEY();
  
  for( int i = 0; i < this->GetN(); i++ ){
    this->SetPoint( i,       x[ i ], factor * y[ i ] );
    this->SetPointError( i, dx[ i ], factor * dy[ i ] );
    model_->SetPoint( i,       x[ i ], factor * m[ i ] );
    model_->SetPointError( i,     0.0, factor * dm[ i ] );
    fit_->SetPoint( i,       x[ i ], factor * f[ i ] );
    fit_->SetPointError( i,     0.0, factor * df[ i ] );
  }
  
}

void RExpData::offset( const double& value ){
  double *x  = this->GetX();
  double *y  = this->GetX();
  double *m  = this->model_->GetY();
  double *f  = this->fit_->GetY();
  
  for( int i = 0; i < this->GetN(); i++ ){
    this->SetPoint( i,        x[ i ], y[ i ] + value );
    model_->SetPoint( i,        x[ i ], m[ i ] + value );
    fit_->SetPoint( i,        x[ i ], f[ i ] + value );
  }
}

void RExpData::draw( const bool& fFit, const bool& fModel ){
  this->Draw("P");
  if( fFit && fit_ ) fit_->Draw("*");
  if( fModel && model_ ) model_->Draw("l");
}

ClassImp( RExpData )
