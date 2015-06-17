// $Id: DSSFFParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _PDFSET_DSSFFParam_hh_
#define _PDFSET_DSSFFParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigDSSFF.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for MRST parametrizations
  */
  class DSSFFParam : public QCD::PdfParametrization {
  public:
    
    DSSFFParam();                         //!< a default constructor
    DSSFFParam( DSSFFParam& config );     //!< a copy constructor
    DSSFFParam( ConfigDSSFF& config );    //!< a constructor
    virtual ~DSSFFParam();                //!< destructor
    
    //! initialization with Configuration file
    virtual DSSFFParam& operator<<( ConfigDSSFF& conf );
    
    //! check if the information in config is enough or not
    virtual bool check();
    
    virtual std::complex< double > q( const QCD::Flavor::PARTON& q );
    //!< see q( const QCD::Flavor::TYPE&   q );
    virtual std::complex< double > q( const QCD::Flavor::TYPE&   q );
    
    
  protected:
    QCD::Flavor::HADRON h_; // hadron type
  };
}
#endif //  _PDFSET_DSSFFParam_hh_
