#ifndef _MomentView_hh_
#define _MomentView_hh_

#include <TCanvas.h>

#include <QCD/Flavor.hh>
#include <Tranform/ComplexFunction.hh>
#include <Evolution/Kernel.hh>
#include <Evolution/PDF.hh>
#include <Xsec/Xsec.hh>

#include <string>

class TH2;

class MomentView : public TCanvas {
public:
  
  MomentView( Evolution::Kernel *kernel, const QCD::Flavor::PARTON& q );
  MomentView( Evolution::Kernel *kernel, const QCD::Flavor::TYPE&   q );
  MomentView( Xsection::Xsec* fcn );
  virtual ~MomentView();
  
  void draw( std::string type = "real" );

private:
  
  Evolution::Kernel          *k_;
  Transform::ComplexFunction *fcn_;
  Xsection::Xsec             *xsec_;  
  Evolution::PDF             *q_;
  TH2                        *nspace_;
  
};


#endif // _MomentView_hh_
