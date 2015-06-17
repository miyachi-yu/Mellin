#ifndef _ErrorCalc_PDFCorrCalc_hh_
#define _ErrorCalc_PDFCorrCalc_hh_

#include "ErrCalc.hh"

namespace ErrorCalc{

  class PDFCorrCalc: public ErrCalc{
  public:
    PDFCorrCalc( Xsection::Xsec* xsec );
    virtual ~PDFCorrCalc();    

    virtual std::complex< double > operator()
      ( const std::complex< double >& n );
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n, const std::complex< double >& m );
    
    virtual double operator()( const double& x );
    
    virtual double operator()( const double& x, const double& z );

    void setPID_1( const QCD::Flavor::PARTON& p ); // x direction
    void setPID_2( const QCD::Flavor::PARTON& p ); // y direction

    void setcvmtx( const bool& cvmtx ){ covmtx_ = cvmtx; }
    void setflout( const bool& flout ){ outfile_ = flout; }
    void setcvout( const bool& cvout ){ outcurv_ = cvout; }
    void setnom( const bool& nom ){ nom_ = nom; }

  private:
    
    bool covmtx_;
    bool outfile_;
    bool outcurv_;
    bool nom_;

    void covout( Transform::ComplexFunction* f, 
		 Transform::ComplexFunction* g );
    void covout( Transform::RealFunction* f,
		 Transform::RealFunction* g );
    
    std::vector< Evolution::PDF* > vpdf_;
    std::vector< Evolution::PDFx* > vpdfx_;
    
    //    QCD::Flavor::PARTON parton1_;
    //    QCD::Flavor::PARTON parton2_;
    std::vector< const QCD::Flavor::PARTON* > vparton_;

    std::vector< std::vector< double > > errmx_;

    void fileout();
    void curveout();

    std::string name();

    std::complex< double > NN_;
    std::complex< double > MM_;
    double XX_;
    double ZZ_;

  };

};

#endif // _ErrorCalc_PDFCorrCalc_hh_
