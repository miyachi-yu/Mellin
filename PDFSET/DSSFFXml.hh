// $Id: DSSFFXml.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _DSSFFXml_hh_
#define _DSSFFXml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  
  
  /*!
    \class  DSSFFXml   DSSFFXml.hh   "QCD/DSSFFXml.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, DSSFF objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    QCD::PdfBase derived class, it should be also done that 
    the correspoinding DSSFFXml derive class is generated,
    where QCD::PdfBase* pdf() method should be implemented with 
    newly created QCD::PdfBase derived class.
    
  */
  class DSSFFXml : public QCD::PdfSimpleXml {
  public:
    
    DSSFFXml() throw( int );     //!< a constructor
    virtual ~DSSFFXml();         //!< a destructor
    
  protected:  
    //! create QCD::PdfBase object with the stored parameters in configuration file
    virtual QCD::PdfBase* pdf( xmlpp::Element* elm );

  };
};
#endif // _DSSFFXml_hh_
