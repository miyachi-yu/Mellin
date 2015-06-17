// $Id: StraightLine.hh,v 1.5 2008/10/17 09:59:32 miyachi Exp $
/*!
  \file  StraightLine.hh
  \brief class definition of straight line contour in Mellin inversion
 */
#ifndef __Transform_StraightLine_hh__
#define __Transform_StraightLine_hh__

#include "IntegPath.hh"

namespace Transform {
  
  /*!
    \brief  straight line contour in Mellin inversion

    This is a class for a straight line contour in Mellin inversion.
    It can be parameterized as
    \f[
    z(t) = c_{0} + t * {\rm e}^{i \phi}
    \f]

   */
  class StraightLine : public IntegPath {
  public:
    StraightLine();               //!< a default constructor
    virtual ~StraightLine();      //!< a destructor

    //! get a complex point on the path
    virtual std::complex< double > operator()( const double& t );
    
    //! get a differential on the path
    virtual std::complex< double > differential( const double& t );
    
    //! set parameters
    void set( const double& c0, const double& phi );
    void set( const double& c0 );
    
    double offset(){ return c0_.real(); }
    double angle();
    
  private:
    std::complex< double > c0_;
    std::complex< double > e_;
    std::complex< double > ce_;

  };
}

#endif //__Transform_StraightLine_hh__
