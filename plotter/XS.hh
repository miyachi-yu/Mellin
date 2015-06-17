#ifndef _plotter_XS_hh_
#define _plotter_XS_hh_

#include <Utility/Arguments.hh>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TGraph.h>
#include <algorithm>

using namespace std;

struct XSdata {
  double z;
  double q2;
  double q;
  double qerr;
  
  XSdata() : z( 0.0 ), q2( 0.0 ), q( 0.0 ), qerr( 0.0 )
  { }
  
  friend istream& operator>>( istream& is, XSdata& data ) throw( int ) {
    double tmp;
    
    if( !( is >> data.z >> data.q2 >> data.q ) )
      throw( 1 );
    
    if( is >> tmp ) data.qerr = tmp;
    
    return is;
  }
  
  friend ostream& operator<<( ostream& os, XSdata& data ){
    os << setw(11) << data.z << '\t' 
       << setw(11) << data.q2 << '\t' 
       << setw(11) << data.q << '\t' 
       << setw(11) << data.qerr;
    return os;
  }
  
};

class XS {
  vector< double > z_;
  vector< double > q2_;
  map< int, map< double, map< double, double > > > D_q_;
  // ( q, qerr )

public:
  
  //constructor //
  XS( string file ) : z_( 0 ), q2_( 0 ){
    ifstream ifs( file.c_str() );
    if( ifs ){
      cout << "Load:\t" << file << endl;
      readDataFrom( ifs );
      cout << "Done:\t" << file << endl;
    } else {
      cout << "Fail:\t" << file << endl;
    }
  };
  
  vector< TGraph* > getData( const double& q2, const double& scale = 1.0 ){

    vector< TGraph* > tmp_tgraph( 0 );

    double tmpq2 = -1.0;

    sort( q2_.begin(), q2_.end() );
    for( int i = 0; i < q2_.size(); i++ ) 
      if( q2_[ i ] >= q2 ){ tmpq2 = q2_[ i ]; break; }

    if( tmpq2 == -1.0 ){
      cout << "exceed Q2 region" << endl;
      return vector< TGraph* >( 0 );
    }
    
    vector< double > x_cent_( 0 ), y_cent_( 0 );

    for( int j = 0; j < 2 ; j++ ){

    vector< double > x_( 0 ), y_( 0 );

    for( map< double, map< double, double > >::iterator itrz
	   = D_q_[ j ].begin(); itrz != D_q_[ j ].end(); itrz++ ){

      map< double, double >::iterator itrq2 
	= itrz->second.find( tmpq2 );
      if( itrq2 == itrz->second.end() ) continue;

      x_.push_back( itrz->first ); 
      y_.push_back( itrq2->second * scale ); 
    }

    if( j == 0 ){ x_cent_ = x_; y_cent_ = y_; }

    double xx1_[ x_.size() ], yy1_[ y_.size() ], yy2_[ y_.size() ];

    int xcount( 0 );

    for( int i = 0; i < x_.size(); i++ ) if( y_[ i ] != 0.0 )
      if( !( Utility::Arguments::instance()->hasOpt( "FF" ) && y_[ i ] <= 0 ) )
	{
	  xx1_[ xcount ] = x_[ i ];
	  yy1_[ xcount ] = y_[ i ];
	  if( j != 0 ){
	    yy1_[ xcount ] = y_cent_[ i ] + yy1_[ xcount ];
	    yy2_[ xcount ] = 2.0 * y_cent_[ i ] - yy1_[ xcount ];
	  }
	  xcount++;
	}
    
    if( xcount != 0 ){
      tmp_tgraph.push_back( new TGraph( xcount, xx1_, yy1_ ) );
      if( j != 0 ) tmp_tgraph.push_back( new TGraph( xcount, xx1_, yy2_ ) );
    }

    }

    return tmp_tgraph;

  }
  
private:

  bool readDataFrom( istream& is ){
    
    XSdata data; 
    string str;

    while( getline( is, str ) ){

      istringstream iss( str );
      iss >> data;

      vector< double >::iterator itrz 
	= find( z_.begin(), z_.end(), data.z );
      if( itrz == z_.end() ){ 
	z_.push_back( data.z );
	itrz = --z_.end(); 
      }

      vector< double >::iterator itrq2 
	= find( q2_.begin(), q2_.end(), data.q2 );
      if( itrq2 == q2_.end() ){ 
	q2_.push_back( data.q2 );
	itrq2 = --q2_.end();
      }

      D_q_[ 0 ][ *itrz ][ *itrq2 ] = data.q;
      D_q_[ 1 ][ *itrz ][ *itrq2 ] = data.qerr;

    }
  }
  
};

#endif // _plotter_XS_hh_
