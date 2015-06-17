// $Id: Evolution.hh,v 1.5 2008/11/13 09:06:22 miyachi Exp $
/*!
  \file    Evolution.hh
  \version $Revision: 1.5 $
  \date    $Date: 2008/11/13 09:06:22 $
  \author  $Author: miyachi $
  \brief   Definition of QCD Evolution handling class for polDIS
 */

#ifndef _unpolPDF_Evolution_hh_
#define _unpolPDF_Evolution_hh_

#include <Utility/Arguments.hh>
#include <Evolution/Kernel.hh>

/*!
  Any class related to polaried DIS implementation should be 
  defined inside unpolPDF namespace.
  Which avoid possible conflicts among physics case implementation
*/
namespace unpolPDF {
  
  /*!
    \class Evolution Evolution.hh "unpolPDF/Evolution.hh"
  */
  class Evo : public Evolution::Kernel {
  public:

    Evo( Utility::Arguments& args,
	 const std::string& option = "" ) throw( int ) ; //!< a constructor

    virtual ~Evo();                                //!< a destructor
    
    //!< get options for unpolPDF::Evo object
    static std::vector< std::string > options();
    
    static Evo* instance() throw( int );

  private:

    static Evo* pdf_;
    static void atExit();

  };
  
}

#endif //  _unpolPDF_Evolution_hh_
