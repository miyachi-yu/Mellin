/*!
  \file    PartialInverseMellinIntegrand.hh
  \version $Revision: 1.8 $
  \date    $Date: 2008/09/03 06:38:42 $
  \author  $Author: imazu $
  \brief   class definition of integrand of the Mellin inversion ( this is not in use from 2008-09-03 )
*/

#ifndef _PartialInverseMellinIntegrand_hh_
#define _PartialInverseMellinIntegrand_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {

/*!
  \class  PartialInverseMellinIntegrand  PartialInverseMellinIntegrand.hh  "Tranform/PartialInverseMellinIntegrand.hh"
  \brief  a class for an integrand of the Mellin inversion

  In Mellin inversion, the integrand is x^-z * g(z).
  
*/
class PartialInverseMellinIntegrand : public RealFunction {
  
public:

  //! a default constructor
  PartialInverseMellinIntegrand();
  /*!
    \par [in]  g is a pointer to ComplexFunction 
    \brief a constructor with a pointer to a ComplexFunction to be transform.
  */
  PartialInverseMellinIntegrand( ComplexFunction *g );

  //! a destructor
  virtual ~PartialInverseMellinIntegrand();
  
  //! set complex mellin integration parameters  
  void setParameter( const double& c, const double&  phi );
  
  //! set the value of required moment ( default -> 1 )
  void setMom( const double& moment ){ mom_ = moment; };
  
  //! set a pointer to ComplexFunction
  PartialInverseMellinIntegrand& set( ComplexFunction *g );
  
  //! set a real variable in the integrand
  PartialInverseMellinIntegrand& set( const double& x1, const double& x2);
 
  //! return a value to be integrated by Mellin inversion
  //  virtual std::complex< double > operator() ( const std::complex< double >& z );
  
  /*! 
    \par [in]  r is absolute value of poler coodinates in complex space. 
    \brief return a value to be integrated by Mellin inversion.
  */
  virtual double operator() ( const double& r );

  double c() { return c_; } //!< get offset on a real axis
  
private:
  
  ComplexFunction* g_;
  double x1_;
  double x2_;
  double dx_;
  double c_;
  double mom_;
  std::complex< double > e_;

};
};

#endif 
