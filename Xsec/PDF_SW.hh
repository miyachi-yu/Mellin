//
// PDF_SW.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Thu Nov 20 15:42:30 2008 Yoshiyuki Miyachi
// Last update Thu Nov 20 15:42:30 2008 Yoshiyuki Miyachi
//

#ifndef _Xsection_PDF_SW_HH_
#define _Xsection_PDF_SW_HH_

#include <vector>
#include <complex>

#include "Xsec.hh"
#include "XsecComp.hh"

#define PDF_SWAP( T ) class T: public PDF_SW {		\
  public:						\
  T( Xsection::Xsec* xsec ) :				\
    xsec_( xsec ),					\
    vpdf_( xsec->xsecComp()->vpdf()[ 0 ] )		\
    {}							\
  virtual ~T() {}					\
  virtual std::complex< double > operator()		\
  ( const int& i, const std::complex< double >& m ) ;	\
  private:						\
  Xsection::Xsec* xsec_;				\
  std::vector< Transform::ComplexFunction* >& vpdf_;		\
  }

namespace Xsection {
  
  //! abstract class for PDF swapping
  class PDF_SW {
  public:
    
    virtual ~PDF_SW(){};                    //!< a virtual destructor
    virtual std::complex< double > 
    operator()( const int& i, const std::complex< double >& m ) = 0;
    
  protected:
    int su2swap( const int& i );
    
  };

  PDF_SWAP( PDF_P );
  PDF_SWAP( PDF_N );
  PDF_SWAP( PDF_D );
  
};

#endif	    /* !PDF_SW_HH_ */
