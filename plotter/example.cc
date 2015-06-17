/*!
  \brief  a sample code to print out PDFs

  This is a sample program to print out FF or PDFs, in order to plot
  with ROOT.

  $Id: example.cc,v 1.1 2008/07/25 09:01:26 miyachi Exp $

 */
#include <iostream>                        // for I/O
#include <iomanip>                         // for I/O manipulation
#include <cmath>                           // math library

#include <Utility/Arguments.hh>            // option handling
#include <QCD/Flavor.hh>
#include <Evolution/PDF.hh>                // D(m)               
#include <Evolution/PDFx.hh>               // D(z)
#include <Fragmentation/Evolution.hh>      // FF evolution

using namespace std;

int main( int argc, char* argv[] ){
  
  // Prepare Arguments object
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( ! args.hasOpt( "FF" ) ) {
    cerr << argv[0] << " requires --FF=[FF config file path]"
	 << endl;
    return 1;
  }
  
  /*
    Arguments::get( "name", value ) method return a value with the 
    given option --name. If the option --name was not given,  
    the default, value, will be return, instead.
   */
  double q2     = args.get( "Q2", 2.5 );     // get Q2 value from option 
  
  // for straight line integration path parameters
  double offset = args.get( "offset", 3.0 ); 
  double angle  = args.get( "angle",  0.55 );
  double length = args.get( "length", 18.0 );

  try {
    
    // create FF Evolution kernel object
    Fragmentation::Evo evo( args, QCD::Flavor::PIp );
    
    // set Q^2
    evo.setQ2( q2 );
    
    // create u quark fragmentation function ( z-space )
    Evolution::PDFx ux( &evo, QCD::Flavor::g );
    ux.setParameter( offset, angle );
    ux.upper( length );

    // create u quark fragmentation function ( m-space )
    //    Evolution::PDF& u = ux.pdf();
    //    cout << setw(18) << u( 2.0 ) << endl;
    
    // calculate D( z, Q^2 )
    for( double lz = -2.0; lz < 0.0; lz += 0.1 ){
      double z = pow( 10.0, lz );
      
      cout << setw( 16 ) << z << flush;
      cout << setw( 16 ) << ux( z ) << flush;
      cout << endl;
      
    }
    
  }
  
  catch( int err ){
  }

  return 0;
}



