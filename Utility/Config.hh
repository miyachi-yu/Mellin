// $Id: Config.hh,v 1.9 2008/12/02 10:29:55 miyachi Exp $
/*!
  \file     Config.hh
  \version  $Revision: 1.9 $
  \author   $Author: miyachi $
  \brief    Definition of class for the configuration XML file handling
 */
#ifndef _Config_hh_
#define _Config_hh_

#include <string>
#include <iostream>

namespace xmlpp {
  class DomParser;
  class Element;
}

#include "Arguments.hh"

namespace Utility {
  
  /*!
    \class  Config    Config.hh    "Utility/Config.hh"
    \brief  class for configuraiton file 
    
    XML is chosen for the standard configuration file format.
    This is a helper class for the XML configuration file,
    and is implemented using libxml++ class library.
  */
  class Config {
  public:
    
    Config(); //!< a defalut constructor
    
    Config( const std::string& path ); //!< a constructor
    
    /*!
      \param[in]  args is a Arguments object
      \param[in]  option is a name of option looked up
      \brief  construct an object using the given Arguments object
      
      This tries to construct an object using the command line argument 
      object, Arguments. The name of option which used to get a path
      of the configuration xml file can be defined with the option
      argument.
      
    */
    Config( Arguments& args, 
	    const std::string& option = "config" ) throw( int ) ;
    
    virtual ~Config();                //!< a destructor
    
    xmlpp::Element* rootNode();       //!< get a pointer to the root node
    
    std::string rootName();           //!< get name of root node
    
    //! dump cofiguration contents to the given output stremam
    friend std::ostream& operator<<( std::ostream& os, Config& conf );
    
    //! dump cofiguration contents to the given output stream
    void dump( std::ostream& os );
    
    //! get path of the configuration file
    std::string path() { return path_; }
    
    bool parse( const std::string& path ); //!< parse the given xml file
    bool parse();                          //!< parse the given xml file
    
  private:
    xmlpp::DomParser *parser_;       //!< XML parser object
    bool owner_;
    
    std::string path_;
    
  };
  
};

#endif //  _Config_hh_
