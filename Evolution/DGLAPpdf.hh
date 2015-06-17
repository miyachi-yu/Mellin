// $Id: DGLAPpdf.hh,v 1.9 2008/10/28 09:12:26 miyachi Exp $
/*!
  \file  DGLAPpdf.hh
  \brief Definition of PDF vector used for DGLAP equation
 */
#ifndef _DGLAPpdf_hh_
#define _DGLAPpdf_hh_

#include <vector>
#include <complex>
#include <iostream>

#include <Tranform/Matrix.hh>

namespace QCD {
  class PdfParametrization;
}

namespace Evolution {
  /*!
    
    \class DGLAPpdf  DGLAPpdf.hh  "Evolution/DGLAPpdf.hh"
    \brief Class to handle pdf set for DGLAP equation.
    
    The pdf for the DGLAP equation consists of
    the flavor singlet components, quark and gluon,
    and the flavor non-singlet components both + and
    - type for each quark flavor.
    
    This DGLAPpdf class maintains the set of PDF for the
    evolution calculation.
    
  */
  class DGLAPpdf {
  public:
    
    DGLAPpdf();                             //!< a default constructor
    DGLAPpdf( const DGLAPpdf& pdf );        //!< copy constructor
    virtual ~DGLAPpdf();                    //!< a destructor
    
    DGLAPpdf& operator=( const DGLAPpdf& pdf ); //!< assignment operator
    
    Transform::Matrix< std::complex< double > >& S() { return S_; } //!< get singlet
    std::vector< std::complex< double > >& NS( int sign ) {   //!< get non-singlet
      return ( sign < 0 ? NSm_ : NSp_ );
    }
    
    DGLAPpdf& load( QCD::PdfParametrization* par ); //!< load from pdf set
    DGLAPpdf& operator<<( QCD::PdfParametrization* par ); //!< load from pdf set
    
    void renormalize( const double& nf ); //!< renormalize 
    
    double nf( const double& n ) { return ( nf_ = n ); } //!< set nf
    double nf() const { return nf_; } //!< get nf
    
    //! dump PDF information
    friend std::ostream& operator<<( std::ostream& os, DGLAPpdf& pdf );
    
  private:
    
    Transform::Matrix< std::complex< double > > S_;   //!< singlet components
    std::vector< std::complex< double > > NSp_; //!< non-singlet + components
    std::vector< std::complex< double > > NSm_; //!< non-singlet - components
    
    QCD::PdfParametrization *par_;     //!< a pointer to PdfParameterization object
    double nf_;                        //!< present value of number of flavor
    
  };
};

#endif //  _DGLAPpdf_hh_
