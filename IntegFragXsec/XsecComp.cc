#include "XsecComp.hh"

#include <iostream>
#include <iomanip>

#include <Fragmentation/Evolution.hh>

#include "CoeffFunc.hh"
#include "WC.hh"

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace IntegFragXsec;

XsecComp::XsecComp( Arguments& args, const QCD::Flavor::HADRON& h,
		    const int& low, const int& high, const double& precision,
		    const double& length, const double& offset, 
		    const double& angle ) throw( int ) :
  IntegXsec::XsecIntegrand()
{
  
  try {

    // parameters for FF calc
    this->setIMparam( length, offset, angle );
    this->setIMIntegparam( low, high, precision );

    // prepare Evolution kernel for FF function
    if( args.hasOpt( "FF" ) ){
      Fragmentation::Evo *FF = new Fragmentation::Evo( args, h );
      FF->constNf( false );
      this->insert( FF );
    } 
    else {
      cerr 
	<< __FILE__ << ":" << __LINE__
	<< "--FF is required." << endl;
      throw( 1 );
    }
    
    //    cout << "self vevo size: " << this->vevo().size() << endl;
    //    cout << "actual vevo size: " << this->getvevoXC()->vevo().size() 
    //   << endl;

    // register CKernel object
    CoeffFunc *coeff = new CoeffFunc( args, this );
    coeff->constNf( false );
    this->coeff( coeff );
    dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
      ->wilsons( new WC( this ) );
    
    // Scale rescaling function 
    // if you want to control 'scale' in integ calc
    // insert several functions here (ref. nXsecComp.cc)
    this->rescaleFcn().resize( 1, (RealFunction*) NULL );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

XsecComp::XsecComp( Xsection::XsecCompBase* xc, Arguments& args ) 
  throw( int ) :
  IntegXsec::XsecIntegrand( xc )
{
  
  try {

    //    cout << "self vevo size: " << this->vevo().size() << endl;
    //    cout << "actual vevo size: " << this->getvevoXC()->vevo().size() 
    //   << endl;

    // register CKernel object
    CoeffFunc *coeff = new CoeffFunc( args, this );
    coeff->constNf( false );
    this->coeff( coeff );
    dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
      ->wilsons( new WC( this ) );
    
    // Scale rescaling function 
    // scale functions should not be manipulated here (only in nXsecComp)
    this->rescaleFcn().resize( 1, (RealFunction*) NULL );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
    throw error;
  }
  
}

XsecComp::~XsecComp(){
  
  // delete all dynamically allocated objects by this
  for( int i = 0; i < this->vevo().size(); i++ )
    if( this->vevo()[ i ] ) delete this->vevo()[ i ];
  
  if( dynamic_cast< IntegXsec::CKernel* >( this->coeff() )->wilsons() ) 
    delete dynamic_cast< IntegXsec::CKernel* >( this->coeff() )->wilsons();
  if( this->coeff() ) delete this->coeff();
  
}

void XsecComp::update(){
}

