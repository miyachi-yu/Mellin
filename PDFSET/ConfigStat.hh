// $Id: ConfigStat.hh,v 1.2 2010/02/05 04:31:13 miyachi Exp $
/*!
  \file     ConfigStat.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PDFSET_ConfigStat_hh_
#define _PDFSET_ConfigStat_hh_

#include <Utility/Config.hh>

#include "PdfStatXml.hh"

namespace PDFSET {

  /*!
    \brief  Class for PDF configuration file handling
  */
  class ConfigStat : public Utility::Config, public PdfStatXml {
  public:
    ConfigStat( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigStat();                      //!< a destructor
    
    virtual std::vector< QCD::PdfBase* > pdfs();    //!< get vector of PdfBase*
    
  };
  
};

#endif // _PDFSET_ConfigStat_hh_
