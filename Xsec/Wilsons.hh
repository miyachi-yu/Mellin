// $Id: Wilsons.hh,v 1.3 2008/05/23 02:08:03 miyachi Exp $
/*!
  \file     Wilsons.hh
  \version  $Revision: 1.3 $
  \date     $Date: 2008/05/23 02:08:03 $
  \author   $Author: miyachi $
  \brief    class definition of Wilson coefficient function collection
 */
#ifndef _Xsec_Wilsons_hh_
#define _Xsec_Wilsons_hh_

#include "Coefficient.hh"
#include <vector>

namespace Xsection {
  
  /*!
    \class Wilsons
    \brief collection class for Winson Coefficient function objects.

    Wilson coefficient function object must be stored in the collection
    class. CKernl uses Wilsons object to handle Wilson coefficient which 
    are required to calculate cross section .....

  */
  class Wilsons : 
    public std::vector< std::vector< std::vector< Coefficient* > > > 
  {
  public:
    
    Wilsons();                         //!< a default constructor
    virtual ~Wilsons();                //!< a destructor
    
  };
  
};
#endif //  _Xsec_Wilsons_hh_
