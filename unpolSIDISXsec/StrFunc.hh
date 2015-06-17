#ifndef _unpolSIDISXsec_StrFunc_hh_
#define _unpolSIDISXsec_StrFunc_hh_

#include <Utility/Arguments.hh>
#include <Evolution/KernelBase.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/Charge.hh>

namespace unpolSIDISXsec {
  
  class CoeffFunc;

  class StrFunc : public Xsection::Xsec {
  public:
    
    StrFunc( Utility::Arguments& arg, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    StrFunc( Evolution::KernelBase *pdf,
	     Evolution::Kernel *ff,
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int );
    
    virtual ~StrFunc();                          //!< a destructor
    
    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m );
    
    virtual void update();
    
    void setS( const double& s );
    void setX( const double& x );
    
    void setDirect( const bool& dir );
    
  private:
    
    CoeffFunc* coeff_;
    
  };

};
#endif // _unpolSIDISXsec_StrFunc_hh_

