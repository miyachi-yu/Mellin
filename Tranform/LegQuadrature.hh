// $Id: LegQuadrature.hh,v 1.9 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file      LegQuadrature.hh
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/07/28 05:44:48 $
  \brief     Definition of Legendre-Gauss integration class
  
  $Log: LegQuadrature.hh,v $
  Revision 1.9  2009/07/28 05:44:48  miyachi
  Big update...... PDF parameterization sets are moved to PDFSET subdirectory

  Revision 1.8  2009-01-30 15:18:11  miyachi
  *** empty log message ***

  Revision 1.7  2008-08-28 10:13:50  imazu
  *** empty log message ***

  Revision 1.6  2008-08-19 03:47:10  miyachi
  clean up, a bit....

  Revision 1.5  2008-07-24 21:28:13  miyachi
  Double inversion is implemented in also InverseMellin, in bit different way. Acoordingly, the integration classes were modified...

  Revision 1.4  2008-07-17 07:24:25  imazu
  short changes

  Revision 1.3  2008/05/23 02:08:02  miyachi
  Huge update. Now each library has own namespace. One has to explcitly specify the namespace.

  Revision 1.2  2007-12-01 11:59:00  miyachi
  Big changes: headers are stored into each subdirectory with libirary name.

  Revision 1.1  2007/04/19 10:01:22  miyachi
  First release of libTransform.so

  Revision 1.10  2007/01/19 02:12:41  miyachi
  Add another version of GaussLegendre integration.

  Revision 1.9  2007/01/10 02:16:46  miyachi
  The majour differences are:

  - Modify the algorythm of precision checking used in TrapInt class.
    Now it check the difference between trapezoid approximation and
    simple mid-point integration.

  - Add comments for doxygen. Including some CVS tag in the file comment.

 */
#ifndef _LegQuadrature_h_
#define _LegQuadrature_h_

#include "Integration.hh"
#include <vector>

namespace Transform {
  
  /*!
    \class  LegQuadrature  LegQuadrature.hh   "Tranform/LegQuadrature.hh"
    \brief  Gauss Legendre Integration Class
    
    This class perform integration of the given function,
    and a child class which overwrite function-like operator().
    For the integration, the Lagurange-Gauss integration method
    is used.
    
    Header of the original code
    
    the Legendre-Gauss quadrature formula
    for the integral of a function f(x) over a finite interval [a, b]
    
    copyright : Hidenori Ogata, 8 June 2004, ver.1
  */
  class LegQuadrature : public Integration {
  
  public:
    
    /*!
      \param[in] n    a number of grid used in the integration
      \brief If you want to compute an integral by n-point formula,
      you have to specify the value of the argument n.
    */
    LegQuadrature( const int& n = 4 );
    
    /*!
      \param[in] n    a number of grid used in the integration
      \param[in] m    a number of grid used in the integration
      \brief If you want to compute an integral by n-point formula,
      you have to specify the value of the argument n.
    */
    LegQuadrature( const int& n, const int& m );
    
    //! a destructor
    virtual ~LegQuadrature();
    
    //! integrate the given function with Gass Legendre method
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f );
    
    //! integrate the given function with Gass Legendre method
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, 
	       ComplexFunction& f );

    //! integrate in 2D the given function with Gass Legendre method
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f );
    
    //! integrate in 2D the given function with Gass Legendre method
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f );
    
    //! set number of nodes
    void setngrid( const int& n );
    void setngrid( const int& n, const int& m );

    void setloginteg( const bool& logint ){ loginteg_ = logint; }
    
    static std::vector< double > X( const int& n );
    static std::vector< double > Weight( const std::vector< double >& x );

    
  protected:
    
    //! return number of grid to be used for the integral
    int n() { return n_; }
    
    //! return number of grid to be used for the integral
    int m() { return m_; }
    
    /*!
      \brief compute the abscissae and the weights of the Legendre-Gauss quadrature
    */
    void get_abscissa_weight();
    
    //! set array of x for the integration
    double setX();
    
    //! set i-th element of array of x for the integration
    static double calcX( const int& i, const int& n );
    
    //! set weight based on the value set by setX()
    void setWeight();
    
    //! grid of x for 1st dimension
    std::vector< double > xk_;
    
    //! grid of y for 2st dimension
    std::vector< double > yk_;
    
    //! weight at xk
    std::vector< double > wxk_;
    std::vector< double > wyk_;
    
    //! 2D weigth at ( xk, yk )
    std::vector< std::vector< double > > wk2_;
    
    /*!
      \param[in]  n: order n of the Legendre polynomial Pn(x)
      \param[in]  x: the variable x
      \return value of Legendre polynominal in double
      \brief  Legendre polynomial of order n
    */
    static double Pn_Legendre(  const int& n, const double& x );
    
    /*!
      \fn     static double dPn_Legendre( int n, double x );
      \param[in]  n: order n of the Legendre polynomial Pn(x)
      \param[in]  x: the variable x
      \return derivative of the Legendre polynomial of order n: Pn'(x)
      \brief  This calculate derivative of the Legendre polynomial of order n
      at the given x
    */
    static double dPn_Legendre( const int& n, const double& x );
    
    //! torrelance of the integration
    static double torrelance;
    
    //! convert the array of x based on the given integral range
    double convertX( const double& x, const double& center, 
		     const double& halfwidth ) {
      return center + halfwidth * x;
    }
    
  private:
    bool loginteg_;
    
    double logintegral( const double& min, 
			const double& max, RealFunction& f );
    int n_;
    int m_;
  };
};
#endif //  _LegQuadrature_h_
