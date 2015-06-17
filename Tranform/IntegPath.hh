// $Id: IntegPath.hh,v 1.2 2008/07/17 10:09:21 miyachi Exp $
/*!
  \file   IntegPath.hh
  \brief  base class definition of Mellin inversion contour
 */
#ifndef __Transform_IntegPath_hh__
#define __Transform_IntegPath_hh__

#include "ComplexFunction.hh"

namespace Transform {
  
  /*!
    \brief  class for Mellin inversion contour
  */
  class IntegPath : public ComplexFunction {
  public:
    
    IntegPath();                         //!< a constructor
    virtual ~IntegPath();                //!< a destructor
    
    //! disable standard function operator
    virtual std::complex< double > 
    operator()( const std::complex< double >& z );
    
    //! get a complex point on the path
    virtual std::complex< double > operator()( const double& t ) = 0;
    
    //! get a differential on the path
    virtual std::complex< double > differential( const double& t ) = 0;
    
  private:
    
  };

};

#endif // __Transform_IntegPath_hh__
