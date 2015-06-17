#include "CKernel.hh"
#include "Wilsons.hh"
#include "IntegXsecComp.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Transform;
using namespace IntegXsec;
using namespace Evolution;

CKernel::CKernel( Arguments& args,  IntegXsecComp* xc ) :
  Xsection::CKernelBase(),
  xc_( xc ),
  q2_( 0.1 ),
  alpha_( args ),
  order_( alpha_.scheme().order() ), 
  Nf_( 3.0 ), 
  X_( 0.0 ), 
  Z_( 0.0 ),
  charge_( NULL ), 
  Nf_qfactor_( 1.0 ),
  vcoeffr_( 13, vector< double >( 13, 0.0 ) ),
  //  dim_( 1 ),
  alpha_pi_2_( 0.0 ),
  constNf_( true ),
  vchar_( 13, 0.0 ),
  vvchar_( 13, vector< double >( 13, 0.0 ) ),
  pp_judge_( false ),
  i_( 0 ), j_( 0 )
{
}

CKernel::~CKernel(){
}

void CKernel::setMoment( const double& X ){
  if( dim_ != 1 ) throw( 1 );
  X_ = X;
  this->update();
}

void CKernel::setMoment( const double& X, const double& Z ){
  if( dim_ != 2 ) throw( 1 );
  X_ = X;
  Z_ = Z;
  this->update();  
}

void CKernel::setMoment( const complex< double >& N ){
  if( dim_ != 1 ) throw( 1 );
  N_ = N;
  this->updatec();
}

void CKernel::setMoment( const complex< double >& N,
			 const complex< double >& M ){
  if( dim_ != 2 ) throw( 1 );
  N_   = N;
  M_   = M;
  this->updatec();
}

void CKernel::setQ2( const double& q2 ){
  this->setQ2Vir( q2 );
}

void CKernel::setQ2Vir( const double& q2 ){
  
  if( q2_ == q2 ) return;
  q2_ = q2;
  
  double alp_pi = ( order_ == Scheme::NLO ? alpha_( q2_ ) / M_PI : 0.0 );
  
  alpha_pi_2_ = 0.5 * alp_pi;  // 0.5 * alpha( Q2 ) / M_PI

  // this will be used somewhere... in deed in FragXsec_T/Charge.cc
  alpha_pi_1_ = 1.0 + alp_pi;  // 1.0 + alpha( Q2 ) / M_PI
  
  if( ! constNf_ ) Nf_ = Alpha::flavor()( sqrt( q2_ ) * Nf_qfactor_ );
  
  int nf = static_cast< int >( Nf_ );

  for( int i = - nf; i <= nf; i++ ) 
    vchar_[ i + 6 ] = (*charge_)( Flavor::PARTON( i ) );

  //  don't execute this->update() because vparam used in ConvIntegPDF is 
  //  not yet updated at this stage
  this->update2();
}

void CKernel::update(){
  
  int nf = static_cast< int >( Nf_ );
  
  for( int i = - nf; i <= nf; i++ ){
    
    if( dim_ == 1 ){

      int par1 = ( i == 0 ? 2 : ( i < 0 ? 1 : 0 ) ); // get parton type 1

      double& cc = vcoeffr_[ i + 6 ][ i + 6 ];
      ConvIntegrand& clo  = *( (*cf_) [ par1 ][ par1 ][ 0 ] );
      ConvIntegrand& cnlo = *( (*cf_) [ par1 ][ par1 ][ 1 ] );
      RealFunction* f1 =  xc_->vpdfx()[ 0 ][ i + 6 ];  

      cc = clo.setxPDF( f1 )( X_ ) + 
	( order_ == Scheme::NLO ? alpha_pi_2_ * cnlo.setxPDF( f1 )( X_ ) 
	  : 0.0 );

      cc *= vchar_[ i + 6 ];

    } else if( dim_ == 2 ) {
      
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
	
	double& cc = vcoeffr_[ i + 6 ][ j + 6 ];
	
	if( ( i == 0 && j == 0 ) ){ 
	  cc = 0.0; continue;//to next loop
	}
	
	//for q + q'( q'bar )-> X ( like direct photon process )
	if( i * j != 0 && i != j ){ 
	  if( !( pp_judge_ ) ){ cc = 0.0; continue; }
	  par1 = 2; par2 = 2; 
	}
	
	ConvIntegrand& clo  = *( (*cf_) [ par1 ][ par2 ][ 0 ] );
	ConvIntegrand& cnlo = *( (*cf_) [ par1 ][ par2 ][ 1 ] );
	RealFunction* f1 =  xc_->vpdfx()[ 0 ][ i + 6 ];  
	RealFunction* f2 =  xc_->vpdfx()[ 1 ][ j + 6 ];  
	
	cc = clo.setxPDF( f1, f2 )( X_, Z_ ) + 
	  ( order_ == Scheme::NLO ? 
	    alpha_pi_2_ * cnlo.setxPDF( f1, f2 )( X_, Z_ ) 
	    : 0.0 );
	
	if( !( pp_judge_ ) ) cc *= vchar_[ ( j != 0 ? j + 6 : i + 6 ) ] ; 
	
      }
      
    }
  }  
}

ostream& IntegXsec::operator<<( ostream& os, CKernel& ck ){
  cout << "IntegXsec::CKernel information" << endl;
  cout << "Q2 = " << setw(10) << ck.q2_ << endl;
  cout << "NF = " << setw(10) << ck.Nf_ << endl;
  return os;
}


void CKernel::updatec(){
  
  int nf = static_cast< int >( Nf_ );
  
  if( dim_ == 1 ){

    int par1 = ( i_ == 0 ? 2 : ( i_ < 0 ? 1 : 0 ) ); // get parton type 1
      
    complex< double >& cc = vcoeff_[ i_ + 6 ][ i_ + 6 ];

    ConvIntegrand& clo  = *( (*cf_) [ par1 ][ par1 ][ 0 ] );
    ConvIntegrand& cnlo = *( (*cf_) [ par1 ][ par1 ][ 1 ] );

    cc = clo( N_ ) + ( order_ == Scheme::NLO ? alpha_pi_2_ * cnlo( N_ ) 
		       : 0.0 );
    /*
    cc *= vchar_[ i_ + 6 ]; 
    // charge manipulation unnecessary! unlike Xsec::CKernel
    // <- because of efficient memory manipulation!
    // moved to setMoment operators in x(z)Xsec (related) objects!
    */    

  } else if( dim_ == 2 ) {

    // flavor identification calc is implemented in xzXsec level
    
    int par1 = ( i_ == 0 ? 2 : ( i_ < 0 ? 1 : 0 ) ); // get parton type 1
    int par2 = ( j_ == 0 ? 2 : ( j_ < 0 ? 1 : 0 ) ); // get parton type 2
    
    complex< double >& cc = vcoeff_[ i_ + 6 ][ j_ + 6 ];

    /* absorbed in x(z)Xsec level (setActflav function)

    if( ( i_ == 0 && j_ == 0 ) ){ 
      cc = 0.0; return;
    }
    
    //for q + q'( q'bar )-> X ( like direct photon process )
    if( i_ * j_ != 0 && i_ != j_ ){ 
      if( !( pp_judge_ ) ){ cc = 0.0; return; }
      par1 = 2; par2 = 2; 
    }
    */
	
    ConvIntegrand& clo  = *( (*cf_) [ par1 ][ par2 ][ 0 ] );
    ConvIntegrand& cnlo = *( (*cf_) [ par1 ][ par2 ][ 1 ] );

    cc = clo( N_, M_ ) + 
      ( order_ == Scheme::NLO ? alpha_pi_2_ * cnlo( N_, M_ ) : 0.0 );
    
    /*
    if( !( pp_judge_ ) ) cc *= vchar_[ ( j_ != 0 ? j_ + 6 : i_ + 6 ) ] ; 
    // charge manipulation unnecessary! unlike Xsec::CKernel
    // <- because of efficient memory manipulation!
    // moved to setMoment operators in x(z)Xsec (related) objects!
    */

  }
  
}
