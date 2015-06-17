#ifndef _IntegPolSIDIS_PartialInteg_hh_
#define _IntegPolSIDIS_PartialInteg_hh_

#include <Tranform/PartialInteg.hh>
#include <Tranform/RealFunction.hh>

namespace IntegPolSIDIS{

  class PartialInteg: public Transform::PartialInteg{
  public:
    PartialInteg( Transform::RealFunction* g = NULL, 
		  const int& low = 4, const int& high = 6,
		  const double& precision = 1.0E-4 ) throw( int ):
      Transform::PartialInteg( low, high, precision ), cfuncc_()
    { this->setfunc( g ); };
    virtual ~PartialInteg(){};

    virtual Transform::PartialInteg::CFunc* getCFunc(){ return &cfuncc_; }
    
  private:
    
    class CCFunc: public Transform::PartialInteg::CFunc{
    public:
      CCFunc(): Transform::PartialInteg::CFunc(){};
      virtual ~CCFunc(){};
      virtual double operator()( const double& z )
      { if( getPIP() == 1 ) return (*getfunc())( z, getx() ) / z ;
      if( getPIP() == 2 ) return (*getfunc())( getx(), z ) / z ; }
    };

    CCFunc cfuncc_;
  };
};

#endif //  _IntegPolSIDIS_PartialInteg_hh_
