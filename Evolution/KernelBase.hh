// $Id: KernelBase.hh,v 1.8 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file     KernelBase.hh
  \brief    Class definition of Evolution Kernel base class
 */
#ifndef _KernelBase_hh_
#define _KernelBase_hh_

#include <iostream>
#include <complex>

#include <QCD/Flavor.hh>
#include <QCD/Alpha.hh>
#include <QCD/PdfParametrization.hh>
#include <Tranform/DataCache.hh>

namespace Evolution {

  /*!
    \brief Base class for evolution Kernel
    
    In order to make transparent machanism between PDFLIB and
    DGLAP evolution inside this framework, Kernel base class is
    introduced.
    
  */
  class KernelBase {
  public:
    
    KernelBase();                 //!< a default constructor
    virtual ~KernelBase();        //!< a destructor
    
    /*!
      \param[in] q2 is a QCD hard scale
      \brief  set current value of Q2 with the given q2
    */
    virtual void setQ2( const double& q2 ) = 0 ; 
    
    /*!
      \param[in] n is a moment value
      \brief set current moment value of N with the given n
    */
    virtual void setN( const std::complex< double >& n ) = 0 ;
    
    //! get current Q2 value
    virtual double Q2() = 0;
    
    //! get current N value
    virtual std::complex< double > n() { return n_; }
    
    /*!
      \param[in] q is a Flavor::PARTON
      \return moment value for the give parton q
      \brief it returns present N-th moment calculated for parton q
    */
    virtual std::complex< double > q( const QCD::Flavor::PARTON& q ) = 0 ; 
  
    /*!
      \param[in] q is a QCD::FlavorTYPE
      \return moment value for the give parton q
      \brief it returns present N-th moment calculated for parton q
    */
    virtual std::complex< double > q( const QCD::Flavor::TYPE&   q ) = 0 ;
    
    //! get a reference to Alpha object
    virtual QCD::Alpha& alpha() { return alpha_; }
    
    //! set pQCD order
    virtual void setOrder( const QCD::Scheme::ORDER& order ); 
    
   //! get a pointer of initial distribution object ( for error calc )
    virtual QCD::PdfParametrization* initialDistribution() = 0;

    //! clear cached data in memory
    virtual void clearCache() { cache_.clear() ; } 
    
    //! output information into the given ostream object
    friend std::ostream& operator<<( std::ostream& os, KernelBase& k );

  protected:
    
    //! get reference of memory cache
    Transform::DataCache& cache() { return cache_; } 
    
    //! get reference of data buffer for memory cache
    std::vector< std::complex< double > >& cdata() { return cdata_; }
    
    //! set moment value, restricted for inherited classes
    void updateN( const std::complex< double >& n ) { n_ = n ; }
    
  private:
    QCD::Alpha      alpha_;                       //!< strong coupling alpha_s
    Transform::DataCache  cache_;                 //!< memory cache
    std::vector< std::complex< double > > cdata_; //!< data for memory cache
    std::complex< double > n_;                    //!< current moment value

    bool calcerr_;  // judge for moment error calc 
    
  };
};

#endif // _KernelBase_hh_
