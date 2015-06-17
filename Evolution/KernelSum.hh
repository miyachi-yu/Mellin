// $Id: KernelSum.hh,v 1.8 2009/02/17 04:50:14 miyachi Exp $
/*!

  \brief Class definition of Evolution based on multiple parameterization

*/

#ifndef _Evolution_KERNELSUM_HH_
#define _Evolution_KERNELSUM_HH_

#include "Kernel.hh"
#include <QCD/PdfParametrization.hh>
#include <vector>

namespace Evolution {
  
  /*!
    \brief class for handling multiple initial parameterizations
    
    This class solves DGLAP equation with initial PDF based on 
    more than one parameterization, such as 
    \f$ D^{h+} = D^{\pi+} + D^{K+} + D^{rest+} \f$.
    
  */
  class KernelSum : public Kernel, public std::vector< Kernel* > {
  public:
    
    KernelSum();                    //!< a default constructor
    virtual ~KernelSum();           //!< a destructor
    
    /*!
      \brief   add Kernel object
      
      Since KernelSum inherits vector container, one can 
      also use push_back, or other methods to add the Kernel pointer,
      but please take this method to register the Kernel object.
      
      This method also check the initial scale and other information....
      
    */
    virtual void add( Kernel* k ) throw( int ) ;
    
    /*!
      \brief  a QCD::PdfParameterization collection class
    */
    class ParamSum : public QCD::PdfParametrization, 
		     public std::vector< QCD::PdfParametrization* > {
    public:
      
      ParamSum();                     //!< a default constructor
      virtual ~ParamSum();            //!< a destructor
      
      //! get current calculated moment for paton q
      virtual std::complex< double > q( const QCD::Flavor::PARTON& q );

      //! get current calculated moment for paton combination q
      virtual std::complex< double > q( const QCD::Flavor::TYPE&   q );
      
      //! add PdfParametrization object
      virtual void add( QCD::PdfParametrization *pdf ) throw( int );
      
      virtual void setN( const std::complex< double >& n );  //!< set complex n

    private:
      
    };
    
  private:
    
    ParamSum params_;
    
  };
  
};

#endif	    /* !KERNELSUM_HH_ */
