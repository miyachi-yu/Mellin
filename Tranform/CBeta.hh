// $Id: CBeta.hh,v 1.13 2010/02/15 18:56:07 imazu Exp $
/*!
  \file    CBeta.hh
  \version $Revision: 1.13 $
  \date    $Date: 2010/02/15 18:56:07 $
  \author  $Author: imazu $
  \brief   Class definition of complex beta function.
*/
#ifndef _CBeta_hh_
#define _CBeta_hh_

#include "ComplexFunction.hh"
#include "CGamma.hh"
#include "IntegFunc.hh"
#include "CKey.hh"
#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class CBeta   CBeta.hh    "Tranform/CBeta.hh"
    \brief complex Beta function
    
    This is not exactly complex Beta function, 
    but a function which is equivalent to the Mellin transrated 
    \f[
    f(x) = x^{\alpha} * ( 1 - x )^{\beta}
    \f]
    funtion. On the construction of CBeta function, one must specify
    these alpha and beta.
    
    \par \f$ \Gamma function representation \f$
    With the given z, CBeta class calculates
    \f[
    {\rm CBeta}( \beta + 1, \alpha + z ) = 
    { \Gamma( \beta + 1 ) * \Gamma( \alpha + z ) \over 
    \Gamma( \alpha + \beta + 1 + z ) },
    \f]
    where \f$ \Gamma( z ) \f$ is a complex gamma function.

    \par Integral representation
    \f[
    B( m, n ) = \int_0^1 dx x^{m - 1} ( 1 - x )^{ n - 1 } =
    \Gamma( m ) \Gamma( n ) \over \Gamma( m + n )
    \f]
    \f[
    B( \beta + 1.0, \alpha + z ) = 
    \int_0^1 dx x^{ \beta } ( 1 - x )^{ \alpha + z - 1}
    \f]
    
  */
  class CBeta : public ComplexFunction, public IntegFunc {
  public:
    
    CBeta( double alpha, double beta );  //! a default constructor
    CBeta( const CBeta& cbeta );         //!< a copy constructor
    virtual ~CBeta();                    //<! destructor
    
    CBeta& operator=( const CBeta& cbeta ); //!< assignment operator
    
    /*!
      \param [in]  z is a complex variable
      \return  Beta( beta + 1, alpha + z ) in complex< double >
      \brief   this calculate the Beta( beta + 1, alpha + z ) at the given z.
      
      As described in the class description, this function calculates
      Beta( beta + 1, alpha + z )
      at the given z.
    */
    virtual std::complex< double > operator()( const std::complex< double >& z );
    
    double a() const { return a_; } //!< get a copy of a
    double b() const { return b_; } //!< get a copy of b
    
    double& a() { return a_; } //!< get a reference to a
    double& b() { return b_; } //!< get a reference to b
    
    //! set paraneters a and b
    void set( const double& a, const double& b );
    
    class Integrand : public ComplexFunction {
    public:
      
      virtual std::complex< double > 
      operator()( const std::complex< double >& z ) { 
	return std::complex< double >( 0.0, 0.0 );
      }
      
      virtual std::complex< double > operator()( const double& x );
      void m( const double& b );
      void n( const std::complex< double >& z );

      std::complex< double > n_;
      std::complex< double > m_;
    };
    
    enum METHOD {
      cgamma = 1, direct = 2
    };
    
    class DbKey {
    public:
      
      DbKey( const std::complex< double >& n = std::complex< double >( 0.0, 0.0 ),
	     const std::complex< double >& m = std::complex< double >( 0.0, 0.0 ) ) :
	n_( n ), m_( m ) {}
      
      DbKey( const DbKey& key ) : 
	n_( key.n_ ), m_( key.m_ ) {}
      
      virtual ~DbKey(){}

      DbKey& operator=( const DbKey& key ){
	if( this != &key ){
	  n_ = key.n_;
	  m_ = key.m_;
	}
	return *this;
      }
      
      bool operator==( const DbKey& key ) const {
	return ( m_ == key.m_ && n_ == key.n_ );
      }
      
      bool operator<( const DbKey& key ) const {
	if( m_ != key.m_ ) return ( m_ < key.m_ );
	return ( n_ < key.n_ );
      }
      
      bool operator>( const DbKey& key ) const {
	if( m_ != key.m_ ) return ( m_ > key.m_ );
	return ( n_ > key.n_ );
      }
      
      CKey n_;
      CKey m_;
    };
    
    static void atExitFunc();

    //! return \f$ x^{\alpha} ( 1.0 - x )^{\beta} \f$
    double x( const double& v );

  private:
    CGamma gamma_;
    Integrand integrand_;

    METHOD meth_;
    
    double a_;
    double b_;
    
    static DbKey key;
    static std::complex< double > data;
    static Utility::LookupTable< DbKey, std::complex< double > > cache;
    static bool registered;

  };
  
};
#endif //_CBeta_hh_
