#ifndef _IntegFunc_hh_
#define _IntegFunc_hh_

#include "Transformer.hh"
#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {
  
  /*!
    \class IntegFunc   IntegFunc.hh  "Tranform/InegFunc.hh"
    \brief Implementation of Transform class using LegQuadrature for integration
    
    The base class implementation for integral functions using 
    LegQuadrature class for integration method.
    
  */
  class IntegFunc : public Transformer {
  public:
    IntegFunc( const int& n = 4, const int& m = 6, 
	       const int& ngrid = 4,
	       const double& precision = 5.0E-3 );
           //!< a default constructor
    IntegFunc( Integration* integration );

    virtual ~IntegFunc();  //!< a destructor
    
  private:
  };
};

#endif // _IntegFunc_hh_
