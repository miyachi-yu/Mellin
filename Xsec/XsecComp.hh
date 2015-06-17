// $Id: XsecComp.hh,v 1.19 2009/04/16 10:31:32 imazu Exp $
/*!
  \file   XsecComp.hh
  \brief  Class definition of Xsection component handling
 */
#ifndef _Xsection_XsecComp_hh_
#define _Xsection_XsecComp_hh_

#include <iostream>

#include <QCD/Flavor.hh>
#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <Utility/Arguments.hh>
#include <Evolution/KernelBase.hh>
#include "XsecCompBase.hh"

namespace Xsection {
  
  class CKernelBase;
  
  /*!
    \class XsecComp
    \brief class to handle components
   */
  class XsecComp : public XsecCompBase {
    
  public:
    
    XsecComp();                                   //!< a default constructor
    virtual ~XsecComp();                          //!< a destructor
    
    double q2(){ return q2_; }                    //!< get Q2 value
    void setQ2( const double& q2 );               //!< set Q2 value
    void setOrder( const QCD::Scheme::ORDER& order );  //!< set order
    QCD::Scheme::ORDER order();                        //!< get order

    CKernelBase* coeff()
    { return dynamic_cast< XsecCompBase* >( this )->coeff(); } 
    //!< get a CKernelBase pointer  
    void coeff( CKernelBase* ck );         //!< set a CKernelBase pointer
    
    /*!
      return stored coefficient function. Before calling this method,
      one must set scale and moments properly.
    */
    std::vector< std::vector< std::complex< double > > >& vCoeff(); 
    std::vector< std::vector< std::complex< double > > >& vCoeff2(); 
    std::vector< std::vector< std::complex< double > > >& vCoeff3(); 
    
    /*!
      the scale for each evolution kernel will be updated.
      if there is any special treatment needed for derived class,
      this function must be overwritten.
    */
    virtual void update() ;
    
    //! get a rescale function for the hard scales
    std::vector< Transform::RealFunction* >& rescaleFcn() 
    { return rescaleFcn_; }
    
    /*!
      \param[in] kernel is a Evolution KernelBase pointer
      \param[in] hadron is a QCD::Flavor::HADRON object
      \return true on success
      \brief  insert Evolution kernel object
      
      Register kernel to this XsecComp object. 
      The hadron option will be used for ordaring in parton and 
      anti-parton. It does not distinguish hadron type, but does 
      distinguish particle and anit-particle, by cheking its sign.
      A default setting is for a particle.
      
    */
    bool insert( Evolution::KernelBase *kernel, 
		 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp );
    
    //! information dump method
    friend std::ostream& operator<<( std::ostream& os, XsecComp& xc );
    
  private:
    
    /*!
      hard scale information
    */
    double q2_;
    
    /*!
      this controls factorization scales for PDF and COFs.
    */
    std::vector< Transform::RealFunction* > rescaleFcn_; 
    
    //! to delete the object at the end... 
    std::vector< Transform::ComplexFunction* > localPdfs_;
    
  public:
    
    class SimpleRescale: public Transform::RealFunction {
    public:
      SimpleRescale( const double& ratio ) : Transform::RealFunction(), 
					     ratio_( ratio ){};
      ~SimpleRescale(){};
      
      virtual double operator() ( const double& x ){ return ratio_; }
      
    private:
      double ratio_;
      
    };
    
  };
  
};

#endif //  _Xsection_XsecComp_hh_
