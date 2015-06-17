//
// DSSFF.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Oct  3 11:25:30 2008 Yoshiyuki Miyachi
// Last update Fri Oct  3 11:42:56 2008 Yoshiyuki Miyachi
//

#ifndef   _PDFSET_DSSFF_HH_
# define  _PDFSET_DSSFF_HH_

#include <QCD/PdfBase.hh>
#include <Tranform/CBeta.hh>

namespace PDFSET {
  
  /*!
    \brief DDS Fragmentation function
    
    \f[
    D(z) = \eta z^{a} ( 1 - z )^{b} [ 1 + c ( 1 - z )^d ]
    \f]
    
  */
  class DSSFF : public QCD::PdfBase {
  public:
    DSSFF();
    virtual ~DSSFF();
    virtual std::complex< double > 
    operator()( const std::complex< double >& z );
    
    //!< calculate PDF value at the given x, q(x)
    virtual double x( const double& x );
    
    //!< update local stored variables using the present parameters
    virtual void update();
    
  private:
    
    Transform::CBeta b1_;              //!< complex gamma function object 1
    Transform::CBeta b2_;              //!< complex gamma function object 2

    std::complex< double > b10_; //!< b1_( 2.0 )
    std::complex< double > b20_; //!< b2_( 2.0 )
    std::complex< double > b00_; //!< b1_( 2.0 ) + c * b2_( 2.0 ) + 
  
    std::complex< double > A_;   //!< second moment
    double a_;  //!< alpha 
    double b_;  //!< beta
    double c_;  //!< gamma
    double d_;  //!< delta
    
  };
  
}

#endif	    /* !DSSFF_HH_ */
