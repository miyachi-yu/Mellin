#include "ExpData.hh"
#include "DataSet.hh"
#include "Tranform/Pvalue.hh"
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace Exp;

string ExpData::xmlTag = "expdata";

ExpData::ExpData() :
  config_( ), data_( 0 ), version_(""), lambda_( 1.0 )
{
}

ExpData::ExpData( Arguments& args, const std::string& option ) :
  config_( args, option ), data_( 0 ), version_(""), lambda_( 1.0 )
{
  config_.dataSet( data_ );
  this->setAttribute();
}

ExpData::ExpData( const string& path ) :
  config_( path ), data_( 0 ), version_(""), lambda_( 1.0 )
{
  config_.dataSet( data_ );
  this->setAttribute();
}

ExpData::ExpData( const ExpData& data ) :
  config_(),
  data_( data.data_ ), version_( data.version_ ), 
  lambda_( data.lambda_ )
{
  this->resetPointers();
}

ExpData::~ExpData() {
}

void ExpData::setAttribute(){
  version_ = config_.version();
  lambda_  = config_.lambda();
  this->resetPointers();
}

ExpData& ExpData::operator=( const ExpData& data ){
  if( this == &data ) return *this;
  data_    = data.data_;
  version_ = data.version_;
  lambda_  = data.lambda_;
  this->resetPointers();
  return *this;
}

ExpData& ExpData::operator+=( const ExpData& data ){
  
  if( this == &data ) {
    ARG_LOG << "Ignore to add myself...." << endl;
    return *this;
  }
  
  if( this->lambda() != data.lambda() ){
    ARG_ERR << "Wegiht for chi2 calculation are differents." << endl;
    ARG_ERR << "Igonore to append the given ExpData object." << endl;
    return *this;
  }
  
  data_.insert( data_.end(), data.data_.begin(), data.data_.end() );
  
  if( this->version() != data.version() )
    version_ += ", " + data.version_;
  
  this->resetPointers();

  return *this;
}

string ExpData::DTD() {
  ostringstream ostr;
  
  ostr 
    << "<!ELEMENT " << ExpData::xmlTag << " (" << DataSet::xmlTag << "*)>" 
    << endl;
  
  ostr 
    << "<!ATTLIST " << ExpData::xmlTag << "\n"
    << "           version CDATA #IMPLIED"
    << "           lambda  CDATA #IMPLIED"
    << ">"
    << endl;
  
  return ostr.str();
}

string ExpData::xmlHeader() {
  ostringstream ostr;
  
  ostr 
    << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
    << "<!DOCTYPE " << ExpData::xmlTag << " [\n"
    << ExpData::DTD()
    << DataSet::DTD()
    << DataPoint::DTD()
    << "]>";
  
  return ostr.str();
}

vector< double > ExpData::findkin( const string& kin ){

  vector< double > tmp( 0 );
  Exp::Kinematic ref( kin, 0.0, "" );
  
  for( int i = 0; i < data_.size(); i++ ){
    for( int j = 0; j < data_[ i ].size(); j++ ){
      list< Exp::Kinematic > kine = data_[ i ][ j ].kinematic();
      list< Exp::Kinematic>::iterator itr = 
	find( kine.begin(), kine.end(), ref );
      
      if( itr == kine.end() ) continue;
      tmp.push_back( itr->data() );
      
    }
  }
  
  return tmp;
}

vector< double > ExpData::finddata( const string& kin ){
  
  vector< double > tmp( 0 );
  Exp::Kinematic ref( kin, 0.0, "" );
  
  for( int i = 0; i < data_.size(); i++ ){
    for( int j = 0; j < data_[ i ].size(); j++ ){
      list< Exp::Kinematic > kine = data_[ i ][ j ].kinematic();
      list< Exp::Kinematic>::iterator itr = 
	find( kine.begin(), kine.end(), ref );
      
      if( itr == kine.end() ) continue;

      tmp.push_back( data_[ i ][ j ].data() );
      
    }
  }
  
  return tmp;
}

void ExpData::summary( ostream& os, const SUMMARY_LEVEL& level ){

  string header = DataSet::summary_header();
  
  os << "+--------------------------------------------------------+" << endl;
  os << "|  ExpData summry                                        +" << endl;
  os << "+--------------------------------------------------------+" << endl;
  os << "   version:  " << this->version() << endl;
  os << "   lambda:   " << this->lambda() << endl;
  os << setw(3) << "" << header << endl;
  for( int i = 0; i < data_.size(); i++ ){
    os << setw(3) << i << data_[ i ].summary( level ) << endl;
  }
  
  for( int i = 0; i < 3 + header.size(); i++ ) os << "-" << flush;
  os << endl;
  os 
    << setw(3)  << ""
    << setw(35) << "Total"
    << setw(4)  << this->fittedDataPoint() 
    << "/" 
    << setw(4)  << this->dataPoint()
    << setw(12) << this->chiSquare() << flush;
  
  if( this->fittedDataPoint() > 0 ){
    Transform::Pvalue p( this->fittedDataPoint() );
    os << setw(12) << p( this->chiSquare() ) << flush;
  } else {
    os << setw(12) << 0.0 << flush;
  }

  os << endl;
}

ostream& Exp::operator<<( ostream& os, ExpData& ed ){
  os << ExpData::xmlHeader() << endl;
  os << "<" << ExpData::xmlTag 
     << " version=\"" << ed.version() << "\""
     << " lambda=\"" << ed.lambda() << "\""
     << " >" << endl;
  for( int i = 0; i < ed.data().size(); i++ ) os << ed.data()[i] << endl;
  os << "</" << ExpData::xmlTag << ">" << flush;
  return os;
}

// return the emNumber of the first Parameter, assuming 
// all emNumbers are the same.
int ExpData::emNumber() {
  for( int i = 0; i < data_.size(); i++ ){
    if( data_[ i ].emNumber() != -1 && 
	data_[ i ].emid()     != -1 ) return data_[ i ].emNumber();
  }
  // no Parameter object with error matrix is found. 
  // return negative value for the error matrix ID,
  // which means there are no errorMatrix defined.
  return -1;
}

double ExpData::chiSquare(){
  double c2 = 0.0;
  for( int i = 0; i < data_.size(); i++ ) c2 += data_[ i ].chiSquare();
  return c2;
}

int ExpData::dataPoint(){
  int n = 0;
  for( int i = 0; i < data_.size(); i++ ) n += data_[ i ].size();
  return n;
}

int ExpData::fittedDataPoint(){
  int n = 0;
  for( int i = 0; i < data_.size(); i++ ) n += data_[ i ].fittedDataPoint();
  return n;
}

void ExpData::disable( const std::vector< int >& id ){

  if( id.size() == 0 ) return;

  ARG_LOG 
    << "+----------------------------------------------------------+" << endl
    << "|          Disable DataSet object                          |" << endl
    << "+----------------------------------------------------------+" << endl
    << endl;
  
  for( int i = 0; i < id.size(); i++ ){
    if( id[ i ] < data_.size() ){
      DataSet& dset = data_[ id[ i ] ];
      ARG_LOG << setw( 35 ) << dset.info() << endl;
      dset.enable() = false;
    }
  }
  
}

void ExpData::enable( const std::vector< int >& id ){
  
  if( id.size() == 0 ) return;
  
  ARG_LOG 
    << "+----------------------------------------------------------+" << endl
    << "|          Enable DataSet object                           |" << endl
    << "+----------------------------------------------------------+" << endl
    << endl;
  
  for( int i = 0; i < id.size(); i++ ){
    if( id[ i ] < data_.size() ){
      DataSet& dset = data_[ id[ i ] ];
      ARG_LOG << setw( 35 ) << dset.info() << endl;
      dset.enable() = true;
    }
  }
  
}

void ExpData::remove( const std::vector< int >& id ){
  if( id.size() == 0 ) return;
  
  ARG_LOG 
    << "+----------------------------------------------------------+" << endl
    << "|          Remove DataSet object                           |" << endl
    << "+----------------------------------------------------------+" << endl
    << endl;
  
  for( int i = 0; i < id.size(); i++ ){
    if( id[ i ] < data_.size() ){
      DataSet& dset = data_[ id[ i ] ];
      ARG_LOG << setw( 35 ) << dset.info() << endl;
      dset.remove() = true;
    }
  }

  this->remove();
}

void ExpData::remove(){
  vector< DataSet >::iterator itr = data_.begin();
  while( itr != data_.end() ){
    if( itr->remove() ) itr = data_.erase( itr );
    else itr++;
  }
}


void ExpData::resetPointers(){
  for( int i = 0; i < data_.size(); i++ ){
    data_[ i ].expdata( this );
  }
}
