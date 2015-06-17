#include <Utility/Arguments.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "Evolution.hh"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Utility;
using namespace QCD;

void print( Evolution::Kernel *kernel, string& type ){
  
  Arguments& args = Arguments::ref();
  double angle  = args.get( "angle",  0.55 );
  double length = args.get( "length", 10.0 );
  double offset = args.get( "offset", 2.0 );
  double prec   = args.get( "precision", 1E-3 );
  int    leg1   = args.get( "leg1", 6 );
  int    leg2   = args.get( "leg2", 8 );
  
  Evolution::PDFx xg( kernel,  QCD::Flavor::g,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xu(  kernel, QCD::Flavor::u,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xub( kernel, QCD::Flavor::ubar, leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xd(  kernel, QCD::Flavor::d,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xdb( kernel, QCD::Flavor::dbar, leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xs(  kernel, QCD::Flavor::s,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xsb( kernel, QCD::Flavor::sbar, leg1, leg2, prec, length, offset, angle );
  
  
  double zmin = args.get( "zmin", 0.2 );
  double zmax = args.get( "zmax", 0.8 );
  int    nz   = args.get( "nz",   20  );
  bool   logz = args.hasOpt( "logz" );
  string oname = args.get( "output", type );
  
  double dz = ( logz ? log10( zmax ) - log10( zmin ) : zmax - zmin ) / nz ;
  
  vector< double > q2( 1, 10.0 );
  q2 = args.get( "Q2", ",", q2 );
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    ostringstream ostr;
    ostr << oname << "_" << q2[ iq2 ] << ".dat"; 
    
    cout << "Create " << ostr.str() << endl;
    
    ofstream ofs( ostr.str().c_str() );
    ofs << "# " << kernel->initialDistribution()->path() << endl;
    ofs << "# Q^2 = " <<  q2[ iq2 ] << endl;
    ofs << "# Inverse mellin configuration: " << endl;
    ofs << "#     angle: " << angle << endl;
    ofs << "#     offset: " << offset << endl;
    ofs << "#     length: " << length << endl;
    ofs << "# Integration configuration: " << endl;
    ofs << "#     Leg: " << leg1 << " - " << leg2 << endl; 
    ofs << "#     Precision: " << prec << endl;
    ofs << "# " << endl;
    ofs << "#[z] [g] [u] [ubar] [d] [dbar] [s] [sbar]" << endl;
    
    kernel->setQ2( q2[ iq2 ] );
    for( int i = 0; i < nz; i++ ){
      double z = ( logz ? 
		   pow( 10, log10( zmin ) + i * dz ) :
		   zmin + i * dz );
      ofs << setw(12) << z << flush;
      ofs << setw(12) << xg( z ) << flush;
      ofs << setw(12) << xu( z ) << flush;
      ofs << setw(12) << xub( z ) << flush;
      ofs << setw(12) << xd( z ) << flush;
      ofs << setw(12) << xdb( z ) << flush;
      ofs << setw(12) << xs( z ) << flush;
      ofs << setw(12) << xsb( z ) << flush;

      ofs << endl;
    }
    
    ofs.close();
  }
  
}

void Usage(){
  Arguments& args = Arguments::ref();

  cout << "# Print out FF values at the given Q2, " << endl;
  cout << "#     for g, u, ubar, d, dbar, s and sbar. " << endl;
  cout << "#  required option" << endl;
  cout << "#      Fragmentation parameterization xml file. " << endl;
  cout << "#  optional option" << endl;
  cout << "#    ** Inverse Mellin transform" << endl;

  args.usage( "angle", 0.55, "path angle" );
  args.usage( "length", 10.0, "path length" );
  args.usage( "offset", 2.0, "path offset" );
  args.usage( "precision", 1E-3, "reqired precision" );
  args.usage( "leg1", 6, "size of Leg grid 1" );
  args.usage( "leg2", 8, "size of Leg grid 2" );

  cout << "#    ** value of z" << endl;
  args.usage( "zmin", 0.2, "minimun value of z" );
  args.usage( "zmax", 0.8, "maximun value of z" );
  args.usage( "nz",   20, "number of data point to be calculated" );
  args.usage( "logz", "[boolen]", "logarithmic in z" );

  cout << "#    ** file name " << endl;
  args.usage( "output", "FF?", "output file name prefix" );
  
}


int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );
  
  if( args.hasOpt( "help" ) || args.hasOpt( "?" ) ){
    Usage();
    return 1;
  }

  vector< string > optPI = Fragmentation::Evo::options( Flavor::PIp );
  vector< string > optKA = Fragmentation::Evo::options( Flavor::KAp );
  vector< string > optRE = Fragmentation::Evo::options( Flavor::REp );
  vector< string > optPR = Fragmentation::Evo::options( Flavor::PRp );
  
  vector< string > opts;
  opts.insert( opts.end(), optPI.begin(), optPI.end() );
  opts.insert( opts.end(), optKA.begin(), optKA.end() );
  opts.insert( opts.end(), optRE.begin(), optRE.end() );
  opts.insert( opts.end(), optPR.begin(), optPR.end() );
  
  vector< string >::iterator itr;
  if( ( itr = args.hasOpt( opts ) ) == opts.end() ){
    args.err() << "one of the following option requires" << endl;
    for( int i = 0; i < opts.size(); i++ ){
      args.err() << "\t--" << opts[ i ] << endl;
    }
    return 1;
  }
  
  try {
    
    string& key = *itr;
    string type = ( key.size() > 2 ? key.substr( key.size() - 3, 3 ) : "FF" );
    QCD::Flavor::HADRON h = QCD::Flavor::PIp;
    if( type == "FFK" ) h = QCD::Flavor::KAp;
    else if( type == "FFR" ) h = QCD::Flavor::REp;
    else if( type == "FFP" ) h = QCD::Flavor::PRp;
    
    // create FF objects
    Fragmentation::Evo* ff = Fragmentation::Evo::instance( h );
    print( ff, type );
    
  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }

  return 0;
}
