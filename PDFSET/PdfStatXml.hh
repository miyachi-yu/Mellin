// $Id: PdfStatXml.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PdfStatXml_hh_
#define _PdfStatXml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  
  /*!
    \brief  Class for Stat type PDF configuration file handling
  */
  class PdfStatXml : public QCD::PdfSimpleXml {
  public:
    
    PdfStatXml() throw( int );     //!< a constructor
    virtual ~PdfStatXml();         //!< a destructor
    
  protected:  
    //! create QCD::PdfBase object with the stored parameters in configuration file
    virtual QCD::PdfBase* pdf( xmlpp::Element* elm );
    
  };
};
#endif // _PdfStatXml_hh_
