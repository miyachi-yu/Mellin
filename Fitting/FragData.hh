/*!
  \brief  definition of FragXsec Data class
 */
#ifndef  _Fit_FRAGDATA_HH_
#define _Fit_FRAGDATA_HH_

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
  class FragData : public FitData {
  public:
    
    FragData();                           //!< a default constructor

    //! a constructor with DataSet and DataPoint objects
    FragData( Exp::DataSet& d, Exp::DataPoint& p );

    FragData( const FragData& data );     //!< a copy constructor
    virtual ~FragData();                  //!< a destructor
    
    FragData& operator=( const FragData& data ); //!< assignment operatpr
    
    std::vector< double > vz_;            //!< z range
    double z_;                            //!< center of z
    double dz_;                           //!< z-bin width
    
    //! calculate error used in Fitting for the Given Data Point
    double error2();
    
    //! hadron name for index i
    static std::string hName( const int& i ); 
    
    static int get_hType( const std::string& dinfo ); //!< get hadron type
    static int get_nType( const std::string& dinfo ); //!< get target type
    
    static int hType( const std::string& name );      //!< get hadron type
    static int hType( const QCD::Flavor::HADRON& h ); //!< get hadron type
    
    static int nType( const std::string& name );      //!< get target type
    static int nType( const Xsection::Charge::TARGET& n ); //!< get target type
    
    static std::string Key;  //!< key for FragXsec data
    
    static std::string header( ); //!< header for one line summary

    //! check if it is FragXsec data or not
    static bool check( const Exp::DataPoint& p );
    
    //! dump one line summary
    friend std::ostream& operator<<( std::ostream& os, const FragData& data );
    
  };
}

#endif	    /* !FRAGDATA_HH_ */
