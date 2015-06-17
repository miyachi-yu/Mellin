#include "ExpData.hh"
#include "DataSet.hh"

#include <Utility/Arguments.hh>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace Exp;

void usage() {
  Arguments& args = Arguments::ref();
  
  ARG_LOG << args.command() << " requires following options:" << endl;
  ARG_LOG << "\t--expdata=[input expdata file]" << endl;
  ARG_LOG << "\t--oexpdata=[output expdata file]" << endl;
  ARG_LOG << "\t--norm=[Normalization tolerance]" << endl;
  ARG_LOG << "\t--ncoef=[Normalization coefficient]" << endl;
  ARG_LOG << "\t--ncerr=[Normalization coefficient error]" << endl;
  ARG_LOG << "\t--ncstat=[Normalization coefficient status]" << endl;
}

int main( int argc, char* argv[] ){
  
  Arguments& args = Arguments::ref( argc, argv );
  
  if( ! args.hasOpt( "expdata" ) || 
      ! args.hasOpt( "oexpdata" ) ||
      args.hasOpt( "help" ) ) {
    usage();
    return 1;
  }
  
  double norm   = args.get( "norm",  0.05 );
  double ncoef  = args.get( "ncoef", 0.00 );
  double ncerr  = args.get( "ncerr", 1.00 );
  string ncstat = args.get( "ncstat", "free" );

  ARG_LOG
    << "# -----------------------------------------------------------\n"
    << "#             change normalizaiton condition                 \n" 
    << "# -----------------------------------------------------------\n"
    << "  Input data file:           " << args.getOpt( "expdata" ) << "\n"  
    << "  Output data file:          " << args.getOpt( "oexpdata" ) << "\n"
    << "  Normalization Tolerance:   " << norm << "\n"
    << "  Normalization Coefficient: " << ncoef << "\n"
    << "  Normalization Error:       " << ncerr << "\n"
    << "  Normalization Status:      " << ncstat << "\n"
    << endl;

  try{ 
    
    ARG_LOG << "Loading " << args.getOpt( "expdata" ) << endl;
    ExpData expd;
    
    vector< DataSet >& data = expd.data();
    for( int i = 0; i < data.size(); i++ ){
      ARG_LOG << endl;
      ARG_LOG << "-- modify " << data[ i ].info() << endl;
      ARG_LOG << "     norm:   " << setw(8) << data[ i ].norm()   << "-->" << setw(8) << norm << endl;
      ARG_LOG << "     ncoef:  " << setw(8) << data[ i ].ncoef()  << "-->" << setw(8) << ncoef << endl;
      ARG_LOG << "     ncerr:  " << setw(8) << data[ i ].ncerr()  << "-->" << setw(8) << ncerr << endl;
      ARG_LOG << "     ncstat: " << setw(8) << data[ i ].ncstat() << "-->" << setw(8) << ncstat << endl;
      data[ i ].norm()   = norm;
      data[ i ].ncoef()  = ncoef;
      data[ i ].ncerr()  = ncerr;
      data[ i ].ncstat() = ncstat;
    }
    
    string ofile = args.get( "oexpdata", "expdata.xml" );
    if( ofile != "" ){
      ARG_LOG << "Writing " << ofile  << endl;
      ofstream ofs( ofile.c_str() );
      ofs << expd << endl;
    }
    
  }
  
  catch( int err ) {
  }

  return 0;
}
