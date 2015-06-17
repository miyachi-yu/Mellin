#include "StrFunc.hh"
#include "XsecComp.hh"
#include <Xsec/XsecCompBase.hh>

using namespace std;
using namespace IntegPolDrellYan;

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

bool StrFunc::setActflav( const int& i, const int& j ){

  if( i == 0 && j == 0 ) return false; // gg elimination
  if( i * j != 0 && i != -j ) return false; // qq'(q'bar) elimination

  (  i == 0 ? acti_ = 0  : acti_ = 1 );
  (  j == 0 ? actj_ = 0  : actj_ = 1 );
  if( acti_ * actj_ != 0 ) actj_ = -1; // acti_ = actj_ = 1
  // ug(10) & gu(01) && uu(1-1) -> qg & gq & qqbar in wc
  // no difference for q <-> qbar & q <-> q' 

  if( actwc2_.size() == 0 ){
    actwc2_[ 1 ][ 0 ] = 0; // qg -> cdata_[ 0 ]
    actwc2_[ 0 ][ 1 ] = 1; // gq -> cdata_[ 1 ]
    actwc2_[ 1 ][ -1 ] = 2; // qqbar -> cdata_[ 2 ]
  }

  return true;
}
