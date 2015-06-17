#include "nXsecComp.hh"
#include "StrFunc.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>
#include <Tranform/Mellin.hh>

#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace QCD;
using namespace IntegUnpolDIS;

nXsecComp::nXsecComp( Arguments& args,
		      const IntegXsec::Charge::TARGET& n ) throw( int ) :
  Xsection::XsecComp()
{
  
  try {  // prepare Evolution kernel for unpolDIS function
    this->insert( unpolPDF::Evo::instance(), Flavor::PRp );
    dynamic_cast< unpolPDF::Evo* >( this->vevo()[ 0 ] )->constNf( false );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->coefficients( n );
}

nXsecComp::nXsecComp( Evolution::KernelBase *kernel,
		      const IntegXsec::Charge::TARGET& n ) throw( int ) :
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
  Xsection::CKernelBase *coeff;
  if( args.hasOpt( "ShortInteg" ) ){ 
    coeff = new IntegUnpolDIS::StrFunc( this, args, n, 4, 0, 1.0E-4 );
  } else {
    coeff = new IntegUnpolDIS::StrFunc( this, args, n, 4, 6, 1.0E-4 );
  }
  coeff->constNf( false );
  this->coeff( coeff );
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
