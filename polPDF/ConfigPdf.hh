// $Id: ConfigPdf.hh,v 1.5 2008/05/29 02:59:02 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.5 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _polPDF_ConfigPdf_hh_
#define _polPDF_ConfigPdf_hh_

#include <QCD/ConfigPdfBase.hh>

namespace polPDF {
  
  /*!
    \class  ConfigPdf   ConfigPdf.hh   "polPDF/ConfigPdf.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, PdfSimple objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    PdfBase derived class, it should be also done that 
    the correspoinding ConfigPdf derive class is generated,
    where PdfBase* pdf() method should be implemented with 
    newly created PdfBase derived class.
    
  */
  class ConfigPdf : public QCD::ConfigPdfBase {
    
  public:
    ConfigPdf() throw( int );    //!< a constructor
    virtual ~ConfigPdf();        //!< a destructor
    
  };
  
};

#endif // _polPDF_ConfigPdf_hh_
