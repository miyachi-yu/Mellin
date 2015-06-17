// $Id: DSSVParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _DSSVParam_hh_
#define _DSSVParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigDSSV.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for DSSV parametrizations
  */
  class DSSVParam : public QCD::PdfParametrization {
  public:
    
    DSSVParam();                             //!< a default constructor
    DSSVParam( DSSVParam& config );          //!< a copy constructor
    DSSVParam( ConfigDSSV& config );         //!< a constructor
    virtual ~DSSVParam();                    //!< destructor
    
    /*!
      \par [in] q is a PARTON type
      \return value of PDF for the given q in the mellin space at n
      \brief  calculate the q(n) ( in the Mellin space )
      
      This calculte the PDF value for parton q at complex number n 
      in the mellin space. n shuld be pre-defined with setN method.
    */
    virtual std::complex< double > q( const QCD::Flavor::PARTON& q );
    
    //!< see q( const QCD::Flavor::TYPE&   q );
    virtual std::complex< double > q( const QCD::Flavor::TYPE& q );
    
    //! check if the information in config is enough or not
    virtual bool check();
    
    //! initialization with Configuration file
    virtual DSSVParam& operator<<( ConfigDSSV& conf );

  };
};
#endif //  _DSSVParam_hh_
