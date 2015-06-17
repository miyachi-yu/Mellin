// $Id: InverseMellin.hh,v 1.35 2008/10/26 02:05:44 miyachi Exp $
/*!
  \file    InverseMellin.hh
  \version $Revision: 1.35 $
  \date    $Date: 2008/10/26 02:05:44 $
  \author  $Author: miyachi $
  \brief   Definition of Mellin inversion class
 */
#ifndef _InverseMellin_hh_
#define _InverseMellin_hh_

#include "Integration.hh"
#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include "Transformer.hh"
#include "IntegPath.hh"
#include "SteepestDescent.hh"
#include "StraightLine.hh"
#include "InverseMellinIntegrand.hh"
#include <Utility/Thread.hh>

#include <vector>

namespace Transform {
  
  /*!
    \class  InverseMellin   InverseMellin.hh   "Tranform/InverseMellin.hh"
    \brief  Mellin inversion class
    
    This is a Mellin inversion class. Mellin inversion is a
    integration of some complex function with a certain weight x^-z 
    where z is a complex in the region 0 < x < infinity.
    This class performs the tranformation 
    using Integration and InverseMellinIntegrand class.
    
    \par formalizm
    \f[
    f( x ) = { 1 \over 2 \pi \imath } \int_{C} {\rm d}z x^{-z} f( z )
    \f]
    In our case, f(-z) is symmetric with respoect to the real axis,
    so that the inversion equation becomes
    \f[
    f( x ) = { -1 \over \pi } 
    \Re \left [ \int_{C} {\rm d}z \imath x^{-z} f( z ) \right ]
    \f]
    or
    \f[
    f( x ) = { 1 \over \pi } 
    \Im \left [ \int_{C} {\rm d}z x^{-z} f( z ) \riht ]
    \f]
    If the integration path \f$ C \f$ is given with a formula,
    \f[
    z = z( t )
    \f]
    This can be re-writen as
    \f[
    f( x ) = { 1 \over \pi } 
    \int \Im \left[ {\rm d}t \left 
    ( { {\rm d}z \over {\rm d}t } \right )x^{-z(t)} f( z(t) )  \right ]
    \f]
    
  */
  
  class InverseMellin : public RealFunction, 
			public Transformer
  {
    
  public:
    
    /*!
      \param[in] g is a reference of ComplexFunction to be tranformed
      \param[in] inter is a pointer to Integration object
      \brief a default constructor.
      
      One must specify one ComplexFunction to be transform
      and a pointer to Integration class to perform the integration.
      
    */
    InverseMellin( ComplexFunction *g, Integration *inter );
    
    //! a constructor with integration method
    InverseMellin( Integration *inter );
    
    //! a destructor
    virtual ~InverseMellin();
    
    /*!
      \param[in] c is a off-set on real axis for the integration path
      \param[in] phi is a angle between integration path and real axis ( without M_PI )
      \brief set integration path condition ( for straight line path )
      
      This configures the integration path for the Mellin inversion.
      c is a starting point on the real x, and phi is a angle between the path and
      the real axis. The phi should be given without M_PI which will be applied 
      later inside InverseMellin::setParameter.
      
      It will configure both old and new implementation of integration path

    */
    void setParameter( const double& c, const double& phi );
    
    //! set parameter with vector
    void setParameter( const std::vector< double >& c,
		       const std::vector< double >& phi );
    
    //! set moment parameter
    void setMomParam( const std::vector< double >& mom );
    
    virtual double upper() ;     //!< get length of path length
    
    //! set integration path length for the straight contour
    virtual void upper( const double& l );
    
    //! set integration path length for the straight contour with vector
    virtual void upper( const std::vector< double >& l );
    
    virtual double lower() ;     //!< get length of path length
    
    //! set integration path length for the straight contour
    virtual void lower( const double& l );
    
    //! set integration path length for the straight contour with vector
    virtual void lower( const std::vector< double >& l );
    
    double c() { return c_; }       //!< get offset value
    double phi() { return phi_; }   //!< get path angle

    virtual std::vector< double > angle();  //!< get angle value
    virtual std::vector< double > offset(); //!< get offset value
    virtual std::vector< double > length() { return vupper_; } //!< get length value
    
    /*!
      \param[in] g is a reference to ComplexFunction 
      \return a reference to Mellin inversed RealFunction
      \brief  set complex function for Mellin inversion
      
      This is a test implementation of Mellin inversion.
      For the moment, this function returns a referene to itself,
      it could be possible that the obtained referece performs
      unexpectedly.... 
      
      Usage:
      InverseMellin  IM(...);
      ComplexFunction g;
      RealFunction mf = IM.transform( g );
      
      cout << mf(  2.0 ) << endl;
      
      It looks more natural.....
    */
    RealFunction& transform( ComplexFunction *g );
    
    //! calculate value of at x
    virtual double operator()( const double& x );
    
    //! calculate value of at ( x, z )
    virtual double operator()( const double& x, const double& z );
    
    //! calcualte value of at ( x )
    virtual double operator()( const std::vector< double >& x );
    
    //! calcualte value of at ( x, z )
    virtual double operator()( const std::vector< double >& x,
			       const std::vector< double >& z );
    
    /*!
      \brief Integrand of Mellin Inverse Integral equation
      
      This integrand consists of 
      \f$ ( {\rm d}z/ {\rm d}t \cdot x^{-z} f(z) \f$,
      where \f$ z(z) \f$ defines the integration path. 
      
      \par partial moment
      In cross section calculation of hadron inclusive cross section
      in e+-e- anihilation, the partial moment is calculated. 
      When the x range is given, this class will calculate the 
      integrand for the partial moment calculation.
      
    */

    /*!
      \brief  Integrand of Mellin Inversion
      
      The integrand of Mellin Inversion of funciton f(z) 
      along a integration path defined with z(t) is 
      
    */
    class Integrand : public RealFunction {
    public:
      
      //!< a constructor
      Integrand( InverseMellinIntegrand* f, IntegPath *p1, IntegPath *p2 = NULL ); 
      virtual ~Integrand(){} //!< a destructor
      
      /*!
	\brief  integrand for a single Mellin inversion
	
	It return a value of the integrand for the single Mellin inversion,
	which is
	\f[
	(dn/dt) F(n(t))
	\f]
	where \f$ F(n) = x^{-n} f(n)\f$.
	
      */
      virtual double operator()( const double& t );
      
      /*!
	\brief  integrand for a double Mellin inversion
	
	It return a value of the integrand for the double Mellin inversion,
	which is
	\f[
	(dn/dt_1) \left( (dm/dt_2) F(n(t_1),m(t_2)) -
	(dm^{*}/dt_2) F(n(t_1),m^{*}(t_2)) \right)
	\f]
	
      */
      virtual double operator()( const double& t1, const double& t2 );
      
      //! set integration path
      void path( IntegPath *p1, IntegPath *p2 = NULL ) { 
	p1_ = p1 ; p2_ = p2; 
      }

      void setIMIntegrand( InverseMellinIntegrand* iminteg );
     
    private:
      InverseMellinIntegrand* f_;           //!< \f$ x^{-z} f(z) \f$
      IntegPath *p1_;     //!< integration contour in Mellin Inversion
      IntegPath *p2_;
    };

    
    class SteepInteg : public RealFunction {
    public:
      
      SteepInteg( InverseMellinIntegrand* f, SteepestDescent& path ) : 
	c2( 0.0 ), c3( 0.0 ), F_( f ), path_( path ) { }
      virtual ~SteepInteg(){}
      
      virtual double operator()( const double& u );
      
      double c2;
      double c3;

      void setIMIntegrand( InverseMellinIntegrand* iminteg );
      
    private:
      InverseMellinIntegrand* F_;
      SteepestDescent& path_;
      
    };
    
    //!< integraion contour type
    enum INTEGPATH {
      STEEPEST = 1, STRAIGHT = 2
    };
    
    //!< select integration contour type
    void integPath( const INTEGPATH& type );
    
    /*!
      \param[in] c0  is a minimum location of \f$ F(z)\f$
      \param[in] c1  is a \f$ F^{3}(c0)/F^{2}(c0)/6.0 \f$
      \param[in] c2  is a \f$ \sqrt{ 2.0 * F(c0)/F^{2}(c0) } \f$
      \brief  set parametes for Steepest Descent
    */
    void steepestDescent( const double& c0, const double& c1, const double& c2 );
    
    InverseMellinIntegrand& F() { return *activefunc_; } 
    //!< get a reference to F(z) ( InverseMellinIntegrand )
    
    void setIMIntegrand( InverseMellinIntegrand* iminteg ); 
    //!< set a pointer to F(z) ( InverseMellinIntegrand )
    InverseMellinIntegrand* getIMIntegrand(){ return activefunc_; };
    //!< get a pointer to F(z) ( InverseMellinIntegrand )
    
    INTEGPATH contour() { return type_; } //!< get a contour type

  protected:
        
    INTEGPATH       type_;
    SteepestDescent steepest_;
    StraightLine    straight1_;
    StraightLine    straight2_;
    
    std::vector< double > vupper_;
    std::vector< double > vlower_;
    
    InverseMellinIntegrand func_;
    Integrand integ_;
    SteepInteg steepInt_;

    InverseMellinIntegrand* activefunc_;
    
    double c_;
    double phi_;

    double upperHalf_;
    double lowerHalf_;
    double integrate_();
    
  public:

    //! set some parameters related x, z ( like y in multiplicity )
    /*
    virtual void setxparams( const std::vector< double >& x ){};
    virtual void setxparams( const double& x1, const double& x2 = 0.0 ){}; 
    virtual void setzparams( const std::vector< double >& z ){};
    virtual void setzparams( const double& z1, const double& z2 = 0.0 ){}; 
    */

  };
};

#endif
