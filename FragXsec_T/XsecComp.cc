#include "XsecComp.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>
#include <Fragmentation/Evolution.hh>

#include "CoeffFunc.hh"
#include "WC.hh"

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Transform;
using namespace FragXsec_T;

XsecComp::XsecComp( Arguments& args,
		    const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::XsecComp()
{
  
  try { // prepare Evolution kernel for fragmentation function
    this->insert( Fragmentation::Evo::instance( h ), h );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

  this->coefficient();
}

XsecComp::XsecComp( Evolution::Kernel *kernel,
		    const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::XsecComp()
{
  this->insert( kernel, h );
  this->coefficient();
}

XsecComp::~XsecComp(){
  // delete all allocated objects in the constructor
  delete this->coeff()->wilsons();
  delete this->coeff();
  for( int i = 0; i < rescaleFcn().size(); i++ )
    if( rescaleFcn()[ i ] ) delete rescaleFcn()[ i ];
}

void XsecComp::update(){
}

void XsecComp::coefficient(){
  
  Arguments& args = Arguments::ref();
  
  // register CKernel object
  CoeffFunc *coeff = new CoeffFunc( args );
  coeff->constNf( false );
  coeff->dimension( this->vevo().size() );
  this->coeff( coeff );
  this->coeff()->wilsons( new WC( this ) );
  //! set Q^2 / \mu_R^2 to alpha_s
  double rescaleQ2 = args.get( "rescaleQ2", 1.0 );
  this->coeff()->alpha().setMURfact( rescaleQ2 );
  
  //! Scale rescaling function 
  //! Q^2 / \mu_F^2 
  double rescaleFF = args.get( "rescaleFF", 1.0 );// for evo[ 1 ]

  //! set \mu_F^2 / \mu_R^2 to alpha_s of each evolutoin
  this->vevo()[ 0 ]->alpha().setMURfact( rescaleQ2 / rescaleFF );
  
  RealFunction* rf_1_ = ( rescaleFF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescaleFF )
			  : ( RealFunction* ) NULL );
  this->rescaleFcn().resize( 0 );
  this->rescaleFcn().push_back( rf_1_ );
  
}

