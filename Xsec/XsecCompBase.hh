#ifndef _Xsection_XsecCompBase_hh_
#define _Xsection_XsecCompBase_hh_

#include <iostream>

#include <QCD/Flavor.hh>
#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <Utility/Arguments.hh>
#include <Evolution/KernelBase.hh>

namespace Xsection {
  
  class CKernelBase;
  
  /*!
    \class XsecCompBase
    \brief class to handle components
   */
  class XsecCompBase {
    
  public:
    
    XsecCompBase();                     //!< a default constructor
    virtual ~XsecCompBase();            //!< a destructor
    
    virtual void setQ2( const double& q2 ) = 0;   //!< set Q2 value
    virtual double q2() = 0;                      //!< get Q2 value
    
    CKernelBase* coeff(){ return coeff_; }  //!< get a CKernelBase pointer
    virtual void coeff( CKernelBase* ck ) = 0;          
    //!< set a CKernelBase pointer
    
    /*!
      get PDF/FF collection
    */
    std::vector< std::vector< Transform::ComplexFunction* > >& vpdf()
    { return vpdf_; }
  
    //! get a vector of KernelBase object pointer  
    std::vector< Evolution::KernelBase* >& vevo() { return vevo_; }

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
    virtual bool insert( Evolution::KernelBase *kernel, 
			 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp )
      = 0;
    
  protected:
    
    /*!
      for PDF/FF and thier evolution
    */
    std::vector< Evolution::KernelBase* > vevo_;

    //! collection of PDF objects
    std::vector< std::vector< Transform::ComplexFunction* > > vpdf_;    

    /*!
      for coefficient function handling
    */
    CKernelBase* coeff_;
    
  };
  
};

#endif //  _Xsection_XsecCompBase_hh_
