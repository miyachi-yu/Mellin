// $Id: ConfigDSSFF.hh,v 1.2 2010/02/05 04:31:13 miyachi Exp $
/*!
  \file     ConfigFF.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
*/
#ifndef _PDFSET_ConfigDSSFF_hh_
#define _PDFSET_ConfigDSSFF_hh_

#include <Utility/Config.hh>
#include "DSSFFXml.hh"

namespace PDFSET {
  
  /*!
    \class  ConfigDSSFF   ConfigDSSFF.hh   "QCD/ConfigDSSFF.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, PdfSimple objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    QCD::PdfBase derived class, it should be also done that 
    the correspoinding ConfigDSSFF derive class is generated,
    where QCD::PdfBase* pdf() method should be implemented with 
    newly created QCD::PdfBase derived class.
    
  */
  class ConfigDSSFF : 
    public Utility::Config, public DSSFFXml {
    
  public:
    
    ConfigDSSFF( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigDSSFF();                              //!< a destructor
    
    //! create vector of pointers of QCD::PdfBase and return
    virtual std::vector< QCD::PdfBase* > pdfs(); 
    
  };
  
};

#endif // _ConfigDSSFF_hh_
