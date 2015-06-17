#ifndef _unpolSIDISXsec_xzXsec_hh_
#define _unpolSIDISXsec_xzXsec_hh_

#include <Xsec/xzXsec.hh>
#include <Xsec/Charge.hh>
#include <QCD/Flavor.hh>

namespace Xsection{
  class Xsec;
}

namespace unpolSIDISXsec {
  
  class xzXsec : public Xsection::xzXsec {
  public:
    
    xzXsec( Xsection::Xsec *xc, 
	    const int& low = 4, const int& high = 6, 
	    const double& precision = 1.0E-4,
	    const double& length = 10.0,
	    const double& offset = 1.8,
	    const double& angle = 0.6 );
    
    xzXsec( Xsection::Xsec* xc, Transform::Integration* integ );
    
    virtual ~xzXsec();      //!< a destructor
    
  private:
    virtual void setxzparams( const std::vector< double >& x, 
                              const std::vector< double >& z );
    
  };
  
};
#endif // _unpolSIDISXsec_xzXsec_hh_
