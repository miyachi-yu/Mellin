#ifndef _InverseMellinHelper_hh_
#define _InverseMellinHelper_hh_

#include <vector>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>

#include <Utility/Timer.hh>
#include <Xsec/xXsec.hh>

#include "RealFunctionWrapper.hh"

class InverseMellinHelper : public TCanvas {
  
public:
  
  InverseMellinHelper();
  virtual ~InverseMellinHelper();
  
  void set( Xsection::xXsec *g ) { func_.set( g ); }
  void add( const std::vector< double >& x );
  void add( const std::vector< double >& x, const std::vector< double >& z );
  
  void draw( const double& min, const double& max, const int& n );

  void Q2( const double& q2 ) { q2_ = q2; }
  
private:
  
  RealFunctionWrapper func_;
  
  //  x and z values to be calculated.
  std::vector< std::vector< double > > x_;
  std::vector< std::vector< double > > z_;
  
  // calcuated xsection values
  std::vector< double > f_;
  double fsum_;
  double msec_;
  
  double c_;
  double phi_;
  double l_;
  
  Utility::Timer timer_;
  
  TLine *line_;
  TLatex  *latex_;

  double q2_;
  
};

#endif // _InverseMellinHelper_hh_
