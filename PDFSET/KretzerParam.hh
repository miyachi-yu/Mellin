// $Id: KretzerParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _PDFSET_KretzerParam_hh_
#define _PDFSET_KretzerParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigKretzerFF.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for MRST parametrizations
  */
  class KretzerParam : public QCD::PdfParametrization {
  public:
    
    KretzerParam();                             //!< a default constructor
    KretzerParam( KretzerParam& config );       //!< a copy constructor
    KretzerParam( ConfigKretzerFF& config );    //!< a constructor
    virtual ~KretzerParam();                    //!< destructor
    
    //! initialization with Configuration file
    virtual KretzerParam& operator<<( ConfigKretzerFF& conf );

  protected:

  };
};
#endif //  _PDFSET_KretzerParam_hh_
