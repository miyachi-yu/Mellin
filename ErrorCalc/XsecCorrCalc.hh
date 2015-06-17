#ifndef _ErrorCalc_XsecCorrCalc_hh_
#define _ErrorCalc_XsecCorrCalc_hh_

#include "ErrCalc.hh"

namespace ErrorCalc{

  class XsecCorrCalc: public ErrCalc {
  public:
    XsecCorrCalc( Xsection::Xsec* xsec1, Xsection::Xsec* xsec2 = NULL );
    XsecCorrCalc( Xsection::xXsec* xsec1, Xsection::xXsec* xsec2 = NULL );
    XsecCorrCalc( Xsection::xzXsec* xsec1, Xsection::xzXsec* xsec2 = NULL );
    virtual ~XsecCorrCalc();    

    virtual 
    std::complex< double > operator()( const std::complex< double >& n1, 
				       const std::complex< double >& n2 );

    virtual 
    std::complex< double > operator()( const std::complex< double >& n1, 
				       const std::complex< double >& m1,
				       const std::complex< double >& n2,
				       const std::complex< double >& m2 );
    
    virtual double operator()( const double& x1, const double& x2 );

    virtual double operator()( const double& x1, const double& z1,
			       const double& x2, const double& z2 );

    virtual double operator()( const std::vector< double >& vx1,
			       const std::vector< double >& vx2 );

    virtual double operator()( const std::vector< double >& vx1,
			       const std::vector< double >& vz1,
			       const std::vector< double >& vx2,
			       const std::vector< double >& vz2 );

    void idswap();

  private:
    
    std::vector< Xsection::Xsec* > vxsec_;
    std::vector< Xsection::xXsec* > vxxsec_;
    std::vector< Xsection::xzXsec* > vxzxsec_;

  };

};

#endif // _ErrorCalc_XsecCorrCalc_hh_
