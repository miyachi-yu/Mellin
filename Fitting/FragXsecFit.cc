#include "FragXsecFit.hh"

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

#include "FragData.hh"

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

FragXsecFit::FragXsecFit( Arguments& args ) throw( int ) : 
  Fit::Fitting( args ),
  xsec_( 0 ), f_( 0 ),
  z_( 0 ), dz_( 0 ), vz_( 0 ), type_( 0 ), htype_( 0 ),
  zrange_( 2, 0.0 ),
  lag_( args.get( "laguerre", 5 ), -0.5 ),
  cmap_( 0 ),
  steepestContourStatus_( false ),
  dKey_( "e+e-_" ), fragData_( 0 )
{
  
  // defalut z range
  this->zrange( args.get( "zmin", 0.05 ), args.get( "zmax", 0.80 ) );
  
  // load experimental data
  this->loadData();
  
  // re-set intergration path parameters
  this->offset( args.get( "offset", 3.0  ) );
  this->length( args.get( "length", 8.0  ) );
  this->angle(  args.get( "angle",  0.55 ) );
  
  // check Fragmentation function
  if( pion_ == NULL && kaon_ == NULL && rest_ == NULL && proton_ == NULL ) {
    
    ARG_ERR << "You must give at least one FF intial file." << endl;
    
    ARG_ERR << "\t for pion" << endl;
    vector< string > optsPi = Fragmentation::Evo::options( QCD::Flavor::PIp );
    for( int i = 0; i < optsPi.size(); i++ )
      ARG_ERR << "\t--" << optsPi[ i ] << "=[xml file]" << endl;
    
    ARG_ERR << "\t for kaon" << endl;
    vector< string > optsKa = Fragmentation::Evo::options( QCD::Flavor::KAp );
    for( int i = 0; i < optsKa.size(); i++ )
      ARG_ERR << "\t--" << optsKa[ i ] << "=[xml file]" << endl;
    
    ARG_ERR << "\t for rest" << endl;
    vector< string > optsRe = Fragmentation::Evo::options( QCD::Flavor::REp );
    for( int i = 0; i < optsRe.size(); i++ )
      ARG_ERR << "\t--" << optsRe[ i ] << "=[xml file]" << endl;
    
    ARG_ERR << "\t for proton" << endl;
    vector< string > optsPr = Fragmentation::Evo::options( QCD::Flavor::PRp );
    for( int i = 0; i < optsPr.size(); i++ )
      ARG_ERR << "\t--" << optsPr[ i ] << "=[xml file]" << endl;
    
    throw( 1 );
  }
  
  try {
    // prepare cross section
    // xsec_[ i ][ j ]
    // i = 0, 1, 2, 3 :  pion, kaon, rest, hadron
    // j = 0, 1, 2:      normal, c-enriched, b-enriched sample
    xsec_.resize( this->htype( "ALL" ) );
    f_.resize( xsec_.size() );
    
    static const int ipi = this->htype( "PI" );
    static const int ika = this->htype( "KA" );
    static const int ipr = this->htype( "PR" );
    static const int ire = this->htype( "RE" );
    static const int iha = this->htype( "HA" );
    static const int ip0 = this->htype( "P0" );
 
    for( int i = 0; i < xsec_.size(); i++ ) {
      
      xsec_[ i ].resize( this->dataType( "ALL" ), NULL );
      f_[ i ].resize( xsec_[ i ].size(), NULL );
      
      Evolution::Kernel *evo = NULL;
      QCD::Flavor::HADRON h;
      
      if( i == ip0 ) continue; // use charge pion ff
      else if( i == ipi ) { evo = pion_;   h = QCD::Flavor::PIp; }
      else if( i == ika ) { evo = kaon_;   h = QCD::Flavor::KAp; }
      else if( i == ipr ) { evo = proton_; h = QCD::Flavor::PRp; }
      else if( i == ire ) { evo = rest_;   h = QCD::Flavor::REp; }
      else if( i == iha ) { evo = hadron_; h = QCD::Flavor::HAp; }
      
      if( evo == NULL ) continue;
      
      if( fragData_[ i ][ 0 ] )
	xsec_[ i ][ 0 ] = new FragXsec_T::FragXsec( evo, h );
      
      // for c enriched sample
      if( fragData_[ i ][ 1 ] ){
	xsec_[ i ][ 1 ] = new FragXsec_T::FragXsec( evo, h );
	xsec_[ i ][ 1 ]->setIndiv( Flavor::c );  
      }
      
      // for b enriched sample
      if( fragData_[ i ][ 2 ] ){
	xsec_[ i ][ 2 ] = new FragXsec_T::FragXsec( evo, h );
	xsec_[ i ][ 2 ]->setIndiv( Flavor::b );  
      }
      
      // cross section in z-space
      for( int j = 0; j < xsec_[ i ].size(); j++ ) {
	if( xsec_[ i ][ j ] ) f_[ i ][ j ] = this->xf( xsec_[ i ][ j ] );
      }

    }
  }
  
  catch( int e ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "\tcatch error (" << e << ")" << endl;
    throw( e );
  }
  
  // load steepest descent contour parameters
  if( args.hasOpt( "laguerre" ) ){
    
    cmap_.resize( f_.size() );
    for( int i = 0; i < f_.size(); i++ ) cmap_[ i ].resize( f_[ i ].size() );

    string mapfile = args.get( "steepestMap", "cmap.dat" );  
    this->loadSteepestContour( mapfile.c_str() );
    this->loadSteepestContour();
    this->saveSteepestContour( mapfile.c_str() );
  }

}

FragXsecFit::~FragXsecFit() {
  
  for( int i = 0; i < f_[ i ].size(); i++ ) 
    for( int j = 0; j < f_[ i ].size(); j++ ) 
      if( f_[ i ][ j ] ) delete f_[ i ][ j ];
  
  for( int i = 0; i < xsec_.size(); i++ ) 
    for( int j = 0; j < xsec_[ i ].size(); j++ )
      if( xsec_[ i ][ j ] ) delete xsec_[ i ][ j ];
  
}

bool FragXsecFit::calc( FitData* fd ){

  FragData* data = dynamic_cast< FragData* >( fd );
  
  if( data == NULL ) return false;
  
  static int ip0 = this->htype( "P0" );
  static int ipi = this->htype( "PI" );
  
  vector< double >& vz = data->vz_;
  
  // check z value
  if( vz[ 0 ] < zrange_[ 0 ] || 
      vz[ ( vz.size() == 1 ? 0 : 1 ) ] > zrange_[ 1 ] ) return false;
  
  // xsection type
  bool fpi0 = ( data->ih_ == ip0 );
  int& it = data->it_;
  int ih = ( fpi0 ? ipi : data->ih_ ) ; // use charged pion for neutral pion
  
  if( f_[ ih ][ it ] == NULL ) return false;
  
  // get reference of xsection objects
  Xsection::Xsec&  xs  = *( xsec_[ ih ][ it ] );
  Xsection::xXsec& xxs = *( f_[ ih ][ it ] );
  
  double& z = vz[ 0 ];
  // for Laguerre Quadrature
  if( xxs.contour() == Transform::InverseMellin::STEEPEST ){
    map< double, vector< double > >& cm = cmap_[ ih ][ it ];
    xxs.steepestDescent( cm[ z ][ 0 ], cm[ z ][ 1 ], cm[ z ][ 2 ] );
  }
  
  // set Q2 value
  xs.setQ2( data->q2_ );
  
  // do not forget about factor 2 for the cross section 
  // to account for both charged hadrons
  // for b and c enriched sample one must count 2.0 in addition,
  // for quark and anti-quark.
  data->fit_ = ( it == 0 ? 2.0 : 4.0 ) * xxs( vz );
  
  // for neutal pion, additional factor 0.5 should be applied.
  if( fpi0 ) data->fit_ *= 0.5;
  
  /*
  ARG_LOG 
  << setw(3) << i << setw(2) << ih << setw(2) << it 
  << setw(8) << q2_[ i ] 
  << setw(12) << vz[0] << setw(12) << vz[ vz.size() == 1 ? 0 : 1 ]
    << setw(16) << fit_[ i ]
    << endl;
  */

  return true;
}

bool FragXsecFit::calc( const int& i ) {
  
  static int ip0 = this->htype( "P0" );
  static int ipi = this->htype( "PI" );

  vector< double >& vz = vz_[ i ];
  
  // check z value
  if( vz[ 0 ] < zrange_[ 0 ] || 
      vz[ ( vz.size() == 1 ? 0 : 1 ) ] > zrange_[ 1 ] ) return false;
  
  // xsection type
  bool fpi0 = ( htype_[ i ] == ip0 );
  int it = type_[ i ];
  int ih = ( fpi0 ? ipi : htype_[ i ] ) ; // use charged pion for neutral pion
  
  if( f_[ ih ][ it ] == NULL ) return false;
  
  // get reference of xsection objects
  Xsection::Xsec&  xs  = *( xsec_[ ih ][ it ] );
  Xsection::xXsec& xxs = *( f_[ ih ][ it ] );
  
  double& z = vz[ 0 ];
  // for Laguerre Quadrature
  if( xxs.contour() == Transform::InverseMellin::STEEPEST ){
    map< double, vector< double > >& cm = cmap_[ ih ][ it ];
    xxs.steepestDescent( cm[ z ][ 0 ], cm[ z ][ 1 ], cm[ z ][ 2 ] );
  }
  
  // set Q2 value
  xs.setQ2( q2_[ i ] );
  
  // do not forget about factor 2 for the cross section 
  // to account for both charged hadrons
  // for b and c enriched sample one must count 2.0 in addition,
  // for quark and anti-quark.
  fit_[ i ] = ( it == 0 ? 2.0 : 4.0 ) * xxs( vz );
  
  // for neutal pion, additional factor 0.5 should be applied.
  if( fpi0 ) fit_[ i ] *= 0.5;

  /*
  ARG_LOG 
    << setw(3) << i << setw(2) << ih << setw(2) << it 
    << setw(8) << q2_[ i ] 
    << setw(12) << vz[0] << setw(12) << vz[ vz.size() == 1 ? 0 : 1 ]
    << setw(16) << fit_[ i ]
    << endl;
  */

  return true;
}

void  FragXsecFit::loadData(){
  
  vector< DataSet >& data = this->expr()->data();
  
  fragData_.resize( this->htype( "ALL" ),
		    vector< bool >( this->dataType( "ALL" ), false ) );
  
  for( int i = 0; i < data.size(); i++ ){
    
    DataSet& ds = data[ i ];
    
    // if no data stored, just ignore.
    if( ds.size() == 0 ) continue;
    
    string dInfo = ds[ 0 ].info();
    
    // check data type of the first stored data.
    if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) continue;
    
    // check hadron type
    int hType = this->htype( dInfo.substr( dKey_.size(), 2 ) );
    int iType = this->dataType( dInfo.substr( dKey_.size() + 2 ) );
    
#ifdef DEBUG_FITTING
    
    ARG_LOG << "Data Info:" << setw(35) << ds.info() << endl; 
    ARG_LOG << "Data Type:" << setw(16) << dInfo << flush;
    
    ARG_LOG << " Hadron(" << hType << ":" << dInfo.substr( dKey_.size(), 2 ) 
	    << ")"
	    << flush;
    
    ARG_LOG << " Data(" << iType 
	    << ":" << dInfo.substr( dKey_.size() + 2 ) << ")" << flush;
    
    ARG_LOG << "  NORM:"  << setw(12) << ds.norm()
	    << "  NCOEF:" << setw(12) << ds.ncoef()
	    << "  NCERR:" << setw(12) << ds.ncerr()
	    << "  ( " << ds.ncstat() << ")" << flush;
    
    ARG_LOG << endl;
    
#endif 
    
    fragData_[ hType ][ iType ] = true;
    
    // loop over data point 
    for( int id = 0; id < ds.size(); id++ ){
      
      DataPoint& d = ds[ id ];
      FragData *fd = new FragData();
      
      double z1 = d.kine("zmin");
      double z2 = d.kine("zmax");
      double z  = 0.5 * ( z1 + z2 );
      double dz = z2 - z1;
      
      vector< double > vz;
      if( z1 != z2 ){
	vz.resize( 2 );
	vz[ 0 ] = z1;
	vz[ 1 ] = z2;
	
      } else {
	vz.resize( 1 );
	vz[ 0 ] = z1;
      }
      
      double error2 = pow( d.stat(), 2 );
      for( int j = 0; j < d.syst().size(); j++ ) 
	error2 += pow( d.syst()[ j ], 2 );
      if( error2 > 0.0 ) error2 = sqrt( error2 );
      
      
      if( useFData_ ){
	
	fd->vz_.resize( 2 );
	fd->vz_[ 0 ] = z1;
	fd->vz_[ 1 ] = z2;
	if( fd->vz_[ 0 ] == fd->vz_[ 1 ] ) fd->vz_.resize( 1 );
	
	fd->z_       = z;
	fd->dz_      = dz;
	fd->sig_     = d.data();
	fd->dsig_    = error2;
	fd->q2_      = d.kine("Q^2");
	fd->expInfo_ = ds.info();
	fd->it_      = iType;
	fd->ih_      = hType;
	fd->dset_    = &ds;
	fd->point_   = &d;
	
	fData_.push_back( fd );
	
#ifdef DEBUG_FITTING
	ARG_LOG << *( fd ) << endl;
#endif
      } else {
	z_.push_back( z );
	dz_.push_back( dz );
	vz_.push_back( vz );
	sig_.push_back(  d.data() );
	dsig_.push_back( error2 );
	q2_.push_back( d.kine("Q^2") );
	expInfo_.push_back( ds.info() );
	type_.push_back( iType );
	htype_.push_back( hType );
	dset_.push_back( &ds ); // add pointer to DataSet
	point_.push_back( &d );
	
#ifdef DEBUG_FITTING
	ARG_LOG << setw(10) << vz_.back()[ 0 ] << flush;
	if( vz_.back().size() == 1 ) { 
	  ARG_LOG << setw(10) << "******" << flush;
	} else {
	  ARG_LOG << setw(10) << vz_.back()[ 1 ] << flush;
	}
	ARG_LOG << setw(10) << q2_.back() << flush;
	ARG_LOG << setw(10) << sig_.back() << flush;
	ARG_LOG << setw(12) << dsig_.back() << flush;
	ARG_LOG << setw(30) << expInfo_.back() << flush;
	ARG_LOG << endl;
#endif      
      }
    }
  }
}

void FragXsecFit::zrange( const double& zmin, const double& zmax ){
  if( zmin < zmax ) {
    zrange_[0] = zmin;
    zrange_[1] = zmax;
  } else {
    zrange_[1] = zmin;
    zrange_[0] = zmax;
  }
}

string FragXsecFit::ofilename( const int& index ){
  
  Fragmentation::Evo *evo = 
    dynamic_cast< Fragmentation::Evo* >( this->kernels()[ index ] );
  
  ostringstream  ostr;
  
  // base name 
  if( evo->hadron() == QCD::Flavor::PIp ){
    ostr << "FF_";
  } else if( evo->hadron() == QCD::Flavor::KAp ){
    ostr << "FFK_";
  } else if( evo->hadron() == QCD::Flavor::PRp ){
    ostr << "FFP_";
  } else if( evo->hadron() == QCD::Flavor::REp ){
    ostr << "FFR_";
  } else {
    ostr << "FFX_";
  }
  
  // pQCD order
  ostr << ( evo->alpha().scheme().order() == Scheme::NLO ? "NLO_" : "LO_" ); 
  
  // z range
  ostr << zrange_[0] << "-" << zrange_[1];
  
  // rescale
  if( Arguments::ref().hasOpt( "rescaleFF" ) ) 
    ostr << "_r" << Arguments::ref().get( "rescaleFF", 1.0 );
  
  // error matrix calculation
  if( this->emflag() ) ostr << "_werr";

  // suffix
  ostr << ".xml";
  
  return ostr.str();
}

void FragXsecFit::data_summary( std::ostream& os ) {
  
  os 
    << "*******************  Fitting condition *********************\n"
    << "  z minimum:     " << setw(5) <<  zrange_[ 0 ] << endl
    << "  z maximum:     " << setw(5) <<  zrange_[ 1 ] << endl
    << endl;
  
  
  os << "********************  Data listing  ***********************"
     << endl;

  if( useFData_ ){

    os << FragData::header() << endl;
    for( int i = 0; i < fData_.size(); i++ ){
      if( fData_[ i ]->sigUsed_ ) 
	os << dynamic_cast< FragData& >( *fData_[ i ] ) << endl;
    }
    
  } else {
    
    os << setw( 10 ) << "zmin"
       << setw( 10 ) << "zmax"
       << setw( 10 ) << "Q2"
       << setw( 12 ) << "sigma"
       << setw( 12 ) << "dsigma"
       << setw( 12 ) << "fit"
       << setw( 3 )  << "id"
       << setw( 12 ) << "chi2"
       << setw( 16 ) << "Info"
       << endl;
    
    for( int i = 0; i < sig_.size(); i++ ){
      
      os << setw( 10 ) << vz_[ i ][ 0 ] << flush;
      if( vz_[ i ].size() > 1 ) {
	os << setw( 10 ) << vz_[ i ][ 1 ] << flush;
      } else {
	os << setw( 10 ) << "******" << flush;
      }
      os << setw( 10 ) << q2_[ i ]
	 << setw( 12 ) << sig_[ i ]
	 << setw( 12 ) << dsig_[ i ]
	 << setw( 12 ) << fit_[ i ]
	 << setw( 3 )  << type_[ i ]
	 << setw( 12 ) << c2_[ i ]
	 << setw( 30 ) << expInfo_[ i ]
	 << endl;
    }
  }
}

void FragXsecFit::postUpdate() {

  if( this->kernels().size() == 0 ) return;

  for( int i = 0; i < xsec_.size(); i++ ) 
    for( int j = 0; j < xsec_[ i ].size(); j++ ) 
      if( xsec_[ i ][ j ] ) xsec_[ i ][ j ]->clearCache();
}

void FragXsecFit::loadSteepestContour(){
  
  if( steepestContourStatus_ ) return ;
  
  for( int i = 0; i < sig_.size(); i++ ){
    
    vector< double >& vz = vz_[ i ];
    
    // check z value
    if( vz[ 0 ] < zrange_[ 0 ] || 
	vz[ ( vz.size() == 1 ? 0 : 1 ) ] > zrange_[ 1 ] ) continue;
    
    // xsection type
    int it = type_[ i ];
    int ih = htype_[ i ];
    
    if( f_[ ih ][ it ] == NULL ) continue;
    
    Xsection::Xsec& xs = *( xsec_[ ih ][ it ] );
    Xsection::xXsec& f = *( f_[ ih ][ it ] );
    
    double& z = vz[ 0 ];
    map< double, vector< double > >& cm = cmap_[ ih ][ it ];
    
    if( cm.find( z ) == cm.end() ){
      
#ifdef DEBUG_FITTING
      ARG_LOG << setw( 2 )  << it << flush;
      ARG_LOG << setw( 10 ) << z  << flush;
#endif 
      
      // set x range 
      f.F().x( vz );
      
      // prepare container
      cm[ z ] = vector< double >( 3, 0.0 );
      
      double q2 = q2_[ i ];
      double fit;
      double c2;
      
      do {
	
	if( q2 > 8317.44 ) q2 = 1000.0;

	xs.setQ2( q2 );
	
	cm[ z ][ 0 ] = f.F().c0();
	cm[ z ][ 1 ] = f.F().c1( cm[ z ][ 0 ] );
	cm[ z ][ 2 ] = f.F().c2( cm[ z ][ 0 ] );
	
	// second parameter must be negative, otherwise .....
	if( cm[ z ][ 1 ] > 0.0 ) continue;
	
	// Lets set the contour parameter
	f.steepestDescent( cm[ z ][ 0 ], cm[ z ][ 1 ], cm[ z ][ 2 ] );
	
	// check cross section value
	fit = ( it == 0 ? 2.0 : 4.0 ) * f( vz );
	if( isnan( fit ) || isinf( fit ) || fit <= 0.0 ) continue;
	
	// check chi^2 
	c2 = pow( ( fit - sig_[ i ] ) / dsig_[ i ], 2.0 );
	if( c2 > 1.0E+4 ) continue;
	
	// if all conditions are fullfilled, let's take the present contour
	// parameters.
	break;
	
      } while( q2 += 50.0 );
      
      
#ifdef DEBUG_FITTING
      ARG_LOG << setw(  8 ) << cm[ z ][ 0 ] << flush;
      ARG_LOG << setw( 12 ) << cm[ z ][ 1 ] << flush;
      ARG_LOG << setw( 12 ) << cm[ z ][ 2 ] << flush;
      
      xs.setQ2( q2_[ i ] );
      ARG_LOG 
	<< setw(12) 
	<< ( fit = ( it == 0 ? 2.0 : 4.0 ) * f( z_[ i ] ) ) << flush;
      ARG_LOG
	<< setw(12)
	<< ( fit = ( it == 0 ? 2.0 : 4.0 ) * f( vz_[ i ] ) )
	<< flush;
      ARG_LOG << setw(12) <<  sig_[i] << flush;
      ARG_LOG << setw(12) << ( fit - sig_[i] ) / dsig_[ i ] << flush;
      ARG_LOG << endl;
#endif // DEBUG_FITTING
    }
  }
  
  steepestContourStatus_ = true;
}

void FragXsecFit::loadSteepestContour( const std::string& fname ){
  ifstream ifs( fname.c_str() );
  if( ! ifs ) return;
  
  ARG_LOG << "************ load steepest descent parameters from " 
       << fname 
       << endl;
  
  int i, j;
  double z, c0, c1, c2;
  
  while( ifs >> i >> j >> z >> c0 >> c1 >> c2 ){
    
    if( cmap_[ i ][ j ].find( z ) == cmap_[ i ][ j ].end() )
      cmap_[ i ][ j ][ z ] = vector< double >( 3, 0.0 );
    
    cmap_[ i ][ j ][ z ][ 0 ] = c0;
    cmap_[ i ][ j ][ z ][ 1 ] = c1;
    cmap_[ i ][ j ][ z ][ 2 ] = c2;
    
#ifdef DEBUG_FITTING
    ARG_LOG << setw(3)  << i
	    << setw(3)  << j
	    << setw(8)  << z
	    << setw(12) << c0
	    << setw(12) << c1
	    << setw(12) << c2
	    << endl;
#endif // DEBUG_FITTING
    
  }
  
  ARG_LOG << "**************************** end ********************************"
	  << endl;
  
  ifs.close();
}

void FragXsecFit::saveSteepestContour( const std::string& fname ){
  
  ofstream ofs( fname.c_str() );
  
  for( int i = 0; i < cmap_.size(); i++ ){
    for( int j = 0; j < cmap_[ i ].size(); j++ ){
      map< double, vector< double > >::iterator itr;
      for( itr = cmap_[ i ][ j ].begin(); itr != cmap_[ i ][ j ].end(); itr++ ){
	ofs << setw(3)  << i
	    << setw(3)  << j
	    << setw(8)  << itr->first
	    << setw(12) << itr->second[0]
	    << setw(12) << itr->second[1]
	    << setw(12) << itr->second[2]
	    << endl;
      }
    }
  } 
  ofs.close();
}

int FragXsecFit::htype( const std::string& name ){
  if( name == "PI" )  return 0;
  if( name == "KA" )  return 1;
  if( name == "PR" )  return 2;
  if( name == "RE" )  return 3;
  if( name == "HA" )  return 4;
  if( name == "P0" )  return 5;
  if( name == "ALL" ) return 6; // return number of hadron type
  return 0;
}

int FragXsecFit::dataType( const std::string& name ){
  if( name == "_c_enriched" ) return 1;
  if( name == "_b_enriched" ) return 2;
  if( name == "ALL" ) return 3; // return number of hadron type
  return 0;
}

bool FragXsecFit::postFit(){
  
  vector< DataSet >& data = this->expr()->data();
  
  for( int i = 0; i < data.size(); i++ ){
    
    // skip if there are no data point
    if( data[ i ].size() == 0 ) continue; 
    
    DataSet& ds = data[i];         // get reference
    string dInfo = ds[ 0 ].info();  // get data info
    
    if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) continue; 
    
    int htyp = this->htype( dInfo.substr( dKey_.size(), 2 ) );
    int ih = ( htyp != this->htype( "P0" ) ? htyp : this->htype( "PI" ) );
    int it = this->dataType( dInfo.substr( dKey_.size() + 2 ) );
    
    if( f_[ ih ][ it ] == NULL ) continue;
#ifdef DEBUG_FITTING
    ARG_LOG << "* " << setw(30) << ds.info() << endl;
#endif // DEBUG_FITTING

    for( int id = 0; id < ds.size(); id++ ){
      DataPoint& d = ds[ id ];
      xsec_[ ih ][ it ]->setQ2( d.kine("Q^2") );
      vector< double > z( 2 );
      z[ 0 ] = d.kine("zmin");
      z[ 1 ] = d.kine("zmax");
      if( z[ 0 ] == z[ 1 ] ) z.resize( 1 );
      
      // it is impossible to calculate for the case of z[ 0 ] == 0 
      if( z[ 0 ] < 0.001 ) continue;
      
      d.model() = 
	( htyp == this->htype( "P0" ) ? 0.5 : 1.0 ) *
	( it == 0 ? 2.0 : 4.0 ) * ( *f_[ ih ][ it ] )( z );
      
      //      if( zrange_[ 0 ] < z[ 0 ] &&
      //	  z[ ( z.size() == 1 ? 0 : 1 ) ] < zrange_[ 1 ] ) d.fitted() = true;
      
    }
  }
  
  return true;
};

bool FragXsecFit::postErrorMatrix(){
  
  vector< DataSet >& data = this->expr()->data();
  for( int i = 0; i < data.size(); i++ ){
    
    if( data[ i ].size() == 0 ) continue;
    
    DataSet& ds = data[i];
    string dInfo = ds[ 0 ].info();  // get data info
    
    if( dInfo.substr( 0, dKey_.size() ) != dKey_ ) continue; 
    
    int htyp = this->htype( dInfo.substr( dKey_.size(), 2 ) );
    int ih = ( htyp != this->htype( "P0" ) ? htyp : this->htype( "PI" ) );
    int it = this->dataType( dInfo.substr( dKey_.size() + 2 ) );
    
    if( f_[ ih ][ it ] == NULL ) continue;
    
#ifdef DEBUG_FITTING
    ARG_LOG << "* " << setw(30) << ds.info() << endl;
#endif // DEBUG_FITTING

    for( int id = 0; id < ds.size(); id++ ){
      DataPoint& d = ds[id];
      xsec_[ ih ][ it ]->setQ2( d.kine("Q^2") );

      ErrorCalc::ErrCalc eCalc( f_[ ih ][ it ] );
      // for normalization
      setNormMp( &eCalc, &ds );

      vector< double > z( 2 );
      z[ 0 ] = d.kine("zmin");
      z[ 1 ] = d.kine("zmax");
      if( z[ 0 ] == z[ 1 ] ) z.resize( 1 );

      // it is impossible to calculate for the case of z[ 0 ] == 0 
      if( z[ 0 ] < 0.001 ) continue;

      d.modelError() = 
	( htyp == this->htype( "P0" ) ? 0.5 : 1.0 ) *
	( it == 0 ? 2.0 : 4.0 ) * sqrt( eCalc( z ) );
    }
  }
  return true;
}

Xsection::xXsec* FragXsecFit::xf( Xsection::Xsec *xsec ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( xsec == NULL ) return NULL;
  Xsection::xXsec *xs = NULL;
  if( args.hasOpt( "laguerre" ) ){
    xs = new Xsection::xXsec( xsec, &lag_ );
  } else {
    xs = new Xsection::xXsec( xsec, 
			      args.get( "leg1", 6 ),
			      args.get( "leg2", 8 ),
			      args.get( "precision", 5.0E-3 ) );
    xs->setParameter( this->offset(), this->angle() );
    xs->upper( this->length() );
    Transform::GridIntegration* grid =
      dynamic_cast< Transform::GridIntegration* >( xs->integration() );
    grid->ngrid( args.get( "ngrid", 2 ) );
  }
  return xs;
}

bool FragXsecFit::condition(){

  if( ! Arguments::ref().hasOpt( "NoCondition" ) ){
    if( hadron_ ){
      hadron_->setN( 2.0 );
      hadron_->setQ2( hadron_->initialScale() );
      for( int i = -6; i < 7; i++ )
	if( abs( hadron_->q( Flavor::PARTON( i ) ) ) >= 0.5 ) 
	  return false;
    }
  }
  return true;
}
