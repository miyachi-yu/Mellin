// $Id: MSTW2008.hh,v 1.1 2009/07/28 05:44:29 miyachi Exp $
/*!
  \file      MSTW2008.hh
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/07/28 05:44:29 $
  \brief     Definition of MSTW2008 interface class 
 */
#ifndef _PDFLIB_MSTW2008_hh_
#define _PDFLIB_MSTW2008_hh_

#include "Package.hh"

#include <string>

namespace PDFLIB {

  class c_mstwpdf;

  /*!
    \class MSTW2008   MSTW2008.hh   "MSTW2008.hh"
    \brief Interface Class for MSTW2008 PDFLIB library
  */
  class MSTW2008 : public Package {
  public:
    
    /*!
      \param[in]  set is the PDF parameterization ID
      \param[in]  dir is a directory path which contains tables
      \brief  default constructor
    */
    MSTW2008( const std::string& prefix = "mstw2008nlo", 
	      const int& cl = 0,
	      const int& eid = 0,
	      const std::string& dir    = "mstw2008_table" );
    
    //! copy constructor
    MSTW2008( const MSTW2008& pdf );
    
    //! destructor
    virtual ~MSTW2008();
    
    virtual double parton();
    virtual double type();
    
  protected:
    virtual void loadGrid();
    
  private:
    std::string prefix_;
    c_mstwpdf *mstwpdf_;
    int cl_;
    int eid_;
    
  };
  
};
#endif

