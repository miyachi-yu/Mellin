//
// LSS.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:08:13 2008 Yoshiyuki Miyachi
// Last update Thu Oct  2 18:36:18 2008 Yoshiyuki Miyachi
//

#ifndef _PDFLIB_LSS_HH_
#define _PDFLIB_LSS_HH_

#include "Package.hh"

namespace PDFLIB {
  
  class LSS : public Package {
  public:
    
    enum VERSION {
      v2006,
      v2005,
      v2001
    };
    
    LSS( const int& set,
	 const std::string& path = "lss06_table",
	 const VERSION& version = v2006 );
    
    virtual ~LSS();

    virtual double parton();
    virtual double type();
    virtual double func();

  protected:
    virtual void loadGrid() ;
    
  private:
    VERSION version_;
    static int iset_;
    double q2_;
    double uub_;
    double ddb_;
    double ssb_;
    double gl_;
    double uv_;
    double dv_;
    double ub_;
    double db_;
    double st_;
    double g1plt_;
    double g1p_;
    double g1nlt_;
    double g1n_;

    
  };
  
};

#endif	    /* !LSS_HH_ */
