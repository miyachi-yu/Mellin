#ifndef _ConfigDataSet_hh_
#define _ConfigDataSet_hh_

#include <Utility/Config.hh>

#include <vector>

#include "DataSet.hh"

namespace Exp {
  
  /*!
    \class ConfigDataSet ConfigDataSet.hh "ExpData/ConfigDataSet.hh"
    \brief class for Experiment Data Configuration file handling
  */
  class ConfigDataSet : public Utility::Config {
  public:

    ConfigDataSet(); //!< a default constructor
    
    //! a constructor with Utility::Arguments arguments
    ConfigDataSet( Utility::Arguments& args, 
		   const std::string& option = "expdata" );
    
    //! a constructor with path for the configuration xml file
    ConfigDataSet( const std::string& path );

    virtual ~ConfigDataSet();  //!< a destructor
    
    //! load DataSet objects to the given container
    void dataSet( std::vector< DataSet >& data );
    
    //! get version information
    std::string version() { return version_; }

    //! get weight for chi2 calculation
    double lambda() { return lambda_; }
    
  private:

    //! create a DataSet object from the given XML element
    DataSet dataSet( xmlpp::Element* elm );

    std::string version_;  //!< version information
    
    double lambda_;   //!< weight for chi2
    
    void loadRootNodeInfo();
  
  };
};
#endif // _ConfigDataSet_hh_
