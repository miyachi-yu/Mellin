//$Id: PDFs.hh,v 1.3 2008/05/23 02:08:04 miyachi Exp $
/*!

  \file PDFs.hh
  \version $Revision: 1.3 $
  \author  $Author: miyachi $
  \date    $Date: 2008/05/23 02:08:04 $
  \brief   Definition of Initial fragmentation function parameterization.
  
  Definition of initial fragmentation function class, polFrag::PDFs.
  
*/

#ifndef _polFrag_PDFs_hh_
#define _polFrag_PDFs_hh_

#include <QCD/PdfParametrization.hh>
#include "ConfigFF.hh"

namespace polFrag {
  
  
  /*!
    \class  PDFs   polFrag::PDFs  "polFrag/PDFs.hh"
    \brief  class for initial distribution of fragmentation function
    
  */
  class PDFs : public QCD::PdfParametrization {
  public:
    
    PDFs();                                 //!< a default constructor
    virtual ~PDFs();                        //!< a destructor
    
    PDFs& operator<<( ConfigFF& ff );

  };
};

#endif //  _PDFs_hh_
