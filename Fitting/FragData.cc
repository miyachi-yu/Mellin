//
// FragData.cc
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

#include "FragData.hh"

#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;
using namespace Fit;

string FragData::Key = "e+e-_";

FragData::FragData() : 
  FitData(),
  vz_( 1, 0.0 ), z_( 0.0 ), dz_( 0.0 )
{
}

FragData::FragData( Exp::DataSet& d, Exp::DataPoint& p ) : 
  FitData(),
  vz_( 2, 0.0 ), z_( 0.0 ), dz_( 0.0 )
{
  
  vz_[ 0 ] = p.kine( "zmin" );
  vz_[ 1 ] = p.kine( "zmax" );
  z_       = 0.5 * ( vz_[ 0 ] + vz_[ 1 ] );
  if( ( dz_ = vz_[ 1 ] - vz_[ 0 ] ) == 0.0 ) vz_.resize( 1 );
  
  q2_      = p.kine("Q^2");
  sig_     = p.data();
  it_      = FragData::get_nType( p.info() );
  ih_      = FragData::get_hType( p.info() );
  
  point_   = &p;
  dsig_    = FragData::error2();
  
  this->dataSet( &d );
  
}

FragData::FragData( const FragData& data ) : 
  FitData( data ),
  vz_( data.vz_ ), z_( data.z_ ), dz_( data.dz_ )
{
}

FragData::~FragData() {
}

FragData& FragData::operator=( const FragData& data ) {
  if( &data != this ){
    dynamic_cast< FitData& >( *this ) = dynamic_cast< const FitData& >( data );
    vz_ = data.vz_;
    z_  = data.z_;
    dz_ = data.dz_;
  }
}

string FragData::header(){
  ostringstream os;
  os << FitData::header();
  os << setw(12) << "zmin" << flush;
  os << setw(12) << "zmax" << flush;
  return os.str();
}


int FragData::hType( const QCD::Flavor::HADRON& h ){

  switch( h ){
    
  case QCD::Flavor::PIp : return 0;
  case QCD::Flavor::KAp : return 1;
  case QCD::Flavor::PRp : return 2;
  case QCD::Flavor::REp : return 3;
  case QCD::Flavor::HAp : return 4;

  case QCD::Flavor::PIm : return 0;
  case QCD::Flavor::KAm : return 1;
  case QCD::Flavor::PRm : return 2;
  case QCD::Flavor::REm : return 3;
  case QCD::Flavor::HAm : return 4;
    
  case QCD::Flavor::PI0 : return 5;
    
  }
  return 0;
}

int FragData::hType( const std::string& name ){
  
  if( name.substr( 0, 2 ) == "PI" ) return 0;
  if( name.substr( 0, 2 ) == "KA" ) return 1;
  if( name.substr( 0, 2 ) == "PR" ) return 2;
  if( name.substr( 0, 2 ) == "RE" ) return 3;
  if( name.substr( 0, 2 ) == "HA" ) return 4;
  if( name.substr( 0, 2 ) == "P0" ) return 5;

  if( name == "ALL" )               return 6; // number of hadron type
 
  return 0;
}

int FragData::get_hType( const std::string& dinfo ){
  return FragData::hType( dinfo.substr( FragData::Key.size(), 2 ) );
}

int FragData::get_nType( const std::string& dinfo ) {
  return FragData::nType( dinfo.substr( FragData::Key.size() + 2 ) );
}

int FragData::nType( const std::string& name ) {
  
  // FragXse
  if( name == ""    )         return 0;
  if( name == "_c_enriched" ) return 1;
  if( name == "_b_enriched" ) return 2;
  if( name == "ALL" )         return 3;  // number of data type
  
  return 0;
}

int FragData::nType( const Xsection::Charge::TARGET& n ){
  switch( n ){
  case Xsection::Charge::p : return 0;
  case Xsection::Charge::n : return 1;
  case Xsection::Charge::d : return 2;
  }
  return 0;
}

string FragData::hName( const int& i ){
  switch( i ){
  case  0: return "PIpm";
  case  1: return "KApm";
  case  2: return "PRpm";
  case  3: return "REpm";
  case  4: return "HApm";
  case  5: return "P0";
  case  6: return "ALL";
  }
  return "NDEF";
}

double FragData::error2(){

  if( point_ == NULL ) return 0.0;
  
  double e = pow( point_->stat(), 2 );
  for( int i = 0; i < point_->syst().size(); i++ ) 
    e += pow( point_->syst()[ i ], 2 );
  
  return ( e > 0.0 ? sqrt( e ) : 0.0 );
}

namespace Fit {
  
  ostream& operator<<( ostream& os, const FragData& data ){
    os << dynamic_cast< const FitData& >( data ) << flush;
    os << setw(12)  << data.vz_[ 0 ] << flush;
    if( data.vz_.size() == 1 ) {
      os << setw(12)  << "******" << flush;
    } else {
      os << setw(12)  << data.vz_[ 1 ] << flush;
    }
  }
  
}

bool FragData::check( const Exp::DataPoint& p ){
  return( p.info().substr( 0, FragData::Key.size() ) == FragData::Key );
}
