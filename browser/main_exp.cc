#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Xsec/xXsec.hh>
#include <polDIS/StrFunc.hh>
#include <unpolDIS/StrFunc.hh>
#include <FragXsec_T/FragXsec.hh>
#include <ErrorCalc/ErrCalc.hh>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace Utility;
using namespace Evolution;
using namespace QCD;
using namespace ErrorCalc;

bool q2judge( const double& q2 );
int swapID( const int& ii );

//! for pdf output
void set_xs_( const double& qq2, const double& x );
//! for error output
void set_exs_( const double& qq2, const double& x );

string output;
string SORT;
// pdf values
double xs_q;
// errors of pdfs ( note sigma^2 )
double exs_q;

Xsection::xXsec* g_f1x;
ErrCalc* eCalc;

vector< double > q2( 0 );
double qq2max( 0.0 );
double qq2tmp( 0.0 );

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{

    //! ----- plot Q2 ------ //
    vector< double > q2tmp( 0 );
    q2tmp.push_back(   1.0 );
    q2tmp.push_back(   2.0 );
    q2tmp.push_back(   4.0 );
    //    q2tmp.push_back(  10.0 );
    q2tmp.push_back(  20.0 );
    //    q2tmp.push_back( 100.0 );
    q2tmp.push_back( 200.0 );

    q2 = q2tmp;

    //!----------------- strfunc set --------------------//
    Xsection::Xsec* strfunc;
    
    if( ! args.hasOpt( "dtype" ) ) throw( 1 );
    SORT = args.get( "dtype", "FF" );

    if ( SORT == "FF" || SORT == "KretzerFF" || SORT == "DSSFF" )
      { strfunc = new FragXsec_T::FragXsec( args, QCD::Flavor::PIp ); }
    else if( SORT == "polPDF" )
      { strfunc = new polDIS::StrFunc( args, Xsection::Charge::p ); }
    else if( SORT == "unpolPDF" || SORT == "MRST" )
      { strfunc = new unpolDIS::StrFunc( args, Xsection::Charge::p ); }
    else return 0;

    //!----------- parameters needed for imazu x space calc -------------// 
    int nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    double q2max, q2maxPDF=200.0, q2maxFF=200.0; 
    ( ( SORT == "FF" || SORT == "KretzerFF" || SORT == "DSSFF" ) ? 
      q2max = q2maxFF: q2max = q2maxPDF ); 
    qq2max = q2max;
    
    //! ------------- strfunc output ------------------//
    
    //! ------------- set filename ---------------//
    string filename;
    
    filename = "xsout" + args.getOpt( "order" ) + SORT;
    if( args.hasOpt( "rescaleFF" ) ) 
      filename += "_r" + args.get( "rescaleFF", "1.0" );
    if( args.hasOpt( "wError" ) ) filename += "_werr";
    if( args.hasOpt( "kret" ) ) filename += "_kret";
    filename += ".dat";	 
    
    //  ofstream outfile( filename.data() );
    ofstream outfile( filename.c_str() );

    //!---------- set pdfxs -----------//
    Xsection::xXsec f1x( strfunc, 4, 6,
			 args.get( "precision", 1.0E-4 ) ); 
    
    double offset = args.get( "offset", 2.0  );  
    double angle  = args.get( "angle",  0.50  );  
    double length = args.get( "length", 15.0 );  

    f1x.setParameter( offset, angle );
    f1x.upper( length );

    g_f1x = &f1x;

    //!---------- set error calc -----------//    
    ErrCalc ecal( &f1x );
    eCalc = &ecal;

    if ( outfile ){

      //!------- q2 loop --------//      
      for( int kk = 0; kk < q2.size(); kk++ ){
        
        double qq2 = q2[ kk ];

          strfunc->setQ2( qq2 );

	    for( int jj = 0; jj <= nx; jj ++ ){
	      
	      double x = xmin * pow( 10.0, jj * dx );
	      double fact =
		( SORT == "FF" || SORT == "KretzerFF" ) ? 2.0 : x / 2.0;
	      
	      set_xs_( qq2, x );

	      if( args.hasOpt( "wError") ) if( !( jj % 5 ) )
		set_exs_( qq2, x );

	      outfile << setw(16) << x << setw(16) << qq2 
		      << setw(16) << flush;

	      outfile << fact * xs_q << setw(16) << flush;
	      if( args.hasOpt( "wError") ) if( !( jj % 5 ) )
		outfile << fact * sqrt( exs_q ) 
			<< setw(16) << flush; // error 
	      outfile << endl;

	      
	    } // x loop

      }	// q2 loop
      
    } // outfile
    
  } // try
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }
  
  return 0;
}

bool q2judge( const double& qq2 ){

  for( int i = 0; i < q2.size(); i++ )
    if( q2[ i ] - 1.0E-4 <= qq2 || qq2tmp == qq2 ){
      qq2tmp = qq2; q2[ i ] = qq2max;
      return true;
    } 
  
  return false;
}

void set_xs_( const double& qq2, const double& x ){
  xs_q = ( q2judge( qq2 ) ? (*g_f1x)( x ) : 0.0 );  
}

void set_exs_( const double& qq2, const double& x ){
  exs_q = ( q2judge( qq2 ) ? (*eCalc)( x ) : 0.0 );
}
