// $Id: Evolution.hh,v 1.7 2008/11/13 09:06:21 miyachi Exp $
/*!
  \file    Evolution.hh
  \version $Revision: 1.7 $
  \date    $Date: 2008/11/13 09:06:21 $
  \author  $Author: miyachi $
  \brief   Definition of QCD Evolution handling class for polDIS
 */

#ifndef _polPDF_Evolution_hh_
#define _polPDF_Evolution_hh_

#include <string>
#include <Utility/Arguments.hh>
#include <Evolution/Kernel.hh>

/*!
  Any class related to polaried DIS implementation should be 
  defined inside polPDF namespace.
  Which avoid possible conflicts among physics case implementation
*/
namespace polPDF {
  
  /*!
    \class Evolution Evolution.hh "polPDF/Evolution.hh"
  */
  class Evo : public Evolution::Kernel {
  public:

    //! a constructor
    Evo( Utility::Arguments& args, 
	 const std::string& option = "" ) throw( int ) ; 

    virtual ~Evo();                       //!< a destructor
    
    //! get option for polPDF::Evo
    static std::vector< std::string > options();
    
    //! create singleton polPDF::Evo object
    static Evo* instance() throw( int );
    
  private:
    
    static Evo *pdf_;
    static void atExit();

  };
  
}

#endif //  _polPDF_Evolution_hh_
