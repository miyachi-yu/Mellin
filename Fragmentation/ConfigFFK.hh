// $Id: ConfigFFK.hh,v 1.3 2008/05/23 02:08:00 miyachi Exp $
/*!
  \file     ConfigFFK.hh
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _Fragmentation_ConfigFFK_hh_
#define _Fragmentation_ConfigFFK_hh_

#include <Utility/Config.hh>
#include <QCD/FFSimpleXml.hh>

namespace QCD {
  class PdfBase;
};

namespace Fragmentation {
  
/*!
  \class  ConfigFFK   ConfigFFK.hh   "QCD/ConfigFFK.hh"
  \brief  Class for PDF configuration file handling
  
  This derived class from Config reads PDF configuration 
  given in XML format ( see sample pdfset.xml ).
  
  This sample implementation, PdfSimple objects will be created
  and stored in PdfParameterization object.
  When different type of functional form is implemented as a
  PdfBase derived class, it should be also done that 
  the correspoinding ConfigFFK derive class is generated,
  where PdfBase* pdf() method should be implemented with 
  newly created PdfBase derived class.
  
*/
  class ConfigFFK : public Utility::Config, public QCD::FFSimpleXml {
  public:
    
    ConfigFFK( Utility::Arguments& args ) throw( int ); //!< a constructor
    virtual ~ConfigFFK();                      //!< a destructor
    
    //! create vector of pointers of PdfBase and return
    virtual std::vector< QCD::PdfBase* > pdfs(); 
    
  };
  
}

#endif // _ConfigFFK_hh_
