// $Id: R1998.hh,v 1.2 2008/05/20 00:44:20 miyachi Exp $
/*!
  \file  R1998.hh
  \brief class definition of R1998 implementation
 */
#ifndef _R1998_hh_
#define _R1998_hh_

#include "R1990.hh"

namespace StrFncParam {

  /*!
    \class R1998 R1998.hh "StrFncParam/R1998.hh"
    \brief Implementation of R1998 parameterization
    
    This is implementation of structure function R parameterization, R1998,
    K. Abe, et al., Phys. Lett. B452 (1998) 194.
    
  */
  class R1998 : public R1990 {
  public:
    
    R1998();                             //!< a default constructor
    virtual ~R1998();                    //!< a destuctor
    
    //!< evaluation of R1998 parameterization
    virtual double eval( const double& x );
    
    //!< uncertainty evaluation of R1998 parameterization
    virtual double evalError( const double& x );
    
  private:
    std::vector< double > a;
    std::vector< double > c;
    
    double th;                       //!< Theta( x, Q2 ) / log( Q2 / L2 )
    double Ra( const double& x );
    double Rb( const double& x );
    double Rc( const double& x );
    
  };
};

#endif // _R1998_hh_
