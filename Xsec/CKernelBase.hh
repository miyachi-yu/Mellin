#ifndef _Xsec_CKernelBase_hh_
#define _Xsec_CKernelBase_hh_

#include <iostream>

#include <Tranform/ComplexFunction.hh>
#include <Utility/Arguments.hh>
#include <QCD/ChargeBase.hh>
#include <QCD/Alpha.hh>
#include <QCD/Scheme.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>

namespace Xsection {
  
  class Wilsons;

  /*!
    \class CKernl
    \brief This class manage Coefficient function ........
  */
  class CKernelBase {
    
  public:
    
    CKernelBase();    //!< a constructor
    virtual ~CKernelBase();                      //!< a destructor

    virtual void setQ2( const double& q2 ) = 0;  //!< set hard scale Q2
    virtual double q2() = 0;                      //!< get Q2 value
    virtual double nf() = 0;                     //!< get number of flavors
    virtual QCD::ChargeBase* charge() = 0;       //!< get ChargeBase pointer
    virtual void constNf( const bool& frag ) = 0; 

    virtual QCD::Alpha& alpha() = 0;    //!< get a reference of Alpha
    
    //! set moments n and m
    virtual void setMoment( const std::complex< double >& N ) = 0;
    
    virtual void setMoment( const std::complex< double >& N, 
			    const std::complex< double >& M ) = 0;
    
    //! get current value of coefficients
    std::vector< std::vector< std::complex< double > > >& vCoeff() 
    { return vcoeff_; }
    std::vector< std::vector< std::complex< double > > >& vCoeff2() 
    { return vcoeff2_; }
    std::vector< std::vector< std::complex< double > > >& vCoeff3() 
    { return vcoeff3_; }

    std::complex< double > getN(){ return N_; }  //!< get moment 1
    std::complex< double > getM(){ return M_; }  //!< get momnet 2
    void setN( std::complex< double > n ){ N_ = n; }  //!< set moment 1
    void setM( std::complex< double > m ){ M_ = m; }  //!< set momnet 2

    void  dimension( int d ) { dim_ = d; }
    int dimension() { return dim_; }

    void wilsons( Xsection::Wilsons* wc ) { cf_ = wc; } 
    //!< set wilson coefficients
    Xsection::Wilsons* wilsons() { return cf_; }

    virtual void wilsons2( Xsection::Wilsons* wc ){} 
    //!< set 2nd wilson coefficients 
    virtual Xsection::Wilsons* wilsons2() { return NULL; }

  protected:

    int    dim_;  
    std::complex< double > N_;    //!< moment 1
    std::complex< double > M_;    //!< momnet 2
    
    /*!
      calculated wilson coefficients using Wilsons object will be 
      stored in this member variable.
    */
    std::vector< std::vector< std::complex< double > > > vcoeff_;
    std::vector< std::vector< std::complex< double > > > vcoeff2_;
    std::vector< std::vector< std::complex< double > > > vcoeff3_;

    Xsection::Wilsons*     cf_;   //!< wilson coefficients collection

  private:
    
  };

};

#endif //  _Xsec_CKernelBase_hh_
