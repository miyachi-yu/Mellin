#ifndef _FragXsecPanel_hh_
#define _FragXsecPanel_hh_

#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLatex.h>
#include <TH1.h>
#include <TGraph.h>

#include <QCD/Flavor.hh>
#include <Xsec/xXsec.hh>
#include <ExpData/ExpData.hh>
#include <Fragmentation/Evolution.hh>
#include <FragXsec_T/FragXsec.hh>

#include <vector>

class PdfGraph;

class FragXsecPanel : public TCanvas {
public:
  
  FragXsecPanel() throw( int );
  FragXsecPanel( Utility::Arguments& args ) throw( int );
  
  virtual ~FragXsecPanel();
  
  void set( Fragmentation::Evo *evolution );
  void set( Exp::ExpData *data );
  
  void draw();
  void drawXsec();
  void drawData();
  void update();
  
  Fragmentation::Evo* ff() throw( int ) { return evolution_; }
  
private:
  
  void initialize();
  
  void drawFrame();
  void xsections();
  
  Fragmentation::Evo *evolution_;
  std::vector< Xsection::Xsec* >xsec_;
  std::vector< Xsection::xXsec* >xxsec_;
  
  QCD::Flavor::HADRON h_;
  bool toBeDelXsec_;

  Exp::ExpData         *expr_;
  bool toBeDelExpr_;
  
  TLegend         *legeExp_;
  TLegend         *legeThe_;
  TLine           *line_;
  TLatex          *text_;
  
  TH1             *frame_;
  std::vector< double > x_;
  std::vector< double > y_;
  
  std::vector< double > q2_;
  std::vector< double > scale_;
  
  std::vector< std::vector< PdfGraph* > > graph_;
  
  double c_;
  double phi_;
  double length_;
  
  Fragmentation::Evo* ff( const QCD::Flavor::HADRON& h ) throw( int ) ;
  std::string hadronName( const QCD::Flavor::HADRON& h ) ;
  std::string hadronPrintName( const QCD::Flavor::HADRON& h ) ;
  
  std::string convertExpLeg( const std::string& str );
  
};

#endif // _FragXsecPanel_hh_
