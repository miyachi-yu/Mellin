// $Id: DSSVParam.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    StatParam.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition for Statistical Model PDF parameterization class
 */

#ifndef _StatParam_hh_
#define _StatParam_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigStat.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for Stat parametrizations
  */
  class StatParam : public QCD::PdfParametrization {
  public:
    
    StatParam();                             //!< a default constructor
    StatParam( StatParam& config );          //!< a copy constructor
    StatParam( ConfigStat& config );         //!< a constructor
    virtual ~StatParam();                    //!< destructor
    
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
    virtual StatParam& operator<<( ConfigStat& conf );

  };
};
#endif //  _StatParam_hh_
