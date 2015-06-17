#ifndef _unpolSIDISXsec_XsecComp_hh_
#define _unpolSIDISXsec_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>
#include <Fragmentation/Evolution.hh>

namespace unpolSIDISXsec {
  
  class XsecComp : public Xsection::XsecComp {
  public:
    
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n,
	      const QCD::Flavor::HADRON& h ) throw( int ) ;

    XsecComp( Evolution::KernelBase *pdf, 
              Evolution::Kernel *ff,
              const Xsection::Charge::TARGET& n = Xsection::Charge::p,
              const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int );
    
    virtual ~XsecComp();
    
    virtual void update();

    void setS( const double& s );
    void setX( const double& x );

  private:
    
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );
  
  };
  
};

#endif //  _unpolSIDISXsec_XsecComp_hh_
