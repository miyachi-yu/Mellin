// $Id: Alpha.hh,v 1.14 2008/11/17 15:52:58 imazu Exp $
/*!
  \file    Alpha.hh
  \version $Revision: 1.14 $
  \date    $Date: 2008/11/17 15:52:58 $
  \author  $Author: imazu $
  \brief   definition of strong coupling constant class
 */
#ifndef _Alpha_hh_
#define _Alpha_hh_

#include <Utility/Arguments.hh>

#include <Tranform/RungeKutta.hh>
#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>

#include <iostream>
#include <cmath>

#include "Flavor.hh"
#include "Scheme.hh"

namespace QCD {
  
/*!
  \class Alpha      Alpha.hh       "QCD/Alpha.hh"
  \brief Class for the strong coupling constant in QCD
  
  This is a class for the strong coupling constant in QCD.
  
  a value of alpha_s at the given scale, Q, can be calculated
  with different methods: 1) Expantion of the renormalization 
  equation to the given order of alpha_s, as given in
  Particle Data Book. 2) Numerical computation from Z-boson 
  scale with Rungekutta method (default).

 */
  class Alpha : public Transform::RealFunction {
  public:
    
    //! alpha_s computation type
    enum MODE { LAMBDA, RUNGEKUTTA };
    
    //! default constructor
    Alpha( const Scheme::ORDER& order = Scheme::NLO );
    
    //! constructor with arguments
    Alpha( Utility::Arguments& args ); 
    
    /*!
      \par [in] lambda is a Lambda QCD
      \brief  a default constructor
      
      One can define Lambda_QCD on construction of Alpha object.
      
      In order to obtain alpha( Mz^2 ) = 0.1187, you must set
      
      lambda = 0.091 ( Scheme::LO )
      lambda = 0.235 ( Scheme::NLO )
      lambda = 0.217 ( Scheme::NNLO )
      
    */
    Alpha( const double& lambda, 
	   const Scheme::ORDER& order = Scheme::NLO );
    
    virtual ~Alpha();                         //!< a destructor
    
    virtual Alpha& operator=( const Alpha& as ); //!< assignment operator
    virtual bool operator!=( const Alpha& as ); //!< not equal operator
    virtual bool operator==( const Alpha& as ); //!< equal operator
    
    virtual double operator() ( const double& mu2 ); //!< calculate alpha( mu^2 )
    double lambda( const double& v );         //!< set and get Lambda value
    double lambda() const { return lambda_; } //!< get Lambda value
    Scheme& scheme() { return scheme_; }      //!< get a reference to Scheme object
    
    void atMz( const double& v ); //!< set alpha_s at M_Z
    double atMz() { return asMz_; }

    double beta( const int& i, const double& q ); //! get beta 
    
    static Flavor& flavor() { return nf; } //!< get a reference to Flavor object
    
    //! calculate bete function using the sored Lambda_QCD
    static double Beta( const int& i, const double& n );
    
    //! dump information of the given alpha object to os
    friend std::ostream& operator <<( std::ostream& os, Alpha& alpha );
    
    static Flavor nf;   //!< information on quarks flavor

    //! set rescaling factor of \mu_R^2 / Q^2 
    void setMURfact( const double& fact ){ factor_ = fact; }
    double getMURfact(){ return factor_; }

    /*!
      \class Alpha::Differential
      \brief A class to calculate differential equation with RungeKutta 
      method.
    */
    class Differential : public Transform::RungeKutta::Differential {
    public:
    
      //! default constructor
      Differential( Alpha *alpha ) : alpha_( alpha ) {}
      
      //! destructor
      virtual ~Differential(){}
      
      //! calculate differential at the given point
      virtual double operator()( const double& t, const double& alpha ) const ;
      
    private:
      Alpha *alpha_;
    };
    
    //! set computation mode
    void mode( const MODE& mode ) { mode_ = mode; }
    MODE mode() { return mode_; }

  private:
    double lambda_;     //!< Lambda_QCD
    double lambda2_;    //!< Lambda_QCD sqared
    Scheme scheme_;     //!< information of QCD renormalization scheme
    
    Differential da_;   //!< differential equation 
    Transform::RungeKutta   rg_;  //!< RungeKutta method
    double       dt_;   //!< step size
    double       asMz_; //!< alphas at Mz
    double       Mz_;   //!< mass of Z
    
    MODE mode_;
    
    static double C;    //!< 4.0 * M_PI

    double factor_; //! arbitrary factor ( q2 / \mu_R^2 ) for input q2 
                    //! against renormalization scale \mu_R^2.
                    //! alpha( q2 ) always returns the valus at \mu_R^2.

  };
  
};
#endif // _Alpha_hh_
