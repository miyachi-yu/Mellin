// $Id: FFSimple.hh,v 1.13 2010/02/25 09:13:50 miyachi Exp $
/*!
  \file     FFSimple.hh
  \version  $Revision: 1.13 $
  \author   $Author: miyachi $
  \brief    Definition of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#ifndef _FFSimple_hh_
#define _FFSimple_hh_

#include <Tranform/CBeta.hh>
#include <complex>
#include "PdfBase.hh"

namespace QCD {
  
  /*!
    \class   FFSimple  FFSimple.hh  "Fragmentation/FFSimple.hh"
    \brief   class for the simple PDF parameterization
    
    This is example of PDF functional form implementation.
    The functional form implemented in this class is
    
    \f[
    f(x) = 
    \eta * x^{a} * ( 1 - x )^{b} * 
    ( 1 + c * x^{d} + e * ( 1 - x )^{e} ) / g( 2 ),
    \f]
    where g( n ) is a Mellin transformed functional form which follows
    \f[
    g( n ) =\eta * 
    ( B( a, b; n ) + c * B( a + d, b; n ) + d * B( a, b + f ) ) / g( 2 ),
    \f]
    where \f$ B( a, b; n )\f$ is the complex Beta function implmented
    as CBeta.
    
  */
  class FFSimple : public PdfBase {
  public:
    
    FFSimple();             //!< a default constructor
    virtual ~FFSimple();    //!< a destructor
    
    //!< calculate z-th moment of the PDF
    virtual std::complex< double > operator() ( const std::complex< double >& z );
    
    //!< calculate PDF value at the given x, q(x)
    virtual double x( const double& x );
    
    //!< update local stored variables using the present parameters
    virtual void update();

    virtual void favored( PdfBase *pdf ) {
      favored_ = pdf; 
      if( pdf ) this->update(); 
    }
    
    virtual void disfavored( PdfBase *pdf ) {
      disfavo_ = pdf;
      if( pdf ) this->update();
    }
    
    PdfBase* favored() { return favored_; }
    PdfBase* disfavored() { return disfavo_; }

  protected:
    
    Transform::CBeta b1_;        //!< B( a,     b )
    Transform::CBeta b2_;        //!< B( a + d, b )
    Transform::CBeta b3_;        //!< B( a, b + f )
    std::complex< double > b00_; //!< b1_( 2.0 ) + c * b2_( 2.0 ) 
                                 //!< + e * b3_( 2.0 )
    std::complex< double > A_;   //!< eta / b00_ ( 0th parameter )
    
    double a_; //!< 1st parameter 
    double b_; //!< 2nd parameter 
    double c_; //!< 3rd parameter 
    double d_; //!< 4th parameter 
    double e_; //!< 3rd parameter 
    double f_; //!< 4th parameter 

    //! for averaged 2nd moment between dist.s of fav and dfav options in xmls
    PdfBase *favored_;
    PdfBase *disfavo_;
    
  };
};

#endif //  _FFSimple_hh_
