// $Id: PdfSimple.hh,v 1.14 2009/01/26 03:00:02 miyachi Exp $
/*!
  \file     PdfSimple.hh
  \version  $Revision: 1.14 $
  \author   $Author: miyachi $
  \brief    Definition of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#ifndef _PdfSimple_hh_
#define _PdfSimple_hh_

#include "PdfBase.hh"
#include <Tranform/CBeta.hh>

#include <complex>

namespace QCD {
  
  /*!
    \class   PdfSimple  PdfSimple.hh  "QCD/PdfSimple.hh"
    \brief   class for the simple PDF parameterization
    
    This is example of PDF functional form implementation.
    The functional form implemented in this class is
    
    \f[
    x \cdot f(x) = 
    \eta * x^{a} * ( 1 - x )^{b} * 
    ( 1 - c * x + d * \sqrt{x}) / g( 1 ),
    \f]
    where g( 1 ) is a Mellin transformed functional form which follows
    \f[
    g( n ) =
    \eta * ( B( a - 1.0, b; n ) - c * B( a, b; n ) + 
    d * B( a - 0.5, b ) /  g( 1 ),
    \f]
    where \f$ B( a, b; n )\f$ is the complex Beta function implmented
    as CBeta.
    
  */
  class PdfSimple : public PdfBase {
  public:
    
    PdfSimple();             //!< a default constructor
    virtual ~PdfSimple();    //!< a destructor
    
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

    std::complex< double > A_;         //!< eta / b00_

    double a_;
    double b_;
    double c_;
    double d_;
    
  };
};
#endif //  _PdfSimple_hh_
