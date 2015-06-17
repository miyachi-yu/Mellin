#ifndef _ExpData_hh_
#define _ExpData_hh_

#include "ConfigDataSet.hh"
#include "DataSet.hh"

namespace Exp {
  
  /*!
    \class ExpData ExpData.hh "ExpData/ExpData.hh"
  */
  class ExpData {
  public:

    ExpData();                             //!< a defalut constructor
    ExpData( Utility::Arguments& args,
	     const std::string& option = "expdata" );   //!< a constructor
    ExpData( const std::string& path );    //!< a constructor with path
    ExpData( const ExpData& data );        //!< a copy constructor
    virtual ~ExpData();
    
    ExpData& operator=( const ExpData& data );  //!< assignment operator
    ExpData& operator+=( const ExpData& data ); //! insersion operator
    
    std::vector< DataSet >& data() { return data_; }
    std::vector< DataSet >  data() const { return data_; }
    
    std::string& version() { return version_; }
    std::string  version() const { return version_; }
    
    //! get xml header ....
    static std::string xmlHeader();
    static std::string xmlTag;
    static std::string DTD();
    
    std::vector< double > findkin( const std::string& kin );
    std::vector< double > finddata( const std::string& kin );
    
    int emNumber();
    
    friend std::ostream& operator<<( std::ostream& os, ExpData& ed );
    
    void summary( std::ostream& os, const SUMMARY_LEVEL& level = NORMAL );
    
    ConfigDataSet& config() { return config_; }
    
    double chiSquare();
    
    int dataPoint();
    
    //! get number of data point used in fit
    int fittedDataPoint();
    
    void disable( const std::vector< int >& id ); //!< disable DataSet
    void enable( const std::vector< int >& id );  //!< enable DataSet
    
    //! set delete flag and delete element
    void remove( const std::vector< int >& id ); 
    
    double& lambda() { return lambda_; }       //!< get weight for chi2
    double  lambda() const { return lambda_; } //!< get weight for chi2
    
    void resetPointers(); //!< set ExpData pointer to DataSet objects
    
  private:
    
    ConfigDataSet config_;
    std::vector< DataSet > data_;
    std::string version_;
    
    double lambda_; //!< weight for chi^2 calculation
    
    //! delete element with delete flag
    void remove(); 
    
    void setAttribute();
    
  };
};
#endif //  _ExpData_hh_
