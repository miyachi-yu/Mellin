#include <Tranform/RealFunction.hh>
#include <IntegXsec/ConvIntegrand.hh>
#include <IntegXsec/Wilsons.hh>

#include "CoeffFunc.hh"
#include "Charge.hh"

using namespace std;
using namespace Utility;
using namespace IntegUnpolVBoson;
using namespace IntegXsec;
using namespace Transform;
using namespace QCD;
using namespace PDFLIB;

CoeffFunc::CoeffFunc( Arguments& args, IntegXsec::IntegXsecComp* xc, 
		      const IntegXsec::Charge::TARGET& n ) :
  IntegXsec::CKernel( args, xc ), PDF_( ( PDF_SW* ) NULL ),
  cteq6_1( NULL ), cteq6_2( NULL ), xcteq6_( false ), wboson_( false ),
  wmboson_( false )
{
  this->charge_     = new  IntegUnpolVBoson::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
  this->setPDFswitch( n ); // harmless for complex calc

  if( args.hasOpt( "xCTEQ6" ) ){
    xcteq6_ = true;
    // cteq6_i returns f(x) not xf(x)
    int i = ( getOrder() == Scheme::NLO ? 1 : 3 );
    //  int i = ( getOrder() == Scheme::NLO ? 1 : 1 );
    cteq6_1 = new PDFLIB::CTEQ6( i );
    cteq6_2 = new PDFLIB::CTEQ6( i );
    //
  }

  // w boson production ( default z production )
  if( args.hasOpt( "WPBPro") ||  args.hasOpt( "WMBPro") ) wboson_ = true; 
  if( args.hasOpt( "WMBPro") ) wmboson_ = true;
}

CoeffFunc::~CoeffFunc(){
  if( this->charge_ ) delete this->charge_;
  if( this->PDF_ ) delete PDF_;  
  //tmp
  if( cteq6_1 ) delete cteq6_1;
  if( cteq6_2 ) delete cteq6_2;
  //
}

void CoeffFunc::setQ2( const double& q2 ){
  this->setQ2Vir( q2 );
  // operate setQ2Vir in CKernel first for setting appropriate Nf_

  // for w boson production
  if( wboson_ ){
    int nf = static_cast< int >( this->nf() );
    for( int i = - nf; i <= nf; i++ ) 
      for( int j = - nf; j <= nf; j++ ) 
	vvchar()[ i + 6 ][ j + 6 ] 
	  = (*charge_)( Flavor::PARTON( i ), Flavor::PARTON( j ) );
  }

}

void CoeffFunc::update2(){
  // ref hep-ph/9710508 (Gehrmann)
  double Mv2 = pow( ( wboson_ ? 80.40 : 91.188 ), 2 );
  if( q2() != Mv2 ){ 
    cerr << "q2 must be vector boson mass squared ( Mv^2 )" << endl;
    throw( 100 ); 
  }

  double GF = 1.1664 * pow( 10.0, -5 ); 
  // GeV^(-2) ( GF = g^2 sqrt(2) / 8 Mw^2 )
  double factor = M_PI * sqrt( 2.0 ) / 3.0 * GF * Mv2;
  // S factor was moved to cross section definition! 
  // ( d sigms / d y d M^2 * S[ GeV^2 ] 
  // -> calculated quantity : dimensionless )
  this->getIntXsecC()->globalScale( factor );
  // note that the dimention of this sigma is [ GeV^-2 ] !!!
}

void CoeffFunc::update(){

  if( xcteq6_ ){
    cteq6_1->setQ2( this->q2() );
    cteq6_2->setQ2( this->q2() );
    cteq6_1->xWeight( 0.0 );
    cteq6_2->xWeight( 0.0 );
  }
  
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

	if( !wboson_ ){ // Z process ( i, -j ) should be orthogonal

	  if( i * j != 0 && i != - j ){  // like drell-yan ( q + qbar )
	    cc = 0.0; continue;
	  }

	} else { // W process ( i, -j ) should not be always orthogonal

	  if( i * j > 0 || udjudge( i ) * udjudge( j ) > 0 ){ 
	    //  case i * j == 0 implicitly excluded
	    cc = 0.0; continue;
	  }

	  // W+- judge
	  int ii = ( i != 0 ? i : j ) * udjudge( ( i != 0 ? i : j ) );
	  if( wmboson_ && ii > 0 || !wmboson_ && ii < 0 ){
	    //W- boson              W+ boson
	    cc = 0.0; continue;
	  }
	  // W+: i(j) > 0 & up kind or i(j) < 0 & down kind
	  // W-: i(j) > 0 & down kind or i(j) < 0 & up kind
	  // charge conservation...

	}

	/*
	if( i * j != 0 ){
	  cout << i << " " << j << " " << vvchar()[ i + 6 ][ j + 6 ] << endl;
	} else {
	  cout << i << " " << j << " " << sumvvchar( j != 0 ? j : i ) << endl;
	}
	*/

        ConvIntegrand& clo  = *( (*wilsons()) [ par1 ][ par2 ][ 0 ] );
        ConvIntegrand& cnlo = *( (*wilsons()) [ par1 ][ par2 ][ 1 ] );

        RealFunction* f1;
	RealFunction* f2;

	if( xcteq6_ ){
	  cteq6_1->setPID( Flavor::PARTON( i ) );
	  cteq6_2->setPID( Flavor::PARTON( j ) );
	  f1 = cteq6_1;
	  f2 = cteq6_2;
	} else {
	  f1 = PDF_->seti( i ); //PDF1
	  f2 = getIntXsecC()->vpdfx()[ 1 ][ j + 6 ];  //PDF2
	}

        cc = clo.setxPDF( f1, f2 )( getX(), getZ() ) + 
          ( getOrder() == Scheme::NLO ? 
            alpha_pi_2() * cnlo.setxPDF( f1, f2 )( getX(), getZ() ) 
            : 0.0 );

	if( !wboson_ ){ // Z process
	  cc *= vchar()[ ( j != 0 ? j + 6 : i + 6 ) ]; 
        } else { // W process
	  if( i * j != 0 ){
	    cc *= vvchar()[ i + 6 ][ j + 6 ];
	  } else {
	    cc *= this->sumvvchar( j != 0 ? j : i );
	  }
	}
      }
      
    }
  }  
}

int CoeffFunc::udjudge( const int& iflav ){
  if( abs( iflav ) == 1 || abs( iflav ) == 4 || abs( iflav ) == 6 ){
    // up kind
    return 1;
  } else {
    // down kind
    if( abs( iflav ) != 0 ) return -1;
  }
  // gluon
  return 0;
}

double CoeffFunc::sumvvchar( const int& iflav ){
  double tmpchar( 0.0 );
  if( udjudge( iflav ) == 1 ){
    // up kind
    tmpchar += vvchar()[ abs( iflav ) + 6 ][ 2 + 6 ];
    tmpchar += vvchar()[ abs( iflav ) + 6 ][ 3 + 6 ];
    tmpchar += vvchar()[ abs( iflav ) + 6 ][ 5 + 6 ];
    //sum over down kind
  } else {
  // down kind
    tmpchar += vvchar()[ 1 + 6 ][ abs( iflav ) + 6 ];
    tmpchar += vvchar()[ 4 + 6 ][ abs( iflav ) + 6 ];
    tmpchar += vvchar()[ 6 + 6 ][ abs( iflav ) + 6 ];
    //sum over up kind
  }
  return tmpchar;
};

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

