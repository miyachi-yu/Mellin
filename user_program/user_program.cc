//
// user_program.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Sep 18 17:49:54 2008 Yoshiyuki Miyachi
// Started on  Thu Sep 18 17:49:54 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <fstream>

#include <Utility/Arguments.hh>      // for arguments list handling
#include <Utility/Timer.hh>          // stop watch

#include <polPDF/Evolution.hh>          // Polarized PDF 
#include <polDIS/StrFunc.hh>  // Structure function g1( M, Q^2 )

#include <Xsec/Charge.hh>            // Charge type 
#include <Xsec/xXsec.hh>             // Mellin inversion

using namespace std;

void usage(){
  Utility::Arguments& args = Utility::Arguments::ref();
  
  args.log() 
    << "# ----------------------------------------------------------------- #\n"
    << "# Usage: " << args.command() << "\n"
    << "#\n"
    << "# This is an example user program.\n"
    << "# It try to calculate g1p( x, Q2 ) at different x values.\n"
    << "#\n"
    << endl;
  
  args.log() << "arguments list" << endl;
  
  args.usage( "--polPDF", "path", "polPDF parameterization file");
  args.usage( "--writeDB", "", "Write to the external cache files");
  args.usage( "--Q2", "2.5", "QCD scale to be calculated" );
  args.usage( "--offset", "2.0", "Offset value of Mellin inversion path" );
  args.usage( "--angle", "0.55", "angle of Mellin inversion path" );
  args.usage( "--length", "15.0", "length of Mellin inversion path" );
  args.usage( "--xmin", "0.001", "minimum value of x" );
  args.usage( "--xmax", "1.0", "maximum value of x" );
  args.usage( "--nx", "10", "number of points to be calculated" );
  args.usage( "--output", "path", "output file name" );

  args.log()
    << "# ----------------------------------------------------------------- #\n"
    << endl;

}


int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  Utility::Timer stopWatch;
  
  if( args.hasOpt( "help" ) ){
    usage();
    return 1;
  }
  
  double Q2 = args.get( "Q2", 2.5 );
  double xmin = args.get( "xmin", 1.0E-3 );
  double xmax = args.get( "xmax", 1.0 );
  int nx = args.get( "nx", 10 );
  
  double offset = args.get( "offset", 2.0  );  
  double angle  = args.get( "angle",  0.55  );  
  double length = args.get( "length", 15.0 );  

  // set up output file
  ofstream* ofs = NULL;
  ostream& os = 
    ( args.hasOpt( "output" ) ?
      *( ofs = new ofstream( args.getOpt( "output" ).c_str() ) ) : cout );
  
  try {
    
    // create polarzed PDF set
    polPDF::Evo* pdf = polPDF::Evo::instance();
    
    // create g1 in Moment space
    polDIS::StrFunc g1Np( pdf, Xsection::Charge::p );
    g1Np.setQ2( Q2 );
    
    // create g1 in x space
    Xsection::xXsec g1Xp( &g1Np );
    g1Xp.setParameter( offset, angle );
    g1Xp.upper( length );
    
    double lxmin = log10( xmin );
    double lxmax = log10( xmax );
    double dlx = ( lxmax - lxmin ) / nx;
    
    stopWatch.start();
    
    for( double lx = lxmin; lx < lxmax; lx += dlx ){
      double x = pow( 10.0, lx );
      os << setw(16) << x
	 << setw(16) << g1Xp( x )
	 << endl;
    }
    
    stopWatch.stop();
    
    args.err()
      << "Total spent time:  " 
      << setw(16) << stopWatch.check( Utility::Timer::Msec )
      << " ms\n"
      << "                :  "
      << setw(16) << stopWatch.check( Utility::Timer::Msec ) / nx
      << " ms / points"
      << endl;
  }
  
  catch( int err ){
    args.err() << "Error occured ( error code = " << err << " )" << endl;
  }
  
  // delete created object
  if( ofs ) delete ofs;
  
  return 0;
}
