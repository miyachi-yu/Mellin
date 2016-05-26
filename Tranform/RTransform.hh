#ifndef _RTransform_hh_
#define _RTransform_hh_

#include "RealFunction.hh"
#include "Transformer.hh"
#include "Integration.hh"

namespace Transform {
  /*!
    Transformer for RealFunction.
   */
  class RTransform : public RealFunction,
		     public Transformer {
  public:
    
    class Kernel : public RealFunction {
    public:
      virtual ~Kernel(){}
      double t() const { return t_; }
      RealFunction& set( const double& t ){ t_ = t; return *this; }
      virtual double eval( const double& x ) = 0;
      virtual double operator()( const double& x );
      void integrand( RealFunction *f ) { f_ = f; }
      
    private:
      double t_;
      RealFunction *f_;      // integrand
    };
    
    RTransform();
    virtual ~RTransform();
    
    virtual double operator()( const double& t );
    
    void kernel( Kernel *K ) { K_ = K; }
    void integrand( RealFunction *f );
    void nLeg( const int& n1, const int& n2 );
    void precision( const double& p );
    void nGrid( const int& n );
    
  private:
    Integration *integ_;  // Integration method
    Kernel      *K_;      // Transform Kernel
  };
};

#endif // _RTransform_hh_
