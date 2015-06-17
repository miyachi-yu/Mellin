// $Id: DInverseMellinIntegrand.hh,v 1.6 2008/08/28 10:13:49 imazu Exp $
/*!
  \file   DInverseMellinIntegrand.hh
 */
#ifndef _DInverseMellinIntegrand_hh_
#define _DInverseMellinIntegrand_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include "Transformer.hh"
#include "DInverseMellinIntegInteg.hh"

namespace Transform {
  
  /*!
    \brief Dual Mellin Inversion Integrand class
  */
  class DInverseMellinIntegrand : public RealFunction, public Transformer {
    
  public:
    
    //! a constructor
    DInverseMellinIntegrand( ComplexFunction *g );
    //! a destructor
    virtual ~DInverseMellinIntegrand();
    
    //! set complex mellin integration parameters  
    void setParameter( const std::vector< double >& c, 
		       const std::vector< double >&  phi ){
      integinteg_.setParameter( c, phi ); 
    };
    
    //! set moment parameters  
    void setMomParam( const std::vector< double >& mom ){
      integinteg_.setMomParam( mom );
    };

    //! set a pointer to ComplexFunction
    DInverseMellinIntegrand& set( ComplexFunction *g );
    
    //! set a real variable in the integrand
    DInverseMellinIntegrand& set( const std::vector< double >& x, 
				  const std::vector< double >& z );
    
    //! return value of function at the given value
    virtual double operator() ( const double& r );
    
    //! get a reference of the integland
    DInverseMellinIntegInteg& integinteg() { return integinteg_; }
    
  private:
    
     DInverseMellinIntegInteg  integinteg_;
    
  };
};

#endif 
