// $Id: XsecComp.cc,v 1.16 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file    XsecComp.cc
  \version $Revision: 1.16 $
  \author  $Author: miyachi $
  \date    $Date: 2009/02/13 12:23:41 $
  \brief   Class implementation for cross section components for h production
 */
#include "XsecComp.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>
#include <polPDF/Evolution.hh>

#include "CoeffFunc.hh"
#include "Wilson.hh"

using namespace std;
using namespace polDIS;

XsecComp::XsecComp( Utility::Arguments& args,
		    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  try {
    // insert evolution kernel
    this->insert( polPDF::Evo::instance(), QCD::Flavor::PRp );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->coefficients( n );
}

XsecComp::XsecComp( Evolution::Kernel *evo,
		    const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  this->insert( evo, QCD::Flavor::PRp );
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
  Utility::Arguments& args = Utility::Arguments::ref();

  // register CKernel object
  CoeffFunc *coeff = new CoeffFunc( args, n );
  coeff->constNf( true );
  this->coeff( coeff );
  this->coeff()->wilsons( new Wilson( this ) );
  //! set Q^2 / \mu_R^2 to alpha_s
  double rescaleQ2 = args.get( "rescaleQ2", 1.0 );
  this->coeff()->alpha().setMURfact( rescaleQ2 );
  
  // Scale rescaling function 
  //! Q^2 / \mu_F^2 
  double rescalePPDF = args.get( "rescalePPDF", 1.0 );// for evo[ 0 ]

  //! set \mu_F^2 / \mu_R^2 to alpha_s of each evolutoin
  this->vevo()[ 0 ]->alpha().setMURfact( rescaleQ2 / rescalePPDF );
  
  Transform::RealFunction* rf_1_ = ( rescalePPDF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescalePPDF )
			  : ( Transform::RealFunction* ) NULL );
  this->rescaleFcn().resize( 0 );
  this->rescaleFcn().push_back( rf_1_ );
  
}
