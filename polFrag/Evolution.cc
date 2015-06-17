#include "Evolution.hh"
#include "P.hh"

#include <iostream>
#include <QCD/PdfParametrization.hh>
#include <QCD/ConfigFFbase.hh>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;
using namespace polFrag;

Evo::Evo( Arguments& args, 
	  const Flavor::HADRON& hadron ) throw( int ) : 
  Kernel( args ),
  hadron_( hadron )
{
  
  try {
    
    PdfParametrization *init = NULL;
    
    if( ( hadron_ == Flavor::PIp || hadron_ == Flavor::PIm ) &&
	args.hasOpt( "FF" ) ) {
      ConfigFFbase config( "FF" );
      init = new PdfParametrization( config );
    } else if( ( hadron_ == Flavor::KAp || hadron_ == Flavor::KAm ) &&
	       args.hasOpt( "FFK" ) ){
      ConfigFFbase config( "FFK" );
      init = new PdfParametrization( config );
    } else {
      cerr << __FILE__ << ":" << __LINE__ << "\t"
	   << "--" 
	   << ( hadron_ == Flavor::PIp || hadron_ == Flavor::PIm ? 
		"FF" : "FFK" ) 
	   << "=[initial FF file]  must be given to initializa FFs."
	   << endl;
      throw 1;
    }
    
    if( ! init->check() ) throw 2;
    this->initialDistribution( init );
    this->anomalousDimension( new P( this ) );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

Evo::~Evo() {
  delete this->initialDistribution();
  delete this->anomalousDimension();  
}

