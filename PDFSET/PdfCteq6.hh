// $Id: PdfCteq6.hh,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file PdfCteq6.hh
*/
#ifndef _PDFSET_PdfCteq6_hh_
#define _PDFSET_PdfCteq6_hh_

#include <Tranform/RealFunction.hh>
#include <Tranform/Mellin.hh>
#include <QCD/PdfBase.hh>

namespace PDFSET {

  /*!
    \brief  PDF functional form used in CTEQ6 parameterization
    
    CTEQ6 uses the following functional form
    \f[
    x \cdot f( x ) =
    A_{0} x^{A_1} ( 1 - x )^{A_2} {\rm e}^{A_3 x} ( 1 + {\rm e}^{A_4} x )^{A_5}
    \f]
    
    Mellin transform of above formula is not trivial.
    The transformation requires integration on the complex plane,
    more than once. 
    
  */
  class PdfCteq6 : public QCD::PdfBase, public Transform::Mellin {
  public:
    
    PdfCteq6();                           //!< a default constructor
    virtual ~PdfCteq6();                  //!< a destructor
    
    virtual double x( const double& x );  //!< get value at the given x
    virtual void update();                //!< update pdf
    
    class Form : public Transform::RealFunction {
    public:
      virtual double operator()( const double& x );
      double a;
      double b;
      double c;
      double d;
      double e;
      double f;
      bool ratio_;
    };
    
  private:
    Form f_;
  };
};

#endif //  _PDFSET_PdfCteq6_hh_
