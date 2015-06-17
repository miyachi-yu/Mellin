#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace std;
using namespace unpolDIS;

StrFunc::StrFunc( Utility::Arguments& args, 
		  const Xsection::Charge::TARGET& n ) throw( int ):
  Xsection::Xsec()
{
  
  try{
    this->xsecComp( new XsecComp( args, n ) );
    if( this->xsecComp() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::StrFunc( Evolution::KernelBase* kernel,
		  const Xsection::Charge::TARGET& n ) throw( int ) :
  Xsection::Xsec()
{
  
  try{
    this->xsecComp( new XsecComp( kernel, n ) );
    if( this->xsecComp() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::~StrFunc(){
  delete this->xsecComp() ;
}

