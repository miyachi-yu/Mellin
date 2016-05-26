// $Id: PdfStat.hh,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfStat.hh
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#ifndef _PDFSET_PdfStat_hh_
#define _PDFSET_PdfStat_hh_

#include <QCD/PdfBase.hh>
#include <Tranform/CGamma.hh>
#include <Tranform/CPolyLog.hh>
#include <Tranform/CZeta.hh>

#include <complex>

namespace PDFSET {
  
  /*!
    \brief   class for the statistical model PDF parameterization
    
    The statistical model PDF given in NPA941(2015)307
    is implemented for the unpolarized part only.
    
  */
  class PdfStat : public QCD::PdfBase {
  public:
    
    PdfStat();             //!< a default constructor
    virtual ~PdfStat();    //!< a destructor
    
    //!< calculate z-th moment of the PDF
    virtual std::complex< double > operator() ( const std::complex< double >& z );
    
    //!< calculate PDF value at the given x, q(x)
    virtual double x( const double& x );
    
    //!< update local stored variables using the present parameters
    virtual void update();
    
  private:
    double A1_;  //! A in ref.
    double b1_;  //! b in ref.
    double A2_;  //! \tilde{A} in ref.
    double b2_;  //! \tilde{b} in ref.
    double Xp_;  //! X_0^+ in ref.
    double Xm_;  //! X_0^- in ref.
    double xb_;  //! bar{x} in ref.
    
    std::complex< double > eXp_; // exp( \pm X_0 / xbar ) for q, qbar.
    std::complex< double > eXm_; // exp( \pm X_0 / xbar ) for q, qbar.
    
    double G( const double& x );
    std::complex< double > G( const std::complex< double >& z );

    static Transform::CGamma   Ga_;
    static Transform::CPolyLog Li_;
    static Transform::CZeta    ze_;
    
  };
};
#endif //  _PdfStat_hh_
