// $Id: XsecComp.cc,v 1.19 2009/01/09 08:57:40 miyachi Exp $
/*!
  \brief  Class implementation for unpolarized structure function
 */
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

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace QCD;
using namespace unpolDIS;

XsecComp::XsecComp( Arguments& args,
		    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  
  try {  // prepare Evolution kernel for unpolDIS function
    this->insert( unpolPDF::Evo::instance(), Flavor::PRp );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->coefficients( n );
}

XsecComp::XsecComp( Evolution::KernelBase *kernel,
		    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  
  if( dynamic_cast< unpolPDF::Evo* >( kernel ) ||
      dynamic_cast< unpolPDF::CTEQ6pdf* >( kernel ) ){
    this->insert( kernel, Flavor::PRp );
  } else {
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tkernel should be either unpolPDF::Evo or unpolPDF::CTEQ6pdf"
	 << endl;
    throw( 1 );
  }
  this->coefficients( n );
}

XsecComp::~XsecComp(){
  // delete all dynamically allocated objects by this
  if( this->coeff()->wilsons() ) delete this->coeff()->wilsons();
  if( this->coeff() ) delete this->coeff();
  for( int i = 0; i < rescaleFcn().size(); i++ )
    if( rescaleFcn()[ i ] ) delete rescaleFcn()[ i ];
}

void XsecComp::update(){
}

void XsecComp::coefficients( const Xsection::Charge::TARGET& n ){
  Arguments& args = Arguments::ref();

  // register CKernel object
  CoeffFunc *coeff = new CoeffFunc( args, n );
  coeff->constNf( false );
  this->coeff( coeff );
  this->coeff()->wilsons( new WC( this ) );
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
