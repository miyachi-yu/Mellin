#ifndef _IntegPolDrellYan_DYLogPlusFunc_hh_
#define _IntegPolDrellYan_DYLogPlusFunc_hh_

#include <IntegUnpolDrellYan/DYLogPlusFunc.hh>

namespace IntegPolDrellYan{
  
  class DYLogPlusFunc : public IntegUnpolDrellYan::DYLogPlusFunc {
  public:
    DYLogPlusFunc( IntegXsec::ConvIntegPDF* xpdf, 
		   const std::vector< std::complex< double > >& param ) : 
      IntegUnpolDrellYan::DYLogPlusFunc( xpdf, param ){};
    ~DYLogPlusFunc(){};
    
  };
};

#endif // _IntegPolDrellYan_DYLogPlusFunc_hh_
