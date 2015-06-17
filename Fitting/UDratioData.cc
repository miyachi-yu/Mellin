//
// UDratioData.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Tue Jan 27 13:09:46 2009 Yoshiyuki Miyachi
// Started on  Tue Jan 27 13:09:46 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "UDratioData.hh"

#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;
using namespace Fit;

string UDratioData::Key = "dbar/ubar";

UDratioData::UDratioData() : 
  FitData(), x_( 0.0 )
{
}

UDratioData::UDratioData( Exp::DataSet& d, Exp::DataPoint& p ) : 
  FitData(), x_( 0.0 )
{
  
  x_       = p.kine( "x" );
  q2_      = pow( p.kine("M"), 2 );
  sig_     = p.data();
  point_   = &p;
  dsig_    = UDratioData::error2();
  
  this->dataSet( &d );
  
}

UDratioData::UDratioData( const UDratioData& data ) : 
  FitData( data ),
  x_( data.x_ )
{
}

UDratioData::~UDratioData() {
}

UDratioData& UDratioData::operator=( const UDratioData& data ) {
  if( &data != this ){
    dynamic_cast< FitData& >( *this ) = dynamic_cast< const FitData& >( data );
    x_ = data.x_;
  }
}

string UDratioData::header(){
  ostringstream os;
  os << FitData::header();
  os << setw(12) << "x" << flush;
  return os.str();
}

double UDratioData::error2(){

  if( point_ == NULL ) return 0.0;
  
  double e = pow( point_->stat(), 2 );
  for( int i = 0; i < point_->syst().size(); i++ ) 
    e += pow( point_->syst()[ i ], 2 );
  
  return ( e > 0.0 ? sqrt( e ) : 0.0 );
}

namespace Fit {
  
  ostream& operator<<( ostream& os, const UDratioData& data ){
    os << dynamic_cast< const FitData& >( data ) << flush;
    os << setw(12) << data.x_ << flush;
  };
  
};

bool UDratioData::check( const Exp::DataPoint& p ){
  return( p.info().substr( 0, UDratioData::Key.size() ) == UDratioData::Key );
}
