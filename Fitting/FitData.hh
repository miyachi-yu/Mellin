/*!
  \brief  definition of Fitting Data Base class
 */
#ifndef  _Fit_FITDATA_HH_
#define  _Fit_FITDATA_HH_

#include <string>
#include <iostream>

namespace Exp {
  class DataSet;
  class DataPoint;
}

namespace Fit {

  /*!
    \brief  Fitting data base class
    
    Data to be fitted must be handled with a classes which 
    inherit this base class. Fitting class has a container 
    class object of pointers for this base class object, 
    fitData_. 

   */
  class FitData {
  public:
    
    FitData();                       //!< a default constructor
    FitData( const FitData& data );  //!< a copy    constructor
    
    virtual ~FitData();              //!< a destructor
    
    //! assignment operator
    FitData& operator=( const FitData& data );
    
    void dataSet( Exp::DataSet* dset );
    
    int ih_;                //!< hadron type
    int it_;                //!< data/target   type

    double sig_;            //!< data value
    double dsig_;           //!< data error
    double q2_;             //!< scale
    double fit_;            //!< fit value
    double c2_;             //!< chi^2 for each data point
    double c2weight_;       //!< chi^2 for each data point
    bool   sigUsed_;        //!< data used in chi2 calculation

    std::string expInfo_;   //!< data information
    Exp::DataSet*   dset_;  //!< DataSet object pointer
    Exp::DataPoint* point_; //!< DataPoint object pointer

    //! create one line string of header information for summary ouput
    static std::string header( ); 

    //! summary of FitData in one line
    friend std::ostream& operator<<( std::ostream& os, const FitData& data );

  };
  
}

#endif // _Fit_FITDATA_HH_
