#include "DataPoint.hh"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <Utility/Arguments.hh>

using namespace std;
using namespace Exp;

string DataPoint::xmlTag = "point"; 

string DataPoint::DTD(){
  ostringstream ostr;
  
  ostr << "<!ELEMENT " << DataPoint::xmlTag << " (data*, ems*)>" << endl;
  ostr << "<!ATTLIST " << DataPoint::xmlTag 
       << "                    stat (fitted) #IMPLIED \n"
       << "                    emid CDATA    #IMPLIED \n"
       << ">" 
       << endl;
  ostr 
    << "<!ELEMENT data EMPTY>\n"
    << "<!ATTLIST data value CDATA #REQUIRED \n"
    << "               type (data|stat|syst|kine|theo|terr|scale1|scale2|chi2) #REQUIRED \n"
    << "               info CDATA #REQUIRED \n"
    << "               unit CDATA #IMPLIED >"
    << endl;
  
  return ostr.str();
}

DataPoint::DataPoint() :
  info_( "" ), data_( 0.0 ), stat_( 0.0 ), syst_( 0 ), systInfo_( 0 ), 
  unit_( "" ),
  kinematic_( 0 ),
  model_( 0.0 ), modelError_( 0.0 ), fitted_( false ),
  scale1_( 0.0 ), scale2_( 0.0 ),
  emNumber_( 0 ), emid_( -1 ), ems_( 0 ), chi2_( 0.0 ),
  dset_( NULL )
{
}

DataPoint::DataPoint( const DataPoint& dp ) :
  info_( dp.info_ ),
  data_( dp.data_ ), stat_( dp.stat_ ),
  syst_( dp.syst_ ), systInfo_( dp.systInfo_ ), unit_( dp.unit_ ),
  kinematic_( dp.kinematic_ ),
  model_( dp.model_ ), modelError_( dp.modelError_ ),
  fitted_( dp.fitted_ ),
  scale1_( dp.scale1_ ), scale2_( dp.scale2_ ),
  emNumber_( dp.emNumber_ ), emid_( dp.emid_ ), ems_( dp.ems_ ), 
  chi2_( dp.chi2_ ),
  dset_( dp.dset_ )
{
}

DataPoint::~DataPoint() {
}

DataPoint& DataPoint::operator=( const DataPoint& dp ){
  if( this != &dp ){
    info_ = dp.info_;
    data_ = dp.data_;
    stat_ = dp.stat_;
    syst_ = dp.syst_;
    systInfo_ = dp.systInfo_;
    unit_ = dp.unit_;
    kinematic_ = dp.kinematic_;
    model_ = dp.model_;
    modelError_ = dp.modelError_;
    fitted_ = dp.fitted_;
    scale1_ = dp.scale1_;
    scale2_ = dp.scale2_;
    emNumber_ = dp.emNumber_;
    emid_     = dp.emid_;
    ems_      = dp.ems_;
    chi2_     = dp.chi2_;
    dset_     = dp.dset_;
  }
  return *this;
}

bool DataPoint::operator==( const DataPoint& dp ) {
  
  // ckech data type
  if( info_ != dp.info_ ) return false;
  
  // check kinematic condition
  list< Kinematic > kine = dp.kinematic();
  for( list< Kinematic >::iterator itr = kine.begin();
       itr != kine.end(); itr++ ){
    
    if( find( kinematic_.begin(), kinematic_.end(), *itr ) ==
	kinematic_.end() ) return false;
  }
  return true;
  //  return data_ == dp.data_;
}

bool DataPoint::operator<( const DataPoint& dp ) {
  if( info_ != dp.info_ ) return info_ < dp.info_;
  
  return data_ < dp.data_;
}

bool DataPoint::operator>( const DataPoint& dp ){
  if( info_ != dp.info_ ) return info_ < dp.info_;
  return data_ > dp.data_;
}

double DataPoint::syst( DataPoint::SYSTMODE mode ){ 
  double sum = 0.0 ;
  for( int i = 0; i < syst_.size(); i++ )
    sum += ( mode == lin ? syst_[ i ] : pow( syst_[ i ], 2 ) );
  if( mode == quad && sum > 0.0 ) sum = sqrt( sum );
  return sum;
}

void DataPoint::syst( const double& value, const std::string& info ) {
  syst_.push_back( value );
  systInfo_.push_back( info );
}

void DataPoint::kine( const double& value, const std::string& info,
		      const std::string& unit ){
  kinematic_.push_back( Kinematic( info, value, unit ) );
  kinematic_.sort();
}

bool DataPoint::hasKine( const std::string& info ) {
  Kinematic ref( info, 0.0, "" );
  list< Kinematic >::iterator itr = 
    find( kinematic_.begin(), kinematic_.end(), ref );
  return ( itr != kinematic_.end() );
}

double DataPoint::kine( const std::string& info ) {
  
  Kinematic ref( info, 0.0, "" );
  list< Kinematic >::iterator itr = 
    find( kinematic_.begin(), kinematic_.end(), ref );

  if( itr == kinematic_.end() ){
    ARG_ERR << "no " << info << " : return 0.0" << endl; 
    return 0.0;
  }
  
  return itr->data();
}

double DataPoint::error(){
  double error = stat_ * stat_;
  for( int i = 0; i < syst_.size(); i++ )
    error += syst_[ i ] * syst_[ i ];
  return ( error > 0.0 ? sqrt( error ) : 0.0 );
}

namespace Exp {
  
  ostream& operator<<( ostream& os, DataPoint& dp ){
    os << "<point" << flush;
    if( dp.fitted() ) os << " stat=\"fitted\"" << flush;
    if( dp.emid() > -1 ){
      os << " emid=\"" << flush;
      if( dp.emNumber() > 0 ) os << dp.emNumber() << "-" << flush;
      os << dp.emid() << "\" " << flush;
    }
    os << " >" << endl;
    
    list< Kinematic >& kinematic = dp.kinematic();
    for( list< Kinematic >::iterator itr = kinematic.begin();
	 itr != kinematic.end(); itr++ ){
      os << *itr << endl;
    }
    
    os << "<data type=\"data\" value=\"" << dp.data() << "\" "
       << "info=\"" << dp.info() << "\" "
       << "unit=\"" << dp.unit() << "\" />" << endl;
    
    os << "<data type=\"stat\" value=\"" << dp.stat() << "\" "
       << "info=\"stat\" "
       << "unit=\"" << dp.unit() << "\" />" << endl;
    
    vector< double >& sys = dp.syst();
    vector< string >& sinf = dp.systInfo();
    for( int i = 0; i < sys.size(); i++ ){
      os << "<data type=\"syst\" value=\"" << sys[i] << "\" "
	 << "info=\"" << sinf[i] << "\" "
	 << "unit=\"" << dp.unit() << "\" />" << endl;
    }
    
    os << "<data type=\"theo\" value=\"" << dp.model() << "\" "
       << "info=\"" << dp.info() << "\" "
       << "unit=\"" << dp.unit() << "\" />" << endl;
    
    os << "<data type=\"terr\" value=\"" << dp.modelError() << "\" "
       << "info=\"" << dp.info() << "\" "
       << "unit=\"" << dp.unit() << "\" />" << endl;
    
    os << "<data type=\"scale1\" value=\"" << dp.scale1() 
       << "\" info=\"scale factor 1\" unit=\"\"/>" << endl;
    
    os << "<data type=\"scale2\" value=\"" << dp.scale2() 
       << "\" info=\"scale factor 2\" unit=\"\"/>" << endl;
    
    os << "<data type=\"chi2\"   value=\"" << dp.chiSquare() 
       << "\" info=\"chi2 in fit\" unit=\"\"/>" << endl;
    
    // ------------- error matrix handling -------------- //
    for( int i = 0; i < dp.ems().size(); i++ ){
      os << "<ems emid=\"" << i << "\" "
	 << "value=\"" << dp.ems()[ i ] << "\"" << " />" << endl;
    }
    
    os << "</point>" << flush;

    return os;
  }
}

string DataPoint::summary_header(){
  ostringstream ostr;
  ostr << setw(12) << "data" << setw(12) << "stat" << setw(12) << "model";
  ostr << setw(12) << "chi2" ;
  for( int i = 0; i < systInfo_.size(); i++ ) ostr << setw(12) << systInfo_[ i ] ;
  for( list< Kinematic >::iterator itr = kinematic_.begin();
       itr != kinematic_.end(); itr++ ) ostr << setw(12) << itr->info();
  return ostr.str();
}

string DataPoint::summary(){
  ostringstream ostr;

  ostr << setw(12) << data_ << setw(12) << stat_ << setw(12) << model_;
  ostr << setw(12) << chi2_;
  for( int i = 0; i < syst_.size(); i++ ) ostr << setw(12) << syst_[ i ];
  for( list< Kinematic >::iterator itr = kinematic_.begin();
       itr != kinematic_.end(); itr++ ) ostr << setw(12) << itr->data();

  return ostr.str();
}
