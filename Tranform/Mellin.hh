// $Id: Mellin.hh,v 1.8 2009/06/04 09:22:22 miyachi Exp $
/*!
  \file    Mellin.hh
  \version $Revision: 1.8 $
  \date    $Date: 2009/06/04 09:22:22 $
  \author  $Author: miyachi $
  \brief   Definition of Mellin transform class
 */
#ifndef _Mellin_hh_
#define _Mellin_hh_

#include "Integration.hh"
#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include "MellinIntegrand.hh"
#include "Transformer.hh"

namespace Transform {

  /*!
    \class  Mellin   Mellin.hh   "Tranform/Mellin.hh"
    \brief  Mellin transform class
    
    This is a Mellin transform class. Mellin transform is a
    integration of some function with a certain weight x^z 
    where z is a complex in the region 0 < x < infinity.
    \f[
    \phi( z ) = \int_{0}^{\inf} {\rm d}x x^{z - 1} \cdof f( x )
    \f]
    
    This class performs the tranformation 
    using Integration and MellinIntegrand class.
    
  */
  class Mellin : public ComplexFunction, public Transformer {
  public:
    
    /*!
      \param[in] f is a reference of RealFunction to be tranformed
      \param[in] inter is a pointer to Integration object
      \brief a default constructor.
      
      One must specify one RealFunction to be transform
      and a pointer to Integration class to perform the integration.
      
    */
    Mellin( RealFunction *f, Integration *inter );
    
    //! a constructor with integration method
    Mellin( Integration *inter );
    
    //! a destructor
    virtual ~Mellin();
    
    //! calculate a value of M(f)(z) where M means Mellin transform
    virtual std::complex< double > operator() ( const std::complex< double >& z );
    
    //! calculate a value of M(f)(z) where M means Mellin transform
    virtual std::complex< double > 
    operator() ( const std::complex< double >& n,
		 const std::complex< double >& m );
    
    /*!
      \param [in]  f is a reference to RealFunction 
      \return a reference to Mellin transformed ComplexFunction
      \brief  set real function for Mellin transformation
      
      This is a test implementation of Mellin tranformation.
      For the moment, this function returns a referene to itself,
      it could be possible that the obtained referece performs
      unexpectedly.... 
      
      Usage:
      Mellin  M(...);
      RealFunction f;
      ComplexFunction mf = M.transform( f );
      
      cout << mf( std::complex< double >( 2.0, 3.0 ) << endl;
      
      It looks more natural.....
    */
    ComplexFunction& transform( RealFunction *f );
    
  private:
    MellinIntegrand integrand_;
    
  };
  
};

#endif //  _Mellin_hh_
