// $Id: FFSimpleXml.hh,v 1.3 2008/06/14 14:10:57 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _FFSimpleXml_hh_
#define _FFSimpleXml_hh_

#include "PdfSimpleXml.hh"

namespace QCD {
  
  class PdfBase;
  
  /*!
    \class  FFSimpleXml   FFSimpleXml.hh   "QCD/FFSimpleXml.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, FFSimple objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    PdfBase derived class, it should be also done that 
    the correspoinding FFSimpleXml derive class is generated,
    where PdfBase* pdf() method should be implemented with 
    newly created PdfBase derived class.
    
  */
  class FFSimpleXml : public PdfSimpleXml {
  public:
    
    FFSimpleXml() throw( int );     //!< a constructor
    virtual ~FFSimpleXml();         //!< a destructor
    
  protected:  
    //! create PdfBase object with the stored parameters in configuration file
    virtual PdfBase* pdf( xmlpp::Element* elm );

  };
};
#endif // _FFSimpleXml_hh_
