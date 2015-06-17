#ifndef _IntegUnpolSIDIS_PartialInteg_hh_
#define _IntegUnpolSIDIS_PartialInteg_hh_

#include <Tranform/PartialInteg.hh>
#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>

#include <iostream>
using namespace std;

namespace IntegUnpolSIDIS{

  class PartialInteg: public Transform::PartialInteg{
  public:
    PartialInteg( Transform::RealFunction* g, 
		  const int& low = 4, const int& high = 6,
                  const int& ngrid = 2,
		  const double& precision = 1.0E-4 ) throw( int ):
      Transform::PartialInteg( low, high, ngrid, precision ), cfuncc_()
    { this->setfunc( g ); };

    PartialInteg( Transform::RealFunction* g, 
		  Transform::Integration* integ ) throw( int ):
      Transform::PartialInteg( integ ), cfuncc_()
    { this->setfunc( g ); };

    PartialInteg( Transform::ComplexFunction* f, 
		  const int& low = 4, const int& high = 6,
                  const int& ngrid = 2,
		  const double& precision = 1.0E-4 ) throw( int ):
      Transform::PartialInteg( low, high, ngrid, precision ), cfuncc_()
    { this->setfunc( f ); };

    PartialInteg( Transform::ComplexFunction* f, 
		  Transform::Integration* integ ) throw( int ):
      Transform::PartialInteg( integ ), cfuncc_()
    { this->setfunc( f ); };
    
    virtual ~PartialInteg(){};

    virtual Transform::PartialInteg::CFunc* getCFunc(){ return &cfuncc_; }
    
  private:
    
    class CCFunc: public Transform::PartialInteg::CFunc{
    public:
      CCFunc(): Transform::PartialInteg::CFunc(){};
      virtual ~CCFunc(){};

      virtual double operator()( const double& z )
      {
	if( getPIP() == 1 ) return (*getfunc())( z, getx() ) / z ;
	if( getPIP() == 2 ) return (*getfunc())( getx(), z ) / z ; 
      }

      // complex part kept unchanged (PStrFunc.cc)
      
    };

    CCFunc cfuncc_;
  };
};

#endif //  _IntegUnpolSIDIS_PartialInteg_hh_
