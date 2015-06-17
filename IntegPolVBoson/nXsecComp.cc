#include "nXsecComp.hh"
#include "StrFunc.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>

#include <polPDF/Evolution.hh>

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace QCD;
using namespace IntegPolVBoson;

nXsecComp::nXsecComp( Arguments& args,
		      const IntegXsec::Charge::TARGET& n,
		      const QCD::Flavor::HADRON& h  ) throw( int ) :
  Xsection::XsecComp()
{
  
  try {
    
    // prepare 1st Evolution kernel for polVBoson function ( n )
    this->insert( polPDF::Evo::instance(), Flavor::PRp );
    dynamic_cast< polPDF::Evo* >( this->vevo()[ 0 ] )->constNf( false );
    //    dynamic_cast< polPDF::Evo* >( this->vevo()[ 0 ] )->constNf( true );
    
    // prepare 2nd Evolution kernel for polVBoson function ( m )
    this->insert( this->vevo()[ 0 ], h );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->coefficients( n );  

}

nXsecComp::nXsecComp( Evolution::KernelBase *pdf, 
		      const IntegXsec::Charge::TARGET& n,
		      const QCD::Flavor::HADRON& h ) 
  throw( int ) : 
  Xsection::XsecComp()
{
  
  // prepare 1st Evolution kernel for polVBoson function ( n )
  if( dynamic_cast< polPDF::Evo* >( pdf ) ){
    this->insert( pdf, Flavor::PRp );
  } else {
    ARG_ERR << __FILE__ << ":" << __LINE__ 
	    << "\tpdf should be either polPDF::Evo"
	    << endl;
    throw( 1 );
  }
  
  // prepare 2nd Evolution kernel for polVBoson function ( m )
  if( dynamic_cast< polPDF::Evo* >( pdf ) ){
    this->insert( pdf, h );
  } else {
    ARG_ERR << __FILE__ << ":" << __LINE__ 
	    << "\tpdf should be either polPDF::Evo"
	    << endl;
    throw( 1 );
  }
  
  this->coefficients( n );
}


nXsecComp::~nXsecComp(){
  // delete all dynamically allocated objects by this
  if( this->coeff() ) delete this->coeff();  
  for( int i = 0; i < rescaleFcn().size(); i++ )
    if( rescaleFcn()[ i ] ) delete rescaleFcn()[ i ];
}

void nXsecComp::update(){
}

void nXsecComp::coefficients( const IntegXsec::Charge::TARGET& n ){
  Arguments& args = Arguments::ref();

  // register CKernel object
  Xsection::CKernelBase* coeff;
  if( ! args.hasOpt( "PInteg" ) ){
    if( args.hasOpt( "ShortInteg" ) ){ 
      coeff = new IntegPolVBoson::StrFunc( this, args, n, 4, 0, 1.0E-2 );
    } else {
      coeff = new IntegPolVBoson::StrFunc( this, args, n, 4, 6, 1.0E-4 );
    }
  } else {
    /*
    if( args.hasOpt( "ShortInteg" ) ){ 
      coeff = new IntegPolVBoson::PStrFunc( this, args, n, 4, 0, 1.0E-2 );
    } else {
      coeff = new IntegPolVBoson::PStrFunc( this, args, n, 4, 6, 1.0E-4 );
    }
    */
    throw( 1 );
  }
  coeff->constNf( false );
  //  coeff->constNf( true );
  this->coeff( coeff );
  //! set Q^2 / \mu_R^2 to alpha_s
  double rescaleQ2 = args.get( "rescaleQ2", 1.0 );
  this->coeff()->alpha().setMURfact( rescaleQ2 );
  
  //! Scale rescaling function 
  //! Q^2 / \mu_F^2 
  double rescalePPDF = args.get( "rescalePPDF", 1.0 );// for evo[ 0 ]

  //! set \mu_F^2 / \mu_R^2 to alpha_s of each evolutoin
  if( this->vevo()[ 0 ] != this->vevo()[ 1 ] ) throw( 1 );
  this->vevo()[ 0 ]->alpha().setMURfact( rescaleQ2 / rescalePPDF );
  
  RealFunction* rf_1_ = ( rescalePPDF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescalePPDF )
			  : ( RealFunction* ) NULL );
  this->rescaleFcn().resize( 0 );
  this->rescaleFcn().push_back( rf_1_ );
  this->rescaleFcn().push_back( rf_1_ );

}
