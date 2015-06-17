//
// Kinematic.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Sep 26 16:35:48 2008 Yoshiyuki Miyachi
// Started on  Fri Sep 26 16:35:48 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "Kinematic.hh"

using namespace std;
using namespace Exp;

Kinematic::Kinematic( ) :
  info_( "" ), data_( 0.0 ), unit_( "" )
{
}

Kinematic::Kinematic( const std::string& info,
		      const double& data,
		      const std::string& unit ) :
  info_( info ), data_( data ), unit_( unit )
{
}

Kinematic::Kinematic( const Kinematic& kine ) : 
  info_( kine.info_ ), data_( kine.data_ ), unit_( kine.unit_ )
{
}

Kinematic::~Kinematic(){
}

Kinematic& Kinematic::operator=( const Kinematic& kine ){
  if( this != &kine ){
    info_ = kine.info_;
    data_ = kine.data_;
    unit_ = kine.unit_;
  }
  return *this;
}

bool Kinematic::operator==( const Kinematic& kine ) {
  return ( info_ == kine.info_ );
}

bool Kinematic::operator>( const Kinematic& kine ) {
  return ( info_ > kine.info_ );
}

bool Kinematic::operator<( const Kinematic& kine ) {
  return ( info_ < kine.info_ );
}

namespace Exp {
  ostream& operator<<( ostream& os, const Kinematic& kine ){
    os << "<data type=\"kine\" value=\"" << kine.data() << "\" "
       << "info=\"" << kine.info() << "\" " 
       << "unit=\"" << kine.unit() << "\" />";
    return os;
  }
}
