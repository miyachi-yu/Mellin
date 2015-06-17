// $Id: PdfMRST.hh,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfMRST.hh
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#ifndef _PdfMRST_hh_
#define _PdfMRST_hh_

#include <QCD/PdfBase.hh>
#include <Tranform/CBeta.hh>

#include <complex>

namespace PDFSET {
  
  /*!
    \brief  class for the MRST PDF parameterization
    
    This is a MRST type PDF functional form implementation.
    The functional form implemented in this class is
    \f[
    x \cdot f(x) = 
    \eta * x^{a} * ( 1 - x )^{b} * ( 1 + c * \sqrt{x} +  d * x ),
    \f]

    In 2001 version, gluon has an additional  
    \f[
    e * x^f * ( 1-x )^g
    \f]
    term.

    So in total, 8 parameters are requires.....

  */
  class PdfMRST : public QCD::PdfBase {
  public:
    
    PdfMRST();             //!< a default constructor
    virtual ~PdfMRST();    //!< a destructor
    
    //! calculate z-th moment of the PDF
    virtual std::complex< double > 
    operator() ( const std::complex< double >& z );
    
    //! calculate PDF value at the given x, q(x)
    virtual double x( const double& x );
    
    //! update local stored variables using the present parameters
    virtual void update();
    
    //! set sea destribution
    virtual void sea( QCD::PdfBase *sea ) { this->reference( sea ); }
    
    //! get sea distribution
    virtual QCD::PdfBase* sea() { return this->reference(); }
    
  private:
    
    Transform::CBeta b1_;              //!< complex gamma function object 1
    Transform::CBeta b2_;              //!< complex gamma function object 2
    Transform::CBeta b3_;              //!< complex gamma function object 3
    Transform::CBeta b4_;              //!< complex gamma function object 4
    
    double A_;
    double a_;
    double b_;
    double c_;
    double d_;
    double e_;
    double f_;
    double g_;
    
    
  };
};
#endif //  _PdfMRST_hh_
