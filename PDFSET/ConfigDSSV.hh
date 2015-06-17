// $Id: ConfigDSSV.hh,v 1.2 2010/02/05 04:31:13 miyachi Exp $
/*!
  \file     ConfigDSSV.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PDFSET_ConfigDSSV_hh_
#define _PDFSET_ConfigDSSV_hh_

#include <Utility/Config.hh>

#include "PdfDSSVXml.hh"

namespace PDFSET {

  /*!
    \brief  Class for PDF configuration file handling
  */
  class ConfigDSSV : public Utility::Config, public PdfDSSVXml {
  public:
    ConfigDSSV( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigDSSV();                      //!< a destructor
    
    virtual std::vector< QCD::PdfBase* > pdfs();    //!< get vector of PdfBase*
    
  };
  
};

#endif // _PDFSET_ConfigDSSV_hh_
