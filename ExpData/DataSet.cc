
#include "DataSet.hh"
#include "ExpData.hh"
#include "Tranform/Pvalue.hh"

#include <sstream>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace Exp;

#ifdef DATA_DIR_PATH
string Exp::data_path = DATA_DIR_PATH;
#else
string Exp::data_path = "../Data/";
#endif

string DataSet::xmlTag = "dataset";

string DataSet::DTD() {
  ostringstream ostr;
  
  ostr
    << "<!ELEMENT " << DataSet::xmlTag 
    << " (ems*, " << DataPoint::xmlTag << "*)"
    << ">"
    << endl;
  
  ostr
    << "<!ATTLIST " << DataSet::xmlTag << "\n"
    << "            info  CDATA #REQUIRED\n" 
    << "            ref   CDATA #IMPLIED\n" 
    << "            cite  CDATA #IMPLIED\n" 
    << "            norm  CDATA #IMPLIED\n" 
    << "            ncoef CDATA #IMPLIED\n" 
    << "            ncerr CDATA #IMPLIED\n" 
    << "            nstat CDATA #IMPLIED\n" 
    << "            emid  CDATA #IMPLIED" 
    << "            stat  (enable|disable) #IMPLIED\n" 
    << " >"
    << endl;
  
  ostr 
    << "<!ELEMENT ems EMPTY >\n"
    << "<!ATTLIST ems\n" 
    << "          emid   CDATA  #REQUIRED\n"
    << "          value  CDATA  #REQUIRED >" 
    << endl;
  
  return ostr.str();
}


DataSet::DataSet() :
  vector< DataPoint >(),
  info_( "" ), ref_( "" ), bib_( "" ), chi2_( 0.0 ), enable_( true ),
  norm_( 0.0 ), ncoef_( 0.0 ), ncerr_( 0.0 ), ncstat_( "fixed" ), 
  emNumber_( 0 ), emid_( -1 ), ems_( 0 ), remove_( false ),
  expdata_( NULL )
{
}

DataSet::DataSet( const DataSet& dset ) : 
  vector< DataPoint >( dset ), 
  info_( dset.info_ ), ref_( dset.ref_ ), bib_( dset.bib_ ), 
  chi2_( dset.chi2_ ), enable_( dset.enable_ ),
  norm_( dset.norm_ ), ncoef_( dset.ncoef_ ),
  ncstat_( dset.ncstat_ ),
  ncerr_( dset.ncerr_ ), 
  emNumber_( dset.emNumber_ ), emid_( dset.emid_ ), ems_( dset.ems_ ),
  remove_( dset.remove_ ),
  expdata_( dset.expdata_ )
{
  for( int i = 0; i < this->size(); i++ ) (*this)[ i ].dataSet( this );
}

DataSet::~DataSet(){
}

DataSet& DataSet::operator=( const DataSet& dset ){
  if( this != &dset ){
    
    this->clear();
    this->assign( dset.begin(), dset.end() );
    
    info_   = dset.info_;
    ref_    = dset.ref_;
    
    chi2_   = dset.chi2_;
    enable_ = dset.enable_;
    
    norm_   = dset.norm_;
    ncoef_  = dset.ncoef_;
    ncerr_  = dset.ncerr_;
    ncstat_ = dset.ncstat_;
    
    emNumber_ = dset.emNumber_;
    emid_     = dset.emid_;
    ems_      = dset.ems_;

    remove_  = dset.remove_;
    expdata_ = dset.expdata_;

    for( int i = 0; i < this->size(); i++ ) (*this)[ i ].dataSet( this );
    
  }
  return *this;
}

double DataSet::normalization() {
  if( norm_ == 0.0 || ncoef_ == 0.0 ) return 1.0;
  return 1.0 + norm_ * ncoef_;
}

double DataSet::normalizationError() {
  if( norm_ == 0.0 || ncoef_ == 0.0 || ncerr_ == 0.0 ) return 0.0;
  return norm_ * ncerr_;
}

double DataSet::chiSquare() {
  
  chi2_ = 0.0;
  
  for( DataSet::iterator itr = this->begin(); 
       itr != this->end();
       itr++ ){
    if( itr->error() == 0.0 || ! itr->fitted() ) continue;
    chi2_ += itr->chiSquare();
    //      pow( ( itr->data() - this->normalization() * itr->model() ) /
    //	   itr->error(), 2 );    
  }
  
  return chi2_;
}

int DataSet::fittedDataPoint(){
  int n = 0;
  for( DataSet::iterator itr = this->begin(); 
       itr != this->end(); itr++ )
    if( itr->fitted() ) n++ ;
  return n;
}


string DataSet::abbreviation() const {

  if( info_ == "TASSO_44_1989_PIpm" )              return "TASSO(44)_PIpm";
  if( info_ == "TASSO_34_1989_PIpm" )              return "TASSO(34)_PIpm";
  if( info_ == "TPC_29_1988_PIpm" )                return "TPC_PIpm";
  if( info_ == "SLD__inclusive_91.2_1999_PIpm" )   return "SLD_PIpm";
  if( info_ == "SLD_c_91.2_1999_PIpm" )            return "SLD(c)_PIpm";
  if( info_ == "SLD__b_91.2_1999_PIpm" )           return "SLD(b)_PIpm";
  if( info_ == "ALEPH__91.2_1995_PIpm" )           return "ALEPH_PIpm";
  if( info_ == "OPAL_91.2_1994_PIpm" )             return "OPAL_PIpm";
  if( info_ == "DELPHI_inclusive_91.2_1998_PIpm" ) return "DELPHI_PIpm";
  if( info_ == "DELPHI_b_91.2_1998_PIpm" )         return "DELPHI(b)_PIpm";

  if( info_ == "TPC_29_1985_PI0" )                 return "TPC_PI0";
  if( info_ == "L3_91.2_1994_PI0" )                return "L3_PI0";
  if( info_ == "ALEPH__91.2_2000_PI0" )            return "ALEPH_PI0";
  if( info_ == "OPAL_91.2_1998_PI0" )              return "OPAL_PI0";
  if( info_ == "DELPHI_91.2_1996_PI0" )            return "DELPHI_PI0";
  if( info_ == "TASSO_44_1989_PI0" )               return "TASSO_PI0";

  if( info_ == "HERMES_Multi_PRO_PIp" )            return "HERMES_P_PIp";
  if( info_ == "HERMES_Multi_PRO_PIm" )            return "HERMES_P_PIm";
  if( info_ == "HERMES_Multi_DEU_PIp" )            return "HERMES_D_PIp";
  if( info_ == "HERMES_Multi_DEU_PIm" )            return "HERMES_D_PIm";

  return info_;
}


string DataSet::summary_header(){
  ostringstream ostr;
  ostr << setw(35) << "Name"
       << setw(9)  << "Data"
       << setw(12) << "chi^2"
       << setw(12) << "p-value"
       << setw(28) << "Normalization"
       << setw(2)  << ""
    ;
  return ostr.str();
}

string DataSet::summary( const SUMMARY_LEVEL& level ){
  ostringstream ostr;
  ostr << setw(35) << info_
       << setw(4)  << this->fittedDataPoint() 
       << "/" 
       << setw(4)  << this->size()
       << setw(12) << this->chiSquare();
  
  if( this->fittedDataPoint() > 0 ) {
    Transform::Pvalue p( this->fittedDataPoint() );
    ostr << setw(12) << p( this->chiSquare() );
  } else {
    ostr << setw(12) << 0.0 ;
  }

  ostr << setw(12) << this->normalization() << " +- "
       << setw(12) << this->normalizationError()
       << setw(2) << ( this->enable() ? "E" : "D" )
    ;


  if( level == DETAIL && this->size() > 0 ){
    ostr << endl;
    ostr << this->begin()->summary_header() << endl;
    for( DataSet::iterator itr = this->begin(); itr != this->end(); itr++ )
      ostr << itr->summary() << endl;
  }

  return ostr.str();
}

namespace Exp {
  ostream& operator<<( ostream& os, DataSet& set ){
    os << "<" << DataSet::xmlTag << flush;
    os << " info=\"" << set.info() << "\" " << flush;
    if( set.ref() != "" ) os << "ref=\"" << set.ref() << "\" " << flush;
    if( set.norm() != 0.0 ) { 
      os << "norm=\"" << set.norm() << "\" " << flush;
      os << "ncoef=\"" << set.ncoef() << "\" " << flush;
      os << "ncerr=\"" << set.ncerr() << "\" " << flush;
      os << "nstat=\"" << set.ncstat() << "\" " << flush;
      if( set.emid() > -1 ){
	os << "emid=\"" << flush;
	if( set.emNumber() > 0 ) os << set.emNumber() << "-" << flush;
	os << set.emid() << "\" " << flush;
      }
    }
    
    if( set.cite() != "" ) os << "cite=\"" << set.cite() << "\" " << flush;
    os << "stat=\"";
    os << ( set.enable() ? "enable" : "disable" ) << flush;
    os << "\"" << flush;
    os << ">" << endl;
    
    if( set.norm() != 0.0 ) { 
      for( int i = 0; i < set.ems().size(); i++ ){
	os << "<ems emid=\"" << i << "\" "
	   << "value=\"" <<  set.ems()[ i ] << "\"" << " />" << endl;
      }
    }
    
    for( DataSet::iterator itr = set.begin(); itr != set.end(); itr++ )
      os << *(itr) << endl;
    os << "</" << DataSet::xmlTag << ">" << flush;
    return os;
  }

}
