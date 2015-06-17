/*!
  \brief class definition of A1 and A1h data for Fitting
 */
#ifndef  _Fit_A1DATA_HH_
# define _Fit_A1DATA_HH_

#include "FitData.hh"
#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <Xsec/Charge.hh>

namespace Exp {
  class DataSet;
  class DataPoint;
}

namespace Fit {
  
  /*!
    \brief  A1 and A1h data class for Fitting
   */
  class A1Data : public FitData {
  public:
    
    A1Data( );                      //!< a default constructor
    //! a constructor with DataSet and DataPoint objects
    A1Data( const Exp::DataSet& dset, const Exp::DataPoint& dp );
    A1Data( const A1Data& data );   //!< a copy constructor
    virtual ~A1Data();              //!< a destructor
    
    A1Data& operator=( const A1Data& data ); //!< assignemnt operator
    
    std::vector< double > vx_; //!< x range
    double x_;                 //!< center of x
    std::vector< double > vz_; //!< z range
    double z_;                 //!< center of z
    double dz_;                //!< z-bin width
    bool a1h_;                 //!< SIDIS data
    double f1_;                //!< denominator
    double original_;          //!< for difference mode

    static std::string disKey;
    static std::string sdisKey;
    
    static int hType( const QCD::Flavor::HADRON& h );
    static int nType( const Xsection::Charge::TARGET& n );
    
    static int hType( const std::string& name );
    static int nType( const std::string& name );
    
    static int hType( bool dis, const std::string& dinfo );
    static int nType( bool dis, const std::string& dinfo );
    
    static bool isDIS( const std::string& dinfo );
    static bool isSIDIS( const std::string& dinfo );
    
    static std::string hadronName( const int& i );
    static std::string targetName( const int& i );

    static std::string header(); //!< header for one line summary
    
    //! one line summary
    friend std::ostream& operator<<( std::ostream& os, const A1Data& data );


    
  };
}

#endif	    /* !FRAGDATA_HH_ */
