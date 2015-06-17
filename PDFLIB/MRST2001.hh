// $Id: MRST2001.hh,v 1.1 2009/02/04 07:05:09 miyachi Exp $
/*!
  \file      MRST2001.hh
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/02/04 07:05:09 $
  \brief     Definition of MRST2001 interface class 
 */
#ifndef _MRST2001_hh_
#define _MRST2001_hh_

#include "Package.hh"
#include <string>

namespace PDFLIB {

  /*!
    \class MRST2001   MRST2001.hh   "MRST2001.hh"
    \brief Interface Class for MRST2001 PDFLIB library
  */
  class MRST2001 : public Package {
  public:
    
    /*!
      \param[in]  set is the PDF parameterization ID
      \param[in]  dir is a directory path which contains tables
      \brief  default constructor
    */
    MRST2001( int set = 0, std::string dir = "mrst2001_table" );
    
    //! copy constructor
    MRST2001( const MRST2001& pdf );
    
    //! destructor
    virtual ~MRST2001();
    
    virtual double parton();
    virtual double type();

    int set() const { return set_; }
    void set( int id ) { set_ = id; }
    
  protected:
    virtual void loadGrid();
    
  private:
    int set_;
    double uv_;
    double dv_;
    double us_;
    double ds_;
    double s_;
    double c_;
    double b_;
    double g_;
  };
  
};
#endif

