// $Id: Rectangle.hh,v 1.4 2008/07/24 21:28:17 miyachi Exp $
/*!
  \file    Rectangle.hh
  \version $Revision: 1.4 $
  \date    $Date: 2008/07/24 21:28:17 $
  \author  $Author: miyachi $
  \brief   class definition of Rectangle approximation
 */
#ifndef _Rectangle_hh_
#define _Rectangle_hh_

#include "Integration.hh"

namespace Transform {
  
  /*!
    \class  Rectangle  Rectangle.hh "Tranform/Rectangle.hh"
    \brief  Rectangle integral approximation
  */
  
  class Rectangle : public Integration {
  public:
    //! a destructor
    virtual ~Rectangle();
    
    //! return ( max - min ) * f( 0.5 * ( max + min ) )
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) ;
    
    //! return ( max - min ) * f( 0.5 * ( max + min ) )
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, 
	       ComplexFunction& f );
    
    //! return dx * dz  * f( ( xmax + xmin )/2, ( zmin + zmax )/2 )
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f ) ;
    
    //! return dx * dz  * f( ( xmax + xmin )/2, ( zmin + zmax )/2 )
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f );
  private:
    
  };

};
#endif //  _Rectangle_hh_
