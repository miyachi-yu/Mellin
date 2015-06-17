// $Id: Cteq6Param.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file    QCD::PdfParametrization.hh
  \version $Revision: 1.2 $
  \author  $Author: miyachi $
  \brief   Definition of interface of QCD::PdfParametrization class
 */

#ifndef _Cteq6Param_hh_
#define _Cteq6Param_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

#include "ConfigCteq6.hh"

namespace PDFSET {
  
  /*!
    \brief  abstruct class for Cteq6 parametrizations
  */
  class Cteq6Param : public QCD::PdfParametrization {
  public:
    
    Cteq6Param();                             //!< a default constructor
    Cteq6Param( Cteq6Param& config );          //!< a copy constructor
    Cteq6Param( ConfigCteq6& config );         //!< a constructor
    virtual ~Cteq6Param();                    //!< destructor
    
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
    virtual Cteq6Param& operator<<( ConfigCteq6& conf );

  };
};
#endif //  _Cteq6Param_hh_
