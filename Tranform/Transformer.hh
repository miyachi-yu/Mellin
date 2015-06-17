// $Id: Transformer.hh,v 1.2 2008/08/05 08:53:37 miyachi Exp $
/*!
  \file    Transform.hh
  \version $Revision: 1.2 $
  \date    $Date: 2008/08/05 08:53:37 $
  \author  $Author: miyachi $
  \brief   Definition of Transformation base class
 */

#ifndef _Transform_Transformer_hh_
#define _Transform_Transformer_hh_

#include "Integration.hh"

namespace Transform {
  
  /*!
    \class Transformer  Transformer.hh   "Tranform/Transformer.hh"
    \brief a basic class for Transformeration 
    
    This is an abstract class for Transformation;
    \f[
    \phi( z ) = \int_{a}^{b} {\rm d} x K( x, z ) \cdot f( x )
    \f]
    for such, Mellin, Mellin inversion and so on. 
    
  */
  class Transformer {
  public:
    
    //! a default constructor
    Transformer( ) : integration_( NULL ), 
		     lower_( -1.0E+4 ), upper_( 1.0E+4 ) {}
    
    //! a destructor
    virtual ~Transformer() {}
    
    //! set lower edge of the integration
    virtual void lower( const double& v ) { lower_ = v; }
    
    //! set upper edge of the integration
    virtual void upper( const double& v ) { upper_ = v; }
    
    //! set integration method
    void integration( Integration *method ) { integration_ = method; }
    
    //! get a lower edget
    virtual double lower() { return lower_; }
    
    //! get a upper edget
    virtual double upper() { return upper_; }
    
    //! get a pointer to the integration method
    Integration* integration() { return integration_; }
    
  protected:
    Integration *integration_;
    
    double lower_;
    double upper_;
    
  };
  
};
#endif //  _Transformer_hh_
