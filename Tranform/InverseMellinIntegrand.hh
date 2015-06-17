/*!
  \file    InverseMellinIntegrand.hh
  \version $Revision: 1.9 $
  \date    $Date: 2008/09/05 02:30:22 $
  \author  $Author: imazu $
  \brief   class definition of integrand of the Mellin inversion
 */

#ifndef _InverseMellinIntegrand_hh_
#define _InverseMellinIntegrand_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {
  /*!
    \class  InverseMellinIntegrand  InverseMellinIntegrand.hh  "Tranform/InverseMellinIntegrand.hh"
    \brief  a class for an integrand of the Mellin inversion
    
    In Mellin inversion, the integrand is 
    \f$ weight(n,x) g(n,x)\f$. 
    
  */

    class InverseMellinIntegrand : public ComplexFunction {
    public:
      
      //      InverseMellinIntegrand( ComplexFunction *f = NULL, InverseMellin* invm = NULL );
      InverseMellinIntegrand( ComplexFunction *f = NULL );
      //! a constructor
      virtual ~InverseMellinIntegrand() {}                      //! a destructor
      
      /*!
	\param[in] n is complex value
	\return value of integrand 
	\brief  calculate value needed for the Mellin inversion integrand
	
	If only single value is given with x() method, it will return
	\f$ x^{-n} f(n) \f$.
	If the x ragne is given, it will return value for the partial 
	integral as
	\f[
	{ ( x_{2}^{N-n} - x_{1}^{N-n} ) \over ( N - n ) ( x_{2} - x_{1} ) }
	f( n )
	\f]
	
      */
      virtual 
      std::complex< double > operator()( const std::complex< double >& n );
      
      /*!
	\param[in] n is complex value
	\param[in] m is complex value
	\return value of integrand 
	\brief  calculate value needed for the Mellin inversion integrand
	
	The function should return 
	\f[
	x^{n} z^{m} f( n, m )
	\f]
	for the partial N-th moment
	\f[
	{ ( x_{2}^{N-n} - x_{1}^{N-n} ) \over ( N - n ) ( x_{2} - x_{1} ) }
	{ ( z_{2}^{N-m} - z_{1}^{N-m} ) \over ( N - m ) ( z_{2} - z_{1} ) }
	f( n, m )
	\f]
	
      */
      virtual 
      std::complex< double > operator()( const std::complex< double >& n,
					 const std::complex< double >& m );

      virtual InverseMellinIntegrand& x( const double& x );                    //!< set value of x
      virtual InverseMellinIntegrand& x( const double& x1, const double& x2 ); //!< set value of x1 and x2
      virtual InverseMellinIntegrand& x( const std::vector< double >& x );     //!< set x
      virtual InverseMellinIntegrand& z( const double& z );  //!< set value of z
      virtual InverseMellinIntegrand& z( const double& z1, const double& z2 ); //!< set value of z1 and z2
      virtual InverseMellinIntegrand& z( const std::vector< double >& z );     //!< set x
      
      //! interface to minimum finding method
      double minimum( const double& x, const double& dx = 0.01 ){
	return rf_.minimum( x, dx ) ;
      }
      
      double minimum( const std::vector< double >& x, 
		      const double& dx = 1.0E-6 ){
	return rf_.minimum( x, dx ) ;
      }
      
      //! set ComplexFunction to be inverted
      void set( ComplexFunction *f ) { f_ = f; }
    
      
      double c0();                        //!< get c0
      double c1( const double& c0 );      //!< get c1
      double c2( const double& c0 );      //!< get c2

      void setmin( const double& min ){ minimum_ = min; }
      
      //! set moment parameters
      void setMomParam( const std::vector< double >& mom );

      
      /*!
	\brief General Complex to Real cast class ( for steepest path )
      */
      class RealFunc : public RealFunction {
      public:
	
	RealFunc( ComplexFunction &f ) : f_( f ) {} //!< a constructor
	virtual ~RealFunc(){}                       //!< a destructor
	
	//! functional operator
	virtual double operator()( const double& x ){
	  return f_( std::complex< double >( x, 0.0 ) ).real();
	}
	
      private:
	ComplexFunction &f_; //!< complex function to be casted
      };

      ComplexFunction* f(){ return f_; }
      
    protected:
      
      ComplexFunction *f_;   //!< f(n)
      double x1_;            //!< base of mellin inversion kernel 
      double x2_;            //!< base of mellin inversion kernel 
      double dx_;            //!< difference of x
      double z1_;            //!< base of mellin inversion kernel 
      double z2_;            //!< base of mellin inversion kernel 
      double dz_;            //!< difference of x
      RealFunc rf_;          //!< complex-real cast function
      bool partialx_;         //!< flag for partial integral
      bool partialz_;         //!< flag for partial integral
      double momx_;           //!< moment of partial integration
      double momz_;           //!< moment of partial integration
      int dim_; 
      double minimum_;      //!< minimum in differentiation
      //      InverseMellin* invm_;

    };

};
#endif 
