// $Id: CZeta.hh,v 1.13 2010/02/15 18:56:07 imazu Exp $
/*!
  \file    CZeta.hh
  \version $Revision: 1.13 $
  \date    $Date: 2010/02/15 18:56:07 $
  \author  $Author: imazu $
  \brief   Class definition of complex zeta function.
*/
#ifndef _CZeta_hh_
#define _CZeta_hh_

#include "ComplexFunction.hh"

namespace Transform {
  
  /*!
    \class CZeta   CZeta.hh    "Tranform/CZeta.hh"
    \brief complex Zeta function
    
    In this Mellin framework, 
    Re(s)>0 is used in the Mellin inversion in the most of case.
    So this class implement zeta function in the positive half plane as
    zeta( s ) = 1 / ( 1 - 2^{1 - s} ) sum_1^infinity (-1)^{n-1} / n^s
    
  */
  class CZeta : public ComplexFunction {
  public:

    enum MODE {
      simple, havil2003
    };
    
    CZeta();                             //! a default constructor

    /*!
      \param [in]  z is a complex variable
      \brief   this calculate the Zeta( z ) at the given z.
    */
    virtual std::complex< double >
    operator()( const std::complex< double >& z );


    void mode( const MODE& m ) { mode_ = m; }
    
  private:

    static int C( int n, int k );
    
    std::complex< double > val( const std::complex< double >& z,
				const int& n );
    
    std::complex< double > val( const std::complex< double >& z,
				const int& n, const int& k );
    
    MODE mode_;

  };
  
  
};
#endif //_CZeta_hh_
