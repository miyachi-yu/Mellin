#include "nXsecComp.hh"
#include "StrFunc.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <Fragmentation/Evolution.hh>

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace QCD;
using namespace IntegFragXsec;

nXsecComp::nXsecComp( Arguments& args,
		      const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::XsecComp()
{
  
  try { // prepare Evolution kernel for fragmentation function
    this->insert( Fragmentation::Evo::instance( h ), h );
    dynamic_cast< Fragmentation::Evo* >( this->vevo()[ 0 ] )->constNf( false );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

  this->coefficient();
}

nXsecComp::nXsecComp( Evolution::KernelBase *kernel,
		      const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::XsecComp()
{

  if( dynamic_cast< Fragmentation::Evo* >( kernel ) ){
    this->insert( kernel, h );
  } else {
    cerr << __FILE__ << ":" << __LINE__ 
         << "\t kernel should be Fragmentation::Evo"
         << endl;
  }
  this->coefficient();
}

nXsecComp::~nXsecComp(){
  // delete all allocated objects in the constructor
  if( this->coeff() ) delete this->coeff();
  for( int i = 0; i < rescaleFcn().size(); i++ )
    if( rescaleFcn()[ i ] ) delete rescaleFcn()[ i ];
}

void nXsecComp::update(){
}

void nXsecComp::coefficient(){  
  Arguments& args = Arguments::ref();
  
  // register CKernel object
  Xsection::CKernelBase *coeff;
  if( args.hasOpt( "ShortInteg" ) ){ 
    coeff = new IntegFragXsec::StrFunc( this, args, 4, 0, 1.0E-4 );
  } else {
    coeff = new IntegFragXsec::StrFunc( this, args, 4, 6, 1.0E-4 );
  }
  coeff->constNf( false );
  this->coeff( coeff );
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
