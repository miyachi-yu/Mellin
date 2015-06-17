#include "XsecComp.hh"

#include <iostream>
#include <iomanip>

#include <polPDF/Evolution.hh>

#include "CoeffFunc.hh"
#include "WC.hh"

using namespace std;
using namespace Transform;
using namespace Utility;
using namespace IntegPolVBoson;

XsecComp::XsecComp( Arguments& args, const IntegXsec::Charge::TARGET& n,
		    const QCD::Flavor::HADRON& h,
                    const int& low, const int& high, const double& precision,
                    const double& length, const double& offset, 
                    const double& angle ) throw( int ) :
  IntegXsec::XsecIntegrand()
{
  
  try {

    // parameters for xPDF calc
    this->setIMparam( length, offset, angle );
    this->setIMIntegparam( low, high, precision );

    // prepare Evolution kernel for polVBoson function
    if( args.hasOpt( "polPDF" ) || args.hasOpt( "BB" ) 
	|| args.hasOpt( "DSSV" ) ){
      polPDF::Evo *polPDF = new polPDF::Evo( args );
      polPDF->constNf( false );
      //      polPDF->constNf( true );
      //1st PDFs
      this->insert( polPDF, QCD::Flavor::PRp );
      //2nd PDFs // need deconstrunctor modification
      this->insert( polPDF, h );
    } 
    else {
      //      /*
      cerr 
	<< __FILE__ << ":" << __LINE__
	<< "--polPDF or --BB or --DSSV is required." << endl;
      //      */
    }

    //    cout << "self vevo size: " << this->vevo().size() << endl;
    //    cout << "actual vevo size: " << this->getvevoXC()->vevo().size() 
    //         << endl;

    // register CKernel object
    CoeffFunc *coeff = new CoeffFunc( args, this, n );
    coeff->constNf( false );
    //    coeff->constNf( true );
    this->coeff( coeff );
    dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
      ->wilsons( new WC( this ) );
    
    // Scale rescaling function 
    // if you want to control 'scale' in integ calc
    // insert several functions here (ref. nXsecComp.cc)
    this->rescaleFcn().resize( 2, (RealFunction*) NULL );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

XsecComp::XsecComp( Xsection::XsecCompBase* xc, 
		    Arguments& args, const IntegXsec::Charge::TARGET& n ) 
  throw( int ) :
  IntegXsec::XsecIntegrand( xc )
{
  
  try {

    //    cout << "self vevo size: " << this->vevo().size() << endl;
    //    cout << "actual vevo size: " << this->getvevoXC()->vevo().size() 
    //         << endl;

    // register CKernel object
    CoeffFunc *coeff = new CoeffFunc( args, this, n );
    coeff->constNf( false );
    //    coeff->constNf( true );
    this->coeff( coeff );
    dynamic_cast< IntegXsec::CKernel* >( this->coeff() )
      ->wilsons( new WC( this ) );
    
    // Scale rescaling function 
    // scale functions should not be manipulated here (only in nXsecComp)
    this->rescaleFcn().resize( 2, (RealFunction*) NULL );
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

XsecComp::~XsecComp(){
  
  // delete all dynamically allocated objects by this
  // pdfs are deleted in IntegXsecComp deconstructor
  for( int i = 0; i < this->vevo().size(); i++ )
    if( this->vevo()[ i ] )
      // delete self vevos only!
      if( this->vevo()[ i ] == this->vevo()[ i + 1 ] ){
	delete this->vevo()[ i + 1 ]; i++;
	continue;
      }else{
	delete this->vevo()[ i ];
      }
  if( dynamic_cast< IntegXsec::CKernel* >( this->coeff() )->wilsons() ) 
    delete dynamic_cast< IntegXsec::CKernel* >( this->coeff() )->wilsons();
  if( this->coeff() ) delete this->coeff();
  
}

void XsecComp::update(){
}

