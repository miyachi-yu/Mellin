// $Id: DGLAPMatrix.hh,v 1.10 2008/07/10 04:01:31 miyachi Exp $
/*!
  \file    DGLAPMatirx.hh
  \version "$Revision: 1.10 $"
  \date    "$Date: 2008/07/10 04:01:31 $"
  \brief   Definition of DGLAP Evolution Matrix collection class
 */
#ifndef _DGLAPMatrix_hh_
#define _DGLAPMatrix_hh_

#include <vector>
#include <complex>
#include <iostream>
#include <map>

#include <Tranform/Matrix.hh>
#include <QCD/Scheme.hh>
#include <Tranform/DataCache.hh>

#include "AnomalousDimension.hh"
#include "DGLAPpdf.hh"

namespace Evolution {

  class Kernel;
  
  /*!
    \class   DGLAPMatrix    DGLAPMatrix.hh   "Evolution/DGLAPMatrix.hh"
    \brief   class for DGLAP evolution kernel matrices

    \par
    This is a class to store calculated matrix elements for flavor
    singlet and non-singlet combination. The intermediate matrices
    elements for singlet calculation are also stored.
    
    \par
    anomalous dimension does not change, or does not depend on 
    parameterization, so that one can store calculated value for the
    future usage. For the moment, the data will be kept only in 
    memory. It may be worth to store the calculated value for the
    external file. However it is not obious how to handle file names
    for the different running condition. 

    \par
    While the cached information in Kernel should be cleared when
    inital parameterization is changed, the DGLAPMatrix can keep the
    calculated value till the end of program.

  */
  class DGLAPMatrix {
  public:
    
    DGLAPMatrix( Kernel *kernel );  //!< a default constructor
    virtual ~DGLAPMatrix();         //!< a destructor
    
    /*!
      \param[in] q02  initial scale
      \param[in] q2   scale where DGLAP kernel will be calculated.
      \return itself
      \brief  set scales, and calculate DGLAP kernel matrix at the given scale
    */
    DGLAPMatrix& set( double q02, double q2 );
    
    //! get singlet component
    Transform::Matrix< std::complex< double > >& singlet() { return PS_; } 
    
    //! get nonsingle components
    std::complex< double >& nonSinglet( int sign ) {
      return ( sign < 0 ? PNSm_ : PNSp_ );
    }
    
    //! dump matrix information to the given output stream
    friend std::ostream& operator<<( std::ostream& os, DGLAPMatrix& m );
    
    //! get value of lambda
    std::complex< double >& lambda( int sign ){
      return ( sign == 0 ? dLambdaPM_ : ( sign < 0 ? lambdaM_ : lambdaP_ ) );
    }
    
    /*!
      \brief  information on the QCD scale and related parameters
    */
    struct Scales {
      //! a default constructor
      Scales( Kernel *kernel ) : 
	k_( kernel ),
	beta_( 3, 0.0 ), rbeta_( 3, 0.0 ),
	q_( 2, 0.0 ), q2_( 2, 0.0 ),  
	alpha_( 2, 0.0 ),
	beta2_( 0.0 ), ralpha_( 0.0 ), dalpha_( 0.0 ),
	da_4pi_( 0.0 ), da_4pi_2b_( 0.0 )
      {}
      
      bool set( double q02, double q2 ); //!< set scales
      
      Kernel *k_;
      std::vector< double > beta_;
      std::vector< double > rbeta_;  
      std::vector< double > q_;      //!< initial scale
      std::vector< double > q2_;     //!< initial scale^2
      std::vector< double > alpha_;  //!< value of alpha_s
      double beta2_;                 //!< 2 * beta_0
      double ralpha_;                //!< ratio of alpha_s
      double dalpha_;                //!< difference of alpha_s
      double da_4pi_;
      double da_4pi_2b_;
      
      friend std::ostream& operator<<( std::ostream& os, Scales& s );
      
    };
    
    DGLAPpdf operator*( DGLAPpdf& pdf ); //!< Solve the DGLAP equation 

    int cache_size(); //!< get cached data size

    void cache_dump( std::ostream& os ); //!< dump cached data ( just for debugging )
    
    
  protected:

    bool setLambdas();    //!< calculate lambda for singlet matrix
    bool setSinglet();    //!< calculate singlet matrix
    bool setNonSinglet(); //!< calculate non-singlet matrix ( variables )
    
    //! get a reference of Scale object
    Scales& scale() { return scale_; }

    //! get a reference to singlet matrix
    std::vector< Transform::Matrix< std::complex< double > > >& r() { return S_; }
    //! get a reference to non-singlet values
    std::vector< std::vector< std::complex< double > > >& rns() { return NS_; }
    
    //! get a reference to intermediate matrix for singlet
    Transform::Matrix< std::complex< double > >& R() { return R_; }
    
    //! get a reference to intermediate matrix for singlet, P+ or P-
    Transform::Matrix< std::complex< double > >& P( int sign ) { 
      return ( sign < 0 ? Pm_ : Pp_ ) ; 
    }
    
    Transform::Matrix< std::complex< double > >& PS( int sign = 0 ) {
      return ( sign == 0 ? PS_ : ( sign < 0 ? PSp_ : PSm_ ) );
    }
    
    std::complex< double >& PNS( int sign ){
      return ( sign < 0 ? PNSm_ : PNSp_ );
    }
    
    std::complex< double >& CR( int s1, int s2 ){
      if( s1 < 0 ) return ( s2 < 0 ? CmRm_ : CmRp_ );
      return ( s2 < 0 ? CpRm_ : CpRp_ );
    }
    
  private:
    
    Kernel *k_;                    //!< a pointer to Kernel object
    Scales scale_;                 //!< scale handling object
    
    // anomalous dimension normalized by beta2
    std::vector< Transform::Matrix< std::complex< double > > > S_; //!< single dimension
    std::vector< std::vector< std::complex< double > > > NS_; //!< non-singlet dimension
    
    std::complex< double > lambdaP_;    //!< + eigenvalue of singlet matrix 
    std::complex< double > lambdaM_;    //!< - eigenvalue of singlet matrix 
    std::complex< double > dLambdaPM_;  //!< eigenvalue difference
    
    Transform::Matrix< std::complex< double > > Pp_;  //!< P+
    Transform::Matrix< std::complex< double > > Pm_;  //!< P-
    Transform::Matrix< std::complex< double > > R_;   //!< R
    
    Transform::Matrix< std::complex< double > > PS_;  //!< Flavor singlet matrix ( PSp_ + PSm_ )
    Transform::Matrix< std::complex< double > > PSp_; //!< + component of PS_
    Transform::Matrix< std::complex< double > > PSm_; //!< - component of PS_
    
    std::complex< double > CmRm_;          //!< coefficient for (P-)*R*(P-)
    std::complex< double > CmRp_;          //!< coefficient for (P-)*R*(P+)
    std::complex< double > CpRm_;          //!< coefficient for (P+)*R*(P-)
    std::complex< double > CpRp_;          //!< coefficient for (P+)*R*(P+)
    
    // for non-singlet calculation
    std::complex< double > PNSp_;          //!< Non-singlet(+) element
    std::complex< double > PNSm_;          //!< Non-singlet(-) element
    

    // -------------  for memory regident data cache ---------------------- //
    static const int max_cache_size;
    
    //! cache object for singlet
    std::map< double, 
	      std::map< Transform::CKey, 
			Transform::Matrix< std::complex< double > > > > vcs_;
    
    //! cache object for non-singlet +
    std::map< double, 
	      std::map< Transform::CKey, 
			std::complex< double > > > vcnsp_;
    
    //! cache object for non-singlet -
    std::map< double, 
	      std::map< Transform::CKey, 
			std::complex< double > > > vcnsm_;
    
    
  };
};
#endif //  _DGLAPMatrix_hh_
