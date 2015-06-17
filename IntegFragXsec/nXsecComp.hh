#ifndef _IntegFragXsec_nXsecComp_hh_
#define _IntegFragXsec_nXsecComp_hh_

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <Xsec/XsecComp.hh>
#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>

namespace IntegFragXsec {
  
  class nXsecComp : public Xsection::XsecComp {
  public:
    
    nXsecComp( Utility::Arguments& args,
	       const QCD::Flavor::HADRON& h ) throw( int ) ;
    
    nXsecComp( Evolution::KernelBase *kernel,
	       const QCD::Flavor::HADRON& h ) throw( int ) ;
    
    virtual ~nXsecComp();       //!< a destructor
    virtual void update();     //!< update method
    
  private:
    void coefficient();        //!< create coefficient function
  };
  
};

#endif //  _IntegFragXsec_nXsecComp_hh_
