#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace std;
using namespace IntegFragXsec;

StrFunc::StrFunc( Utility::Arguments& args, const QCD::Flavor::HADRON& h,
		  const int& low, const int& high, const double& precision,
		  const double& length, const double& offset, 
		  const double& angle )
  throw( int ):
  IntegXsec::xXsec( low, high, precision ) // integration params
{
  
  try{
    
    if( low == 0 || high == 0 ){
      this->xsecInteg( new XsecComp( args, h, 4, 6, 1.0E-4, 
                                     length, offset, angle ) );
      // xPDF parames
    }else{
      this->xsecInteg( new XsecComp( args, h, low, high, precision, 
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
                  const int& low, const int& high, const double& precision )
  throw( int ):
  IntegXsec::xXsec( low, high, precision ) // integration params
{
  
  this->setGngrid( 12 );

  try{
    this->xsecInteg( new XsecComp( xc, args ) );
    
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

