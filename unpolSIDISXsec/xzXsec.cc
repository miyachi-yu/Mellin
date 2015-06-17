#include "xzXsec.hh"
#include "StrFunc.hh"
#include <Xsec/Xsec.hh>

using namespace std;
using namespace unpolSIDISXsec;

xzXsec::xzXsec( Xsection::Xsec* xc,
		const int& low, const int& high, const double& precision,
		const double& length, const double& offset, 
		const double& rangle ) : 
  Xsection::xzXsec( xc, low, high, precision, length, offset, rangle ){
}

xzXsec::xzXsec( Xsection::Xsec* xc, Transform::Integration* integ ) :
  Xsection::xzXsec( xc, integ ){
}

xzXsec::~xzXsec(){
}

void xzXsec::setxzparams( const vector< double >& x, 
			  const vector< double >& z ){
  dynamic_cast< StrFunc* >( this->moment() )->setX( x[ 0 ] );
}
