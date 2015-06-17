//
// BB.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:08:13 2008 Yoshiyuki Miyachi
// Last update Thu Oct  2 18:36:18 2008 Yoshiyuki Miyachi
//

#ifndef _PDFLIB_BB_HH_
#define _PDFLIB_BB_HH_

#include "Package.hh"

namespace PDFLIB {
  
  class BB : public Package {
  public:
    
    BB( const int& set, const std::string& path = "bb_table" );
    virtual ~BB();

    virtual double parton();
    virtual double type();
    virtual double func();
    
  protected:
    virtual void loadGrid() ;
    
  private:

    int iset_;
    double q2_;
    double uv_;
    double duv_;
    double dv_;
    double ddv_;
    double gl_;
    double dgl_;
    double qb_;
    double dqb_;
    double g1p_;
    double dg1p_;
    double g1n_;
    double dg1n_;
  };
};

#endif	    /* !BB_HH_ */
