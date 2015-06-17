// $Id: CLogGamma.hh,v 1.3 2008/05/23 02:08:01 miyachi Exp $
/*!
  \file    CLogGamma.hh
  \version $Revision: 1.3 $
  \author  $Author: miyachi $
  \brief   Complex Log Gamma Function Class (Definition)
  
  This is C++ implementation of Log Gamma function. The code was generated
  absed on the 
  
  Algorithms and coefficient values from "Computation of Special
  Functions", Zhang and Jin, John Wiley and Sons, 1996.
  
  The algorithm was largely modified, keeping the basic structure.
  
 */
#ifndef _CLogGamma_hh_
#define _CLogGamma_hh_

#include "ComplexFunction.hh"

namespace Transform {


/*!
  \class CLogGamma   CLogGamma.hh    CLogGamma.hh
  \brief This is a class to calculate log Gamma function.

  The algorithm was based on the 
  "Computation of Special Functions", 
  Zhang and Jin, John Wiley and Sons, 1996.

  The original Fortran code can be obtained from 
  http://jin.ece.uiuc.edu/specfunc.html

 */
class CLogGamma : public ComplexFunction {
public:
  //! a destuctor
  virtual ~CLogGamma();

  //! return log( Gamma(z) )
  virtual std::complex< double > operator()( const std::complex< double >& z );

private:
  //! coefficient given in the above reference
  static double a[ 10 ];

};
};

#endif // _CLogGamma_hh_
