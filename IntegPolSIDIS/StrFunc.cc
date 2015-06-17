#include "StrFunc.hh"
#include "XsecComp.hh"
#include <Xsec/XsecCompBase.hh>

using namespace std;
using namespace IntegPolSIDIS;

StrFunc::StrFunc( Utility::Arguments& args, const IntegXsec::Charge::TARGET& n,
                  const QCD::Flavor::HADRON& h,
		  const int& low, const int& high, const double& precision,
                  const double& length, const double& offset, 
                  const double& angle )
  throw( int ):
  IntegXsec::xzXsec( low, high, precision ) // integration params
{
  
  try{
    if( low == 0 || high == 0 ){
      this->xsecInteg( new XsecComp( args, n, h, 4, 6, 1.0E-4, 
                                     length, offset, angle ) );
      // xPDF parames
    }else{
      this->xsecInteg( new XsecComp( args, n, h, low, high, precision, 
				     length, offset, angle ) );
      // xPDF parames
    }

    if( this->xsecInteg() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }

}

StrFunc::StrFunc( Xsection::XsecCompBase* xc, Utility::Arguments& args, 
		  const IntegXsec::Charge::TARGET& n,
		  const int& low, const int& high, const double& precision )
  throw( int ):
  IntegXsec::xzXsec( low, high, precision ) // integration params
{
  
  this->setGngrid( 12 );

  try{
    this->xsecInteg( new XsecComp( xc, args, n ) );
    
    if( this->xsecInteg() == NULL ) throw 1 ;
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
}

StrFunc::~StrFunc(){
  delete this->xsecInteg() ;
}

