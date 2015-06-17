#include "Charge.hh"
#include "CKernel.hh"

using namespace std;
using namespace QCD;
using namespace Xsection;

Charge::Charge( CKernel* coeff, const TARGET& n ) :
  coeff_( coeff ),
  target_( n )
{
}

Charge::~Charge(){
}

double Charge::bare( const Flavor::PARTON& q ){
  
  static const double e1 = 4.0 / 9.0;
  static const double e2 = 1.0 / 9.0;
  static const double e3 = 5.0 / 18.0;
  
  switch( q ){
  case Flavor::tbar: 
  case Flavor::cbar: 
  case Flavor::c:    
  case Flavor::t:    return e1;
  case Flavor::bbar: 
  case Flavor::sbar: 
  case Flavor::s:    
  case Flavor::b:    return e2;
  case Flavor::dbar: 
  case Flavor::d:    return ( target_ == p ? e2 : ( target_ == n ? e1 : e3 ) );
  case Flavor::ubar: 
  case Flavor::u:    return ( target_ == p ? e1 : ( target_ == n ? e2 : e3 ) );
  case Flavor::g:    return this->sum(); 
    
  }
  return e1;
}

double Charge::sum(){

  int nf  = static_cast< int >( coeff_->nf() );
  double r = 0.0;
  for( int i = 1; i <= nf; i++ ) r += this->bare( Flavor::PARTON( i ) ) ;
  
  return r;
}

double Charge::operator()( const Flavor::PARTON& q ){
//     return this->bare( q ) / this->sum();
     return this->bare( q ); // this should be default!
}

string Charge::tname( const TARGET& t ){
  switch( t ){
  case Charge::p: return "pro";
  case Charge::n: return "neu";
  case Charge::d: return "deu";
  case Charge::any: return "any";
  }
  return "";
}

double Charge::single( const Flavor::PARTON& q ){
  
  static const double e1 = 2.0 / 3.0;
  static const double e2 = 1.0 / 3.0;
  
  switch( q ){
  case Flavor::tbar: 
  case Flavor::cbar:
  case Flavor::ubar:  return -1 * e1;
  case Flavor::t:    
  case Flavor::c:    
  case Flavor::u:     return e1;
  case Flavor::bbar: 
  case Flavor::sbar: 
  case Flavor::dbar:  return -1 * e2;
  case Flavor::b:   
  case Flavor::s:    
  case Flavor::d:     return e2; 
  case Flavor::g:     return 0.0; 
    
  }
  return e1;
}
