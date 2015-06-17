//$Id: PDFs.hh,v 1.6 2008/05/23 02:08:00 miyachi Exp $
/*!

  \file PDFs.hh
  \version $Revision: 1.6 $
  \author  $Author: miyachi $
  \date    $Date: 2008/05/23 02:08:00 $
  \brief   Definition of Initial fragmentation function parameterization.
  
  Definition of initial fragmentation function class, Fragmentation::PDFs.
  
*/

#ifndef _Fragmentation_PDFs_hh_
#define _Fragmentation_PDFs_hh_

#include <QCD/PdfParametrization.hh>
#include "ConfigFF.hh"
#include "ConfigFFK.hh"

namespace Fragmentation {
  
  
  /*!
    \class  PDFs   Fragmentation::PDFs  "Fragmentation/PDFs.hh"
    \brief  class for initial distribution of fragmentation function
    
  */
  class PDFs : public QCD::PdfParametrization {
  public:
    
    PDFs();                                 //!< a default constructor
    virtual ~PDFs();                        //!< a destructor
    
    PDFs& operator<<( ConfigFF& ff );
    PDFs& operator<<( ConfigFFK& ff );
    
  private:
    
  };
};

#endif //  _PDFs_hh_
