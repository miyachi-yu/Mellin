/*!
  \brief class definition of PDF/FF parameter
 */
#ifndef _QCD_Parameter_hh_
#define _QCD_Parameter_hh_

#include <vector>
#include <string>
#include <iostream>

namespace QCD {
  
  /*!
    \brief class for Parameter for PDF/FF parametrization
  */
  class Parameter {
  public:
    
    //! status of Parameter object
    enum STATUS {
      FIXED, FREE, ADD, MULTI
    };
    
    Parameter();                           //!< a default constructor
    Parameter( const Parameter& param );   //!< a copy constructor
    
    virtual ~Parameter();                  //!< a destructor
    
    //! assignment operator
    Parameter& operator=( const Parameter& param );
    
    int& emid() { return emid_; }          //!< get error matrix ID
    int& emNumber() { return emNumber_; }  //!< get error matrix number
    
    int& index() { return index_; }        //!< get index number
    int index() const { return index_; }   //!< get index number

    std::string& name() { return name_; }       //!< get parameter name
    std::string name() const { return name_; }  //!< get parameter name

    double& value() { return value_; }          //!< get parameter value
    double value() const { return value_; }     //!< get parameter value

    double& error() { return error_; }          //!< get error
    double error() const { return error_; }     //!< get error

    double& upper() { return upper_; }          //!< get upper limit
    double upper() const { return upper_; }     //!< get upper limit

    double& lower() { return lower_; }          //!< get lower limit
    double lower() const { return lower_; }     //!< get lower limit
    
    std::string& state() { return state_; }      //!< get parameter status
    std::string state() const { return state_; } //!< get parameter status

    void state( const std::string& state ); //!< set parameter status
    void status( const STATUS& status );    //!< set parameter status
    
    //! true if Parameter is free
    bool free()  { return ( status_ == FREE ); } 
    
    //! true if Parameter is not free, including fixed, add, multi
    bool fixed() { return !( this->free() ); }

    //! true if Parameter is additive parameter
    bool add()   { return ( status_ == ADD ); }

    //! true if Parameter is multicative parameter
    bool multi() { return ( status_ == MULTI ); }
    
    //! get error matrix elements
    std::vector< double >& ems() { return ems_; }
    
    //! output paramerer info in XML format
    friend std::ostream& operator<<( std::ostream& os, Parameter& param );
    
    //!< get reference of srcid variable
    int& srcid() { return srcid_;}

    //!< check if srcid is given or not
    bool checkSrcid() { return srcidsrc_ != ""; }
    
    //!< set reference id 
    int& setSrcid( const std::string& val );

  private:
    
    int emNumber_;
    int emid_;
    int index_;
    std::string srcidsrc_;
    int srcid_;
    std::string name_;
    double value_;
    double error_;
    double upper_;
    double lower_;
    std::string state_;
    std::vector< double > ems_; // error matrix elements_;


    
    STATUS status_;

  };
  
}

#endif // _QCD_Parameter_hh_
