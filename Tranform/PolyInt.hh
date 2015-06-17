/*!
  \brief  class definition for Polynominal interpolation
 */
#ifndef _Transform_PolyInt_hh_
#define _Transform_PolyInt_hh_

#include "RealFunction.hh"

#include <map>

namespace Transform {
  
  /*!
    \brief polynominal interpolation of discreated points
    
    This is a class for Polynominal Interpolation described in 
    Numerical Receipt in C.
    
    \par Usage
    The discreated data points can be stored through map< double, double >
    interface since PolyInt inherits map class. Before to perform interpolation
    one must register the data points. An example can be found in polyint.cc
    sample code. The interpolation can be used via function operator (), 
    which overwrite RealFunction::operator(). Use can use a PolyInt object
    as a normal RealFunction, as well.

  */
  class PolyInt : public RealFunction, public std::map< double, double > {
  public:
    
    PolyInt( const int& K = 6 );           //!< a default constructor
    PolyInt( const PolyInt& poly );        //!< a copy constructor
    virtual ~PolyInt();                    //!< a destructor
    
    PolyInt& operator=( const PolyInt& poly ); //!< assignment operator

    //! function operator
    virtual double operator()( const double& x );
    
    double x() { return x_; }     //!< get a x for the last calculation
    double f( const double& x );  //!< get a interpolated value at x
    double df( const double& x ); //!< get a error at x
    
    bool calculated() { return calculated_; } //!< get calculation status
    
    /*!
      \brief set calculation status

      The interpolate operation, called by operator(), f(), or df(),
      check if the last interpolation was done at x or not.
      If use add more discrete point to PolyInt object, 
      interpolation should be re-done, for the save x value.

    */
    void calculated( const bool& state ) { calculated_ = state ;}
    
  private:
    
    int K_;                     //!< number points used for interpolation
    bool calculated_;           //!< status of calculation
    double x_;                  //!< last calculated x point
    double f_;                  //!< last calculated y value
    double df_;                 //!< last calculated error on y

    std::vector< double > c_;
    std::vector< double > d_;
    std::vector< double > xa_;


    //! find nearest point and copy data for interpolation
    int nearest();
    
    double interpolate();

  };
  
};

#endif // _Transform_PolyInt_hh_
