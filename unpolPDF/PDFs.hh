// $Id: PDFs.hh,v 1.3 2008/05/23 02:08:05 miyachi Exp $
/*!
  \file    PDFs.hh
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/23 02:08:05 $
  \author  $Author: miyachi $
  \brief   Definition of PDF container class
 */

#ifndef _unpolPDF_PDFs_hh_
#define _unpolPDF_PDFs_hh_

#include <QCD/PdfParametrization.hh>
#include "ConfigPdf.hh"

namespace unpolPDF {
  
  /*!
    \brief   PDF set handling class
    
    This class contains a set of pointer to PdfBase objects
    which must be parameterized in an external configuration 
    file for ConfigPdf class.
    
  */
  class PDFs : public QCD::PdfParametrization {
  public:
    
    PDFs();                   //!< a default constructor
    virtual ~PDFs();          //!< a destructor
    
    PDFs& operator<<( ConfigPdf& conf );
    
  };
};

#endif //  _PDFs_hh_
