#ifndef _IntegXsec_Charge_hh_
#define _IntegXsec_Charge_hh_

#include <QCD/Flavor.hh>
#include <QCD/ChargeBase.hh>

namespace IntegXsec {
  
  class CKernel;
  
  class Charge : public QCD::ChargeBase {
    
  public:
    
    enum TARGET { p, n, d, any };
    static std::string tname( const TARGET& t );  //!< name of the target
    
    Charge( CKernel* coeff, const TARGET& n = p ); //!< a constructor
    virtual ~Charge();             //!< a destructor

    /*!
      \par [in] q is a parton type
      \brief  return charge at present scale for the given parton type
      
      This overwrite the abstract method for ChargeBase::operator()...
    */
    virtual double operator()( const QCD::Flavor::PARTON& q );    

    //! for W production
    virtual double operator()( const QCD::Flavor::PARTON& q1,
			       const QCD::Flavor::PARTON& q2 );
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );

    //! for W production
    virtual double bare( const QCD::Flavor::PARTON& q1, 
			 const QCD::Flavor::PARTON& q2 ){ return 0.0; }

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

#endif //  _IntegXsec_Charge_hh_
