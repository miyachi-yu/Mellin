// $Id: GridIntegration.hh,v 1.15 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file    GridIntegration.hh
  \version $Revision: 1.15 $
  \date    $Date: 2009/07/28 05:44:48 $
  \author  $Author: miyachi $
  \brief   class definition of integrand of based on the grid.
*/
#ifndef _GridIntegration_hh_
#define _GridIntegration_hh_

#include "Integration.hh"

namespace Transform {
  /*!
    \class GridIntegration GridIntegration "Tranform/GridIntegration.hh"
    \brief Integration on the grid.
    
    This is a basic integration implemention on the grid.
    User can choose the intergration method in the each grid 
    element, such as trapezoid apploximaiton, recutangular 
    using the mid point, or Gauss Legendre integration.
    
    
    The integration is perfomed chaning size of grid 
    by checking the difference between two different integration methods.
    User can choose any kind of the integration method which
    inherits Integration base class.
    
    If there in only one integration method give, 
    this performs simple grid integration on the given number of grid,
    without considering the target precision.
    
  */
  class GridIntegration : public Integration {
  public:
    
    /*!
      \param[in] method1 is a first integration method
      \param[in] method2 is a second integration method
      \param[in] n is a number of basic grid for the integration.
      \param[in] precision is a required relative precision of the integration.
      \brief This is a default constructor of the grid based integration.
    */
    GridIntegration( Integration *method1, Integration *method2,
		     int n = 2, 
		     double precision = 0.01 );
    
    //! a destructor
    virtual ~GridIntegration();
    
    /*!
      \param[in] min  is a lower edge of the integration region
      \param[in] max  is a upper edge of the integration region
      \param[in] f    is a referene to RealFunction to be integrated
      \return  a partial integral of the given function.
      \brief   This calculates the partial integration int_{min}^{max} dx f(x)
      
      This integrates the given function in the region min < x < max.
      On starting, the given integration region will be devived by n 
      which specified in the constructor. 
      
      If the two methods give non-negiligible difference in one of the
      grid, that integration will be performed in the corresponding grid
      again dividing into n subgrid.
      
    */
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f );

    //! 2D integration
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f );
    
    //! perform integration on the given ComplexFunction
    virtual std::complex< double > 
    cintegral( const double& min, const double& max,
	       ComplexFunction& f ) ;
    
    //! perform 2D integration on the given ComplexFunction
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax,
	       const double& zmin, const double& zmax,
	       ComplexFunction& f ) ;
    
    Integration* method1() { return method1_; } //!< get integration method 1
    Integration* method2() { return method2_; } //!< get integration method 2
    void setmethod1( Integration* integ );  //!< set integration method 1
    void setmethod2( Integration* integ );  //!< set integration method 2
    
    //! set initial number of grid
    void ngrid( const int& n );
    
    //! set initial number of grid
    void ngrid( const int& n1, const int& n2 );

    int ngrid() { return nGridX_; } //!< get number of Grid
    
    //! set precision
    void precision( const double& v ) { precision_ = v; }
    
    static int maxDepth_;      //!< maximum depth of integration
    
  private:
    int nGridX_;               //!< initial grid size
    int nGridY_;               //!< initial grid size
    double precision_;         //!< target precision
    Integration *method1_;     //!< integration method 1
    Integration *method2_;     //!< integration method 2

    int depth_;                //!< depth of integration
    
    //! if one of methods is NULL, set methods_ to be NULL
    void sort_methods(); 
    
    /*!
      \param [in] x is a minimum value of integral region
      \param [in] dx is a grid size
      \param [in] f is a integrant
      \param [in] v1 is a container for method1 result
      \param [in] v2 is a container for method2 result
      \brief calulated integral value for each grid
      
      It will calculate integral in each grid point and store 
      results to the given std::vector< double > containter.
      The container v1 and v2 should have size of nGridX_.
      The size won't be checked......
    */
    void integ_( const double& x, const double& dx, 
		 RealFunction& f, 
		 std::vector< double >& v1,
		 std::vector< double >& v2 );
    
    //! see 1D case
    void integ_( const double& x, const double& dx, 
		 const double& z, const double& dz, 
		 RealFunction& f, 
		 std::vector< std::vector< double > >& v1,
		 std::vector< std::vector< double > >& v2 );
    
    //! see integ_ case
    void cinteg_( const double& x, const double& dx, 
		  ComplexFunction& f, 
		  std::vector< std::complex< double > >& v1,
		  std::vector< std::complex< double > >& v2 );
    
    //! see integ_ case
    void cinteg_( const double& x, const double& dx,
		  const double& z, const double& dz,  
		  ComplexFunction& f, 
		  std::vector< std::vector< std::complex< double > > >& v1,
		  std::vector< std::vector< std::complex< double > > >& v2 );
    
    /*!
      \param [in] v1 is a integral value with method1
      \param [in] v1 is a integral value with method2
      \return true if the difference of the given value is acceptable
      \brief this will check if the difference of the given two values 
      is smaller than the defined precision.
     */
    bool acceptable( const double& v1, const double& v2 );

    /*!
      \param [in] v1 is a integral value with method1
      \param [in] v1 is a integral value with method2
      \param [in] ref is a referene value 
      \return true if the difference of the given value is acceptable
      \brief this will check if the difference of the given two values 
      is smaller than the defined precision against to the given
      reference value/
     */
    bool acceptable( const double& v1, const double& v2, const double& ref );
    
    //! see real value case
    bool acceptable( const std::complex< double >& v1, 
		     const std::complex< double >& v2 );

    //! see real value case
    bool acceptable( const std::complex< double >& v1, 
		     const std::complex< double >& v2, 
		     const std::complex< double >& ref );

    //! return sum of the contents of the given container
    template< class T >
    T vectorSum( const std::vector< T >& v );
    
    //! return sum of the contents of the given container
    template< class T >
    T vectorSum( const std::vector< std::vector< T > >& v );
    
  };
  
  template< class T >
  T GridIntegration::vectorSum( const std::vector< T >& v ){
    T sum = v[ 0 ];
    for( int i = 1; i < nGridX_; i++ ) sum += v[ i ];
    return sum;
  }
  
  template< class T >
  T GridIntegration::vectorSum( const std::vector< std::vector< T > >& v ){
    T sum;
    for( int i = 0; i < nGridX_; i++ ) 
      for( int j = 0; j < nGridY_; j++ )
	if( i == 0 && j == 0 ) sum = v[ i ][ j ];
	else sum += v[ i ][ j ];
    return sum;
  }
  
};
#endif //  _GridIntegration_hh_
