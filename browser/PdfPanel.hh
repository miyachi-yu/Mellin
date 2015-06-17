// $Id: PdfPanel.hh,v 1.8 2008/07/09 03:11:04 miyachi Exp $
/*!
  \file    PdfPanel.hh
  \version $Revision: 1.8 $
  \author  $Author: miyachi $
  \date    $Date: 2008/07/09 03:11:04 $
 */
#ifndef _PdfPanel_hh_
#define _PdfPanel_hh_

#include <TGFrame.h>
#include <TGTextEntry.h>
#include <TCanvas.h>

namespace QCD {
  class PdfParametrization;
  class PdfBase;
}

namespace Xsection {
  class Xsec;
}

namespace Evolution {
  class Kernel;
}

/*!
  \brief   Initial distribution handling class
*/
class PdfPanel : public TGVerticalFrame {
public:
  
  //! a constructor
  PdfPanel( const TGWindow *p );
  
  //! a destructor
  virtual ~PdfPanel();
  
  //! set cross section model
  void set( Evolution::Kernel *kernel );

  //! draw pdf itself
  void drawPdf();

protected:
  
  void insert( TGCompositeFrame *p, QCD::PdfParametrization* param );
  void insert( TGCompositeFrame *p, QCD::PdfBase* pdf );
  
private:
  void menu();
  
  Evolution::Kernel *kernel_; // DGLAP Kernel
  TGTextEntry       *path_;   // store pdf configration name

  TCanvas           *pdfPanel_; // for PDF drawing


  ClassDef( PdfPanel, 1 )
};


#endif // _PdfPanel_hh_
