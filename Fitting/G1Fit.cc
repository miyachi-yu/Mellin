#include "G1Fit.hh"

#include <Tranform/GridIntegration.hh>
#include <ExpData/DataSet.hh>
#include <Evolution/Kernel.hh>
#include <Evolution/DGLAPMatrix.hh>

#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/CKernel.hh>

#include <ErrorCalc/ErrCalc.hh>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

G1Fit::G1Fit() throw( int ) : 
  Fit::Fitting( Utility::Arguments::ref() ),
  g1_(0 ), xg1_( 0 ), x_( 0 ), type_( 0 ), xrange_( 2, 0.0 )
{
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( ppdf_ == NULL ){
    
    ARG_ERR << "# " << args.command() << endl;
    ARG_ERR << "#  requires polPDF initial distribuion." << endl;
    ARG_ERR << "#  please specify with one of following options." << endl;
    
    vector< string > opts = polPDF::Evo::options();
    for( int i = 0; i < opts.size(); i++ ){
      ARG_ERR << "#\t--" << opts[ i ] << "=[xml file]" << endl; 
    }

    throw( 1 );
  }
  
  // prepare g1( N ) and g1( x ) container
  g1_.resize( 3 ); xg1_.resize( 3 );
  
  // create g1( N ) with the pdf
  g1_[ 0 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::p );
  g1_[ 1 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::d );
  g1_[ 2 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::n );
  
  // create g1( x ) using mellin inversion of g1( n )
  // re-set intergration path parameters
  this->offset( args.get( "offset",  2.0  ) );
  this->length( args.get( "length", 15.0  ) );
  this->angle(  args.get( "angle",   0.55 ) );
  
  for( int i = 0; i < g1_.size(); i++ ){
    xg1_[ i ] = 
      new Xsection::xXsec( g1_[ i ], 
			   args.get( "leg1", 6 ),
			   args.get( "leg2", 8 ),
			   args.get( "precision", 5.0E-3 ) );
    
    xg1_[ i ]->setParameter( this->offset(), this->angle() );
    xg1_[ i ]->upper( this->length() );
    Transform::GridIntegration *grid = 
      dynamic_cast< Transform::GridIntegration* >( xg1_[ i ]->integration() );
    if( grid ) grid->ngrid( args.get( "ngrid", 2 ) );
  }

  this->xrange( 1.0E-4, 0.99 );    // defalut x range
  this->loadData();               // load data to be fitted
  
}

G1Fit::~G1Fit() {
  
  for( int i = 0; i < g1_.size(); i++ )
    if( xg1_[ i ] ) { delete g1_[ i ]; delete xg1_[ i ]; }
  
  
}

bool G1Fit::calc( const int& i ) {

  // check x value
  if( x_[ i ] < xrange_[ 0 ] || 
      x_[ i ] > xrange_[ 1 ] ||
      q2_[ i ] < q2limit_ ) return false;
  
  int N = type_[ i ];
  
  // set Q2 value
  g1_[ N ]->setQ2( q2_[ i ] );
  
  // set integration path length depending on x value
  if( x_[ i ] < 0.01 )      xg1_[ N ]->upper( 16.0 );
  else if( x_[ i ] < 0.1  ) xg1_[ N ]->upper(  8.0 );
  else                      xg1_[ N ]->upper(  6.5 );
  
  fit_[ i ] = 0.5 * (*xg1_[ N ])( x_[i] );
  
  return true;
}

void  G1Fit::loadData(){
  
  vector< DataSet* >& data = this->expr()->data();
  
  for( int i = 0; i < data.size(); i++ ){
    
    if( data[ i ]->size() == 0 ) continue;
    
    DataSet& ds = *data[i];
    
    if( ds[0].info() != "g1p" && 
	ds[0].info() != "g1d" &&
	ds[0].info() != "g1n" ) continue;
    
    for( int id = 0; id < ds.size(); id++ ){
      DataPoint& d = ds[id];
      
      x_.push_back( d.kine("x") );
      q2_.push_back( d.kine("Q^2") );
      sig_.push_back(  d.data() );
      dsig_.push_back( d.stat() );
      type_.push_back( ( d.info() == "g1p" ? 
			 0 : ( d.info() == "g1d" ? 1 : 2 ) ) );
      
      expInfo_.push_back( data[ i ]->info() );
      dset_.push_back( data[ i ] ); // add pointer to DataSet
      
    }
  }
}

void G1Fit::xrange( const double& xmin, const double& xmax ){
  if( xmin < xmax ) {
    xrange_[0] = xmin;
    xrange_[1] = xmax;
  } else {
    xrange_[1] = xmin;
    xrange_[2] = xmax;
  }
}

string G1Fit::ofilename( const int& index ){
  
  Evolution::Kernel *kernel = this->kernels()[ index ];
  
  ostringstream  ostr;
  if( dynamic_cast< polPDF::Evo* >( kernel ) ){
    ostr << "polPDF_" ;
  } else {
    ostr << "g1Fit_" << index << "_" ;
  }
  
  ostr<< ( kernel->alpha().scheme().order() == Scheme::NLO ? 
	   "NLO" : "LO" ) ;
  
  if( Arguments::ref().hasOpt( "rescale" ) ) 
    ostr << "_r" << Arguments::ref().get( "rescale", 1.0 );
  
  if( this->emflag() ) ostr << "_werr";
  ostr << ".xml" ;
  
  return ostr.str();
}

void G1Fit::data_summary( std::ostream& os ) {
  
  os << endl;
  os
    << "************************ Fitting condition  ************************\n"
    << "  x minimum    " << setw(16) << xrange_[ 0 ] << "\n"
    << "  x maximum    " << setw(16) << xrange_[ 1 ] << "\n"
    << "  Q^2 minimum  " << setw(16) << q2limit_     << "\n"
    << endl;
  
  os
    << "*************************** Data Listing ***************************\n"
    << setw( 10 ) << "x"
    << setw( 10 ) << "Q^2"
    << setw( 12 ) << "g1"
    << setw( 12 ) << "stat"
    << setw( 12 ) << "fit"
    << setw( 12 ) << "chi^2"
    << setw( 24 ) << "info"
    << endl;
  
  for( int i = 0; i < sig_.size(); i++ ){
    os 
      << setw( 10 ) << x_[ i ]
      << setw( 10 ) << q2_[ i ]
      << setw( 12 ) << sig_[ i ]
      << setw( 12 ) << dsig_[ i ]
      << setw( 12 ) << fit_[ i ]
      << setw( 12 ) << c2_[ i ]
      << setw( 24 ) << expInfo_[ i ]
      << endl;
  }
  
}

//! clear cache
void G1Fit::postUpdate(){

  if( this->kernels().size() == 0 ) return;

  g1_[0]->clearCache();
  g1_[1]->clearCache();
  g1_[2]->clearCache();
}

bool G1Fit::postFit(){

  vector< DataSet* >& data = this->expr()->data();
  
  for( int i = 0; i < data.size(); i++ ){
    
    if( data[ i ]->size() == 0 ) continue;
    
    DataSet& ds = *data[i];
    
    if( ds[0].info() != "g1p" && 
	ds[0].info() != "g1d" &&
	ds[0].info() != "g1n" ) continue;
    
    for( int id = 0; id < ds.size(); id++ ){
      DataPoint& d = ds[id];
      double x = d.kine("x");
      double q2 = d.kine("Q^2");
      
      int N = ( d.info() == "g1p" ? 0 : ( d.info() == "g1d" ? 1 : 2 ) );
      
      g1_[ N ]->setQ2( q2 );
      
      // set integration path length depending on x value
      if( x < 0.01 )     xg1_[ N ]->upper( 16.0 );
      else if( x < 0.1 ) xg1_[ N ]->upper(  8.0 );
      else               xg1_[ N ]->upper(  6.5 );
      
      d.model() = 0.5 * ( *xg1_[ N ] )( x );
      
    }
  }
  
  return true;
};

bool G1Fit::postErrorMatrix(){
  vector< DataSet* >& data = this->expr()->data();
  
  for( int i = 0; i < data.size(); i++ ){
    
    if( data[ i ]->size() == 0 ) continue;
    
    DataSet& ds = *data[i];
    
    if( ds[0].info() != "g1p" && 
	ds[0].info() != "g1d" &&
	ds[0].info() != "g1n" ) continue;
    
    for( int id = 0; id < ds.size(); id++ ){
      DataPoint& d = ds[id];
      double x  = d.kine("x");
      double q2 = d.kine("Q^2");
    
      int N = ( d.info() == "g1p" ? 0 : ( d.info() == "g1d" ? 1 : 2 ) );
      
      g1_[ N ]->setQ2( q2 );
      
      // set integration path length depending on x value
      if( x < 0.01 )     xg1_[ N ]->upper( 16.0 );
      else if( x < 0.1 ) xg1_[ N ]->upper(  8.0 );
      else               xg1_[ N ]->upper(  6.5 );
      
      ErrorCalc::ErrCalc eCalc( xg1_[ N ] );
      // for normalization
      setNormMp( &eCalc, &ds );
      d.modelError() = 0.5 * sqrt( eCalc( x ) );
      
    }
  }
  
  return true;
}
