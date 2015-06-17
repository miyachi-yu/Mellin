// $Id: PDFx.hh,v 1.13 2008/08/27 18:30:34 imazu Exp $
/*!
  \file     PDFx.hh
  \Version  $Revision: 1.13 $
  \Author   $Author: imazu $
  \Date     $Date: 2008/08/27 18:30:34 $
  \brief    Class definition of x-space PDF
*/
#ifndef _PDFx_hh_
#define _PDFx_hh_

#include <Tranform/InverseMellin.hh>
#include "PDF.hh"

namespace Evolution {
  /*!
    \class PDFx  PDFx.hh  "Evolution/PDFx.hh"
    \brief class for PDF in x-space
    
    It calculate PDFs in x-space from PDF in moment-space 
    calculated with the given Evolution KernelBase object.
    
  */
  class PDFx : public Transform::InverseMellin {
  public:
    
    /*!
      \param[in] kernel is a pointer ot KernelBase object
      \param[in] q is a quark flavor to be calculated
      \param[in] low is a lower number of node in Gaus integration
      \param[in] high is a upper number of node in Gaus integration
      \param[in] precision is a required precision in integration routine
      \param[in] length of integration path
      \param[in] offset is starting point of integration path on the real axis
      \param[in] rangle of relative angle between real axis and integration path
      \brief  constructor for parton type q
    */
    PDFx( KernelBase *kernel, const QCD::Flavor::PARTON& q,
	  const int& low = 4, const int& high = 6, 
	  const double& precision = 1.0E-4,
	  const double& length = 10.0,
	  const double& offset = 1.8,
	  const double& rangle = 0.6 );
    
    /*!
      \param[in] kernel is a pointer ot KernelBase object
      \param[in] q is a quark flavor to be calculated
      \param[in] low is a lower number of node in Gaus integration
      \param[in] high is a upper number of node in Gaus integration
      \param[in] precision is a required precision in integration routine
      \param[in] length of integration path
      \param[in] offset is a integration starting point on the real axis
      \param[in] rangle of relative angle between real axis and integration path
      \brief  constructor for parton type q
    */
    PDFx( KernelBase *kernel, 
	  const QCD::Flavor::TYPE&   q,
	  const int& low = 4, 
	  const int& high = 6, 
	  const double& precision = 1.0E-4,
	  const double& length = 10.0,
	  const double& offset = 1.8,
	  const double& rangle = 0.6 );
    
    virtual ~PDFx();      //!< a destructor
    
    std::string name() const { return pdf_.name(); }
    QCD::PdfBase::TYPE type() const { return pdf_.type(); }
    
    //! get a reference to PDF object
    PDF& pdf() { return pdf_; }
    
    friend std::ostream& operator<<( std::ostream& os, PDFx& pdfx );

  private:

    PDF pdf_;
    
    void initialization( const int& low = 4, const int& high = 6, 
			 const double& precision = 1.0E-4,
			 const double& length = 10.0,
			 const double& offset = 1.8,
			 const double& rangle = 0.6 );
    
  };
};
#endif // _PDFx_hh_
