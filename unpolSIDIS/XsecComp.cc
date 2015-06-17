#include "XsecComp.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>

#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>

#include "CoeffFunc.hh"
#include "WC.hh"

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Utility;
using namespace unpolSIDIS;

XsecComp::XsecComp( Arguments& args,
		    const Xsection::Charge::TARGET& n,
		    const QCD::Flavor::HADRON& h  ) throw( int ) :
  Xsection::XsecComp()
{
  
  try {
    
    // prepare 1st Evolution kernel for polSIDIS function ( n )
    this->insert( unpolPDF::Evo::instance(), Flavor::PRp );
    
    // prepare 2nd Evolution kernel for polSIDIS function ( m )
    this->insert( Fragmentation::Evo::instance( h ), h );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
  this->coefficients( n );  

}

XsecComp::XsecComp( Evolution::KernelBase *pdf, 
		    Evolution::Kernel *ff,
		    const Xsection::Charge::TARGET& n,
		    const QCD::Flavor::HADRON& h ) 
  throw( int ) : 
  Xsection::XsecComp()
{
  
  // prepare 1st Evolution kernel for polSIDIS function ( n )
  if( dynamic_cast< unpolPDF::Evo* >( pdf ) ||
      dynamic_cast< unpolPDF::CTEQ6pdf* >( pdf ) ){
    this->insert( pdf, Flavor::PRp );
  } else {
    ARG_ERR << __FILE__ << ":" << __LINE__ 
	    << "\tpdf should be either unpolPDF::Evo or unpolPDF::CTEQ6pdf"
	    << endl;
    throw( 1 );
  }
  
  // prepare 2nd Evolution kernel for polSIDIS function ( m )
  this->insert( ff, h );
  
  this->coefficients( n );
}


XsecComp::~XsecComp(){
  // delete all dynamically allocated objects by this
  /*
  for( int i = 0; i < this->vevo().size(); i++ ){
    //    for( int j = -6; j <= 6; i++ ) this->vpdf()[ i ][ j ];
    delete this->vevo()[ i ];
  }
  */
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
  double rescaleFF = args.get( "rescaleFF", 1.0 );// for evo[ 1 ]

  //! set \mu_F^2 / \mu_R^2 to alpha_s of each evolutoin
  this->vevo()[ 0 ]->alpha().setMURfact( rescaleQ2 / rescaleUPDF );
  this->vevo()[ 1 ]->alpha().setMURfact( rescaleQ2 / rescaleFF );
  
  RealFunction* rf_1_ = ( rescaleUPDF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescaleUPDF )
			  : ( RealFunction* ) NULL );
  RealFunction* rf_2_ = ( rescaleFF != 1.0 ? 
			  new Xsection::XsecComp::SimpleRescale( rescaleFF )
			  : ( RealFunction* ) NULL );
  this->rescaleFcn().resize( 0 );
  this->rescaleFcn().push_back( rf_1_ );
  this->rescaleFcn().push_back( rf_2_ );

}
