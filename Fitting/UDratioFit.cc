#include "UDratioFit.hh"

#include <Tranform/GridIntegration.hh>
#include <ExpData/DataSet.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/CKernel.hh>
#include <ErrorCalc/ErrCalc.hh>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "UDratioData.hh"

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

UDratioFit::UDratioFit( Arguments& args ) throw( int ) : 
  Fit::Fitting( args ),
  x_( 0 ), dKey_( UDratioData::Key ), ratio_( NULL )
{
  
  
  useFData_ = true;
  
  // re-set intergration path parameters
  this->offset( args.get( "offset",  2.5  ) );
  this->length( args.get( "length", 15.0  ) );
  this->angle(  args.get( "angle",  0.55 ) );
  
  // check Fragmentation function
  if( updf_ == NULL ){
    
    ARG_ERR << "You must give at least one unpol PDF intial file." << endl;
    
    ARG_ERR << "\t for pion" << endl;
    vector< string > opts = unpolPDF::Evo::options();
    for( int i = 0; i < opts.size(); i++ )
      ARG_ERR << "\t--" << opts[ i ] << "=[xml file]" << endl;
    
    throw( 1 );
  }
  
  try {
    
    ratio_ = new UDratio( updf_ );
    
    ratio_->ubar( new Evolution::PDFx( updf_, QCD::Flavor::ubar,
				       6, 8, 1.0E-4,
				       this->length(), 
				       this->offset(), 
				       this->angle() ) );
    
    ratio_->dbar( new Evolution::PDFx( updf_, QCD::Flavor::dbar,
				       6, 8, 1.0E-4,
				       this->length(), 
				       this->offset(), 
				       this->angle() ) );
  }
  
  catch( int e ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "\tcatch error (" << e << ")" << endl;
    throw( e );
  }
  
  dynamic_cast< Fitting* >( this )->loadData();
}

UDratioFit::~UDratioFit() {
  delete ratio_->ubar();
  delete ratio_->dbar();
}

bool UDratioFit::calc( FitData* fd ){
  
  UDratioData* data = dynamic_cast< UDratioData* >( fd );
  
  if( data == NULL ) return false;
  
  ratio_->setQ2( data->q2_ ); // set scale
  data->fit_ =  (*ratio_)( data->x_ );
  
  return true;
}

bool UDratioFit::calc( const int& i ) {
  return true;
}

void UDratioFit::loadData( Exp::DataSet& ds ){

  // if no data stored, just ignore.
  if( ds.size() == 0 ) {
    ARG_ERR << "No data stored. skip." << endl;
    ARG_ERR << ds << endl;
    return;
  }
  string dInfo = ds[ 0 ].info();

  // check data type of the first stored data.
  if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) return;
  
#ifdef DEBUG_FITTING
  
  ARG_LOG << "Data Info:" << setw(35) << ds.info() << endl; 
  ARG_LOG << "Data Type:" << setw(16) << dInfo << flush;
  ARG_LOG << endl;
  
#endif 
  
  // loop over data point 
  for( int id = 0; id < ds.size(); id++ ){
    
    DataPoint& d = ds[ id ];
    UDratioData *fd = new UDratioData( ds, d );
    
    fData_.push_back( fd );
    
    ratio_->setQ2( fd->q2_ ); // set scale
    
    double& x = fd->x_;

    ARG_LOG 
      << setw(6) << x 
      << setw(12) << fd->q2_
      << setw(12) << fd->sig_
      << setw(16) << (*( ratio_->ubar() ))( x )
      << setw(16) << (*( ratio_->dbar() ))( x )
      << setw(16) << ( *ratio_ )( x )
      << endl;
    
#ifdef DEBUG_FITTING
    ARG_LOG << *( fd ) << endl;
#endif
    
  }
}

string UDratioFit::ofilename( const int& index ){
  
  ostringstream  ostr;
  ostr << "unpolPDF_" ;
  
  // pQCD order
  ostr << ( updf_->alpha().scheme().order() == Scheme::NLO ? "NLO" : "LO" ); 
  
  // error matrix calculation
  if( this->emflag() ) ostr << "_werr";

  // suffix
  ostr << ".xml";
  
  return ostr.str();
}

void UDratioFit::data_summary( std::ostream& os ) {
  
  os << "********************  Data listing  ***********************"
     << endl;
  
  os << UDratioData::header() << endl;
  for( int i = 0; i < fData_.size(); i++ ){
    if( fData_[ i ]->sigUsed_ ) 
      os << dynamic_cast< UDratioData& >( *fData_[ i ] ) << endl;
  }
  
}

void UDratioFit::postUpdate() {
}

bool UDratioFit::postFit( Exp::DataSet& ds ){
  
  string dInfo = ds[ 0 ].info();  // get data info
  
  if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) return false;
  
#ifdef DEBUG_FITTING
  ARG_LOG << "* " << setw(30) << ds.info() << endl;
#endif // DEBUG_FITTING
  
  for( int id = 0; id < ds.size(); id++ ){
    DataPoint& d = ds[ id ];
    double x = d.kine( "x" );
    ratio_->setQ2( pow( d.kine("M"), 2 ) );
    d.model() = (*ratio_)( x );
  }

  return true;
};

bool UDratioFit::postErrorMatrix( Exp::DataSet& ds ){
  
  string dInfo = ds[ 0 ].info();  // get data info
  
  ErrorCalc::ErrCalc eCalc( updf_ );
  eCalc.setRealFunc( ratio_ );
  eCalc.setPDFcalc( false ); 
  eCalc.rfunccalc( true );
  
  // for normalization
  setNormMp( &eCalc, &ds );
  
  if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) return false; 
  
#ifdef DEBUG_FITTING
  ARG_LOG << "* " << setw(30) << ds.info() << endl;
#endif // DEBUG_FITTING
  
  for( int id = 0; id < ds.size(); id++ ){
    
    DataPoint& d = ds[id];
    ratio_->setQ2( pow( d.kine("M"), 2 ) );
    d.modelError() = sqrt( eCalc( d.kine( "x" ) ) );
    
  }
  return true;
}

bool UDratioFit::condition(){
  return true;
}
