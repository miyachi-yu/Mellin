/*!
  \brief  class definition of multiplicity data for Fitting
 */
#ifndef  _Fit_MultiDATA_HH_
#define _Fit_MultiDATA_HH_

#include "FitData.hh"

#include <vector>
#include <iostream>

#include <QCD/Flavor.hh>
#include <Xsec/Charge.hh>

namespace Exp {
  class DataSet;
  class DataPoint;
}

namespace Fit {
  
  /*!
    \brief  multiplicity class for Fitting
   */
  class MultiData : public FitData {
  public:
    
    MultiData();                         //!< a default constructor
    //! a constructor with DataSet and DataPoint objects
    MultiData( Exp::DataSet& d, Exp::DataPoint& p );
    MultiData( const MultiData& data );  //!< a copy constructor
    virtual ~MultiData();                //!< a destructor
    
    MultiData& operator=( const MultiData& data ); //!< assignment operator
    
    std::vector< double > vz_; //!< z range
    double z_;                 //!< center of z
    double dz_;                //!< z-bin width
    double zave_;              //!< z average
    std::vector< double > vx_; //!< x range
    double xmin_;              //!< x minimum
    double xmax_;              //!< x maximum
    double xave_;              //!< x average
    double denom_;             //!< denominator
    bool   useAverage_;        //!< use averaged value
    double s_;                 //!< s
    
    double error2(); //!< calculate error to be used in Fitting

    static std::string hName( const int& i ); //!< get name of hadron i
    
    static int get_hType( const std::string& dinfo ); //!< get hadron type
    static int get_nType( const std::string& dinfo ); //!< get target type
    
    static int hType( const QCD::Flavor::HADRON& h );        //!< get hadron type
    static int hType( const std::string& name );             //!< get hadron type
    
    static int nType( const std::string& name );             //!< get target type
    static int nType( const Xsection::Charge::TARGET& n );   //!< get target type
    
    static std::string Key;  //!< Key for multiplicity data
    static std::string KeyZ; //!< Key for multiplicity data
    
    //! check if the given DataPoint is multiplicity data or not
    static bool check( const Exp::DataPoint& p );

    //! header for one line summary
    static std::string header();

    //! one line summary
    friend std::ostream& operator<<( std::ostream& os, const MultiData& data );
  };
}

#endif	    /* !FRAGDATA_HH_ */
