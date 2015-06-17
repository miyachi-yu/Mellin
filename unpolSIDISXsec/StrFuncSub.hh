#ifndef _unpolSIDISXsec_STRFUNCSUB_HH_
#define _unpolSIDISXsec_STRFUNCSUB_HH_

#include <Utility/Arguments.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/Charge.hh>
#include <Evolution/KernelBase.hh>
#include <Fragmentation/Evolution.hh>

namespace unpolSIDISXsec {

  class CoeffFunc;

  class StrFuncSub : public Xsection::Xsec {
    
  public:
    
    StrFuncSub( Utility::Arguments& arg, 
		const int& sub,
		const Xsection::Charge::TARGET& n = Xsection::Charge::p,
		const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;

    StrFuncSub( Evolution::KernelBase *pdf,
		Evolution::Kernel *ff,
		const int& sub,
		const Xsection::Charge::TARGET& n = Xsection::Charge::p,
		const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int );
    
    virtual ~StrFuncSub();                          //!< a destructor
    
    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m );
    
    virtual void update();
    
    void setS( const double& s );
    void setX( const double& x );
    void setDirect( const bool& dir );
    
    int sub() { return sub_; }
    
  private:
    CoeffFunc* coeff_;
    int sub_;
    
  };

};

#endif	    /* !STRFUNCSUB_HH_ */
