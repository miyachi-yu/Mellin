// $Id: Xsec.hh,v 1.20 2009/04/16 10:31:32 imazu Exp $
/*!
  \file    Xsec.hh
  \version $Revision: 1.20 $
  \date    $Date: 2009/04/16 10:31:32 $
  \author  $Author: imazu $
  \brief   class definition of cross section calculator
*/
#ifndef _Xsec_Xsec_hh_
#define _Xsec_Xsec_hh_

#include <iostream>

#include <Utility/Arguments.hh>
#include <Tranform/ComplexFunction.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>
#include <QCD/Scheme.hh>

#include "Charge.hh"

namespace Xsection {
  
  class XsecComp;
  class PDF_SW;

  /*!
    \class Xsec   Xsec.hh  Xsec/Xsec.hh
    \brief  cross section calculation class
    
    This is class to calculate cross section for various scattering processes.
    In order to use this class, one has to prepare several classes derived from
    XsecComp and Wilsons, at least.
    
    If it is needed, you may inherit CKernel class, as well.
    
  */
  class Xsec : public Transform::ComplexFunction {
    
  public:
    
    Xsec() throw( int ); //!< a default constructor
    virtual ~Xsec();     //!< a destructor
    
    /*!
      \param[in] n is a complev< double >
      \param[in] m is a complev< double >
      \return value of cross section
      \brief return xsec( m, n )
    */
    virtual std::complex< double > 
    operator()( const std::complex< double >& n, 
		const std::complex< double >& m );
    
    /*!
     */
    virtual std::complex< double > 
    operator()( const std::complex< double >& z );
    
    //for individual contribution//
    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const QCD::Flavor::PARTON& q );
    
    //! set a pointer to XsecComp object
    void xsecComp( XsecComp* XC ) { XsecComp_ = XC; } 
    
    //! get a pointer to XsecComp object
    XsecComp* xsecComp() { return XsecComp_; }
    
    void setQ2( const double& q2 ); //!< set hard scale q2
    double q2();                    //!< get hard scale q2
    
    void setOrder( const QCD::Scheme::ORDER& order ); //!< set order of pQCD
    QCD::Scheme::ORDER order();                  //!< get order
    
    virtual void update();
    
    friend std::ostream& operator<<( std::ostream& os, Xsec& xs );
    
    void setIndiv( const QCD::Flavor::PARTON& q ){ indiv_ = true; qtype_ = q; };
    
    //! give false if xsection is needed.
    void setIndiv( const bool& status = true ) { indiv_ = status; }

    //! clear cache ...
    void clearCache();
    
    Transform::DataCache& cache() { return cache_; }
    Transform::DataCache2& cache2() { return cache2_; }
    
    void enableCache() { cacheDisable_ = false; }
    void disableCache() { cacheDisable_ = true; }
    
  protected:
    
    //! set global scale factor
    void globalScale( const double& v ) { gscale_ = v; } 
    double getgscale(){ return gscale_ ; } 
    
    //!< get a reference to PDF collection
    std::vector< std::vector< Transform::ComplexFunction* > >& vpdf();
    XsecComp *XsecComp_;                //!< a pointer to XsecComp object
    
    
    /*!
      \param[in]  n is a moment value
      \param[in]  q2 is a hard scale Q^2
      \param[out] val is a reference of valiable to be reitive from cache
      \return true if there is data in cache.
      \brief find data at ( n, Q2 ) in memory cache.
    */
    virtual bool retrive( const std::complex< double >& n, 
			  const double& q2,
			  std::complex< double >& val );
    
    /*!
      \param[in]  n is a moment value
      \param[in]  m is a moment value
      \param[in]  q2 is a hard scale Q^2
      \param[out] val is a reference of valiable to be reitive from cache
      \return true if there is data in cache.
      \brief find data at ( n, m, Q2 ) in memory cache.
    */
    virtual bool retrive( const std::complex< double >& n, 
			  const std::complex< double >& m, 
			  const double& q2,
			  std::complex< double >& val );
    
    /*!
      \param[in]  n is a moment value
      \param[in]  q2 is a hard scale Q^2
      \param[in]  val is a data to be cached
      \brief inseart val to cache
    */
    virtual void insert( const std::complex< double >& n, 
			 const double& q2,
			 std::complex< double >& val );
    
    /*!
      \param[in]  n is a moment value
      \param[in]  m is a moment value
      \param[in]  q2 is a hard scale Q^2
      \param[in]  val is a data to be cached
      \brief inseart val to cache
    */
    virtual void insert( const std::complex< double >& n, 
			 const std::complex< double >& m, 
			 const double& q2,
			 std::complex< double >& val );
    
    
    void setPDFswitch( const Xsection::Charge::TARGET& n );
    PDF_SW* PDF_;

  private:
    
    double gscale_;                      //!< global scale ( defalut: 1.0 )
    static int maxCacheSize_;            //!< max cache size
    
    // ----------------------------------------------------------------- //
    //                          Memory Cache                             //
    // ----------------------------------------------------------------- //
    Transform::DataCache  cache_;
    Transform::DataCache2 cache2_;
    
    std::vector< std::complex< double > > cdata_;

    bool cacheDisable_;
    
    // ----------------------------------------------------------------- //
    //                 Accecc to individual componenent....              //
    // ----------------------------------------------------------------- //
    bool indiv_;
    QCD::Flavor::PARTON qtype_;
    
  };
  
};

#endif //  _Xsec_Xsec_hh_
