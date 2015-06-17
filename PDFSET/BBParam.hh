// $Id: BBParam.hh,v 1.2 2010/02/05 04:31:13 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _BBParam_hh_
#define _BBParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/ConfigPdfBase.hh>
#include <QCD/PdfParametrization.hh>

namespace PDFSET {
  
  /*!
    \brief  abstruct class for BB parametrizations
  */
  class BBParam : public QCD::PdfParametrization {
  public:
    
    BBParam();                             //!< a default constructor
    BBParam( BBParam& config );          //!< a copy constructor
    BBParam( QCD::ConfigPdfBase& config );         //!< a constructor
    virtual ~BBParam();                    //!< destructor
    
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
    virtual BBParam& operator<<( QCD::ConfigPdfBase& conf );

  };
};
#endif //  _BBParam_hh_
