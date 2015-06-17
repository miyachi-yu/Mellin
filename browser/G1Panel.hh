#ifndef _G1Panel_hh_
#define _G1Panel_hh_

#include <TCanvas.h>
#include <TPad.h>

#include <vector>
#include <string>
#include <map>

class TLine;
class TGraph;
class TLatex;

namespace Exp {
  class ExpData;
  class DataPoint;
};

namespace Xsection {
  class Xsec;
  class xXsec;
};

class PdfGraph;

#include "ScaleFactor.hh"

class G1Panel : public TCanvas {
  
public:
  
  G1Panel();
  virtual ~G1Panel();
  
  void setQ2( const double& q2 );
  void draw();
  
  class Style {
  public:
    int color;
    int marker;
  };
  
  Xsection::Xsec* xsec();
    
private:
  
  Exp::ExpData                *edata_;
  
  std::vector< Xsection::xXsec* > g1_;
  std::vector< std::vector< PdfGraph* > > gg1_;
  
  TLine *line_;
  TLatex *latex_;

  void createPads();
  void pads_x();
  void pads_q2();

  void draw_x();
  void draw_q2();
  
  std::vector< double > xrange_;

  void drawExp();
  
  std::map< std::string, Style > style_;

  std::vector< double > q2_;

  void style();

  double weight_;
  
  ScaleFactor xedge_;
  std::vector< TGraph* > q2p_;
  std::vector< TGraph* > q2d_;
  
  bool vsX_;


};

#endif // _G1Panel_hh_
