#ifndef _IntegPolDrellYan_DYPlusFunc_hh_
#define _IntegPolDrellYan_DYPlusFunc_hh_

#include <IntegUnpolDrellYan/DYPlusFunc.hh>

namespace IntegPolDrellYan{

  class DYPlusFunc : public IntegUnpolDrellYan::DYPlusFunc{
  public:
    DYPlusFunc( IntegXsec::ConvIntegPDF* xpdf, 
		const std::vector< std::complex< double > >& param ) : 
      IntegUnpolDrellYan::DYPlusFunc( xpdf, param ){};
    ~DYPlusFunc(){};

  };
};

#endif // _IntegPolDrellYan_DYPlusFunc_hh_
