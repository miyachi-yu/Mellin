// $Id: Pvalue.hh,v 1.6 2010/03/06 10:13:12 miyachi Exp $
/*!
  \brief  real function to calculate p-value of chi2
 */
#include "RealFunction.hh"
#include "RealFunction.hh"
#include "IntegFunc.hh"

namespace Transform {

  /*!
    \brief  p-value calculataion class
    
    p-value is defined in PDG as a significant level of
    the obtained mimimum chi2 in the expected chi2 distribution
    between a model and data sample.

   */
  class Pvalue : public RealFunction, public IntegFunc {
  public:
    
    //! a constructor with a number of freedom, k
    Pvalue( const double& k = 100.0 ); 

    //! a destructor
    virtual ~Pvalue();
    
    //! a function operator fot Pvalue class
    virtual double operator()( const double& x );

    //! set number of freedom, k
    void k( const double& k );

    //! get number of freedom, k
    double k() const { return f_.k(); }
    
    //! get chi2 function
    RealFunction& f() { return f_; }

    /*!
      \brief integrant class for Pvalue class

      A class to calculate chi2 distribution with number of freedom k.

      In order to get reliable value, one has to manipulate formula


      for odd number of dof

      f(x;k) = ( x^{k/2 -1} / (k-2)!! ) * exp( - z/2 ) / sqrt( 2 PI )

      where, (n)!! is double fractional function.
      
      for even number of dof ( k = 2m )
      f(x; 2m) = x^{m-1}/(2^m (m-1)!) exp(-z/2)

    */
    class Integrant : public RealFunction {
    public:
      Integrant( const int& k = 100.0 );    //!< a constructor
      virtual ~Integrant();                 //!< a destructor
      virtual double operator()( const double& x ); //!< a function operator
      void k( const int& k );               //!< set k
      int k() const { return k_; }          //!< get k

    private:
      int    k_;   //!< a number of freedom
      bool   odd_; // true if k is odd
    };
    
  private:
    Integrant f_; //!< integrant function object (chi2 distribution)
    
  };
  
};
