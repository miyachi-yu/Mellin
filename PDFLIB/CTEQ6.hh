// $Id: CTEQ6.hh,v 1.4 2009/01/20 05:58:30 miyachi Exp $
/*!
  \file      CTEQ6.hh
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/01/20 05:58:30 $
  \brief     Definition of CTEQ6 interface class 
 */
#ifndef _CTEQ6_hh_
#define _CTEQ6_hh_

#include "Package.hh"
#include <string>

namespace PDFLIB {

  /*!
    \class CTEQ6   CTEQ6.hh   "CTEQ6.hh"
    \brief Interface Class for CTEQ6 PDFLIB library
  */
  class CTEQ6 : public Package {
  public:
    
    /*!
      \param[in]  set is the PDF parameterization ID
      \param[in]  dir is a directory path which contains tables
      \brief  default constructor
    */
    CTEQ6( int set = 1, std::string dir = "cteq6_table" );
    
    //! copy constructor
    CTEQ6( const CTEQ6& pdf );
    
    //! destructor
    virtual ~CTEQ6();
    
    //! assignment operator
    CTEQ6& operator=( const CTEQ6& pdf );
    
    /*!
      \param[in]   x  is a Bjorken x ( 0 < x < 1 )
      \return  value of PDF for the given parton type and at Q2.
      with setPID and setQ2 method.
    */
    virtual double operator()( const double& x ) ;
    
    /*!
      \param[in]   x  is a Bjorken x ( 0 < x < 1 )
      \param[in]   q2 is a hard scale Q^{2}
      \return  value of PDF for the given parton type and at Q2.
      with setPID and setQ2 method.
    */
    virtual double operator()( const double& x, const double& q2 ) ;
    
    //! get a PDF for u quark
    CTEQ6 u();
    
    //! get a PDF for d quark
    CTEQ6 d();
    
    //! get a PDF for s quark
    CTEQ6 s();
    
    //! get a PDF for c quark
    CTEQ6 c();
    
    //! get a PDF for b quark
    CTEQ6 b();
    
    //! get a PDF for u-bar quark
    CTEQ6 ubar();
    
    //! get a PDF for d-bar quark
    CTEQ6 dbar();
    
    //! get a PDF for s-bar quark
    CTEQ6 sbar();
    
    //! get a PDF for c-bar quark
    CTEQ6 cbar();
    
    //! get a PDF for b-bar quark
    CTEQ6 bbar();
    
    //! get a PDF for gluon
    CTEQ6 g();
    
    int set() const { return set_; }
    void set( int id ) { set_ = id; }
    
  protected:
    virtual void loadGrid();
    
  private:
    int set_;
  };
  
};
#endif

