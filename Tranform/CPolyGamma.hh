// $Id: CPolyGamma.hh,v 1.11 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file     CPolyGamma.hh
  \version  $Revision: 1.11 $
  \date     $Date: 2008/12/02 08:13:16 $
  \author   $Author: miyachi $
  \brief    Definition of PolyGamma function class
 */
#ifndef _CPolyGamma_hh_
#define _CPolyGamma_hh_

#include "ComplexFunction.hh"
#include "CKey.hh"
#include "CGamma.hh"

#include <Utility/LookupTable.hh>

namespace Transform {

/*!
  \class CPolyGamma   CPolyGamma.hh    "Tranform/CPolyGamma.hh"
  \brief complex polygamma function

  This is implementation of complex PolyGamma function, which 
  you can find elsewhere as
  phi^n(z) = ( d^(n+1)/dz^(n+1) ) logGamma( z )
  
  The apploximation is based on Handbook of Special Functions
  with some apploxymation given in Nucl. Phys. B465 (1996) 267.
  
  Since the default, copy constructors and the assignment operator
  were implemented, one can also use elements of STL vector class.
  (which may sometime becomes life easier...)
*/
  class CPolyGamma : public ComplexFunction {
  public:
    
    CPolyGamma( int m = 0 );            //!< a default constructor
    CPolyGamma( const CPolyGamma& pg ); //!< a copy constructor
  
    CPolyGamma& operator=( const CPolyGamma& pg ); //!< assignment operator
    
    virtual ~CPolyGamma();              //!< a destructor
    
    //! implementation of ComplexFunction::operator( complex< double > )
    virtual std::complex< double > operator() ( const std::complex< double >& n );
    //! with dimensiton 
    virtual std::complex< double > operator() ( const int& m, 
						const std::complex< double >& n ) {
      this->m( m ); return (*this)( n );
    }
    
    int& m() { return m_; }        //!< get a reference of dimension
    int m() const { return m_; }   //!< get a value of dimension
    int m( int v ) { m_ = v; this->setmcore(); return m_; } //!< set dimension
    
  private:
    int m_;              //!< dimension of differencial
    int mcore_;          //!< weight used in the recurrence formula
    
    void setmcore();     //!< calculate the recurrence weight
    
    //! calculate value of polygamma with approxymation for the case n > 10
    std::complex< double > largeN( const std::complex< double >& n );
    
    // these are members for LookupTable
    //! cache buffer
    static Utility::LookupTable< CKey, std::complex< double > >* cache[4]; 
    static CKey key[4];                        //!< key  for cache
    static std::complex< double > data[4];     //!< data for cache
    static bool cacheReady;                    //!< flag for cache buffer
    static void initCache();                   //!< initialize cache buffer
    static void closeCache();                  //!< close cache buffer
    
    CGamma gamma_;                             //!< gamma function
    
  };
  
};
#endif // _CPolyGamma_hh_
