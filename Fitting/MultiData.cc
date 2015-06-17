/*!
  \brief  class implementation of multiplicity data for Fitting
 */
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "MultiData.hh"

#include <Utility/Arguments.hh>
#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;
using namespace Fit;

string MultiData::Key  = "Multi_";
//string MultiData::Key  = "MultiQ2_";
string MultiData::KeyZ = "Multi_Z_";

MultiData::MultiData() : 
  FitData(),
  vz_( 1, 0.0 ), z_( 0.0 ), dz_( 0.0 ), zave_( 0.0 ), 
  vx_(1, 0.0 ), xmin_( 0.0 ), xmax_( 0.0 ), xave_( 0.0 ),
  denom_( 1.0 ), s_( 0.0 ),
  useAverage_( Utility::Arguments::ref().hasOpt( "Ave" ) )
{
}

MultiData::MultiData( Exp::DataSet& d, Exp::DataPoint& p ) : 
  FitData(),
  vz_( 2, 0.0 ), z_( 0.0 ), dz_( 0.0 ), zave_( 0.0 ), 
  vx_( 2, 0.0 ), xmin_( 0.0 ), xmax_( 0.0 ), xave_( 0.0 ),
  denom_( 1.0 ), s_( 0.0 ),
  useAverage_( Utility::Arguments::ref().hasOpt( "Ave" ) )
{
  
  vz_[ 0 ] = p.kine( "zmin" );
  vz_[ 1 ] = p.kine( "zmax" );
  
  z_ = 0.5 * ( vz_[ 0 ] + vz_[ 1 ] );
  if( vz_[ 1 ] == vz_[ 0 ] ){
    dz_ = 0.0;
    vz_.resize( 1 );
  } else {
    dz_ = vz_[ 1 ] - vz_[ 0 ];
  }

  if( ( ! useAverage_ ) && p.hasKine( "Q^2 max" ) && p.hasKine( "Q^2 min" ) ) {
    q2_ = 0.5 * ( p.kine( "Q^2 max" ) + p.kine( "Q^2 min" ) );
  } else {
    q2_ = ( p.hasKine( "Q^2 ave" ) ? p.kine( "Q^2 ave" ) : p.kine( "Q2" ) );
  }
  
  zave_    = p.kine( "zave" );
  
  s_       = p.kine( "s" );
  
  xave_    = p.kine( "x" );
  xmin_    = p.hasKine( "xave" ) ? p.kine( "xave" ) : p.kine( "xmin" );
  xmax_    = p.hasKine( "xave" ) ? p.kine( "xave" ) : p.kine( "xmax" );
  if( xmin_ == xmax_ ){
    vx_.resize( 1 );
    vx_[ 0 ] = xmin_ ;
  } else {
    vx_[ 0 ] = xmin_ ;
    vx_[ 1 ] = xmax_ ;
  }

  sig_     = p.data();
  it_      = MultiData::get_nType( p.info() );
  ih_      = MultiData::get_hType( p.info() );
  
  point_   = &p;
  dsig_    = this->error2();
  
  this->dataSet( &d );
  
}

MultiData::MultiData( const MultiData& data ) : 
  FitData( data ),
  vz_( data.vz_ ), z_( data.z_ ), dz_( data.dz_ ),
  zave_( data.zave_ ), 
  xmin_( data.xmin_ ), xmax_( data.xmax_ ), xave_( data.xave_ ),
  denom_( data.denom_ ),
  useAverage_( data.useAverage_ )
{
}

MultiData::~MultiData() {
}

MultiData& MultiData::operator=( const MultiData& data ) {
  if( &data != this ){
    dynamic_cast< FitData& >( *this ) = dynamic_cast< const FitData& >( data );
    vz_    = data.vz_;
    z_     = data.z_;
    dz_    = data.dz_;
    zave_  = data.zave_;
    xmin_  = data.xmin_;
    xmax_  = data.xmax_;
    xave_  = data.xave_;
    denom_ = data.denom_;
    useAverage_ = data.useAverage_;
  }
}

string MultiData::header( ){
  ostringstream os;
  os << FitData::header();
  os << setw(12) << "zmin" << flush;
  os << setw(12) << "zmax" << flush;
  os << setw(12) << "zave" << flush;
  os << setw(6)  << "xmin" << flush;
  os << setw(6)  << "xmax" << flush;
  os << setw(12) << "xave" << flush;
  return os.str();
}

namespace Fit {
  ostream& operator<<( ostream& os, const MultiData& data ){
    os << dynamic_cast< const FitData& >( data ) << flush;
    os << setw(12)  << data.vz_[ 0 ] << flush;
    if( data.vz_.size() == 1 ) {
      os << setw(12)  << "******" << flush;
    } else {
      os << setw(12)  << data.vz_[ 1 ] << flush;
    }
    os << setw(12) << data.zave_ << flush;
    os << setw(6) << data.xmin_ << flush;
    os << setw(6) << data.xmax_ << flush;
    os << setw(12) << data.xave_ << flush;
    return os;
  }
}

int MultiData::get_hType( const std::string& dinfo ){
  return MultiData::hType( dinfo.substr( MultiData::Key.size() + 4 ) );
}

int MultiData::get_nType( const std::string& dinfo ) {
  return MultiData::nType( dinfo.substr( MultiData::Key.size(), 3 ) );
}

int MultiData::hType( const QCD::Flavor::HADRON& h ){
  switch( h ){
    
  case QCD::Flavor::PIp : return 0;
  case QCD::Flavor::KAp : return 1;
  case QCD::Flavor::PRp : return 2;
  case QCD::Flavor::REp : return 3;
  case QCD::Flavor::HAp : return 4;
    
  case QCD::Flavor::PIm : return 5;
  case QCD::Flavor::KAm : return 6;
  case QCD::Flavor::PRm : return 7;
  case QCD::Flavor::REm : return 8;
  case QCD::Flavor::HAm : return 9;

  case QCD::Flavor::PI0 : return 10;

  }
  return 0;
}

int MultiData::hType( const std::string& name ){
  
  if( name == "PI" || name == "PIp" ) return 0;
  if( name == "KA" || name == "KAp" ) return 1;
  if( name == "PR" || name == "PRp" ) return 2;
  if( name == "RE" || name == "REp" ) return 3;
  if( name == "HA" || name == "HAp" || name == "Hp" ) return 4;
  
  if( name == "PIm" ) return 5;
  if( name == "KAm" ) return 6;
  if( name == "PRm" ) return 7;
  if( name == "REm" ) return 8;
  if( name == "HAm" || name == "Hm" ) return 9;
  
  if( name == "P0" || name == "PI0" )  return 10;
  
  if( name == "ALL" )  return 11; // number of hadron type
  
  return 0;
}

int MultiData::nType( const std::string& name ) {
  
  // Multiplicities
  if( name == "PRO" ) return 0;
  if( name == "NEU" ) return 1;
  if( name == "DEU" ) return 2;

  if( name == "ALL" ) return 3;  // number of data type

  return 0;
}

int MultiData::nType( const Xsection::Charge::TARGET& n ){
  switch( n ){
  case Xsection::Charge::p : return 0;
  case Xsection::Charge::n : return 1;
  case Xsection::Charge::d : return 2;
  }
  return 0;
}

string MultiData::hName( const int& i ){
  
  switch( i ){
  case  0: return "PIp";
  case  1: return "KAp";
  case  2: return "PRp";
  case  3: return "REp";
  case  4: return "HAp";
  case  5: return "PIm";
  case  6: return "KAm";
  case  7: return "PRm";
  case  8: return "REm";
  case  9: return "HAm";
  case 10: return "P0";
  case 11: return "ALL";
  }

  return "NDEF";
}


double MultiData::error2(){

  double e = pow( point_->stat(), 2 );
  
  // HERMES data has RICH systematic error in 1st and 2nd elements.
  if( point_->syst().size() > 1 &&
      point_->systInfo()[ 0 ].substr( 0, 4 ) == "RICH" &&
      point_->systInfo()[ 1 ].substr( 0, 4 ) == "RICH" ) { 
    
    e += ( abs( point_->syst()[ 0 ] ) >= abs( point_->syst()[ 1 ] ) ? 
	   pow( point_->syst()[ 0 ], 2 ) :  // RICH upper //
	   pow( point_->syst()[ 1 ], 2 ) ); // RICH lower //
    
    if( point_->syst().size() > 2 ) e += pow( point_->syst()[ 2 ], 2 ); // MC // 
    
    if( point_->info().substr( 0, MultiData::KeyZ.size() ) == MultiData::KeyZ &&
	point_->syst().size() > 4 ){
      
      e += 	  
	( abs( point_->syst()[ 3 ] ) >= abs( point_->syst()[ 4 ] ) ? 
	  pow( point_->syst()[ 3 ], 2 ) :
	  pow( point_->syst()[ 4 ], 2 ) ); // evo //
      
    }
  } else {
    for( int i = 0; i < point_->syst().size(); i++ ) e += pow( point_->syst()[ i ], 2 );
  }
  
  return ( e > 0.0 ? sqrt( e ) : 0.0 );
}


bool MultiData::check( const Exp::DataPoint& p ){
  const string& info = p.info();
  return( info.substr( 0, MultiData::Key.size() ) == MultiData::Key ||
	  info.substr( 0, MultiData::KeyZ.size() ) == MultiData::KeyZ  );
}
