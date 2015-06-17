#ifndef _IntegSPolVBoson_CoeffFunc_hh_
#define _IntegSPolVBoson_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>
#include <PDFLIB/GRSV2000.hh>
#include <PDFLIB/CTEQ6.hh>
#include <IntegXsec/Charge.hh>

#define PDF_SWAPP( T ) class T: public PDF_SW {        \
  public:                                               \
  T( IntegXsec::CKernel* cker ) : PDF_SW( cker ){};      \
  virtual ~T(){};                                       \
  virtual double operator()( const double& m );         \
  };

namespace IntegSPolVBoson {

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

    virtual void setQ2( const double& q2 );    

    bool getwjudge(){ return wboson_; }
    bool getwmjudge(){ return wmboson_; }

    double sumvvchar( const int& iflav );
    // for GQ(QG) coupling in W production 
    // necessity to sum up down or up kind charges
    // ( defined as a public function for StrFunc )

    int udjudge( const int& iflav );
    // for W production 
    // return 1 ( up kind ) or -1 ( down kind ) or 0 ( gluon )
    // ( defined as a public function for StrFunc )

    double oafactor( const int& i, const int& j );
    // for W production 
    // assume evo[ 0 ] polarized PDF ( i ) & evo[ 1 ] unpolarized PDF ( j )
    // ( defined as a public function for StrFunc )

  protected:

  private:

    bool wboson_; 
    // for w boson production ( default false ( z production ) )
    bool wmboson_; 
    // for w boson production ( default false ( w+ production ) )

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

#endif //  _IntegSPolVBoson_CoeffFunc_hh_
