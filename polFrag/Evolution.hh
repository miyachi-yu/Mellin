// $Id: Evolution.hh,v 1.4 2008/06/09 08:19:00 miyachi Exp $
/*!
  \file  Evolution.hh
  \brief Definition of QCD Evolution handling class for polFrag
 */

#ifndef _polFrag_Evolution_hh_
#define _polFrag_Evolution_hh_

#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>

/*!
  Any class related to polaried DIS implementation should be 
  defined inside polFrag namespace.
  Which avoid possible conflicts among physics case implementation
*/
namespace polFrag {
  
  /*!
    \class Evolution Evolution.hh "polFrag/Evolution.hh"
  */
  class Evo : public Evolution::Kernel {
  public:

    /*!
      \param[in] args is a reference to an argument object 
      \param[in] hadron is QCD::Flavor::HADRON
      \brief constructor for the given hadron type
     */
    Evo( Utility::Arguments& args,
	 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp )
      throw( int ) ;
    
    virtual ~Evo();                      //!< a destructor
    
  private:
    QCD::Flavor::HADRON hadron_;         //!< hadron type 

  };
  
};

#endif //  _polFrag_Evolution_hh_
