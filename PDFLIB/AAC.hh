#ifndef _PDFLIB_AAC_hh_
#define _PDFLIB_AAC_hh_

#include "Package.hh"

#include <string>

namespace PDFLIB {
  
  class AAC : public Package {
  public:
    
    AAC( const int& iset , std::string path = "aac08_table" );
    virtual ~AAC();
    
    virtual double parton();
    virtual double type();

  protected:
    
    virtual void loadGrid() ;
    
  private:
    
    int    set_;
    
    double xppdf_[ 7 ];
    double grad_[ 7 ][ 11 ];
    
  };
};

#endif // _AAC_hh_
