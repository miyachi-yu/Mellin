// $Id: GeneralHarmonic.hh,v 1.5 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file     GeneralHarmonic.cc
  \version  $Revision: 1.5 $
  \date     $Date: 2008/12/02 08:13:16 $
  \author   $Author: miyachi $
  \brief    Definition of General Harmonic Number function class
*/
#ifndef _GeneralHarmonic_hh_
#define _GeneralHarmonic_hh_

#include "ComplexFunction.hh"
#include "CPolyGamma.hh"
#include "CKey.hh"
#include "Li2Int2.hh"

#include <Utility/LookupTable.hh>

namespace Transform {

  /*!
    \class GeneralHarmonic  GeneralHarmonic.hh "Tranform/GeneralHarmonic.hh"
    \brief class for General Harmonic Function
    
    This is a ComplexFunction derived class to calculate
    complex function used in anomalous dimensions for DIS
    processes.
    
    Definition of function can be found in elsewhere,
    such as Nucl. Phys. B465 (1996) 267.
    \f[
    S_m( n ) = \sum_{1}^{n} { 1 \over j^{m} }
    \f]
    whose analytic continuations are
    \f[
    S_m( n ) = \zeta( m ) + ( -1 )^{ m - 1 } * { \Psi_m( n ) \over ( m - 1 )! },
    \f]
    where \f$ \Psi_m( n ) \f$ is the polygamma function implemented
    as CPolyGamma in this library. There is ( m - 1 )! term is missing
    in Nucl. Phys. B465 (1996) 267.
    
    \par
    m runs from 1 to 3. 
    Here the case of 1,2 and 2,1:
    \f[
    S_{2,1}(n) = 
    \zeta(2) * S_1(n) - int_0^1 {\rm d}x { x^n - 1 \over x - 1 } Li2(x)
    \f]
    \f[
    S_{1,2}(n) =  S_1(n) * S_2(n) + S_3(n) - S_{1,2}(n)
    \f]
    are implemented.
    
  */
  class GeneralHarmonic : public ComplexFunction {
  public:
    
    GeneralHarmonic();                                //!< a default constructor
    virtual ~GeneralHarmonic();                       //!< a destructor
    
    //! disable the standard function operator.
    std::complex< double > 
    operator()( const std::complex< double >& n ) { return 0.0; }
    
    /*!
      \param[in] l  is a order for function S_l( N )
      \param[in] n  is a complex value where the function will be evaluated
      \return   a complex value which corresponds to S_l( n )
      \brief    a default function operator instead of ( n ).
    */
    std::complex< double > operator()( const int& l, 
				       const std::complex< double >& n );
    
    static double zeta[4];                            //!< zeta variables
    
    /*!
      \param[in]  i is a first index
      \param[in]  j is a second index
      \return     value of S_{i,j}( n )
      \brief      calculate value of S_{i,j}( n )

      This calculates value of \f$S_{i,2}( n )\f$.
     */
    std::complex< double > operator()( const int& i, const int& j,
				       const std::complex< double >& n );
    
    
  private:
    
    CPolyGamma pg_;   //!< a complex polygamma function class object
    
    static CKey key[4];                                  //!< key for cache
    static std::complex< double > data[4];               //!< data for cache

    //! cache buffer
    static Utility::LookupTable< CKey, std::complex< double > >* cache[4]; 

    static bool cacheReady;               //!< cache status flag
    static void initCache();              //!< initialize cache buffer
    static void closeCache();             //!< close cache buffer
    
    Li2Int2 li2int_;  //!< integration function wuth diLogarithmic function
    
  };
};

#endif // _GeneralHarmonic_hh_
