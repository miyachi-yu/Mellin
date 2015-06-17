// $Id: GeneralHarmonicTilde.hh,v 1.2 2008/05/23 02:08:02 miyachi Exp $
#ifndef _GeneralHarmonicTilde_hh_
#define _GeneralHarmonicTilde_hh_

#include "ComplexFunction.hh"
#include "GeneralHarmonic.hh"
#include "GeneralHarmonicPrime.hh"
#include "CPolyGamma.hh"
#include "Li2Int.hh"
#include "HyperGeomInt.hh"

namespace Transform {
  
  /*!
    \class  GeneralHarmonicTilde GeneralHarmonicTilde.hh "Tranform/GeneralHarmonicTilde.hh"
    \brief  class for GeneralHarmonic related function

    \f[
    \tilde{S}_l( n ) = sum_{j=1}^{n} ( -1 )^j / j^l 
    \f]
    
    \f$\tilde{S}_{1,2}(n)\f$, 
    \f$\tilde{S}_{2,1}(n)\f$, and \f$\tilde{S}(n)\f$ will be also impletented.
    
    \f[
    S_l(n) + \tilde{S}_l(n) = { 1.0 \over 2^{l-1} } S( n \over 2 )   ~~~  ( \eta = 1 )
    \f]
    
    \f[
    S_l(n) + \tilde{S}_l(n) = { 1.0 \over  2^{l-1}} S({ n - 1 \over 2 }) ~~~ ( \eta == -1 )
    \f]
    
    \f[
    \tilde{S}_l(n) = 2^{ 1 - l } S( n/2 - ( \eta > 0 ? 0.0 : -0.5 ) ) - S_l(n)
    \f]
  */
  class GeneralHarmonicTilde : public ComplexFunction {
  public:
    
    GeneralHarmonicTilde();           //!< a default constructor
    virtual ~GeneralHarmonicTilde();  //!< a destructor

    
    virtual std::complex< double > operator()( const std::complex< double >& n ){
      return std::complex< double >( 0.0, 0.0 );
    }
    
    virtual std::complex< double > operator()( const std::complex< double >& n,
					       const int& eta );
    
    virtual std::complex< double > operator()( const int& l, 
					       const std::complex< double >& n,
					       const int& eta );
    
    virtual std::complex< double > operator()( const int& i,
					       const int& j,
					       const std::complex< double >& n,
					       const int& eta );
    
    
  private:
    CPolyGamma pg_;   //!< a complex polygamma function class object
    GeneralHarmonic s_;
    GeneralHarmonicPrime sp_;
    Li2Int li2int_;
    HyperGeomInt hgint_;
  };
};
#endif // _GeneralHarmonicTilde_hh_
