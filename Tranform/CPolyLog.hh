// $Id: CPolyLog.hh,v 1.13 2010/02/15 18:56:07 imazu Exp $
/*!
  \file    CPolyLog.hh
  \version $Revision: 1.13 $
  \date    $Date: 2010/02/15 18:56:07 $
  \author  $Author: imazu $
  \brief   Class definition of complex zeta function.
*/
#ifndef _CPolyLog_hh_
#define _CPolyLog_hh_

#include "ComplexFunction.hh"

namespace Transform {
  
  /*!
    \class CPolyLog   CPolyLog.hh    "Tranform/CPolyLog.hh"
    \brief complex PolyLogrithmic function
    
  */
  class CPolyLog : public ComplexFunction {
  public:
    
    CPolyLog();                             //! a default constructor
    
    /*!
      \param [in]  z is a complex variable
      \brief   this calculate the PolyLog( z ) at the given z.
    */
    virtual std::complex< double >
    operator()( const std::complex< double >& z );
    
    virtual std::complex< double >
    operator()( const std::complex< double >& s,
		const std::complex< double >& z ) {
      return ( this->set( s ) )( z );
    }
    
    CPolyLog& set( std::complex< double > n ) { n_ = n; return *this; }
    
  private:
    
    int max_;
    std::complex< double > n_;
    
    std::complex< double > val( const std::complex< double >& z,
				const int& k );
    
  };
  
  
};
#endif //_CPolyLog_hh_
