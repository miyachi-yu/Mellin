// $Id: PdfCteq6Xml.hh,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PDFSET_PdfCteq6Xml_hh_
#define _PDFSET_PdfCteq6Xml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  /*!
    \class  PdfCteq6Xml   PdfCteq6Xml.hh   "QCD/PdfCteq6Xml.hh"
    \brief  Class for CTEQ6 type PDF configuration file handling
  */
  class PdfCteq6Xml : public QCD::PdfSimpleXml {
  public:
    PdfCteq6Xml() throw( int );    //!< a constructor
    virtual ~PdfCteq6Xml();        //!< a destructor
    
  protected:  
    virtual QCD::PdfBase* pdf(); //! create PdfBase derived class and return the pointer
    
  private:
    
  };
};
#endif // _PDFSET_PdfCteq6Xml_hh_
