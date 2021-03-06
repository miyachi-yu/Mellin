// $Id: P.hh,v 1.3 2008/05/23 02:08:00 miyachi Exp $
/*!
  \file   P.hh
  \brief  Definition of the splitting functions in the Mellin space.

  This is the first trial implementation of polarized DIS splitting function.
  There may be several way to implement, here I adopt one simple macro to 
  create dedicated ComplexFunction derived class for the each splitting
  function in the Mellin space, both LO and NLO.
  (NLO has not been implemented yet.)

 */
#ifndef _Fragmentation_P_hh_
#define _Fragmentation_P_hh_

#include <Evolution/AnomalousDimension.hh>

#include <vector>

/*!
  \def    DEF_ADIM( T )
  \brief  Definition of anomalond dimention class T

  With this macro, one can define class T which is derived from ComplexFunction,
  and posses pointer to class Fragmentation::P as p_. The extended source from the macro
  looks like
<pre>
class T: public ComplexFunction { 
  public: T( P *p ) : p_( p ) {} 
  virtual ~T() {} virtual complex< double > operator() ( complex< double > z ); private: P *p_; };
</pre>
*/
#define DEF_ADIM( T )							\
  class T: public Transform::ComplexFunction {				\
  public:								\
  T( Fragmentation::P *p ) : p_( p ) {}					\
  virtual ~T() {}							\
  virtual std::complex< double >					\
  operator() ( const std::complex< double >& z );			\
  private:								\
  Fragmentation::P *p_;							\
  }

namespace Fragmentation {
  
  /*!
    \class P   P.hh    "Fragmentation/P.hh"
    \brief Anomalous Dimensiton for polarized Deep Inelastic Scattering
    
    This is an inherited class form AnomalousDimension to define anomalous
    dimensitons for helicity distributions appears in polarized deep inelastic
    scattering.
    
  */
  class P : public Evolution::AnomalousDimension {
  public:
    P( Evolution::Kernel *k = NULL );    //!< a default constructor
    virtual ~P();                  //!< a destructor
    
    /*
      The followings are class definition for the several anomalous dimensiton.
      The details can be seen in the macro definition above, or g++ -E P.cc 
      for example.
    */
    DEF_ADIM( QQLO );
    DEF_ADIM( QGLO );
    DEF_ADIM( GQLO );
    DEF_ADIM( GGLO );
    
    DEF_ADIM( QQPS ); 

    DEF_ADIM( QQNLO );
    DEF_ADIM( QGNLO );
    DEF_ADIM( GQNLO );
    DEF_ADIM( GGNLO );
    DEF_ADIM( NSPNLO );
    DEF_ADIM( NSMNLO );
    
    class NSNLO : public Transform::ComplexFunction {
    public:
      NSNLO( Fragmentation::P* p ) : p_( p ) {}
      virtual ~NSNLO(){}
      virtual std::complex< double > 
      operator()( const std::complex< double >& n ) {
	return (*this)( n, 1.0 );
      }
      virtual std::complex< double > 
      operator()( const std::complex< double >& n, 
		  const double& eta );
    private:
      Fragmentation::P* p_;
    };
    
  private:
    
    QQPS  *qqps_;
    NSNLO *nsnlo_;
  };
  
};

#endif // _P_hh_
