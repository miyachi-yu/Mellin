/*!
  \brief  definition of UDratio Data class
*/
#ifndef  _Fit_UDratioData_HH_
#define _Fit_UDratioData_HH_

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
    \brief FragXsec Data class for Fitting
   */
  class UDratioData : public FitData {
  public:
    
    UDratioData();                           //!< a default constructor

    //! a constructor with DataSet and DataPoint objects
    UDratioData( Exp::DataSet& d, Exp::DataPoint& p );
    
    UDratioData( const UDratioData& data );     //!< a copy constructor
    virtual ~UDratioData();                  //!< a destructor
    
    UDratioData& operator=( const UDratioData& data ); //!< assignment operatpr
    
    double x_;                            //!< center of z
    
    //! calculate error used in Fitting for the Given Data Point
    double error2();
    
    static std::string Key;  //!< key for FragXsec data
    
    static std::string header( ); //!< header for one line summary

    //! check if it is FragXsec data or not
    static bool check( const Exp::DataPoint& p );
    
    //! dump one line summary
    friend std::ostream& operator<<( std::ostream& os, const UDratioData& data );
    
  };
}

#endif	    /* !FRAGDATA_HH_ */
