#ifndef _PdfView_hh_
#define _PdfView_hh_

#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>

#include <Evolution/Kernel.hh>
#include <PDFLIB/GRSV2000.hh>
#include <QCD/Flavor.hh>

#include "PdfGraph.hh"

class PdfView : public TCanvas {
public:
  PdfView( Evolution::Kernel *k, const double& w );
  virtual ~PdfView();
  
  void draw();
  
  void GRSV( PDFLIB::GRSV2000 *ptr ) { grsv_ = ptr ; }
  
private:
  Evolution::Kernel *k_;
  double  w_;
  
  TH1 *frame_;
  TLegend *legend_;
  
  double xmin_, xmax_, ymin_, ymax_;
  int n_;
  
  PDFLIB::GRSV2000 *grsv_;
  
  void frame();
  void legend();
  void pdf();
  void GRSV();
  
  PdfGraph& graph( Transform::RealFunction* f, 
		   double xmin, double xmax, int n );
  
  TGraph*   grsv( QCD::Flavor::PARTON q, 
		  double xmin = 1.0E-4, double xmax = 1.0,
		  int n = 40 );
};

#endif // _PdfView_hh_
