#ifndef _GRSV2000_hh_
#define _GRSV2000_hh_

#include "Package.hh"

#include <string>

namespace PDFLIB {
  
  class GRSV2000 : public Package {
  public:
    
    enum TYPE {
      STANDARD_NLO = 1, VALENCE_NLO = 2, STANDARD_LO = 3, VALENCE_LO = 4
    };
    
    GRSV2000( const TYPE& = STANDARD_NLO, std::string path = "grsv2000_table" );
    virtual ~GRSV2000();
    
    virtual double parton();
    virtual double type();
    virtual double func();

  protected:
    
    virtual void loadGrid() ;
    
  private:
    static int    set_;
    double u_;
    double d_;
    double ub_;
    double db_;
    double st_;
    double gl_;
    double g1p_;
    double g1n_;
  };
};

#endif // _GRSV2000_hh_
