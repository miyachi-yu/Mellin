#include "XsecComp.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>
#include <Tranform/Mellin.hh>

#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>

#include "CoeffFunc.hh"
#include "WC.hh"
#include "WC2.hh"

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Utility;
using namespace unpolDISXsec;

XsecComp::XsecComp( Arguments& args,
		    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  
  try {
    
    // prepare ( 1st ) Evolution kernel for unpolDISXsec function
    this->insert( unpolPDF::Evo::instance(), Flavor::PRp );
    
    // register CKernel object
    this->coefficients( n );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

XsecComp::XsecComp( Evolution::KernelBase *kernel,
                    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{

  // prepare ( 1st ) Evolution kernel for unpolDISXsec function
  if( dynamic_cast< unpolPDF::Evo* >( kernel ) ||
      dynamic_cast< unpolPDF::CTEQ6pdf* >( kernel ) ){
    this->insert( kernel, Flavor::PRp );
  } else {
    cerr << __FILE__ << ":" << __LINE__ 
         << "\tkernel should be either unpolPDF::Evo or unpolPDF::CTEQ6pdf"
         << endl;
    throw( 1 );
  }

  // register CKernel object
  this->coefficients( n );
}

XsecComp::~XsecComp(){
  
  // delete all dynamically allocated objects by this
  /* done by atExit
  for( int i = 0; i < this->vevo().size(); i++ ){
    //    for( int j = -6; j <= 6; i++ ) this->vpdf()[ i ][ j ];
    delete this->vevo()[ i ];
  }
  */
  if( this->coeff()->wilsons2() ) delete this->coeff()->wilsons2();
  if( this->coeff()->wilsons() ) delete this->coeff()->wilsons();
  if( this->coeff() ) delete this->coeff();  
  for( int i = 0; i < rescaleFcn().size(); i++ )
    if( rescaleFcn()[ i ] ) delete rescaleFcn()[ i ];
}

void XsecComp::update(){
}


void XsecComp::setS( const double& s ){
  dynamic_cast< CoeffFunc* >( this->coeff() )->setS( s );
}

void XsecComp::setX( const double& x ){
  dynamic_cast< CoeffFunc* >( this->coeff() )->setX( x );
}

void XsecComp::coefficients( const Xsection::Charge::TARGET& n ){
  Arguments& args = Arguments::ref();

  // register CKernel object
  CoeffFunc *coeff = new CoeffFunc( args, n );
  coeff->constNf( false );
  if( Arguments::ref().hasOpt( "xmoment" ) ) 
    coeff->setXmom( true ); // for mom calc 
  this->coeff( coeff );
  this->coeff()->wilsons( new WC( this ) );
  this->coeff()->wilsons2( new WC2( this ) );
  //! set Q^2 / \mu_R^2 to alpha_s
  double rescaleQ2 = args.get( "rescaleQ2", 1.0 );
  this->coeff()->alpha().setMURfact( rescaleQ2 );

  //! Scale rescaling function 
  //! Q^2 / \mu_F^2 
  double rescaleUPDF = args.get( "rescaleUPDF", 1.0 );// for evo[ 0 ]

  //! set \mu_F^2 / \mu_R^2 to alpha_s of each evolutoin
  this->vevo()[ 0 ]->alpha().setMURfact( rescaleQ2 / rescaleUPDF );
  
  RealFunction* rf_1_ = ( rescaleUPDF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescaleUPDF )
			  : ( RealFunction* ) NULL );
  this->rescaleFcn().resize( 0 );
  this->rescaleFcn().push_back( rf_1_ );

}
