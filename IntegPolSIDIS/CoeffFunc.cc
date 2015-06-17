#include <Tranform/RealFunction.hh>
#include <IntegXsec/ConvIntegrand.hh>
#include <IntegXsec/Wilsons.hh>

#include "CoeffFunc.hh"
#include "Charge.hh"

using namespace std;
using namespace Utility;
using namespace IntegPolSIDIS;
using namespace IntegXsec;
using namespace Transform;
using namespace QCD;

CoeffFunc::CoeffFunc( Arguments& args, IntegXsec::IntegXsecComp* xc, 
		      const IntegXsec::Charge::TARGET& n ) :
  IntegXsec::CKernel( args, xc ), PDF_( ( PDF_SW* ) NULL )
{
  this->charge_     = new  IntegPolSIDIS::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
  this->setPDFswitch( n );  // harmless for complex calc
}

CoeffFunc::~CoeffFunc(){
  if( this->charge_ ) delete this->charge_;
  if( this->PDF_ ) delete PDF_;  
}


void CoeffFunc::update(){
  
  int nf = static_cast< int >( this->nf() );
  
  for( int i = - nf; i <= nf; i++ ){
    
    if( dimension() == 1 ){ throw( 1 );
    } else if( dimension() == 2 ) {
      
      for( int j = - nf; j <= nf; j++ ){
        
        // for flavor identification calc
        if( Arguments::ref().hasOpt( "Flav1" ) )
          if( i != Arguments::ref().get( "Flav1", 1 ) ) continue;
        // vcoeffr default 0.0 ( IntegXsec/CKernel )
        if( Arguments::ref().hasOpt( "Flav2" ) )
          if( j != Arguments::ref().get( "Flav2", 1 ) ) continue;
        // vcoeffr default 0.0 ( IntegXsec/CKernel )

        int par1 = ( i == 0 ? 2 : ( i < 0 ? 1 : 0 ) ); // get parton type 1
        int par2 = ( j == 0 ? 2 : ( j < 0 ? 1 : 0 ) ); // get parton type 2
        
        double& cc = vCoeffr()[ i + 6 ][ j + 6 ];
        
        if( ( i == 0 && j == 0 ) ){ 
          cc = 0.0; continue;//to next loop
        }
        
        //for q + q'( q'bar )-> X ( like direct photon process )
        if( i * j != 0 && i != j ){ 
          if( !( getpp_judge() ) ){ cc = 0.0; continue; }
          par1 = 2; par2 = 2; 
        }
        
        ConvIntegrand& clo  = *( (*wilsons()) [ par1 ][ par2 ][ 0 ] );
        ConvIntegrand& cnlo = *( (*wilsons()) [ par1 ][ par2 ][ 1 ] );
        RealFunction* f1 = PDF_->seti( i );
        RealFunction* f2 = getIntXsecC()->vpdfx()[ 1 ][ j + 6 ];  //FF

        cc = clo.setxPDF( f1, f2 )( getX(), getZ() ) + 
          ( getOrder() == Scheme::NLO ? 
            alpha_pi_2() * cnlo.setxPDF( f1, f2 )( getX(), getZ() ) 
            : 0.0 );
        
        if( !( getpp_judge() ) ) 
	  cc *= vchar()[ ( j != 0 ? j + 6 : i + 6 ) ] ; 
        
      }
      
    }
  }  
}

void  CoeffFunc::setPDFswitch( const IntegXsec::Charge::TARGET& n ){
  switch( n ){
  case IntegXsec::Charge::p : PDF_ = new PDF_P( this ); break;
  case IntegXsec::Charge::n : PDF_ = new PDF_N( this ); break;
  case IntegXsec::Charge::d : PDF_ = new PDF_D( this ); break;
  }
}

double PDF_P::operator()( const double& m ){
  return ( *( cker_->getIntXsecC()->vpdfx()[ 0 ][ i_ + 6 ] ) )( m );
}

double PDF_N::operator()( const double& m ){
  return ( *( cker_->getIntXsecC()->vpdfx()[ 0 ][ su2swap( i_ ) + 6 ] ) )( m );
}

double PDF_D::operator()( const double& m ){
  return 0.5 * ( ( *( cker_->getIntXsecC()->vpdfx()[ 0 ][ i_ + 6 ] ) )( m ) + 
                 ( *( cker_->getIntXsecC()->vpdfx()[ 0 ]
                      [ su2swap( i_ ) + 6 ] ) )( m ) );
}

int PDF_SW::su2swap( const int& i ){
  if( i * i == 1 ) return i * 2;
  if( i * i == 4 ) return i / 2;
  return i;
}
