// $Id: RealFunction.hh,v 1.13 2008/11/11 04:06:22 miyachi Exp $
/*!
  \file    RealFunction.hh
  \version $Revision: 1.13 $
  \date    $Date: 2008/11/11 04:06:22 $
  \author  $Author: miyachi $
  \brief   Definition of ComplexFunction class
 */
#ifndef _RealFunction_hh_
#define _RealFunction_hh_

#include <string>
#include <vector>

namespace Transform {
  
  /*!
    \class  RealFunction  RealFunction.hh  "Tranform/RealFunction.hh"
    \brief  a base class for real function
    
    This is a pure abstract class for functions which return 
    double value.
  */
  class RealFunction {
  public:
    
    //!  a default constructor
    RealFunction();
    
    //! destructor
    virtual ~RealFunction();
    
    //! calculate f(x) at the given x
    virtual double operator() ( const double& x ) = 0 ;
    
    virtual double operator() ( const double& x, const double& z )
    { return 0.0; }
    
    virtual double operator()( const std::vector< double >& x ) 
    { return 0.0; }
    
    virtual double operator()( const std::vector< double >& x, 
                               const std::vector< double >& z )
    { return 0.0; }
    
    //! get function name 
    virtual std::string name() const { return "f(x)"; } 
    
    /*!
      \param[in] x is a point where differential will be calculated
      \param[in] n is a rank of differential
      \param[in] dx is a step size ( can be negative also )
      \return    n-th differential at the given x. ( n = 1, 2, ... )
      \brief     this calculate n-th differential of the given function
      
      This calculates
      \f[
      { d^{n} f \over dx^{n} }( x )
      \f]
      
      If the functional form is analytically known, it is better to overwrite
      this public method.
      
    */
    virtual double differential( const double& x, 
				 const int& n = 1,
				 const double& dx = 0.1 );
    
    /*!
      
      \param[in] x is a point where differential will be calculated
      \param[in] z is a point where differential will be calculated
      \param[in] nx is a rank of differential
      \param[in] nz is a rank of differential
      \param[in] dx is a step size ( can be negative also )
      \param[in] dz is a step size ( can be negative also )
      \return    n-th differential at the given (x,z)
      \brief     this calculate nx-th,nz-th differential of the given function

      This calculates
      \f[
      { \partial^{nx + nz } f \over \partial x^{nx} \partial z^{nz}  }( x, z )
      \f]
      
      If the functional form is analytically known, 
      it is better to overwrite this public method.
      
    */
    virtual double differential( const double& x, 
				 const double& z, 
				 const int& nx = 1,
				 const int& nz = 1,
				 const double& dx = 0.1,
				 const double& dz = 0.1 );
    
    /*!
      \param[in] x is a starting point to find minimum closed by.
      \param[in] dx is a step size for minimum finding
     */
    virtual double minimum( const double& x, const double& dx = 0.01 );
    
    /*!
      \param[in] x is a starting point to find minimum closed by.
      \param[in] z is a starting point to find minimum closed by.
      \param[in] dx is a step size for minimum finding
      \param[in] dz is a step size for minimum finding
    */
    virtual double minimum( const double& x,  const double& z,
			    const double& dx, const double& dz );
    
    /*!
      \param[in] x is a x-range where the minimum will be searched
      \param[in] tol is a tollerance
      \brief     try to find minimum point inside the given range
  
      Perform minimum finding in the given range using 
      modified Brent method with differential.
      ( Numerical recipt in C, 10.3 )
    
      x is a vector which has 3 elements: 
      x[1] should exist inside x[0] and x[2], and 
      f( x[1] ) must be smaller than f( x[0] ) and f( x[1] ).
      
    */
    virtual double minimum( const std::vector< double >& x,
			    const double& tol = 1.0E-6 );
    
    
  private:
    double precision_;
    
    
  };
};

/*!
  Macro to create RealFcuntion derived class. 
  The derived function only contains 
*/
#define DEF_REALFUNC( P )						\
  class P: public Transform::RealFunction {				\
  public:								\
  P(){}									\
  virtual ~P() {}							\
  virtual double operator()( const double& x );				\
  }

#define IMP_REALFUNC( P )			\
  double P::operator()( const double& x )


#endif //  _RealFunction_hh_
