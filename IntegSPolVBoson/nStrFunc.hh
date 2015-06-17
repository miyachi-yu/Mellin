#ifndef _IntegSPolVBoson_nStrFunc_hh_
#define _IntegSPolVBoson_nStrFunc_hh_

#include <Utility/Arguments.hh>
#include <polPDF/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <IntegXsec/Charge.hh>

namespace IntegSPolVBoson {
  
  class nStrFunc : public Xsection::Xsec {
  public:
    
    nStrFunc( Utility::Arguments& arg, 
	      const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PRp ) 
      throw( int ) ;
    
    nStrFunc( Evolution::KernelBase *pdf1,
	      Evolution::KernelBase *pdf2,
	      const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PRp ) 
      throw( int ) ;
    
    virtual ~nStrFunc();                          //!< a destructor
    
    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m );
    
  private:
    
  };
  
};
#endif // _IntegSPolVBoson_nStrFunc_hh_

