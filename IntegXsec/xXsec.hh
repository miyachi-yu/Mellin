#ifndef _IntegXsec_xXsec_hh_
#define _IntegXsec_xXsec_hh_

#include <iostream>

#include <Tranform/ComplexFunction.hh>
#include <Tranform/IntegFunc.hh>
#include "XsecBase.hh"

namespace IntegXsec {
  
  class xXsec : public XsecBase, public Transform::IntegFunc, 
		public Transform::ComplexFunction {    
  public:
    xXsec( const int& low = 4, const int& high = 6,
	   const double& precision = 1.0E-4 ) throw( int );
    virtual ~xXsec();

    virtual double operator()( const double& x, const double& z )
    { throw( 1 ); }    
    virtual double operator()( const double& x );

    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m )
    { throw( 1 ); }
    virtual std::complex< double > operator()
    ( const std::complex< double >& n );
    // for partial integral

    //! set moments n and m
    virtual void setMoment( const std::complex< double >& N );
    
    virtual void setMoment( const std::complex< double >& N, 
			    const std::complex< double >& M ){ throw( 1 ); }
    
    void setloginteg( const bool& logint );
    void setGngrid( const int& ngrid );

    virtual void setparams( const double& x );

    void setflavors( const int& i, const int& j ){ i_ = i; j_ = j; }

  protected:

    virtual void update(){};
    
    virtual double charge( const int& i, const int& j );   
    // get charge from CKernel for appropriate flavor

    virtual std::complex< double > getCvalue( const int& i, const int& j );
    // get proper CKernel calc result for the flavor

    virtual bool setActflav( const int& i, const int& j );
    // get proper & efficient flavors for CKernel calc ( acti_, actj )

  private:

    void initialization( const int& low = 4, const int& high = 6, 
                         const double& precision = 1.0E-4 );

    int i_;
    int j_; // for partial integ

  };
  
};

#endif //  _IntegXsec_xXsec_hh_
