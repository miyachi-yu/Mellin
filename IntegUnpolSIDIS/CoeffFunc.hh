#ifndef _IntegUnpolSIDIS_CoeffFunc_hh_
#define _IntegUnpolSIDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>

#include "StrFunc.hh"

#define PDF_SWAPP( T ) class T: public PDF_SW {        \
  public:                                               \
  T( IntegXsec::CKernel* cker ) : PDF_SW( cker ){};      \
  virtual ~T(){};                                       \
  virtual double operator()( const double& m );         \
  };

namespace IntegUnpolSIDIS {

  class PDF_SW : public Transform::RealFunction{
  public:
    PDF_SW( IntegXsec::CKernel* cker ): cker_( cker ){};
    virtual ~PDF_SW(){};
    virtual double operator()( const double& m ) = 0;
    PDF_SW* seti( const int& i ){ i_ = i; return this; } 
  protected:
    int su2swap( const int& i );
    int i_;  
    IntegXsec::CKernel* cker_;  
  };
  
  class CoeffFunc: public IntegXsec::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, IntegXsec::IntegXsecComp* xc,
	       const IntegXsec::Charge::TARGET& n );
    virtual ~CoeffFunc();

  protected:

    virtual void update();

  private:
    PDF_SW* PDF_;    

    void setPDFswitch( const IntegXsec::Charge::TARGET& n );

  };

  PDF_SWAPP( PDF_P );
  PDF_SWAPP( PDF_N );
  PDF_SWAPP( PDF_D );  

};

#endif //  _IntegUnpolSIDIS_CoeffFunc_hh_
