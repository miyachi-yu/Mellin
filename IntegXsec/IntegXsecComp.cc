#include <Xsec/CKernelBase.hh>
#include "IntegXsecComp.hh"
#include "CKernel.hh"

#include <iomanip>

#include <Tranform/RealFunction.hh>
#include <Evolution/PDFx.hh>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace IntegXsec;

IntegXsecComp::IntegXsecComp() :
  Xsection::XsecCompBase(),
  q2_( 1.0 ),
  vpdfx_( 0 ),
  rescaleFcn_( 0 ),
  vparam_( 0 ),
  localPdfs_( 0 ),
  length_( 10.0 ), offset_( 1.8 ), angle_( 0.6 ),
  low_( 4 ), high_( 6 ), precision_( 1.0E-4 ),
  xc_( this )
{
}

IntegXsecComp::IntegXsecComp( Xsection::XsecCompBase* xc ) :
  Xsection::XsecCompBase(),
  q2_( 1.0 ),
  vpdfx_( 0 ),
  rescaleFcn_( 0 ),
  vparam_( 0 ),
  localPdfs_( 0 ),
  length_( 10.0 ), offset_( 1.8 ), angle_( 0.6 ),
  low_( 4 ), high_( 6 ), precision_( 1.0E-4 ),
  xc_( xc )
{
}

IntegXsecComp::~IntegXsecComp(){
  for( vector< RealFunction* >::iterator itr = localPdfs_.begin();
       itr != localPdfs_.end();
       itr++ ){
    delete *itr;
  }
}

void IntegXsecComp::setQ2( const double& q2 ){
  
  q2_ = q2;

  if( xc_ == this ){
    for( int i = 0; i < vevo_.size(); i++ ) {
      double ratio = 1.0;
      if( ( i < rescaleFcn_.size() ) && 
	  ( rescaleFcn_[ i ] != NULL ) ) {
	RealFunction& f = *( rescaleFcn_[ i ] );
	ratio = f( q2_ );
      }
      vevo_[ i ]->setQ2( q2_ / ratio );
    }
  }

  coeff_->setQ2( q2_ );
  this->update();
}

vector< vector< double > >& IntegXsecComp::vCoeffr() {
  return dynamic_cast< IntegXsec::CKernel* >( coeff_ )->vCoeffr();
}

void IntegXsecComp::coeff( Xsection::CKernelBase* ck ){
  coeff_ = ck;
  coeff_->dimension( xc_->vevo().size() );
  vector< complex< double > > vpar( 0 ); 
  vparam_.resize( xc_->vevo().size(), vpar );
  // vparam resize needed for ConvIntegrand constructor ( vparam[ i ] )
  if( coeff_->wilsons() ) throw( 1 );
  // wilsons should be construct after vparam_ definition !
}

void IntegXsecComp::update(){  
}

void IntegXsecComp::setOrder( const Scheme::ORDER& order ){

  if( xc_ == this ){
    for( int i = 0; i < vevo_.size(); i++ ) {
      vevo_[i]->alpha().scheme().order() = order;
    }
  }

}

Scheme::ORDER IntegXsecComp::order() {
  if( vevo_.size() > 0 ){
    return vevo_[ 0 ]->alpha().scheme().order();
  }
  return Scheme::NLO;
}

bool IntegXsecComp::insert( KernelBase* kernel, const Flavor::HADRON& hadron ){

  if( xc_ == this ){

    vevo_.push_back( kernel );

    vector< RealFunction* > vpdfx( 0 );
  
    for( int i = -6; i <= 6; i++ ){
      PDFx *pdfx 
	= new PDFx( kernel, Flavor::PARTON( hadron > 0 ? i : -i ), 
		    low_, high_, precision_, length_, offset_, angle_ );
      pdfx->integPath( InverseMellin::STRAIGHT );
      //    pdfx->integPath( InverseMellin::STEEPEST );
      localPdfs_.push_back( pdfx );
      vpdfx.push_back( pdfx );
    }

    vpdfx_.push_back( vpdfx );

    // CKernel should be created always after evo creations
    // ( related to ConvIntegrand definition )
    if( coeff_ ) throw( 1 );//coeff_->dimension( vevo_.size() );
  
  return true;

  }

  return false;

}

ostream& IntegXsec::operator<<( ostream& os, IntegXsecComp& xc ){
  
  os << "IntegXsec::IntegXsecComp information" << endl;
  
  for( int i = 0; i < xc.vevo().size(); i++ ){
    os << "Evolution::KernelBase "<< setw(4) << i << endl;
    os << *( xc.vevo()[ i ] ) << endl;
    
    os << "PDFx information:" << endl;
    for( int j = 0; j < xc.vpdfx()[ i ].size(); j++ ){
      os << *( dynamic_cast< Evolution::PDFx* >( xc.vpdfx()[ i ][ j ] ) ) 
	 << flush;
    }
  }
  
  if( dynamic_cast< IntegXsec::CKernel* >( xc.coeff() ) ) 
    os << *( dynamic_cast< IntegXsec::CKernel* >( xc.coeff() ) ) << endl;
  
  return os;
}
