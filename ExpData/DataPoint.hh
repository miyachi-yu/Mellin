// $Id: DataPoint.hh,v 1.18 2013/06/25 00:59:11 miyachi Exp $
/*!
  \brief class definition of handling Data Point
 */
#ifndef _DataPoint_hh_
#define _DataPoint_hh_

#include <vector>
#include <list>
#include <string>
#include <iostream>

#include "Kinematic.hh"

namespace Exp {

  class DataSet;
  
  /*!
    \brief class for handling Data Point
  */
  class DataPoint {
  public:
    
    enum SYSTMODE { lin = 0, quad = 1 };
    
    DataPoint();                                //!< a default constructor
    DataPoint( const DataPoint& dp );           //!< a copy constructor
    virtual ~DataPoint();                       //!< a destructor
    
    //! assignment operator
    virtual DataPoint& operator=( const DataPoint& dp ); 
    
    virtual bool operator==( const DataPoint& dp ); //!< equivalent operator
    virtual bool operator<( const DataPoint& dp );  //!< less than operator
    virtual bool operator>( const DataPoint& dp );  //!< grater than operator
    
    std::string& info() { return info_; }  //!< get information
    std::string& unit() { return unit_; }  //!< get unit of data
    double& data() { return data_; }       //!< get data value
    double& stat() { return stat_; }       //!< get statistical error
    
    std::string info() const { return info_; }
    std::string unit() const { return unit_; }  //!< get unit of data
    double data() const { return data_; }
    double stat() const { return stat_; }
    
    //! assign data value
    void data( const double& value, const std::string& info, 
	       const std::string& unit ) {
      data_ = value; info_ = info; unit_ = unit;
    }


    //ykobayashi added. IS it working well?
    void stat( const double& value ){
	 stat_ = value;
    }

    double error(); //!< get total error 
        
    //! get vector of systematic error
    std::vector< double >& syst()          { return syst_; } 
    std::vector< std::string >& systInfo() { return systInfo_; } 
    std::vector< double > syst()          const { return syst_; } 
    std::vector< std::string > systInfo() const { return systInfo_; } 
    
    double syst( SYSTMODE mode );    //!< get total systematic error

    std::list< Kinematic >& kinematic() { return kinematic_; }
    std::list< Kinematic > kinematic() const { return kinematic_; }
    
    void syst( const double& value, const std::string& info );
    
    void kine( const double& value, const std::string& info,
	       const std::string& unit );
       
    double kine( const std::string& info ) ; //!< get value of kinematics info

    bool hasKine( const std::string& info );
    
    static std::string xmlTag;
    
    double& model() { return model_; }
    double  model() const { return model_; }
    
    double& modelError() { return modelError_; }
    double  modelError() const { return modelError_; }
    
    bool& fitted() { return fitted_; }
    bool  fitted() const { return fitted_; }
    
    double& chiSquare() { return chi2_; }
    double chiSquare() const { return chi2_; }

    double& scale1() { return scale1_; }
    double  scale1() const { return scale1_; }
    
    double& scale2() { return scale2_; }
    double  scale2() const { return scale2_; }
    
    static std::string DTD(); //!< XML DTD
    
    friend std::ostream& operator<<( std::ostream& os, DataPoint& kine );
    
    int& emNumber() { return emNumber_; }
    int& emid()     { return emid_; }

    std::vector< double >& ems() { return ems_; }
    
    std::string summary_header();
    
    //! put one line information
    std::string summary();
    
    DataSet*  dataSet() { return dset_; }
    void      dataSet( DataSet* dset ) { dset_ = dset; }
    
  private:
    
    std::string info_;              //!< information of data point
    
    double data_;                   //!< data value
    double stat_;                   //!< statistical error value
    std::vector< double > syst_;    //!< vector of systematic error
    std::vector< std::string > systInfo_; 
    std::string unit_;              //!< unit of these values
    
    std::list< Kinematic > kinematic_;
    
    double model_;
    double modelError_;
    bool   fitted_;
    
    double scale1_;
    double scale2_;
    
    double chi2_;

    // ------------------ error matrix ---------------------- //
    int emNumber_;              //!< error matrix ID
    int emid_;                  //!< error matrix element index
    std::vector< double > ems_; //!< error matrix element list

    DataSet* dset_;
    
  };
  
};
#endif // _DataPoint_hh_
