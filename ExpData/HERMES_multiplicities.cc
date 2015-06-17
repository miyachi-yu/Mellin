#include "HERMES_multiplicities.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <QCD/PdfErrMatrix.hh>

using namespace std;

const string directory = Exp::data_path + "HERMES-multiplicity/";

namespace Exp {

  namespace HERMES_multiplicities {
    
    string filename( const Exp::TARGET& t = Exp::PRO, 
		     const Exp::HADRON& h = Exp::PIp,
		     bool fXZ3D = true ){
      
      string fname = 
	string( fXZ3D ? "zx-3D/zx-proj/" : "" ) +
	"hermes." +
	string( t == Exp::PRO ? "proton" : "deuteron" ) + 
	string( fXZ3D ? ".zx-3D.zx-proj" : ".zQ2-3D.zQ2-proj" ) +
	".vmsub.mults_";
      
      switch( h ){
      case Exp::PIp : fname += "piplus"; break;
      case Exp::PIm : fname += "piminus"; break;
      case Exp::Kp :  fname += "kplus"; break;
      case Exp::Km :  fname += "kminus"; break;
      }
      
      return directory + fname + ".list";
    }
    
    string emfilename( const Exp::TARGET& t = Exp::PRO, 
		       bool fXZ3D = true ){
      string fname = 
	string( fXZ3D ? "zx-3D/zx-proj/" : "" ) +
	"hermes." +
	string( t == Exp::PRO ? "proton" : "deuteron" ) + 
	string( fXZ3D ? ".zx-3D.zx-proj" : ".zQ2-3D.zQ2-proj" ) +
	".vmsub.covmat_mults.list";
      
      return directory + fname;
    }
    
    DataSet multiplicities( const Exp::TARGET& t, 
			    const Exp::HADRON& h ) throw( int ) {

      srandom( time( NULL ) );
      int errorID = random();
      //      int errorID = 872013074029;
      // z-bin information can be obtained from
      // http://hermesmults.appspot.com/
      vector< double > zedge;
      zedge.push_back( 0.1 );
      zedge.push_back( 0.2 );
      zedge.push_back( 0.3 );
      zedge.push_back( 0.4 );
      zedge.push_back( 0.6 );
      zedge.push_back( 0.8 );
      zedge.push_back( 1.0 );

      vector< double > xedge;
      xedge.push_back( 0.023 );
      xedge.push_back( 0.04 );
      xedge.push_back( 0.055 );
      xedge.push_back( 0.075 );
      xedge.push_back( 0.1 );
      xedge.push_back( 0.14 );
      xedge.push_back( 0.2 );
      xedge.push_back( 0.3 );
      xedge.push_back( 0.4 );
      xedge.push_back( 0.6 );

      vector< double > q2edge;
      q2edge.push_back( 1.0 );
      q2edge.push_back( 1.25 );
      q2edge.push_back( 1.5 );
      q2edge.push_back( 1.75 );
      q2edge.push_back( 2.0 );
      q2edge.push_back( 2.25 );
      q2edge.push_back( 2.5 );
      q2edge.push_back( 3.0 );
      q2edge.push_back( 5.0 );
      q2edge.push_back( 15.0 );
      
      DataSet dset;
      dset.emNumber() = errorID;
      dset.emid()     = 1;

      string fname = filename( t, h );
      cout << "Open: " << fname << endl;
      
      ifstream ifs( fname.c_str() );
      string ldata;
      while( ! ( getline( ifs, ldata ) ).eof() ){
	if( ldata[ 0 ] == '#' ) continue;
	
	double index, data, stat, syst, Q2, x, z, php;
	
	istringstream istr( ldata );
	if( ! ( istr >> index >> data >> stat >> syst >> Q2 >> x >> z >> php ) )
	  continue;
	DataPoint dp;
	dp.info() = "Multi_" + tarname( t ) + "_" + hadname( h );
	dp.data() = data;
	dp.stat() = stat;
	dp.syst( syst, "all" );
	dp.kine( Q2, "Q2", "GeV^2" );
	dp.kine( x, "x", "" );
	dp.kine( z, "z", "" );
	dp.kine( php, "php", "GeV/c" );
	dp.kine( index, "index", "" );
	
	int i = 0;
	for( i = 0; i < zedge.size() - 1; i++ )
	  if( zedge[ i ] < z && zedge[ i + 1 ] > z ) break;
	
	dp.kine( zedge[ i ], "zmin", "" );
	dp.kine( zedge[ i + 1], "zmax", "" );
	dp.kine( z, "zave", "" );
	
	for( i = 0; i < xedge.size() - 1; i++ )
	  if( xedge[ i ] < x && xedge[ i + 1 ] > x ) break;
	
	dp.kine( xedge[ i ], "xmin", "" );
	dp.kine( xedge[ i + 1], "xmax", "" );
	dp.kine( x, "xave", "" );
	
	dp.kine( Q2, "Q^2 ave", "GeV^2" );

	dp.kine( 56.2, "s", "GeV^2" ); 
	
	dset.push_back( dp );
      }
      
      dset.info() = "HERMES_Multi_" + tarname( t ) + "_" + hadname( h );
      dset.ref()  = "Phys. Rev. D 87 (2013) 074029";
      dset.cite() = "Airapetian:2012ki";
      
      for( int idata = 0; idata < dset.size(); idata++ ){
	DataPoint& dp = dset[ idata ];
	dp.emNumber() = errorID;
	dp.emid()     = 1;
	dp.ems().resize( dset.size(), 0.0 );
      }
      
      // error matrxi 
      ifstream iem( emfilename().c_str() );
      
      while( ! ( getline( iem, ldata ) ).eof() ){
	if( ldata[ 0 ] == '#' ) continue;
	int i, j;
	double pip, pim, kap, kam;
	istringstream istr( ldata );
	if( ! ( istr >> i >> j >> pip >> pim >> kap >> kam ) ) continue;
	double eval;
	switch( h ){
	case Exp::PIp : eval = pip; break;
	case Exp::PIm : eval = pim; break;
	case Exp::Kp :  eval = kap; break;
	case Exp::Km :  eval = kam; break;
	}
	dset[ i ].ems()[ j ] = eval;
      }
      
      return dset;
    };
  };
};
