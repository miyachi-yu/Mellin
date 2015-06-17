// $Id: main.cc,v 1.7 2008/10/08 12:43:55 miyachi Exp $
/*!
  \file     main.cc
  \date     $Date: 2008/10/08 12:43:55 $
  \version  $Revision: 1.7 $
  \author   $Author: miyachi $
  \brief    This is a sample code for this package.    
  
 */
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

#include "GRSV2000.hh"
#include "AAC.hh"
#include "LSS.hh"
#include "BB.hh"
#include "FF.hh"

#include <Utility/Arguments.hh>

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  vector< PDFLIB::Package* > packages(0);
  
  if( args.hasOpt( "GRSV2000" ) ){
    packages.push_back( 
		       new PDFLIB::GRSV2000( PDFLIB::GRSV2000::STANDARD_NLO,
					     args.getOpt( "GRSV2000" ) ) ) ;
  }
  
  if( args.hasOpt( "AAC" ) ){
    packages.push_back( new PDFLIB::AAC( 1, args.getOpt( "AAC" ) ) );
  }
  
  if( args.hasOpt( "LSS" ) ){
    packages.push_back( new PDFLIB::LSS( 1, args.getOpt( "LSS" ) ) );
  }
  
  if( args.hasOpt( "BB" ) ){
    packages.push_back( new PDFLIB::BB( 1, args.getOpt( "BB" ) ) );
  }

  if( args.hasOpt( "FF" ) ){
    packages.push_back( new PDFLIB::FF( 1, args.getOpt( "FF" ) ) );
  }
  
  QCD::Flavor::PARTON pid = 
    (QCD::Flavor::PARTON) QCD::Flavor::id( args.get( "parton", "gluon" ) );

  double Q2 = args.get( "Q2", 1.0 );
  double weight = args.get( "weight", 0.0 );
  for( int i=0; i < packages.size(); i++ ) {
    packages[i]->setQ2( 1.0 );
    packages[i]->setPID( pid );
    packages[i]->xWeight( weight );
  }
  
  
  double lxmin = log10( args.get( "xmin", 0.001 ) );
  double lxmax = log10( args.get( "xmax", 1.0   ) );
  double dlx   = ( lxmax - lxmin ) / args.get( "nx", 20 );

  cout << endl;
  cout << setw(13) << "x" << flush;
  for( int i=0; i < packages.size(); i++ ) {
    cout << setw(13) << packages[i]->name()  <<flush;
  }
  cout << endl; 
  
  cout << setw(13) << "----------" << flush;
  for( int i=0; i < packages.size(); i++ ) {
    cout << setw(13) << "----------" << flush;
  }
  cout << endl; 
  
  for( double lx = lxmin; lx < lxmax; lx += dlx ){
    double x = pow( 10.0, lx );
    cout << setw(13) << x << flush;
    
    for( int i=0; i < packages.size(); i++ ) {
      PDFLIB::Package& pdf = *( packages[i] );
      cout << setw(13) << pdf( x ) <<flush;
    }
    cout << endl;
  }
  
  for( int i=0; i < packages.size(); i++ ) delete packages[i];

  cout << endl; 


  return 0;
}
