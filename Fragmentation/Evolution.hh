// $Id: Evolution.hh,v 1.11 2013/06/25 00:59:12 miyachi Exp $
/*!
  \file  Evolution.hh
  \brief Definition of QCD Evolution handling class for Fragmentation
 */

#ifndef _Fragmentation_Evolution_hh_
#define _Fragmentation_Evolution_hh_

#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Evolution/Kernel.hh>

/*!
  Any class related to Fragmentation implementation should be 
  defined inside Fragmentation namespace.
  Which avoid possible conflicts among physics case implementation
*/
namespace Fragmentation {
  
  /*!
    \brief Evolution Kernel for Fragmentation fucntions
   */
  class Evo : public Evolution::Kernel {
    
  public:
    
    /*!
      \param [in] args is an Arguments object
      \param [in] hadron is HADRON type ( PIp, PIm, KAp, and KAm )
      \brief constructor with argument for the given HADRON type
      
      \par
      It constructs DGLAP evolution for a fragmentation function.
      QCD::Falvor::HADRON is defined for both particle and anti-particle,
      but the fragmentation function does not distinguish them.

      \par
      If PIp or PIm are given, it try to construct an object reading
      configuration file which is defined with --FF option.
      If KAp or KAm are given --FFK option will be used instead.

    */
    Evo( Utility::Arguments& args, 
	 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp,
	 const std::string& option = "" ) throw( int ) ;
    
    /*!
      \param [in] args is a reference to Arguments object
      \param [in] init is a pointer to PdfParameterization
      \param [in] hadron is a hadron type
      \brief constructor with PdfParameterization
    */
    Evo( Utility::Arguments& args, 
	 QCD::PdfParametrization* init,
	 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp ) throw( int ) ;
    
    virtual ~Evo();  //!< a destructor
    
    QCD::Flavor::HADRON hadron() const { return hadron_; } //!< get hadron type
    
    static std::vector< std::string > 
    options( const QCD::Flavor::HADRON& hadron );
    
    static std::string base( const QCD::Flavor::HADRON& hadron );
    
    static Evo* instance( const QCD::Flavor::HADRON& h ) throw( int );
    
    static QCD::PdfParametrization*
    create_pdf( const QCD::Flavor::HADRON& hadron,
		const std::string& opt ) throw( int ) ;
    
  private:
    
    QCD::Flavor::HADRON hadron_;
    
    void initialization( QCD::PdfParametrization *init ) throw( int );
    
    static Evo* pion_;
    static Evo* kaon_;
    static Evo* proton_;
    static Evo* rest_;
    
    static Evo* init_ff( const QCD::Flavor::HADRON& h ) throw( int );
    static void atExit();
    
  };

}

#endif //  _Fragmentation_Evolution_hh_
