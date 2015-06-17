// $Id: PdfMRSTXml.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PdfMRSTXml_hh_
#define _PdfMRSTXml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  
  /*!
    \brief  Class for MRST type PDF configuration file handling
  */
  class PdfMRSTXml : public QCD::PdfSimpleXml {
  public:
    
    PdfMRSTXml() throw( int );     //!< a constructor
    virtual ~PdfMRSTXml();         //!< a destructor
    
  protected:  
    //! create QCD::PdfBase object with the stored parameters in configuration file
    virtual QCD::PdfBase* pdf( xmlpp::Element* elm );
    
  };
};
#endif // _PdfMRSTXml_hh_
