// $Id: DInverseMellinIntegInteg.hh,v 1.7 2008/08/28 10:13:49 imazu Exp $
/*!
  \file   DInverseMellinIntegInteg.hh
  \brief  class definition of basic integrand for Double Mellin Inversion
 */
#ifndef _DInverseMellinIntegInteg_hh_
#define _DInverseMellinIntegInteg_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include <vector>

namespace Transform {
  
  class FactorFunc: public ComplexFunction {
  public:
    FactorFunc(): x_( 0 ), mom_( 0.0 ) {};
    virtual ~FactorFunc() {};
    
    virtual std::complex< double > 
    operator()( const std::complex< double >& zc ) = 0;
    
    //! set parameters
    FactorFunc* set( const std::vector< double >& x, 
		     const double& mom = 0.0 );

    //!  get x
    std::vector< double > getx_() const { return x_; };

    //! get mom
    double getmom_() const { return mom_; };

  protected:
    
    std::vector< double > x_;
    double mom_;
  };
  
  
  class DInverseMellinIntegInteg : public RealFunction {
    
  public:
    
    DInverseMellinIntegInteg( ComplexFunction *g ); //!< a constructor
    virtual ~DInverseMellinIntegInteg();            //!< a destructor
    
    void setParameter( const std::vector< double >& c, 
		       const std::vector< double >&  phi );
    
    void setMomParam( const std::vector< double >& mom ){ mom_ = mom; };
    
    DInverseMellinIntegInteg& set( ComplexFunction *g );
    
    DInverseMellinIntegInteg& set( const std::vector< double >& x, 
				   const std::vector< double >& z );

    /*
    void set( const std::vector< double >& x, 
				   const std::vector< double >& z );
    */

    DInverseMellinIntegInteg& setR( const double& rr );
    
    virtual double operator() ( const double& r );
    
    class Fac_org: public FactorFunc {
    public:
      Fac_org():FactorFunc(){ x_.resize( 1, 0.0 ); };
      //      Fac_org(){};
      virtual ~Fac_org(){};
      virtual std::complex< double > operator()
	( const std::complex< double >& zc );
    };
    
    class Fac_mom: public FactorFunc{
    public:
      Fac_mom():FactorFunc(){ x_.resize( 2, 0.0 ); };
      //      Fac_mom(){};
      virtual ~Fac_mom(){};
      virtual std::complex< double > operator()
	( const std::complex< double >& zc );
    };
    
  private:
    
    ComplexFunction* g_;
    std::vector< double > c_;
    std::vector< std::complex< double > > e_;
    std::vector< std::complex< double > > ce_;
    std::vector< std::complex< double > > zc_;
    std::vector< std::complex< double > > czc_;
    std::vector< double > mom_;
    std::vector< FactorFunc* > Fac_; 
    std::vector< FactorFunc* > FacORG_; 
    std::vector< std::complex< double > > temp_;
  };
  
};

#endif 
