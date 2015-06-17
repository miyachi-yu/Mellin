// $Id: Trapezoid.hh,v 1.6 2008/08/27 02:49:29 miyachi Exp $
/*!
  \file    Trapezoid.hh
  \version $Revision: 1.6 $
  \date    $Date: 2008/08/27 02:49:29 $
  \author  $Author: miyachi $
  \brief   class definition of Trapezoid approximation
 */
#ifndef _Trapezoid_hh_
#define _Trapezoid_hh_

#include "Integration.hh"

namespace Transform {

  /*!
    \class  Trapezoid  Trapezoid.hh "Tranform/Trapezoid.hh"
    \brief  Trapezoid integral approximation
  */
  class Trapezoid : public Integration {
  public:
    
    Trapezoid( const int& n = 1 );

    //! a destructor
    virtual ~Trapezoid();
    
    //! return 0.5 * ( max - mix ) * ( f(max) + f(min) )
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) ;
    
    //! return 0.5 * ( max - mix ) * ( f(max) + f(min) )
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f, const int& n ) ;
    
    //! return 0.5 * ( max - mix ) * ( f(max) + f(min) )
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, 
	       ComplexFunction& f );
    
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, 
	       ComplexFunction& f, const int& n );
    
    //! return dx * dz * ( f(xmin,zmin) + f(xmax,zmin) + f(xmin,zmax) + f(xmax, zmax) ) / 4
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f ) ;
    
    //! return dx * dz * ( f(xmin,zmin) + f(xmax,zmin) + f(xmin,zmax) + f(xmax, zmax) ) / 4
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f );
    
  private:
    int n_;
    double s_;
    std::complex< double > cs_;
    
  };
};
#endif //  _Trapezoid_hh_
