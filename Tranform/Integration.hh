// $Id: Integration.hh,v 1.7 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file    Integration.hh
  \version $Revision: 1.7 $
  \date    $Date: 2009/07/28 05:44:48 $
  \author  $Author: miyachi $
  \brief   definition of Integration class
 */
#ifndef _Integration_hh_
#define _Integration_hh_

#include <complex>

#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {

  /*!
    \class   Integration   Integration.hh    "Tranform/Integration.hh"
    \brief   a pure abstract class for Integraiton 
    
    This is a pure abtract class for integration mechanism.
    Integration defines interfaces to be required all classes which 
    inherit Integration.
    
  */
  class Integration {
  public:
    
    virtual ~Integration(){}                 //!< a destructor
    
    /*!
      \param[in] min  is a lower edge of the integration
      \param[in] max  is a upper edge of the integration
      \param[in] f    is a RealFunction to be integrated.
      \return  a partial integration of the given integrand f.
    */
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) = 0 ;
    
    //! same as integral but for ComplexFunction
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, ComplexFunction& f ) = 0 ;
    
    /*!
      \param[in] xmin  is a lower edge of the integration
      \param[in] xmax  is a upper edge of the integration
      \param[in] zmin  is a lower edge of the integration
      \param[in] zmax  is a upper edge of the integration
      \param[in] f    is a RealFunction to be integrated.
      \return  a partial integration of the given integrand f.
    */
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f ) = 0 ;
    
    //! same as integral but for ComplexFunction
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f ) = 0 ;

    
  private:
    
  };
};
#endif // _Integration_hh_

