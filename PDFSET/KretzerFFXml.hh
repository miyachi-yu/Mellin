// $Id: KretzerFFXml.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PDFSET_KretzerFFXml_hh_
#define _PDFSET_KretzerFFXml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  
  /*!
    \class  KretzerFFXml   KretzerFFXml.hh   "QCD/KretzerFFXml.hh"
    \brief  Class for PDF configuration file handling
  */
  class KretzerFFXml : public QCD::PdfSimpleXml {
  public:
    
    KretzerFFXml() throw( int );     //!< a constructor
    virtual ~KretzerFFXml();         //!< a destructor
    
  protected:  
    //! create QCD::PdfBase object with the stored parameters in configuration file
    virtual QCD::PdfBase* pdf( xmlpp::Element* elm );
    
  };
};
#endif // _PDFSET_KretzerFFXml_hh_
