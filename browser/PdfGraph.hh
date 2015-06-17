#ifndef _PdfGraph_hh_
#define _PdfGraph_hh_

#include <ExpData/DataSet.hh>
#include <StrFncParam/F1Param.hh>
#include <StrFncParam/ALLM97.hh>


#include <TGraphErrors.h>
#include <string>

namespace Transform {
  class RealFunction;
}

class PdfGraph : public TGraphErrors {
public:
  
  PdfGraph( Transform::RealFunction *f, 
	    double xmin = 1.0E-4, double xmax = 1.0E0, int n = 40,
	    const double& weigth = 0.0, 
	    const double& scale = 1.0 );
  
  PdfGraph( Exp::DataSet& eset, 
	    const double& weigth = 0.0, 
	    const double& scale = 1.0,
	    const std::string& xlabel = "x" );
  
  virtual ~PdfGraph();
  
  std::string legend( const std::string& type = "polPDF" ) const ;
  
  double maxY() const { return maxY_; }
  double minY() const { return minY_; }
  
  void update();

  
private:
  Transform::RealFunction *f_;
  double w_;
  double minX_;
  double maxX_;
  double maxY_;
  double minY_;
  
  StrFncParam::ALLM97  sigmaT_;
  StrFncParam::F1Param f1_;
  
  double scale_;
};

#endif // _PdfGraph_hh_
