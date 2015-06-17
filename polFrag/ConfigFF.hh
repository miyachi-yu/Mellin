// $Id: ConfigFF.hh,v 1.3 2008/05/23 02:08:04 miyachi Exp $
/*!
  \file     ConfigFF.hh
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _polFrag_ConfigFF_hh_
#define _polFrag_ConfigFF_hh_

#include <Utility/Config.hh>
#include <QCD/FFSimpleXml.hh>

namespace QCD {
  class PdfBase;
};

namespace polFrag {
  
/*!
  \class  ConfigFF   ConfigFF.hh   "QCD/ConfigFF.hh"
  \brief  Class for PDF configuration file handling
  
  This derived class from Config reads PDF configuration 
  given in XML format ( see sample pdfset.xml ).
  
  This sample implementation, PdfSimple objects will be created
  and stored in PdfParameterization object.
  When different type of functional form is implemented as a
  PdfBase derived class, it should be also done that 
  the correspoinding ConfigFF derive class is generated,
  where PdfBase* pdf() method should be implemented with 
  newly created PdfBase derived class.
  
*/
  class ConfigFF : 
    public Utility::Config, public QCD::FFSimpleXml {

  public:
    
    ConfigFF( Utility::Arguments& args ) throw( int ); //!< a constructor
    virtual ~ConfigFF();                      //!< a destructor
    
    //! create vector of pointers of PdfBase and return
    virtual std::vector< QCD::PdfBase* > pdfs(); 
    
  };

};
#endif // _ConfigFF_hh_
