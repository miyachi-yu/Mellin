/*!
  \file     Charge.hh
  \version  $Revision: 1.6 $
  \author   $Author: miyachi $
  \date     $Date: 2013/06/25 00:59:12 $
  \brief    Class definition of parton charge handling class
 */
#ifndef _Xsection_Charge_hh_
#define _Xsection_Charge_hh_

#include <QCD/Flavor.hh>
#include <QCD/ChargeBase.hh>

namespace Xsection {
  
  class CKernel;
  
  /*!
    \class  Charge
    \brief  parton charge class which takes care electric and weak charges
  */
  class Charge : public QCD::ChargeBase {
    
  public:
    
    enum TARGET { p = 1,
		  n = 2, 
		  d = 3,
		  any = 4 };
    
    static std::string tname( const TARGET& t );  //!< name of the target
    
    Charge( CKernel* coeff, const TARGET& n = p ); //!< a constructor
    virtual ~Charge();             //!< a destructor
    
    /*!
      \par [in] q is a parton type
      \brief  return charge at present scale for the given parton type
      
      This overwrite the abstract method for ChargeBase::operator()...
    */
    virtual double operator()( const QCD::Flavor::PARTON& q );    
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );
    
    //! get single charge
    virtual double single( const QCD::Flavor::PARTON& q );
    
    //! get sum of bare chage over available flavors
    virtual double sum();
    
    //! get target type
    TARGET target() const { return target_; }
    
  protected:
    CKernel* coeff_;
    
  private:
    TARGET target_;
    

  };
  
};

#endif //  _Xsection_Charge_hh_
