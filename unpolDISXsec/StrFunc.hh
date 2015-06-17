#ifndef _unpolDISXsec_StrFunc_hh_
#define _unpolDISXsec_StrFunc_hh_

#include <Utility/Arguments.hh>
#include <Evolution/KernelBase.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/Charge.hh>

#include "CoeffFunc.hh"

namespace unpolDISXsec {
  
  class StrFunc : public Xsection::Xsec {
  public:
    
    StrFunc( Utility::Arguments& arg, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p ) 
      throw( int ) ;
    
    StrFunc( Evolution::KernelBase* kernel,
             const Xsection::Charge::TARGET& n = Xsection::Charge::p ) 
      throw( int ) ;

    virtual ~StrFunc();                          //!< a destructor
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n );

    virtual void update();

    void setS( const double& s );
    void setX( const double& x );

    void setDirect( const bool& dir );

    static double HERMES_S;
    
  private:
    CoeffFunc* coeff_;
    
  };
  
};

#endif // _unpolDISXsec_StrFunc_hh_
