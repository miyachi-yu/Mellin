// $Id: ConfigKretzerFF.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigFF.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
*/
#ifndef _PDFSET_ConfigKretzerFF_hh_
#define _PDFSET_ConfigKretzerFF_hh_

#include <Utility/Config.hh>

#include "KretzerFFXml.hh"

namespace PDFSET {
  
  /*!
    \class  ConfigKretzerFF   ConfigKretzerFF.hh   "QCD/ConfigKretzerFF.hh"
    \brief  Class for PDF configuration file handling
  */
  class ConfigKretzerFF : 
    public Utility::Config, public KretzerFFXml {
    
  public:
    
    ConfigKretzerFF( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigKretzerFF();                              //!< a destructor
    
    //! create vector of pointers of QCD::PdfBase and return
    virtual std::vector< QCD::PdfBase* > pdfs(); 
    
  };
  
};

#endif // _PDFSET_ConfigKretzerFF_hh_
