#include <stdio.h>
#include <iostream> // cout(cin)
#include <iomanip>  // setw
#include <fstream>  // o(i)fstream
#include <sstream>  // o(i)stringstream
#include <map>
#include <vector>
#include <cmath>
#include <complex>

#include <Utility/Arguments.hh>

using namespace std; 
using namespace Utility;

void tchi2manip( istream& is, const string& fbase );
void dchi2manip( istream& is, const string& fbase );

void tchi2manip_2( istream& is, const string& fbase );

int main( int argc, char* argv[] ){

  Arguments& args = *( Arguments::instance( argc, argv ) );

  // argv[ 0 ] : command
  string path_( argv[ 1 ] );
  /*
  string fname_( argv[ 2 ] );
  */

  bool tchi2_( args.hasOpt( "FNAME1" ) );
  bool dchi2_( args.hasOpt( "FNAME2" ) );
  bool ffsidis_( args.hasOpt( "FFSIDIS" ) );

  if( ! tchi2_ && ! dchi2_ ){
    cout << "Need --FNAME1 (tchi2) or --FNAME2 (dchi2) option" << endl;
    return 0;
  }

  if( ffsidis_ && dchi2_ ){
    cout << "--FFSIDIS requires --FNAME1 (tchi2) option (!= FNAME2)" << endl;
    return 0;
  }

  string fname_ = ( tchi2_ ? args.get( "FNAME1", "" ) 
		    : args.get( "FNAME2", "" ) );

  string ffnamebase_ = path_ + "/";
  string fullfname_ = ffnamebase_ + fname_;

  ifstream ifs( fullfname_.c_str() );

  // Manipulation Starts //
  if( ifs ){
    if( tchi2_ ){ 
      if( ! ffsidis_ ){
	tchi2manip( ifs, ffnamebase_ ); 
      } else {
	tchi2manip_2( ifs, ffnamebase_ ); 
      } 
    }
    if( dchi2_ ) dchi2manip( ifs, ffnamebase_ );
  }

  return 0;
}

void tchi2manip( istream& is, const string& fbase ){

    // for Tchi2
    vector< int >    EVID_( 0 );   // ID info
    vector< vector< double > > Vals_( 7, vector< double >( 0 ) ); // variables
    /*
      vector< double > LAMB_( 0 );   // lambda
      vector< double > TCHI2_( 0 );  // full chi2 for fit
      vector< doulbe > CCHI2_( 0 );  // total chi2
      vector< doulbe > REFVL_( 0 );  // eig vec value
      vector< doulbe > dCCH2_( 0 );  // diff of total chi2 from opt value
      vector< doulbe > dRFVL_( 0 );  // diff of eigvec value from opt value
      vector< doulbe > NdFFV_( 0 );  // diff eigvec value norm. by its error
    */

    string strline;
    int n( 0 );

    // Reading Process //
    while( getline( is, strline ) ){

      istringstream iss( strline );

      EVID_.push_back( 0 );
      for( int i = 0; i < Vals_.size(); i++ )
	Vals_[ i ].push_back( 0.0 );

      iss >> EVID_[ n ];
      for( int i = 0; i < Vals_.size(); i++ )
	iss >> Vals_[ i ][ n ];

      n++;
    }

    // Writing Process //
    string ffbase_ = fbase + "TDATA/LM_EVID_";
    ostringstream ost;

    for( int i = 0; i < EVID_.size(); i++ ){

      ost << ffbase_ << EVID_[ i ] << ".dat";
      ofstream ofs( ost.str().c_str(), ios::out | ios::app );
      ofs << setw( 8 )  << EVID_[ i ]      // i : EVID_
	  << setw( 12 ) << Vals_[ 0 ][ i ] // l : LAMB_
	  << setw( 12 ) << Vals_[ 6 ][ i ] // x : NdFFV_
	  << setw( 12 ) << Vals_[ 4 ][ i ] // y : dCCH2_
	  << endl;
      ofs.close();

      ost.str( "" ); ost.clear(); // ost initialization
      /* find sample
	vector< int > vEVID_( 0 );

	vector< int >::iterator itr // iterator ~ pointer
	= find( vEVID_.begin(), vEVID_.end(), EVID_[ i ] );
	if( itr == vEVID_.end() )	vEVID_.push_back( EVID_[ i ] );
	
	vector< int >::iterator itr = vEVID_.find( EVID_[ i ] );
	#### find : vector -> object search, map -> key search ####
      */
    }

}

void dchi2manip( istream& is, const string& fbase ){

    // for Dchi2
    vector< int >    EVID_( 0 );   // ID info
    vector< string > Dname_( 0 );  // data name
    vector< vector< double > > Vals_( 5, vector< double >( 0 ) ); // variables
    /*
      vector< double > LAMB_( 0 );   // lambda
      vector< doulbe > NdFFV_( 0 );  // diff eigvec value norm. by its error
      vector< double > Dused_( 0 );  // number of used data
      vector< double > Dfree_( 0 );  // number of free data
      vector< double > DCHI2_( 0 );  // chi2 value
    */

    string strline( "" ), judge_( "" );
    int n( 0 );

    // Reading Process //
    while( getline( is, strline ) ){

      istringstream iss( strline );

      EVID_.push_back( 0 );
      Dname_.push_back( "" );
      for( int i = 0; i < Vals_.size(); i++ )
	Vals_[ i ].push_back( 0.0 );

      iss >> EVID_[ n ];
      for( int i = 0; i < 2; i++ )
	iss >> Vals_[ i ][ n ];
      iss >> Dname_[ n ];  
      for( int i = 2; i < Vals_.size(); i++ )
	iss >> Vals_[ i ][ n ];

      iss >> judge_; // E, D judge

      if( judge_ != "D" && Vals_[ 2 ][ n ] != 0.0 ){
	n++; continue;
      }

      EVID_.resize( n, 0 );
      Dname_.resize( n, "" );
      for( int i = 0; i < Vals_.size(); i++ )
	Vals_[ i ].resize( n, 0.0 );

    }

    // Finding 0 Values //
    map< string, vector< double > > Dset_Bchi2_;

    for( int i = 0; i < EVID_.size(); i++ )
      if( ! Vals_[ 0 ][ i ] 
	  && Dset_Bchi2_.end() == Dset_Bchi2_.find( Dname_[ i ] ) ){
	vector< double > tmpv_( 3, 0.0 );
	tmpv_[ 0 ] = Vals_[ 2 ][ i ]; // Dused_
	tmpv_[ 1 ] = Vals_[ 3 ][ i ]; // Dfree_
	tmpv_[ 2 ] = Vals_[ 4 ][ i ]; // DCHI2_
	Dset_Bchi2_[ Dname_[ i ] ] = tmpv_;
      }

    // Data Set Name output //
    string dslist_ = fbase + "DDATA/DSET_list.dat";
    ofstream ofsdsl_( dslist_.c_str(), ios::out );
    cout << "There were " << Dset_Bchi2_.size()
	 << " Kinds of Data Set" << endl;
    for( map< string, vector< double > >::iterator itr = Dset_Bchi2_.begin(); 
	 itr != Dset_Bchi2_.end(); itr++ ){
      cout << itr->first << endl;
      ofsdsl_ << setw( 25 ) << itr->first 
	      << setw( 8 ) << ( itr->second )[ 0 ]
	      << setw( 8 ) << ( itr->second )[ 1 ] 
	      << setw( 8 ) << ( itr->second )[ 2 ] 
	      << endl;
    }
    ofsdsl_.close();

    // Writing Process //
    string ffbase_ = fbase + "DDATA/LM_EVID_";
    ostringstream ost;

    for( int i = 0; i < EVID_.size(); i++ ){

      ost << ffbase_ << EVID_[ i ] << "_" << Dname_[ i ] << ".dat";
      ofstream ofs( ost.str().c_str(), ios::out | ios::app );
      //      ofs << setw( 25 ) << Dname_[ i ]     // n : Dname_
      //	  << setw( 8 )  << EVID_[ i ]      // i : EVID_
      ofs << setw( 8 )  << EVID_[ i ]      // i : EVID_
	  << setw( 12 ) << Vals_[ 0 ][ i ] // l : LAMB_
	  << setw( 12 ) << Vals_[ 1 ][ i ] // x : NdFFV_
	  << setw( 12 ) 
	  << Vals_[ 4 ][ i ] - Dset_Bchi2_[ Dname_[ i ] ][ 2 ] // y : dCCH2_
	  << endl;
      ofs.close();

      ost.str( "" ); ost.clear(); // ost initialization
    }

}

void tchi2manip_2( istream& is, const string& fbase ){

  // for Tchi2 (FF vs SIDIS)
  vector< string >    HADID_( 0 );   // ID info
  vector< vector< double > > Vals_( 6, vector< double >( 0 ) ); // variables
  /*
    vector< double > LAMB_( 0 );   // lambda
    vector< double > TCHI2_( 0 );  // full chi2 for fit
    vector< doulbe > CCHI2_( 0 );  // total chi2 (FF)
    vector< doulbe > REFVL_( 0 );  // total chi2 (A1H)
    vector< doulbe > dCCH2_( 0 );  // diff of total chi2(FF) from opt value
    vector< doulbe > dRFVL_( 0 );  // diff of total chi2(A1H) from opt value
  */
  
  string strline;
  int n( 0 );

  // Reading Process //
  while( getline( is, strline ) ){
    
    istringstream iss( strline );

    HADID_.push_back( "" );
    for( int i = 0; i < Vals_.size(); i++ )
      Vals_[ i ].push_back( 0.0 );
    
    iss >> HADID_[ n ];
    for( int i = 0; i < Vals_.size(); i++ )
      iss >> Vals_[ i ][ n ];
    
      n++;
  }

  // Writing Process //
  string ffbase_ = fbase + "TDATA/LM_HADID_";
  ostringstream ost;
  
  for( int i = 0; i < HADID_.size(); i++ ){
    
    ost << ffbase_ << HADID_[ i ] << ".dat";
    ofstream ofs( ost.str().c_str(), ios::out | ios::app );
    ofs << setw( 8 )  << HADID_[ i ]     // i : HADID_
	<< setw( 12 ) << Vals_[ 0 ][ i ] // l : LAMB_
	<< setw( 12 ) << Vals_[ 5 ][ i ] // x : dRFVL_
	<< setw( 12 ) << Vals_[ 4 ][ i ] // y : dCCH2_
	<< endl;
    ofs.close();
    
    ost.str( "" ); ost.clear(); // ost initialization
  }
  
}
