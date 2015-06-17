//$Id: CTEQ6pdf.hh,v 1.2 2008/05/23 02:08:05 miyachi Exp $
/*!
  \file   CTEQ6pdf.hh
  \brief  class definition for Evoplution Kernel based on CTEQ6 PDFLIB
 */

#ifndef _CTEQ6pdf_hh_
#define _CTEQ6pdf_hh_

#include <Evolution/KernelPdfLib.hh>
#include <PDFLIB/CTEQ6.hh>

namespace unpolPDF {

  /*!
    \brief Evolution Kernel class based on CTEQ6 Parameterization
    
    This class provides the QCD evolution "meta" kernel, based on
    CTEQ6 PDF parameterization. In order to feed PDF-moment information
    at any ( n, Q^2 ), this uses Mellin transformation on CTEQ6 PDF
    parameterization.
    
  */
  class CTEQ6pdf : public Evolution::KernelPdfLib {
  public:
    
    CTEQ6pdf();                  //!< a default constructor
    virtual ~CTEQ6pdf();         //!< a destructor
    
    //! get a pointer to CTEQ6 object
    virtual PDFLIB::CTEQ6* cteq6() { return cteq6_; } 
    
  private:
    PDFLIB::CTEQ6 *cteq6_;
    
  };

};

#endif // _CTEQ6pdf_hh_
