/*!
  \file     Charge.hh
  \version  $Revision: 1.1 $
  \author   $Author: imazu $
  \date     $Date: 2008/08/27 17:16:50 $
  \brief    Class definition of parton charge handling class
 */
#ifndef _IntegFragXsec_Charge_hh_
#define _IntegFragXsec_Charge_hh_

#include <Evolution/Kernel.hh>
#include <QCD/Flavor.hh>
#include <IntegXsec/Charge.hh>

namespace IntegFragXsec {
  
  /*!
    \class  Charge
    \brief  parton charge class which takes care electric and weak charges
  */
  class Charge : public IntegXsec::Charge {
    
  public:
    
    Charge( IntegXsec::CKernel* coeff ); //!< a constructor
    virtual ~Charge();                  //!< a destructor
    
    /*!
      \par [in] q is a parton type
      \brief  return charge at present scale for the given parton type
      
      This overwrite the abstract method for ChargeBase::operator()...
    */
    virtual double operator()( const QCD::Flavor::PARTON& q );    
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );
    
  public:
    
    /*    Physics constans     */
    static const double EVC[ 3 ];  //!< A vector current charge
    static const double WAC[ 3 ];  //!< Z axial  current charge

    static double WVC[ 3 ];  //!< Z vector current charge//
    static double ELEM[ 3 ]; 
    static double INTF[ 3 ]; 
    static double WEAK[ 3 ];
    
    static const double SWVA;    //!< Weinberg Angles
    static const double WVAN;    //!< arcsin( sqrt( SWVA ) )
    
    static const double sinWVAN;
    static const double cosWVAN;
    
    static const double MZ;      //!< Z mass and decay width
    static const double ZW;      //!< Z mass and decay width
    
    static const double MZ2;
    static const double ZW2;
    static const double MZZW;
    
    static double ChIN_( const double& evc, const double& wac );
    
    static bool init;
    static bool initialize();

  private:
    
    // for Z vector current charge //
    
    // decomposition of total effective charge //
    double elem( const int& EUD );
    double intf( const int& EUD );
    double weak( const int& EUD );
    
    // total effective charge //
    double effCh( const int& EUD );
    
    double rho_1();
    double rho_2();
    
  };
  
};

#endif //  _IntegFragXsec_Charge_hh_
