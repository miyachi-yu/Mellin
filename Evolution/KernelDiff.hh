// $Id: KernelDiff.hh,v 1.1 2009/02/13 12:23:40 miyachi Exp $
/*!

  \brief Class definition of Evolution based on multiple parameterization

*/

#ifndef _Evolution_KERNELDIFF_HH_
#define _Evolution_KERNELDIFF_HH_

#include "Kernel.hh"
#include <QCD/PdfParametrization.hh>

namespace Evolution {
  
  /*!
    \brief class for handling multiple initial parameterizations
    
    This class solves DGLAP equation with initial PDF based on 
    more than one parameterization, such as 
    \f$ D^{h+} = D^{\pi+} + D^{K+} + D^{rest+} \f$.
    
  */
  class KernelDiff : public Kernel {
  public:
    
    /*! 
      \brief  a constructor with two Kernel object
     */
    KernelDiff( Kernel* targe, Kernel* base ) throw( int ) ;
    virtual ~KernelDiff();           //!< a destructor
    
  private:
    
    /*!
      \brief  a QCD::PdfParameterization difference
      
      This class will calculate differece bwetween two given parameterization

    */
    class ParamDiff : public QCD::PdfParametrization {
    public:
      

      ParamDiff();                //!< a default constructor
      virtual ~ParamDiff();       //!< a destructor
      
      //! get current calculated moment for paton q
      virtual std::complex< double > q( const QCD::Flavor::PARTON& q );
      
      //! get current calculated moment for paton combination q
      virtual std::complex< double > q( const QCD::Flavor::TYPE&   q );
      
      virtual void setN( const std::complex< double >& n );  //!< set complex n
      
      void set( PdfParametrization* target,
		PdfParametrization* base ) throw( int );
      
    private:
      
      PdfParametrization* p1_; //!< target pdf
      PdfParametrization* p2_; //!< reference pdf 
      
    };
    
    ParamDiff params_;
    
  };
  
};

#endif	    /* !KERNELSUM_HH_ */
