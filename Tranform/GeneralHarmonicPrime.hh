// $Id: GeneralHarmonicPrime.hh,v 1.2 2008/05/23 02:08:02 miyachi Exp $
/*!
  \file   GeneralHarmonicPrime.hh
 */
#ifndef _GeneralHarmonicPrime_hh_
#define _GeneralHarmonicPrime_hh_

#include "GeneralHarmonic.hh"

namespace Transform {
  
  /*!
    \class GeneralHarmonicPrime GeneralHarmonicPrime.hh "Tranform/GeneralHarmonicPrime.hh"
    \brief Class for S'_l(n/2) implementation.
    
    This is class for Generalized Harmonic Number Prime function
    needed in NLO calculation, 
    \f[
    S_{l}^{\prime}( n/2 ) = 
    2^{l-1} sum_{j=1}^{n} { 1 + (-1)^j  \over j^l }
    \f]
    It can be re-written
    \f[
    S_{l}^{\prime}( n/2 ) = 
    = { 1 + eta \over 2 } S_l( { n \over 2 } ) + 
    { 1 - eta \over 2 } S_l( { n - 1 \over 2 } ) 
    \f]
  */
  class GeneralHarmonicPrime : public ComplexFunction {
  public:
    GeneralHarmonicPrime();              //!< a default construtor
    virtual ~GeneralHarmonicPrime();     //!< a destructor
    
    //! disable the standard function operator.
    std::complex< double > operator()( const std::complex< double >& n ) { return 0.0; }
    
    /*!
      \param[in] m  is a order for function S'_l( N/2 )
      \param[in] n  is a complex value where the function will be evaluated
      \param[in] eta is a 
      \return   a complex value which corresponds to S'_l( n/2 )
      \brief    a default function operator instead of ( n ).
    */
    std::complex< double > operator()( const int& l, 
				  const std::complex< double >& n,
				  const int& eta );
    
  private:
    GeneralHarmonic s_;
  };
  
};

#endif //  _GeneralHarmonicPrime_hh_
