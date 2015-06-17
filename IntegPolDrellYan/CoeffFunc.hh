#ifndef _IntegPolDrellYan_CoeffFunc_hh_
#define _IntegPolDrellYan_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>
#include <PDFLIB/GRSV2000.hh>

#include "StrFunc.hh"

#define PDF_SWAPP( T ) class T: public PDF_SW {        \
  public:                                               \
  T( IntegXsec::CKernel* cker ) : PDF_SW( cker ){};      \
  virtual ~T(){};                                       \
  virtual double operator()( const double& m );         \
  };

namespace IntegPolDrellYan {

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
  private:

    virtual void update();  // for integ calc
    virtual void update2();

  private:
    PDF_SW* PDF_;    
    PDFLIB::Package* cteq6_1;
    PDFLIB::Package* cteq6_2;
    bool xcteq6_;

    void setPDFswitch( const IntegXsec::Charge::TARGET& n );

  };

  PDF_SWAPP( PDF_P );
  PDF_SWAPP( PDF_N );
  PDF_SWAPP( PDF_D );  

};

#endif //  _IntegPolDrellYan_CoeffFunc_hh_
