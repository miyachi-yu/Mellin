#ifndef _Fragmentation_KretzerFF_hh_
#define _Fragmentation_KretzerFF_hh_

#include <QCD/FFSimple.hh>
#include <string>

namespace PDFSET{
  
  class KretzerFF : public QCD::FFSimple {
  public:
    KretzerFF();
    virtual ~KretzerFF();
    
    //!< update local stored variables using the present parameters
    virtual void update();

  };
  
};

#endif // _Fragmentation_KretzerFF_hh_
