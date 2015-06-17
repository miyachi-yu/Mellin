// $Id: DInverseMellin.hh,v 1.8 2008/11/11 04:06:22 miyachi Exp $
/*!
  \file  DInverseMelli.hh
  \brief class definition of double Mellin Inversion
 */
#ifndef _DInverseMellin_hh_
#define _DInverseMellin_hh_

#include "ComplexFunction.hh"
#include "DInverseMellinIntegrand.hh"
#include "Transformer.hh"

namespace Transform {
  
  /*!
    \brief  Double Mellin Inversion class
    
    In order to perfome Inverse Mellin Transfromation on
    a 2-Dimentional complex function, g( n, m ).
    
  */
  class DInverseMellin : public RealFunction, public Transformer {
    
  public:
    
    /*!
      \param[in]  g is a pointer to ComplexFunction to be inverted.
      \param[in]  inter is a pointer to Integration object
      \brief a constructor with integrand and integrator
    */
    DInverseMellin( ComplexFunction *g, Integration *inter );
    
    /*!
      \param[in]  inter is a pointer to Integration object
      \brief a constructor with integrator
    */
    DInverseMellin( Integration *inter );
    
    virtual ~DInverseMellin(); //!< a destructor
    
    virtual void upper( const double& v ); //!< set upper limit
    virtual double upper(); //!< get upper limit
    virtual void upper( const std::vector< double >& v ); //!< set upper limit

    /*!
      \param[in] c is a vecotor of double
      \param[in] phi is a vecotor of double
      \brief       set integration path parameters
     */
    void setParameter( const std::vector< double >& c, 
		       const std::vector< double >& phi );
    
    //! set mom parameter
    void setMomParam( const std::vector< double >& mom );
    
    /*!
      \param[in] g is a ComplexFunction pointer
      \return      reference of itsefl
      \brief       set a complex function to be Mellin Inversed.
     */
    DInverseMellin& transform( ComplexFunction *g );
    
    /*!
      \param[in] x is a variable container for 1st variable
      \param[in] z is a variable container for 2nd variable
      \return      value of inversed function at the given point
      \brief perform Mellin Inversion on the given complex function.

      The arguments x and z are vector of double, in which one can 
      set one or two variables. If x ( or z ) has only one element,
      Mellin inversion on the corresponding variable will be perfomed.
      In case x ( or z ) has 2 elements, 
      this try to perform Mellin inversion to get a partial moment,
      \f$ \int_{x[0]}^{x[1]} {\rm d}x f( x, z ) \f$.

     */
    virtual double operator()( const std::vector< double >& x, 
			       const std::vector< double >& z );

    virtual double operator()( const double& x ){ return 0.0; }
    
    //! get a reference of integrand object
    DInverseMellinIntegrand& integrand() { return integrand_; }
    
  private:
    DInverseMellinIntegrand integrand_; // integrand for Mellin Inversion
    
    //! set some parameters related x, z ( like y in multiplicity )
    virtual void setxzparams( const std::vector< double >& x, 
			      const std::vector< double >& z ){}; 
    
  };
  
};
#endif
