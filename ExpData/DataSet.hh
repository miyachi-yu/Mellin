#ifndef _DataSet_hh_
#define _DataSet_hh_

#include <string>
#include <vector>
#include <iostream>

#include "DataPoint.hh"

namespace Exp {

  extern std::string data_path;
  class ExpData;

  enum SUMMARY_LEVEL { NORMAL = 0, DETAIL = 1};
  
  enum HADRON{
    undefined = 0,
    Hp = 20, Hm = 21, Hpm = 22,
    PIp = 30, PIm = 31, PIpm = 32, PI0 = 33,
    Kp = 40, Km = 41, Kpm = 42,
    PRp = 50, PRm = 51, PRpm = 52
  };
  
  static std::string hadname( const Exp::HADRON& h ){
    switch( h ){
    case Hp :   return "Hp";
    case Hm :   return "Hm";
    case Hpm :  return "Hpm";
    case PIp :  return "PIp";
    case PIm :  return "PIm";
    case PIpm : return "PIpm";
    case PI0 :  return "PI0";
    case Kp :   return "KAp";
    case Km :   return "KAm";
    case Kpm :  return "KApm";
    case PRp :  return "PRp";
    case PRm :  return "PRm";
    case PRpm : return "PRpm";
    }
    return "";
  };
  
  enum TARGET {
    PRO = 1, NEU = 2, DEU = 3
  };
  
  enum TYPE {
    FRAGXSEC = 1,
    MULTI    = 2,
    A1       = 3
  };
  
  static std::string tarname( const Exp::TARGET& t ){
    switch( t ){
    case PRO : return "PRO";
    case NEU : return "NEU";
    case DEU : return "DEU";
    }
    return "";
  };
  
  /*!
    \class DataSet DataSet.hh "ExpData/DataSet.hh"
    \brief class for handling collection of DataPoint object

  */
  class DataSet : public std::vector< DataPoint > {
  public:
    
    DataSet();                       //!< a default constructor
    DataSet( const DataSet& dset );  //!< a copy constructor
    virtual ~DataSet();              //!< a destructor
    
    DataSet& operator=( const DataSet& dset ); //!< assignment operator 
    
    std::string& info() { return info_; }      //!< get information
    std::string info() const { return info_; } //!< get information
    
    std::string& ref() { return ref_; }        //!< get referece keyword
    std::string ref() const { return ref_; }   //!< get referece keyword
    
    std::string& cite() { return bib_; }       //!< get bibtex keyword
    std::string cite() const { return bib_; }  //!< get bibtex keyword

    double& norm() { return norm_; }          //!< get normalization tolerance
    double norm() const { return norm_; }     //!< get normalization tolerance

    double& ncoef() { return ncoef_; }     //!< get normalization coefficent
    double ncoef() const { return ncoef_;} //!< get normalization coefficent
    
    double& ncerr() { return ncerr_; }     //!< get normalization coefficent
    double ncerr() const { return ncerr_;} //!< get normalization coefficent

    double& upper() { return upper_; }     //!< get upper bound
    double upper() const { return upper_;} //!< get upper bound

    double& lower() { return lower_; }     //!< get lower bound
    double lower() const { return lower_;} //!< get lower bound
    
    std::string& ncstat() { return ncstat_; }      //!< get coefficent status
    std::string ncstat() const { return ncstat_;} //!< get coefficent status
    
    bool& enable() { return enable_; }          //!< get status flag
    bool  enable() const { return enable_; }    //!< get status flag
    
    friend std::ostream& operator<<( std::ostream& os, DataSet& set );
    
    static std::string xmlTag;                 //!< xml tag name
    
    double chiSquare(); //!< calc chiSquare
    
    static std::string DTD(); //!< XML DTD

    double normalization(); //!< get normalization
    double normalizationError(); //!< get normalization
    
    int& emid() { return emid_; }      //!< get error matrix ID
    int emid() const { return emid_; } //!< get error matrix ID
    
    int& emNumber() { return emNumber_; }      //!< get error matrix ID
    int emNumber() const { return emNumber_; } //!< get error matrix ID
    
    //! get error matrix elements
    std::vector< double >& ems() { return ems_; } 
    
    //! get error matrix elements
    std::vector< double > ems() const { return ems_; } 
    
    static std::string summary_header();
    
    //! put one line information
    std::string summary( const SUMMARY_LEVEL& level = NORMAL );
    
    //! get number of data point used in fit
    int fittedDataPoint();
    
    bool& remove() { return remove_; }
    bool  remove() const { return remove_; }

    void expdata( ExpData* data ) { expdata_ = data; }
    ExpData* expdata() { return expdata_; }
    
    std::string abbreviation() const ;

  private:

    std::string info_;  //!< information of this data set
    std::string ref_;   //!< reference of these data set
    std::string bib_;   //!< bibtex citation keyword
    double chi2_;       //!< value of chi square
    bool enable_;       //!< if DataSet is enable for futher use or not
    
    /*!
      \brief   a normalization tolerance ( default: 0.0 )
      
      This value will be used in Fitting procedure to handle 
      normalization for the given data set. See Fitting class.
      
    */
    double norm_;
    double ncoef_;       //!< normalization coefficient
    double ncerr_;       //!< normalization coefficient error
    std::string ncstat_; //!< normalization coefficient status
    int    emNumber_;    //!< error matrix ID
    int    emid_;        //!< error matrix element ID
    std::vector< double > ems_; //!< error matrix elements

    double upper_;        //! upper and lower bound for fit ( if necessary )
    double lower_;

    bool remove_; //!< delete flag

    ExpData *expdata_; //!< a pointer of parent ExpData object

  };
};

#endif // _DataSet_hh_
