// $Id: MellinIntegrand.hh,v 1.6 2009/06/04 09:22:22 miyachi Exp $
/*!
  \file    MellinIntegrand.hh
  \version $Revision: 1.6 $
  \date    $Date: 2009/06/04 09:22:22 $
  \author  $Author: miyachi $
  \brief   class definition of integrand of the Mellin transform
 */
#ifndef _MellinIntegrand_hh_
#define _MellinIntegrand_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {
  /*!
    \class  MellinIntegrand  MellinIntegrand.hh  "Tranform/MellinIntegrand.hh"
    \brief  a class for an integrand of the Mellin integration
    
    In Mellin transform, the integrand is x^z * f(x).
    This class acts as a complex fucntion with the real function 
    to be Mellin transformed.
    
  */
  class MellinIntegrand : public ComplexFunction {
  public:
    
    //! a default constructor
    MellinIntegrand();
    
    /*!
      \par [in]  f is a pointer to RealFunction 
      \brief a constructor with a pointer to a RealFunction to be transform.
    */
    MellinIntegrand( RealFunction *f );

    //! a destructor
    virtual ~MellinIntegrand();
    
    //! set a pointer to RealFunction
    MellinIntegrand& set( RealFunction *f );
    
    //! set a complex variable in the integrand
    MellinIntegrand& set( const std::complex< double >& z );
    
    //! set a complex variable in the integrand
    MellinIntegrand& set( const std::complex< double >& n,
			  const std::complex< double >& m );
    
    //! return a value to be integrated by Mellin transformation
    virtual std::complex< double > operator() ( const std::complex< double >& z );
    
    //! return a value to be integrated by Mellin transformation
    virtual std::complex< double > 
    operator() ( const std::complex< double >& n,
		 const std::complex< double >& m );
    
    //! return a value to be integrated by Mellin transformation
    virtual std::complex< double > operator() ( const double& x );
    
    //! return a value to be integrated by Mellin transformation
    virtual std::complex< double > operator() ( const double& x,
						const double& z );
    
  private:
    
    RealFunction* f_;
    std::complex< double > n_; 
    std::complex< double > m_;
    
  };
};
#endif //  _MellinIntegrand_hh_
