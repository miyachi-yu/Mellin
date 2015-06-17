// $Id: PdfDSSV.hh,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfDSSV.hh
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#ifndef _PDFSET_PdfDSSV_hh_
#define _PDFSET_PdfDSSV_hh_

#include <QCD/PdfBase.hh>
#include <Tranform/CBeta.hh>

#include <complex>

namespace PDFSET {
  
  /*!
    \brief   class for the simple PDF parameterization
    
    This is example of PDF functional form implementation.
    The functional form implemented in this class is
    
    \f[
    x \cdot f(x) = 
    N * x^{a} * ( 1 - x )^{b} * ( 1 + c * \sqrt{x} + d * x ),
    \f]
    
  */
  class PdfDSSV : public QCD::PdfBase {
  public:
    
    PdfDSSV();             //!< a default constructor
    virtual ~PdfDSSV();    //!< a destructor
    
    //!< calculate z-th moment of the PDF
    virtual std::complex< double > operator() ( const std::complex< double >& z );
    
    //!< calculate PDF value at the given x, q(x)
    virtual double x( const double& x );
    
    //!< update local stored variables using the present parameters
    virtual void update();
    
  private:
    
    Transform::CBeta b1_;              //!< complex gamma function object 1
    Transform::CBeta b2_;              //!< complex gamma function object 2
    Transform::CBeta b3_;              //!< complex gamma function object 3
    
    std::complex< double > A_;
    double a_;
    double b_;
    double c_;
    double d_;
    
    
  };
};
#endif //  _PdfDSSV_hh_
