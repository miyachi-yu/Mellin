#ifndef _Application_hh_
#define _Application_hh_

#include <string>

#include <TApplication.h>
#include <TGraph.h>

#include "PdfGraph.hh"
#include "FragXsecPanel.hh"

#include <QCD/Flavor.hh>

#include <PDFLIB/GRSV2000.hh>
#include <Evolution/PDFx.hh>

#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>

#include <FragXsec_T/FragXsec.hh>

#include "InverseMellinHelper.hh"


class TCanvas;
class TLegend;
class TH1;

namespace Exp {
  class ExpData;
};

class Application : public TApplication {
  
public:
  
  Application( int argc, char* argv[] ) throw( int ) ;
  virtual ~Application();
  virtual void Run( Bool_t retrn = kFALSE );
  
private:
  
  polPDF::Evo               *evo_;
  Fragmentation::Evo        *evoFF_;
  FragXsec_T::FragXsec      *xsec_;
  
  double             w_;
  
  TCanvas            *c_;
  PDFLIB::GRSV2000   *grsv_;
  std::string         grsvPath_;
  
  Exp::ExpData       *exp_;
  
  std::string         mellinType_;

  double             q2_;
  
  void configure() throw ( int ) ;
  
  PdfGraph& createGraph( Transform::RealFunction *f, 
			 double xmin = 1.0E-4, 
			 double xmax = 1.0, 
			 int n = 80 );
  
  void createPDF();
  void createMoment();
  void createG1();
  void createFF();
  
  void drawFF( TH1 *frame, TLegend *legend );
  
  bool viewPDF_;
  bool viewMoment_;
  bool viewMellin_;
  bool viewG1_;
  bool viewFF_;
  bool viewFragXsec_;
  bool viewFragXsecMom_;
  
  int  nucleon_;

  bool isParton_;
  QCD::Flavor::PARTON q_;
  QCD::Flavor::TYPE   type_;
  
  FragXsecPanel *xsecPanel_;

  InverseMellinHelper *helper_;

};

#endif //  _Application_hh_
