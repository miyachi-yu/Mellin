#ifndef _VerView_hh_
#define _VerView_hh_

#include <TGNumberEntry.h>

namespace QCD {
  class PdfBase;
}

namespace Xsection {
  class Xsec;
}

namespace Evolution {
  class Kernel;
}

class PdfPanel;

class VerView : public TGNumberEntry {
public:
  
  VerView( const TGWindow *p, double& v, 
	   Evolution::Kernel *kernel, QCD::PdfBase *pdf,
	   PdfPanel *panel );
  
  ~VerView();
  
  void update();
  
private:
  
  double& ver_;
  Evolution::Kernel *kernel_;
  QCD::PdfBase *pdf_;
  PdfPanel *panel_;
  
  ClassDef( VerView, 1 )
};

#endif // _VerView_hh_
