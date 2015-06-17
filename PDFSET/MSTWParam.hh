// $Id: MSTWParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _PDFSET_MSTWParam_hh_
#define _PDFSET_MSTWParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigMRST.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for MRST parametrizations
  */
  class MSTWParam : public QCD::PdfParametrization {
  public:
    
    MSTWParam();                             //!< a default constructor
    MSTWParam( MSTWParam& config );          //!< a copy constructor
    MSTWParam( ConfigMRST& config );         //!< a constructor
    virtual ~MSTWParam();                    //!< destructor
    
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
    virtual MSTWParam& operator<<( ConfigMRST& conf );

  };
};
#endif //  _PDFSET_MSTWParam_hh_
