// $Id: ConfigCteq6.hh,v 1.2 2010/02/05 04:31:13 miyachi Exp $
/*!
  \file     ConfigCteq6.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
*/
#ifndef _PDFSET_ConfigCteq6_hh_
#define _PDFSET_ConfigCteq6_hh_

#include <Utility/Config.hh>

#include "PdfCteq6Xml.hh"

namespace PDFSET {
  
  /*!
    \brief  Class for PDF configuration file handling
  */
  class ConfigCteq6 : public Utility::Config, public PdfCteq6Xml {
  public:
    ConfigCteq6( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigCteq6();                      //!< a destructor
    
    virtual std::vector< QCD::PdfBase* > pdfs();    //!< get vector of PdfBase*
    
  };
  
};

#endif // _PDFSET_ConfigCteq6_hh_
