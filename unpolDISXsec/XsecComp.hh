#ifndef _unpolDISXsec_XsecComp_hh_
#define _unpolDISXsec_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>

namespace unpolDISXsec {
  
  class XsecComp : public Xsection::XsecComp {
  public:
    
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n ) throw( int ) ;

    XsecComp( Evolution::KernelBase *kernel,
              const Xsection::Charge::TARGET& n ) throw( int ) ;
    
    virtual ~XsecComp();
    
    virtual void update();

    void setS( const double& s );
    void setX( const double& x );

  private:
  
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );

  };
  
};

#endif //  _unpolDISXsec_XsecComp_hh_
