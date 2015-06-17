// $Id: MultiPanel.hh,v 1.1 2010/05/07 14:45:32 miyachi Exp $
#ifndef _MultiPanel_hh_
/* 
   Residual plot for HERMES multiplicity: based on multiPanel.cc
   Display residual in 4by4 layout?
 */
#include <string>

// -------------------------------------------------------------------- //
#include <TApplication.h>
#include <TLatex.h>
#include <TLine.h>

class TPostScript;
class TCanvas;
class TBox;
class TH1;

// -------------------------------------------------------------------- //
#include <Utility/Arguments.hh>

// -------------------------------------------------------------------- //
class MultiPanel : public TApplication {
public:

  MultiPanel( int argc, char* argv[] ) throw( int ) ;
  virtual ~MultiPanel();
  virtual void Run( Bool_t retrn = kFALSE );
  
  void Usage();
  
  virtual void drawPad( const int& co, const int& ro, TH1 *frame );
  virtual void privateStyle(){}
  virtual void setRow();
  virtual void setColumn();

protected:
  virtual TCanvas*    createCanvas();
  virtual std::string ofilename();
  double posX_( const double& x );
  double posY_( const double& y );
  double labelPosX();
  double labelPosY();
  void style();

private:

  
protected:
  // useful tools
  TLatex latex_;
  TLine  line_;
  
  // output file setting
  std::string suffix_;
  TPostScript *ps_;
  std::string ofname_;
  
  // canvas setting
  TCanvas *c_;
  int panelSizeX_;
  int panelSizeY_;
  double marginBottom_;
  double marginLeft_;
  int column_;
  int row_;

  // frame setting
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;
  bool   fLogx_;
  bool   fLogy_;

  // axis label setting
  double labelX_;
  double labelY_;
  double labelSize_;
  double labelAlign_;
  double xlabel_offset_;
  double ylabel_offset_;
  double xlabel_size_;
  double ylabel_size_;

  // axis title setting
  std::string xtitle_;
  std::string ytitle_;
  double xtitle_offset_;
  double ytitle_offset_;
  double xtitle_size_;
  double ytitle_size_;
  
};

#define _MultiPanel_hh_
#endif // _MultiPanel_hh_
