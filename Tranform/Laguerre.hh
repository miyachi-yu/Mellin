// $Id: Laguerre.hh,v 1.2 2008/07/29 08:59:10 miyachi Exp $
/*!
  \file Laguerre.hh
  \brief class definition of generalized Laguerre quadurature
 */
#ifndef _Laguerre_hh_
#define _Laguerre_hh_

#include "RealFunction.hh"
#include "ConfluentHG.hh"
#include "CGamma.hh"

#include <vector>

namespace Transform {
  
  /*!
    \brief  class for generalized Laguerre quadurature.
    
    Generalized Laguerre quadrature is defined as
    \f[
    L_{n}^{\alpha}( x ) =
    {( \alpha + 1 )_{n} \over n ! } 
    _{1}F_{1}( -n; \alpha; x )
    \f]
    where 
    \f$ (\aplha + 1)_n  = \Gamma(\alpha + 1 + n) / \Gamma(\alpha + 1)\f$.
    
  */
  class Laguerre : public RealFunction {
  public:

    Laguerre( const int& n, const double& alpha = 0 ); //!< a constructor
    virtual ~Laguerre();                               //!< a destructor
    
    void set( const int& n, const double& alpha ); //!< re-set parameters
    virtual double operator()( const double& x );  //!< get Luguerre
    
    std::vector< double >& absissa() { return x_; } //!< get absissa
    std::vector< double >& weight(){ return w_; } //!< get weight
    
  private:
    
    ConfluentHG f1_; //!< a confluent hypergeometric function
    double prefix_;
    CGamma gamma_;
    double a_;
    int n_;
    
    std::vector< double > x_;
    std::vector< double > w_;
    
    double findZeroCross( double x = 0.0, const double& dx = 1.0E-3 );
    void getAbsissa();

  };

};

#endif // _Laguerre_hh_
