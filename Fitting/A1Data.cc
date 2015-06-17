//
// A1Data.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Tue Jan 27 13:09:46 2009 Yoshiyuki Miyachi
// Started on  Tue Jan 27 13:09:46 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <sstream>

#include "A1Data.hh"

#include <Utility/Arguments.hh>
#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;
using namespace Fit;

string A1Data::disKey  = "A1_";
string A1Data::sdisKey = "A1h_";

A1Data::A1Data() : 
  FitData(),
  vx_( 1, 0.0 ), x_( 0.0 ), vz_( 0 ), z_( 0.0 ), dz_( 0.0 ), a1h_( false ),
  f1_( 1.0 ), original_( 0.0 )
{
}

A1Data::A1Data( const Exp::DataSet& dset, const Exp::DataPoint& dp ) :
  FitData(),
  vx_( 1, 0.0 ), x_( 0.0 ), vz_( 0 ), z_( 0.0 ), dz_( 0.0 ), a1h_( false ),
  f1_( 1.0 ), original_( 0.0 )
{
  
  Exp::DataPoint& p = const_cast< Exp::DataPoint& >( dp );
  
  x_       = p.kine( "x" );
  q2_      = p.kine( "Q^2" );
  sig_     = p.data();
  dsig_    = p.error();
  point_   = &p;

  this->dataSet( const_cast< Exp::DataSet* >( &dset ) );
  
  if( a1h_ = A1Data::isSIDIS( p.info() ) ){
    vz_.resize( 1 );
    vz_[ 0 ] = p.kine( "z" );
    z_  = p.kine( "z" );
    dz_ = 0.0;
  } else {
    vz_.resize( 0 );
    z_  = 0.0;
    dz_ = 0.0;
  }
  
  // if neigher A1 or A1h, ignore
  it_ = A1Data::nType( ! a1h_, p.info() );
  ih_ = A1Data::hType( ! a1h_, p.info() );

#ifdef DEBUG_FITTING
  ARG_DETAIL << *this << endl;
#endif // DEBUG_FITTING
}


A1Data::A1Data( const A1Data& data ) : 
  FitData( data ),
  vx_( data.vx_ ), x_( data.x_ ),
  vz_( data.vz_ ), z_( data.z_ ), dz_( data.dz_ ),
  a1h_( data.a1h_ ), f1_( data.f1_ ),
  original_( data.original_ )
{
}

A1Data::~A1Data() {
}

A1Data& A1Data::operator=( const A1Data& data ) {
  if( &data != this ){
    
    dynamic_cast< FitData& >( *this ) = dynamic_cast< const FitData& >( data );
    
    vx_ = data.vx_;
    x_  = data.x_;
    
    vz_ = data.vz_;
    z_  = data.z_;
    dz_ = data.dz_;
    
    a1h_ = data.a1h_;
 
    f1_ = data.f1_;
    
    original_ = data.original_;
  }
}

string A1Data::header(){
  ostringstream os;
  os << FitData::header();
  os << setw( 12 ) << "xmin"<< flush;
  os << setw( 12 ) << "xmax"<< flush;
  os << setw( 12 ) << "x"   << flush;
  os << setw( 12 ) << "zmin"<< flush;
  os << setw( 12 ) << "zmax"<< flush;
  os << setw( 12 ) << "z"   << flush;
  return os.str();
}

namespace Fit {
  ostream& operator<<( ostream& os, const A1Data& data ){
    os << dynamic_cast< const FitData& >( data ) << flush;

    os << setw(12) << data.vx_[ 0 ] << flush;
    if( data.vx_.size() > 1 ) os << setw(12) << data.vx_[ 1 ] << flush;
    else os << setw(12) << "******" << flush;
    os << setw(12) << data.x_ << flush;
    
    if( data.a1h_ ){
      os << setw(12)  << data.vz_[ 0 ] << flush;
      if( data.vz_.size() == 1 ) {
	os << setw(12)  << "******" << flush;
      } else {
	os << setw(12)  << data.vz_[ 1 ] << flush;
      }
      os << setw(12)  << data.z_ << flush;
    } else {
      os << setw(12) << "*******" << flush;
      os << setw(12) << "*******" << flush;
      os << setw(12) << "*******" << flush;
    }
    return os;
  }
}

int A1Data::hType( const QCD::Flavor::HADRON& h ){

  switch( h ){
  case QCD::Flavor::PIp : return 0;
  case QCD::Flavor::KAp : return 1;
  case QCD::Flavor::REp : return 2;
  case QCD::Flavor::HAp : return 3;
  case QCD::Flavor::PIm : return 4;
  case QCD::Flavor::KAm : return 5;
  case QCD::Flavor::REm : return 6;
  case QCD::Flavor::HAm : return 7;
  }
  return 0;
}

int A1Data::hType( const string& name ){
  
  if( name == "PIp" )                 return 0;
  if( name == "KAp" )                 return 1;
  if( name == "REp" )                 return 2;
  if( name == "HAp" || name == "Hp" ) return 3;
  if( name == "PIm" )                 return 4;
  if( name == "KAm" )                 return 5;
  if( name == "REm" )                 return 6;
  if( name == "HAm" || name == "Hm" ) return 7;
  
  if( name == "ALL" )                 return 8;
  
  return 0;
}

int A1Data::nType( const Xsection::Charge::TARGET& n ){
  switch( n ){
  case Xsection::Charge::p : return 0;
  case Xsection::Charge::n : return 1;
  case Xsection::Charge::d : return 2;
  }
  return 0;
}

int A1Data::nType( const string& name ){
  if( name == "PRO" ) return 0;
  if( name == "NEU" ) return 1;
  if( name == "DEU" ) return 2;

  if( name == "ALL" ) return 3;

  return 0;
}

bool A1Data::isDIS( const std::string& dinfo ){
  return ( dinfo.substr( 0, disKey.size() )  == disKey  );
}

bool A1Data::isSIDIS( const std::string& dinfo ){
  return ( dinfo.substr( 0, sdisKey.size() )  == sdisKey  );
}

int A1Data::hType( bool dis, const std::string& dinfo ){
  return
    ( dis ? -1 : A1Data::hType( dinfo.substr( sdisKey.size() + 4, 3 ) ) );
}

int A1Data::nType( bool dis, const std::string& dinfo ){
  return 
    A1Data::nType( dinfo.substr( ( dis ? disKey : sdisKey ).size(), 3 ) );
}

string A1Data::hadronName( const int& i ){
  switch( i ){
  case -1: return "";
  case 0: return "PIp";
  case 1: return "KAp";
  case 2: return "REp";
  case 3: return "HAp";
  case 4: return "PIm";
  case 5: return "KAm";
  case 6: return "REm";
  case 7: return "HAm";
  }
  return "NDEF";
}

string A1Data::targetName( const int& i ){
  switch( i ){
  case 0: return "p";
  case 1: return "n";
  case 2: return "d";
  }
  return "NDEF";
}

