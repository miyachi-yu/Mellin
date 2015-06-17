#ifndef _Manager_hh_
#define _Manager_hh_

#include <vector>
#include <map>

#include <string>
#include <TObject.h>

#include "RExpData.hh"

#include <QCD/Flavor.hh>

namespace Exp {
  class ExpData;
};

namespace Evolution {
  class KernelBase;
}

namespace Fragmentation {
  class Evo;
}

namespace unpolPDF {
  class Evo;
}

namespace polPDF {
  class Evo;
}

namespace Xsection {
  class xXsec;
  class xzXsec;
}

namespace FragXsec_T {
  class FragXsec;
}

namespace unpolDISXsec {
  class StrFunc;
}

namespace unpolSIDISXsec {
  class StrFunc;
}

class Manager : public TObject {
public:
  virtual ~Manager();
  
  static Manager* instance();
  void listDataSet();
  
  /*!
    xkey is kinematic variable for the x-axis
  */
  RExpData* getDataSet( const int& index,
			const std::string& xkey );
  
  RExpData* getDataSet( const std::string& info,
			const std::string& xkey );
  
  RExpData* getDataSet( const std::string& info,
			const std::string& min, 
			const std::string& max );
  
  TGraphErrors* getFragXsec_Z( const int& h,
			       const int& n, 
			       const double& zmin, const double& zmax, 
			       const bool& fLog, 
			       const double& q2,
			       const double& scale = 1.0 );
  
  TGraphErrors* getFragXsec_Z( const int& h,
			       TGraphErrors *g,
			       const double& q2,
			       const double& scale = 1.0 );
  
  TGraphErrors* getMulti_Q2( const int& t, // (0: p, 1: n, 2: d)
			     const int& h,
			     const int& n, const double& min, const double& max,
			     const bool& fLog,
			     const double& s,
			     const double& xmin, const double& xmax,
			     const double& zmin, const double& zmax,
			     const double& scale = 1.0 );
  
  TGraphErrors* getMulti_Q2( const int& t, // (0: p, 1: n, 2: d)
			     const int& h,
			     TGraphErrors *g,
			     const double& s,
			     const double& xmin, const double& xmax,
			     const double& zmin, const double& zmax,
			     const double& scale = 1.0 );
  

  TGraphErrors* getMulti_x( const int& t, // (0: p, 1: n, 2: d)
			    const int& h,
			    const int& n, const double& min, const double& max,
			    const bool& fLog,
			    const double& s,
			    const double& q2,
			    const double& z,
			    const double& scale = 1.0 );
  
  TGraphErrors* getMulti_x( const int& t, // (0: p, 1: n, 2: d)
			    const int& h,
			    TGraphErrors *g,
			    const double& s,
			    const double& q2,
			    const double& z,
			    const double& scale = 1.0 );
  


private:
  Manager();
  
  static Manager* instance_;
  
  void initExpData();

  void initFF();
  void initFF( const QCD::Flavor::HADRON& h );

  void initPDF();

  void initFragXsec();
  
  void initDISXsec();
  void initSIDISXsec();

  TGraphErrors *getGraph( const int& n, const double& min, const double& max,
			  const bool& fLog );
  
  Exp::ExpData *expdata_;

  // fragmenatation and parton distribution function kernels
  Fragmentation::Evo *pionF_;
  Fragmentation::Evo *kaonF_;
  Fragmentation::Evo *protonF_;
  Fragmentation::Evo *restF_;
  
  unpolPDF::Evo *updf_;
  polPDF::Evo   *ppdf_;
  
  // cross section objects
  std::map< Evolution::KernelBase*, FragXsec_T::FragXsec* > fragX_;
  std::map< Evolution::KernelBase*, Xsection::xXsec* > fragXX_;
  
  // for SIDIS multiplicities
  std::vector< unpolDISXsec::StrFunc* > sigma_;
  std::vector< Xsection::xXsec* > xsigma_;
  
  std::vector< std::map< Evolution::KernelBase*, 
			 unpolSIDISXsec::StrFunc* > > sigmaHp_;
  
  std::vector< std::map< Evolution::KernelBase*, 
			 Xsection::xzXsec* > > xsigmaHp_;
  
  std::vector< std::map< Evolution::KernelBase*, 
			 unpolSIDISXsec::StrFunc* > > sigmaHm_;
  
  std::vector< std::map< Evolution::KernelBase*, 
			 Xsection::xzXsec* > > xsigmaHm_;
  
  
  ClassDef( Manager, 1 )
};


#endif // _Manager_hh_
