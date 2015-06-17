#include "COFbase.hh"

using namespace IntegPolDrellYan;

Li2 COFbase::li2_ = Transform::Li2();

COFbase::COFbase( IntegXsec::IntegXsecComp* xc ) throw( int ):
  IntegXsec::ConvIntegrand( xc ),
  vy_( xc->vparam() ),
  dyplusf_( 0 ), dydplusf_( 0 ), dylogplusf_( 0 )
{
  if( dim_ != 2 ) throw( 1 );
  for( int i = 0; i < dim_ ; i++ ){
    dyplusf_.push_back( new DYPlusFunc( g_[ i ], xc_->vparam()[ i ] ) );
    dylogplusf_.push_back( new DYLogPlusFunc( g_[ i ], xc_->vparam()[ i ] ) );
  }
  dydplusf_.push_back( new DYDPlusFunc( g_, xc_->vparam() ) );

  dGA_ = new DGA();
  dHA_ = new DHA();
  dGC_ = new DGC();
  dHC_ = new DHC();
}

COFbase::~COFbase(){
  for( int i = 0; i < dim_; i++){
    delete dyplusf_[ i ]; delete dylogplusf_[ i ];
  }
  delete dydplusf_[ 0 ];
  delete dGA_;  delete dHA_;
  delete dGC_;  delete dHC_;
}

double COFbase::rescale( const int& i ) throw( int ){
  if( xc_ == NULL || i >= xc_->vvevo().size() ) return 0.0;
  // because we use Cqg = Cgq in WC, MuF expected to be the same for both PDFs 
  if( xc_->vvevo()[ 0 ]->Q2() ==  xc_->vvevo()[ 1 ]->Q2() )
  return log( xc_->q2() / xc_->vvevo()[ i ]->Q2() );
  throw( 10 );
}
