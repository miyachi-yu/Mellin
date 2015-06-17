#include "Charge.hh"
#include "CoeffFunc.hh"

using namespace std;
using namespace polSIDIS;

Charge::Charge( CoeffFunc* coeff, 
		const Charge::TARGET& n ) :
     Xsection::Charge( coeff, n )
{
}

Charge::~Charge(){
}

double Charge::bare( const Flavor::PARTON& q ){
  
//  static const double e0 = 1.0;
  static const double e1 = 4.0 / 9.0;
  static const double e2 = 1.0 / 9.0;
  
  switch( q ){
  case Flavor::tbar: 
  case Flavor::cbar: 
  case Flavor::c:    
  case Flavor::t:
  case Flavor::ubar: 
  case Flavor::u:     return e1;
  case Flavor::bbar: 
  case Flavor::sbar: 
  case Flavor::s:    
  case Flavor::b:
  case Flavor::dbar: 
  case Flavor::d:    return e2;
  case Flavor::g:    return 0.0; 
    
  }
  return e1;
}
