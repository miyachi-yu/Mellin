// $Id: ComplexFunction.hh,v 1.11 2009/04/16 10:31:32 imazu Exp $
/*!
  \file    ComplexFunction.hh
  \version $Revision: 1.11 $
  \date    $Date: 2009/04/16 10:31:32 $
  \author  $Author: imazu $
  \brief   Definition of ComplexFunction class
*/
#ifndef _ComplexFunction_hh_
#define _ComplexFunction_hh_

#include <complex>
#include <string>

namespace Transform {
  
/*!
  \class  ComplexFunction   ComplexFunction.hh   "Tranform/ComplexFunction.hh"
  \brief  a base class for complex function
  
  This is a pure abstract class for functions which return 
  a complex variable from given complex or double variable.
*/
  class ComplexFunction {
  public:
    
    ComplexFunction();                    //!< a default constructor
    virtual ~ComplexFunction();           //!< a destructor
    
    /*!
      \param[in]  z  is a complex< double >
      \return    a value of the complex function at the given z
      \brief     calculate value of function( z )
    */
    virtual std::complex< double > operator() ( const std::complex< double >& z ) = 0 ;
    
    /*!
      \param[in] n is a std::complex< double >
      \param[in] m is a std::complex< double >
      \return      a value of complex function at the given ( n, m )
      \brief       calculate f( n, m )
    */
    virtual std::complex< double > operator() ( const std::complex< double >& n,
						const std::complex< double >& m  ) {
      return std::complex< double >( 0.0, 0.0 );
    };
    
    /*
    //! calculate complex function at the given real x
    virtual std::complex< double > operator() ( const double& x ) {
      return (*this)( std::complex< double >( x, 0.0 ) );
    }
    
    //! calculate complex function at the given real x
    virtual std::complex< double > operator() ( const double& x,
						const double& z ) {
      return (*this)( std::complex< double >( x, 0.0 ),
		      std::complex< double >( z, 0.0 ) );
    }
    */    

    //! get function name
    virtual std::string name() const { return "f(n, m)"; } 
    
    //! differential
    virtual std::complex< double > 
    differential( const std::complex< double >& z,
		  const int& n,
		  const std::complex< double >& dz = 
		  std::complex< double >( 0.1, 0.0 ) );
    
  private:
  };
  
};
/*!
  Macro to create ComplexFcuntion derived class. 
  The derived function only contains 
*/
#define DEF_COMPFUNC( P )						\
  class P: public Transform::ComplexFunction {				\
  public:								\
  virtual ~P() {}							\
  virtual std::complex< double >					\
  operator()( const std::complex< double >& z );			\
  }

#endif //  _ComplexFunction_hh_
