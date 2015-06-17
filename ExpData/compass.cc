#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Exp;

namespace COMPASS {
  
  DataSet g1d( const string& fname,
	       const string& info,
	       const string& ref ) throw( int ) {
    
    ifstream ifs( fname.c_str() );
    if( ! ifs ) {
      cerr << "Fail to open data file" << endl;
      throw( 1 );
    }
    
    DataSet ep;
    ep.info() = info;
    ep.ref()  = ref;
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      istringstream ist( buff );
      DataPoint dd;
      
      double xmin, xmax, x, q2, y, a1d, da1d, sa1d, g1d, dg1d, sg1d;
      if( ! ( ist 
	      >> x >> xmin >> xmax >> q2 >> y
	      >> a1d >> da1d >> sa1d 
	      >> dd.data() >> dd.stat() >> sg1d ) ) continue;
      
      dd.info() = "g1d";
      dd.unit() = "";
      dd.kine( xmin, "xmin", "" );
      dd.kine( xmax, "xmax", "" );
      dd.kine(    x,    "x", "" );
      dd.kine(   q2,  "Q^2", "GeV^2" );
      dd.kine(    y,    "y", "" );
      dd.syst( sg1d, "syst" );
      
      ep.push_back( dd );
      
      count++ ;
    }
    return ep;
  }
  
}

