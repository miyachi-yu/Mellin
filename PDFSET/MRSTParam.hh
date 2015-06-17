// $Id: MRSTParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _MRSTParam_hh_
#define _MRSTParam_hh_

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
  class MRSTParam : public QCD::PdfParametrization {
  public:
    
    MRSTParam();                             //!< a default constructor
    MRSTParam( MRSTParam& config );          //!< a copy constructor
    MRSTParam( ConfigMRST& config );         //!< a constructor
    virtual ~MRSTParam();                    //!< destructor
    
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
    virtual MRSTParam& operator<<( ConfigMRST& conf );

  };
};
#endif //  _MRSTParam_hh_
