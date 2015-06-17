#ifndef _IntegXsec_xzXsecInteg_hh_
#define _IntegXsec_xzXsecInteg_hh_

#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <Tranform/IntegFunc.hh>

#include <iostream>

namespace IntegXsec{

  class XsecBase;

  class xzXsecInteg: public Transform::RealFunction, 
		     public Transform::ComplexFunction, 
		     public Transform::IntegFunc{
  public:
    xzXsecInteg( XsecBase* xzx, const int& low = 4, const int& high = 6,
		 const double& precision = 1.0E-4 ) throw( int );
    virtual ~xzXsecInteg(){};
    
    virtual double operator()( const double& x );

    virtual std::complex< double > operator()
      ( const std::complex< double >& n );

    void setloginteg( const bool& logint );
    void setGngrid( const int& ngrid );
    
  private:
    
    class CFunc: public Transform::RealFunction, 
		 public Transform::ComplexFunction {
    public:
      CFunc( XsecBase* xzx ): xzXsec_( xzx ), x_( 0.0 ), 
			      n_( std::complex< double >( 0.0, 0.0 ) ){};
      virtual ~CFunc(){};

      CFunc& setx( const double& x ){ x_ = x; return *this; }
      CFunc& setn( const std::complex< double >& n ){ n_ = n; return *this; }

      virtual double operator()( const double& z );

      virtual std::complex< double > operator()
	( const std::complex< double >& m );

    private:
      XsecBase* xzXsec_;
      double x_;
      std::complex< double > n_;
    };

    CFunc cfunc_;
    void initialization( const int& low = 4, const int& high = 6, 
			 const double& precision = 1.0E-4 );
  };
};

#endif //  _IntegXsec_xzXsecInteg_hh_
