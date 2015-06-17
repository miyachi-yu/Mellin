//
// expManip.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Fri Jan 30 09:51:19 2009 Yoshiyuki Miyachi
// Started on  Fri Jan 30 09:51:19 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>


#include "ExpData.hh"

using namespace std;


vector< int > getSubID( const string& str, const string& sep="," ){
  
  vector< int > ids( 0 );
  if( str == "" ) return ids;
  int pos = 0;
  
  do {
    
    int next = str.find( sep, pos );
    if( next == pos  ) {    
      cerr << "empty data between separators. ignore." << endl;
    } else {

      string substr = str.substr( pos, next );
      int seppos = substr.find( "-", 0 );      
      
      if( seppos == string::npos ){
	istringstream istr( substr );
	int value;
	istr >> value;
	ids.push_back( value ); 
      } else {
	
	int begin_id = atoi( substr.substr( 0, seppos ).c_str() );
	int end_id   = atoi( substr.substr( seppos + 1 ).c_str() );
	
	if( begin_id > end_id ){ // swap index
	  int tempv = begin_id;
	  begin_id = end_id;
	  end_id = tempv;
	}
	
	for( int k = 0; k < end_id - begin_id + 1; k++ ){
	  int value = begin_id + k;
	  ids.push_back( value ); 
	}
      }
    }
    
    pos = next + ( next == string::npos ? 0 : 1 );
  } while( pos != string::npos && pos != str.size() );
  
  return ids;
}


void Usage() {

  Utility::Arguments& args = Utility::Arguments::ref();
 
  args.usage( "expdata", "", "multiple data with \",\"" );
  args.usage( "disable", "", "disable the given DetaSet" );
  args.usage( "enable",  "", "enable  the given DetaSet" );
  args.usage( "remove",  "", "remove  the given DetaSet" );
  args.usage( "oexpdata", "expdata.xml", "set output file name" );
  
  args.usage( "normalize", "", "change normalizaiton conditon with --norm,ncerr,ncstat" );
  args.usage( "norm",     "0.0", "normalization coefficient" );
  args.usage( "ncerr",    "0.0", "normalization coeff error" );
  args.usage( "ncstat", "fixed", "normalization condition" );
  args.usage( "lambda", "1.0", "weight for chi2 calculation" );

  args.usage( "level", "0", "0:normal|1:detail" );

  args.usage( "divide", "", "dividing DataSet objects to different expdata files. (ex., 1-3|4,5,6|7,8,9)" );
  
  ARG_LOG << endl;
  ARG_LOG
    << "+-------------------------------------------------------------+\n"
    << "|  How to use expManip                                        |\n"
    << "+-------------------------------------------------------------+\n"
    << " --expdata option only\n"
    << "     show summary of the given experiment data file(s).\n"
    << "\n"
    << endl;
  
}

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  ARG_LOG << args << endl;
  
  if( args.hasOpt( "help" ) ){
    Usage();
    return 0;
  }
  
  vector< string > path( 0 );
  path = args.get( "expdata", ",", path );
  if( path.size() == 0 ){
    Usage();
    ARG_ERR << "at least one experimental data file must to be given." << endl;
    return 1;
  }
  
  vector< Exp::ExpData > data;
  try {
    for( int i = 0; i < path.size(); i++ ){
      data.push_back( Exp::ExpData( path[ i ] ) );
    }
  }
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    return 1;
  }
  
  // chi2 weight
  if( args.hasOpt( "lambda" ) ){
    double lambda = args.get( "lambda", 1.0 );
    for( int i = 0; i < data.size(); i++ ) data[ i ].lambda() = lambda;
  }
  
  // create sum of all expdata file
  Exp::ExpData sum( data[ 0 ] );
  for( int i = 1; i < data.size(); i++ ) sum += data[ i ];
  
  vector< int > id( 0 );
  
  // set disable flag
  id.clear();
  sum.disable( id = getSubID( args.get( "disable", "" ) ) );
  
  // set enable flag
  id.clear();
  sum.enable( id = getSubID( args.get( "enable", "" ) ) );
  
  // normalization
  id.clear();
  id = getSubID( args.get( "normalize", "" ) );
  
  double norm   = args.get( "norm",   0.0 );
  double ncerr  = args.get( "ncerr",  0.0 );
  double ncoef  = args.get( "ncoef",  0.0 );
  string ncstat = args.get( "ncstat", "fixed" );
  
  if( id.size() > 0 ){
    
    ARG_LOG << "change normalization parameters for " << flush;
    for( int i = 0; i < id.size(); i++ ){
      if( id[ i ] >= sum.data().size() ) continue;
      
      if( args.hasOpt( "norm" ) )   sum.data()[ id[ i ] ].norm()   = norm;
      if( args.hasOpt( "ncoef" ) )  sum.data()[ id[ i ] ].ncoef()  = ncoef;
      if( args.hasOpt( "ncerr" ) )  sum.data()[ id[ i ] ].ncerr()  = ncerr;
      if( args.hasOpt( "ncstat" ) ) sum.data()[ id[ i ] ].ncstat() = ncstat;
      
      ARG_LOG << setw(2) << id[ i ] << flush;
      if( i != id.size() - 1 ) ARG_LOG << "," << flush;
    }

    ARG_LOG << endl;
    ARG_LOG << "  --norm="   << norm   << endl;
    ARG_LOG << "  --ncoef="  << ncoef  << endl;
    ARG_LOG << "  --ncerr="  << ncerr  << endl;
    ARG_LOG << "  --ncstat=" << ncstat << endl;
    
  }
  
  // remove DataSet object
  id.clear();
  sum.remove( id = getSubID( args.get( "remove", "" ) ) );
  
  // output ExpData information
  sum.summary( ARG_LOG, (Exp::SUMMARY_LEVEL) args.get( "level", 0 ) );
  
  // dividing DataSet objects to multiple ExpData
  vector< string > subgrp( 0 );
  vector< vector< int > > subgrpid( 0 );
  subgrp = args.get( "divide", "|", subgrp );
  for( int i = 0; i < subgrp.size(); i++ ){
    subgrpid.push_back( vector< int >( 0 ) );
    vector< int >& ids = subgrpid.back();

    /*
    string& str = subgrp[ i ];
    int pos = 0;
    do {
      int next = str.find( ",", pos );
      string substr = str.substr( pos, next );
      int seppos = substr.find( "-", 0 );      
      if( seppos == string::npos ){
	istringstream istr( substr );
	int value;
	istr >> value;
	if( value < sum.data().size() ){
	  ids.push_back( value ); 
	} else {
	  ARG_ERR << "given index " << value << " is out of range." << endl; 
	}
      } else {
	
	int begin_id = atoi( substr.substr( 0, seppos ).c_str() );
	int end_id   = atoi( substr.substr( seppos + 1 ).c_str() );
	
	if( begin_id > end_id ){ // swap index
	  int tempv = begin_id;
	  begin_id = end_id;
	  end_id = tempv;
	}
	
	for( int k = 0; k < end_id - begin_id + 1; k++ ){
	  int value = begin_id + k;
	  ids.push_back( value ); 
	}
      }
      pos = next + ( next == string::npos ? 0 : 1 );
    } while( pos != string::npos );
    */
    ids = getSubID( subgrp[ i ] );
    ARG_LOG << setw(2) << i << " : " << flush;
    for( int j = 0; j < ids.size(); j++ ) ARG_LOG << setw( 3 ) << ids[ j ];
    ARG_LOG << endl;
  }
  
  if( args.hasOpt( "oexpdata" ) ){
    
    vector< string > ofnames( 0 );
    ofnames = args.get( "oexpdata", ",", ofnames );
    
    if( subgrp.size() == 0 ){

      if( ofnames[ 0 ] == Utility::Arguments::NO_VALUE ||
	  ofnames[ 0 ] == "" ){
	ofnames[ 0 ] = "expdata.xml";
      }
      ARG_LOG << "+++  creating " << ofnames[ 0 ] << endl;
      // get output file name
      ofstream ofs( ofnames[ 0 ].c_str() ); 
      ofs << sum << endl;
      
    } else {
      
      if( ofnames.size() != subgrp.size() ) {
	ofnames.resize( subgrp.size(), "" );
	for( int i = 0; i < ofnames.size(); i++ ){
	  if( ofnames[ i ] == "" ||
	      ofnames[ i ] == Utility::Arguments::NO_VALUE ){
	    ostringstream ofsstr;
	    ofsstr << "expdata_" << i << ".xml";
	    ofnames[ i ] = ofsstr.str();
	  }
	}
      }
      
      for( int i = 0; i < subgrpid.size(); i++ ){
	Exp::ExpData edata;
	for( int j = 0; j < subgrpid[ i ].size(); j++ ) {
	  edata.data().push_back( sum.data()[ subgrpid[ i ][ j ] ] ) ;
	}
	ARG_LOG << "+++  creating group" 
		<< setw(2) << i 
		<< " as " << ofnames[ i ] << endl;
	ofstream ofs( ofnames[ i ].c_str() ); 
	ofs << edata << endl;
      }
    }
  }
  
  return 0;
}
