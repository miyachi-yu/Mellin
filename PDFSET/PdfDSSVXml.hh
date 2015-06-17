// $Id: PdfDSSVXml.hh,v 1.2 2010/02/05 04:31:14 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PdfDSSVXml_hh_
#define _PdfDSSVXml_hh_

#include <QCD/PdfSimpleXml.hh>

namespace PDFSET {
  
  /*!
    \brief  Class for DSSV type PDF configuration file handling
  */
  class PdfDSSVXml : public QCD::PdfSimpleXml {
  public:
    
    PdfDSSVXml() throw( int );     //!< a constructor
    virtual ~PdfDSSVXml();         //!< a destructor
    
  protected:  
    //! create QCD::PdfBase object with the stored parameters in configuration file
    virtual QCD::PdfBase* pdf( xmlpp::Element* elm );
    
  };
};
#endif // _PdfDSSVXml_hh_
