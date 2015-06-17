// $Id: TrapInt.hh,v 1.5 2008/07/24 21:28:22 miyachi Exp $
/*!
  \file     TrapInt.hh
  \date     $Date: 2008/07/24 21:28:22 $
  \version  $Revision: 1.5 $
  \author   $Author: miyachi $
  \brief    Definition of Trapezoid approximation integration class
  
  $Log: TrapInt.hh,v $
  Revision 1.5  2008/07/24 21:28:22  miyachi
  Double inversion is implemented in also InverseMellin, in bit different way. Acoordingly, the integration classes were modified...

  Revision 1.4  2008/05/23 02:08:02  miyachi
  Huge update. Now each library has own namespace. One has to explcitly specify the namespace.
  
  Revision 1.3  2007-12-01 11:59:00  miyachi
  Big changes: headers are stored into each subdirectory with libirary name.
  
  Revision 1.2  2007/05/10 05:14:54  miyachi
  *** empty log message ***

  Revision 1.1  2007/04/19 10:01:23  miyachi
  First release of libTransform.so

  Revision 1.3  2007/01/10 04:42:56  miyachi
  *** empty log message ***

  Revision 1.2  2007/01/10 02:16:46  miyachi
  The majour differences are:

  - Modify the algorythm of precision checking used in TrapInt class.
    Now it check the difference between trapezoid approximation and
    simple mid-point integration.

  - Add comments for doxygen. Including some CVS tag in the file comment.
  
*/
#ifndef _TrapInt_hh_
#define _TrapInt_hh_

#include "Integration.hh"

namespace Transform {
  
  /*!
    \class   TrapInt    TrapInt.hh    "Tranform/TrapInt.hh"
    \brief   Integration class based on the trapezoid approximation.
    
    Grid of the integration region is dinamically determined
    looking the difference between the trapezoid approximation
    and the mid-point integration.
    
  */
  class TrapInt : public Integration {
  public:
    
    /*!
      \param[in]  precision  precision level for the integration
      \brief      a default constructor for TrapInt class.
      
      User can change the precision of the integration
      by giving as the second argument.
    */
    TrapInt( double precision = 0.1 );
    
    //! destructor
    virtual ~TrapInt();
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) ;
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, ComplexFunction& f );
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f ) {
      return 0.0;
    }
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f ) {
      return std::complex< double >( 0.0, 0.0 );
    }
    
    
  private:
    
    double precision_;  //!< precision level
    
    /*!
      \param[in]   min   lower boundary of the integration region
      \param[in]   max   upper boundary of the integration region
      \param[in]   n     number of grid in the given integration region
      \param[in]   f     integrand function
      \return      the integral in the given region
      \brief       This performs the trapezoid approximatin integration
      
      in the given intgegration region min < x < max.
      The region will be divided to n sub-region, 
      then the integral will be calculated in each subregions.
    */
    double simpleIntegral_( double min, double max, int n, RealFunction& f );
    
    //! see simpleIntegral_
    std::complex< double > csimpleIntegral_( double min, double max, int n, ComplexFunction& f );
    
    /*!
      \param[in]   min   lower boundary of the integration region
      \param[in]   max   upper boundary of the integration region
      \param[in]   n     number of grid in the given integration region
      \param[in]   f     integrand function
      \return      the integral in the given region
      \brief       Peform the trapezoid integration.
      It tries to compare the integral in the sub-region
      if the integral in determined within the required 
      precision. If not, it tries to divide the sub-region
      into two, then try the integral in the sub-sub-region,
      by reachiing the required precision.
    */
    double integral_( double min, double max, int n, RealFunction& f );
    
    //! see cintegral_
    std::complex< double > cintegral_( double min, double max, int n, ComplexFunction& f );
    
    /*!
      \param[in]   min   lower boundary of the integration region
      \param[in]   max   upper boundary of the integration region
      \param[in]   n     number of grid in the given integration region
      \param[in]   f     integrand function
      \return      the integral in the given region    
      \brief       This calculate the integral using the mid point of the sub-region.
    */
    double midPoint_( double min, double max, int n, RealFunction& f );
    //! see cmidPoint_
    std::complex< double > cmidPoint_( double min, double max, int n, ComplexFunction& f );
    
  };
  
};
#endif
