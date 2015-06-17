#include "Belle.hh"

#include "DataPoint.hh"

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

namespace Exp {
  namespace Belle {
    
    DataSet pion( const Exp::HADRON& h ) throw( int ){
      
      DataSet ds;
      ds.info() = "Belle_2013_"+ Exp::hadname( h );
      ds.ref()  = "arXiv:1301.6183v1";  
      ds.cite() = "";
      ds.norm() = 0.024;
      ds.ncoef() = 0.0;
      ds.ncerr() = 1.0;
      ds.ncstat() ="free";
      double s = 10.52 * 10.52;
      
      double zmin, data, stat, sysp, sysm, frac, efrac;
      
      ifstream ifs1( ( data_path + "/Belle/pion_zmin.dat" ).c_str() );
      ifstream ifs2( ( data_path + "/Belle/pion_multi.dat"  ).c_str() );
      ifstream ifs3( ( data_path + "/Belle/pion_stat.dat"  ).c_str() );
      ifstream ifs4( ( data_path + "/Belle/pion_sys+.dat"  ).c_str() );
      ifstream ifs5( ( data_path + "/Belle/pion_sys-.dat"  ).c_str() );
      ifstream ifs6( ( data_path + "/Belle/pion_strong.dat"  ).c_str() );
      ifstream ifs7( ( data_path + "/Belle/pion_strong_err.dat"  ).c_str() );

      cout << "Loading Belle pion data...." << endl;
      
      while( ifs1 >> zmin && ifs2 >> data && ifs3 >> stat &&
	     ifs4 >> sysp && ifs5 >> sysm &&
	     ifs6 >> frac && ifs7 >> efrac ){
	
	DataPoint p;
	p.info() = "e+e-_PI";
	p.unit() = "";
	p.data() = data * frac;
	p.stat() = stat * frac;
	
	double sys = ( sysp > sysm ? sysp : sysm );
	sys = sqrt( sys * sys + ( data * efrac ) * ( data * efrac ) );
	p.syst( sys, "ALL" );
	
	p.kine( zmin,         "zmin", "" );
	p.kine( zmin + 0.01,  "zmax", "" );
	p.kine( zmin + 0.005, "zmid", "" );
	p.kine( 0.1,          "dz",   "" );
	p.kine( s,            "Q^2",  "GeV^2" );
	
	ds.push_back( p );
	
      }

      return ds;
    }

    DataSet kaon( const Exp::HADRON& h ) throw( int ){

      DataSet ds;
      ds.info() = "Belle_2013_"+ Exp::hadname( h );
      ds.ref()  = "arXiv:1301.6183v1";  
      ds.cite() = "";
      ds.norm() = 0.024;
      ds.ncoef() = 0.0;
      ds.ncerr() = 1.0;
      ds.ncstat() ="free";
      double s = 10.52 * 10.52;
      
      double zmin, data, stat, sysp, sysm, frac, efrac;
      
      ifstream ifs1( ( data_path + "/Belle/kaon_zmin.dat" ).c_str() );
      ifstream ifs2( ( data_path + "/Belle/kaon_multi.dat"  ).c_str() );
      ifstream ifs3( ( data_path + "/Belle/kaon_stat.dat"  ).c_str() );
      ifstream ifs4( ( data_path + "/Belle/kaon_sys+.dat"  ).c_str() );
      ifstream ifs5( ( data_path + "/Belle/kaon_sys-.dat"  ).c_str() );
      ifstream ifs6( ( data_path + "/Belle/kaon_strong.dat"  ).c_str() );
      ifstream ifs7( ( data_path + "/Belle/kaon_strong_err.dat"  ).c_str() );

      cout << "Loading Belle kaon data...." << endl;
      
      while( ifs1 >> zmin && ifs2 >> data && ifs3 >> stat &&
	     ifs4 >> sysp && ifs5 >> sysm &&
	     ifs6 >> frac && ifs7 >> efrac ){
	
	DataPoint p;
	p.info() = "e+e-_KA";
	p.unit() = "";
	p.data() = data * frac;
	p.stat() = stat * frac;
	
	double sys = ( sysp > sysm ? sysp : sysm );
	sys = sqrt( sys * sys + ( data * efrac ) * ( data * efrac ) );
	p.syst( sys, "ALL" );
	
	p.kine( zmin,        "zmin", "" );
	p.kine( zmin + 0.01, "zmax", "" );
	p.kine( zmin + 0.005, "zmid", "" );
	p.kine( 0.1,         "dz",   "" );
	p.kine( s,           "Q^2",  "GeV^2" );
	
	ds.push_back( p );
	
      }

      return ds;
    }
    
  };
};
