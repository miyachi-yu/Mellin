// $Id: ConfigFFbase.hh,v 1.1 2008/05/29 03:58:37 miyachi Exp $
/*!
  \file     ConfigFF.hh
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
*/
#ifndef _QCD_ConfigFFbase_hh_
#define _QCD_ConfigFFbase_hh_

#include <Utility/Config.hh>
#include "FFSimpleXml.hh"

namespace QCD {
  
  class PdfBase;
  
  /*!
    \class  ConfigFFbase   ConfigFFbase.hh   "QCD/ConfigFFbase.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, PdfSimple objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    PdfBase derived class, it should be also done that 
    the correspoinding ConfigFFbase derive class is generated,
    where PdfBase* pdf() method should be implemented with 
    newly created PdfBase derived class.
    
  */
  class ConfigFFbase : 
    public Utility::Config, public FFSimpleXml {
    
  public:
    
    ConfigFFbase( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigFFbase();                              //!< a destructor
    
    //! create vector of pointers of PdfBase and return
    virtual std::vector< QCD::PdfBase* > pdfs(); 
    
  };
  
};

#endif // _ConfigFFbase_hh_
