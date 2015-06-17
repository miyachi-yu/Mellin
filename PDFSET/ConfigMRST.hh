// $Id: ConfigMRST.hh,v 1.3 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigMRST.hh
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PDFSET_ConfigMRST_hh_
#define _PDFSET_ConfigMRST_hh_

#include <Utility/Config.hh>

#include "PdfMRSTXml.hh"

namespace PDFSET {

  /*!
    \brief  Class for PDF configuration file handling
  */
  class ConfigMRST : public Utility::Config, public PdfMRSTXml {
  public:
    ConfigMRST( const std::string& name ) throw( int ); //!< a constructor
    virtual ~ConfigMRST();                      //!< a destructor
    
    virtual std::vector< QCD::PdfBase* > pdfs();    //!< get vector of PdfBase*
    
  };
  
};

#endif // _PDFSET_ConfigMRST_hh_
