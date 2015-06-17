#ifndef _Transform_PartialInteg_hh_
#define _Transform_PartialInteg_hh_

#include "RealFunction.hh"
#include "IntegFunc.hh"

namespace Transform{

  class PartialInteg: public RealFunction, 
		      public IntegFunc{
  public:
    PartialInteg( const int& low = 4, const int& high = 6, 
		  const int& ngrid = 2,
		  const double& precision = 1.0E-4 ) throw( int );
    PartialInteg( Integration* integration );
    virtual ~PartialInteg(){};
    
    virtual double operator()( const double& x );

    virtual std::complex< double > operator()
      ( const std::complex< double >& n );

    void setPIntegParam( const int& i )
    { getCFunc()->setPIP( i ); }

    void setfunc( RealFunction* g )
    { getCFunc()->setfunc( g ); }
    void setfunc( ComplexFunction* f )
    { getCFunc()->setfunc( f ); }

    void setPIntegRange( const double& zmin, const double& zmax )
    { this->min() = zmin; this->max() = zmax; }
    double& min(){ return zmin_; }
    double& max(){ return zmax_; }

    void setloginteg( const bool& logint );

  public: // public for IntegSIDIS //
    class CFunc: public RealFunction, public ComplexFunction{
    public:
      CFunc(): i_( 2 ), x_( 0.0 ), n_( std::complex< double >( 0.0, 0.0 ) ), 
	       g_( NULL ), f_( NULL ) {};
      virtual ~CFunc(){};

      void setfunc( RealFunction* g ){ g_ = g; }
      void setfunc( ComplexFunction* f ){ f_ = f; }

      RealFunction* getfunc(){ return g_; }
      ComplexFunction* getnfunc(){ return f_; }

      CFunc& setx( const double& x ){ x_ = x; return *this; }
      double& getx(){ return x_; }

      CFunc& setn( const std::complex< double >& n ){ n_ = n; return *this; }
      std::complex< double >& getn(){ return n_; }

      void setPIP( const int& i )
      { if ( i != 1 && i != 2 ) throw( 1 ); i_ = i; }

      int& getPIP(){ return i_; }

      virtual double operator()( const double& z )
      { if( i_ == 1 ) return (*g_)( z, x_ ); 
      if( i_ == 2 ) return (*g_)( x_, z ); }

      virtual std::complex< double > operator()
	( const std::complex< double >& m )
      { if( i_ == 1 ) return (*f_)( m, n_ ); 
      if( i_ == 2 ) return (*f_)( n_, m ); }

    private:
      int i_; 
      double x_;
      std::complex< double > n_;
      RealFunction* g_;
      ComplexFunction* f_;

    };

  private:
    CFunc cfunc_;
    double zmin_; double zmax_;

  public:
    virtual PartialInteg::CFunc* getCFunc(){ return &cfunc_; }
  };
};

#endif //  _Transform_PartialInteg_hh_
