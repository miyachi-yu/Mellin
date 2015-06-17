#ifndef _IntegXsec_xzXsec_hh_
#define _IntegXsec_xzXsec_hh_

#include <iostream>
#include <Tranform/ComplexFunction.hh>
#include <Tranform/Transformer.hh>
#include "xzXsecInteg.hh"
#include "XsecBase.hh"

namespace IntegXsec {
  
  class xzXsec : public XsecBase, public Transform::Transformer, 
		 public Transform::ComplexFunction {
  public:
    xzXsec( const int& low = 4, const int& high = 6,
	    const double& precision = 1.0E-4 ) throw( int );
    virtual ~xzXsec();

    virtual double operator()( const double& x, const double& z );
    virtual double operator()( const double& x ){ throw( 1 ); }

    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m );
    virtual std::complex< double > operator()
    ( const std::complex< double >& n ){ throw( 1 ); }
    // for partial integral

    //! set moments n and m
    virtual void setMoment( const std::complex< double >& N ){ throw( 1 ); };
    
    virtual void setMoment( const std::complex< double >& N, 
			    const std::complex< double >& M );

    void setloginteg( const bool& logint ){ xzXIntg_.setloginteg( logint ); }
    void setGngrid( const int& ngrid ){ xzXIntg_.setGngrid( ngrid ); }

    xzXsecInteg& xzXIntg(){ return xzXIntg_; }

    virtual void setparams( const double& x, const double& z );

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
    xzXsecInteg xzXIntg_;

    int i_;
    int j_; // for partial integ

  };
  
};

#endif //  _IntegXsec_xzXsec_hh_
