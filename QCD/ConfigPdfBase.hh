// $Id: ConfigPdfBase.hh,v 1.2 2008/12/02 10:29:54 miyachi Exp $
/*!
  \file     ConfigPdfBase.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _QCD_ConfigPdfBase_hh_
#define _QCD_ConfigPdfBase_hh_

#include <Utility/Config.hh>

#include "PdfSimpleXml.hh"

namespace QCD {

  class PdfBase;
  
  /*!
    \class  ConfigPdfBase   ConfigPdfBase.hh   "QCD/ConfigPdfBase.hh"
  \brief  Class for PDF configuration file handling
  
  This derived class from Config reads PDF configuration 
  given in XML format ( see sample pdfset.xml ).
  
  This sample implementation, PdfSimple objects will be created
  and stored in PdfParameterization object.
  When different type of functional form is implemented as a
  PdfBase derived class, it should be also done that 
  the correspoinding ConfigPdfBase derive class is generated,
  where PdfBase* pdf() method should be implemented with 
  newly created PdfBase derived class.
  
  */
  class ConfigPdfBase : 
    public Utility::Config, public PdfSimpleXml {
    
  public:
    
    ConfigPdfBase() throw( int ); //!< a constructor
    ConfigPdfBase( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigPdfBase();                              //!< a destructor
    
    virtual std::vector< QCD::PdfBase* > pdfs();  //!< get vector of PdfBase*
    
    virtual void loadRootNode() { this->load( this->rootNode() ); }

  };
  
};

#endif // _QCD_ConfigPdfBase_hh_
