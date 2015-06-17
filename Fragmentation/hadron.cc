//
// hadron.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Wed Oct 29 11:41:10 2008 Yoshiyuki Miyachi
// Started on  Wed Oct 29 11:41:10 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

#include <Utility/Arguments.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Evolution/KernelSum.hh>
#include <Evolution/PDF.hh>

#include "Evolution.hh"

using namespace std;

void addem( vector< QCD::PdfErrMatrix >& vem, QCD::PdfParametrization *pdf ) {
  
  if( pdf == NULL || pdf->emNumber() < 0 ) return;
  
  vector< QCD::PdfErrMatrix >::iterator itr = 
    find( vem.begin(), vem.end(), QCD::PdfErrMatrix( pdf->emNumber() ) );
  
  if( itr != vem.end() ){
    (*itr).load( pdf );
  } else {
    vem.push_back( QCD::PdfErrMatrix( pdf ) );
  }
  
}

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  // check if pion fragmentation function is given or not
  vector< string > optPi = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string >::iterator itrPi = args.hasOpt( optPi );
  if( itrPi == optPi.end() ){
    
    ARG_ERR 
      << args.command() 
      << " requires one of the follwing option for pion" 
      << endl;
    
    for( int i = 0; i < optPi.size(); i++ ){
      ARG_ERR << "\t--" << optPi[ i ] << endl;
    }
    
    return 1;
  }
  
  // check if kaon fragmentation function is given or not
  vector< string > optKa = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string >::iterator itrKa = args.hasOpt( optKa );
  if( itrKa == optKa.end() ){
    
    ARG_ERR 
      << args.command() 
      << " requires one of the follwing option for kaon" 
      << endl;
    
    for( int i = 0; i < optKa.size(); i++ ){
      ARG_ERR << "\t--" << optKa[ i ] << endl;
    }
    
    return 1;
  }
  
  // check if rest fragmentation function is given or not
  vector< string > optRe = Fragmentation::Evo::options( QCD::Flavor::REp );
  vector< string >::iterator itrRe = args.hasOpt( optRe );
  if( itrRe == optRe.end() ){
    
    ARG_ERR 
      << args.command() 
      << " requires one of the follwing option for rest" 
      << endl;
    
    for( int i = 0; i < optRe.size(); i++ ){
      ARG_ERR << "\t--" << optRe[ i ] << endl;
    }
    
    return 1;
  }
  
  // check parton type
  QCD::Flavor::PARTON pid;
  QCD::Flavor::TYPE   type;
  string parton = args.get( "parton", "gluon" );
  bool isParton = QCD::Flavor::isParton( parton );
  if( isParton ){
    pid = ( QCD::Flavor::PARTON ) QCD::Flavor::id( parton );
  } else {
    type = (QCD::Flavor::TYPE ) QCD::Flavor::id( parton );
  }
  
  if( isParton == false ){
    ARG_ERR << "--parton should be defined for quark flavor or g" << endl;
    return 2;
  }
  
  // check moment to be computed
  complex< double > moment = 
    args.get( "moment", complex< double >( 2.0, 0.0 ) );
  
  vector< double > vQ2( 1, 1.0 );
  vQ2 = args.get( "Q2", ",", vQ2);
  
  cout << endl;
  cout << "# " << args.command() << endl;
  cout << "#"  << endl;
  cout << "#  --" << *itrPi << "=" << args.getOpt( *itrPi ) << endl;
  cout << "#  --" << *itrKa << "=" << args.getOpt( *itrKa ) << endl;
  cout << "#  --" << *itrRe << "=" << args.getOpt( *itrRe ) << endl;
  cout << "#  --parton=" << parton << endl;
  cout << "#  --moment=" << moment << endl;
  cout << "#  --Q2=" << flush;
  for( int i = 0; i < vQ2.size(); i++ ){
    cout << vQ2[i] << flush;
    if( i != vQ2.size() - 1 ) cout << "," << flush;
    else cout << endl;
  }
  cout << endl;
  


  try{
    
    Fragmentation::Evo *pion = new Fragmentation::Evo( args, QCD::Flavor::PIp, *itrPi );
    pion->constNf( false );
    
    Fragmentation::Evo *kaon = new Fragmentation::Evo( args, QCD::Flavor::KAp, *itrKa );
    kaon->constNf( false );
    
    Fragmentation::Evo *rest = new Fragmentation::Evo( args, QCD::Flavor::REp, *itrRe );
    rest->constNf( false );

    Evolution::KernelSum* hadron = new Evolution::KernelSum;
    hadron->add( pion );
    hadron->add( kaon );
    hadron->add( rest );
    
    if( 1 ){

      Evolution::PDF DqPi( pion, pid );
      Evolution::PDF DqKa( kaon, pid );
      Evolution::PDF DqRe( rest, pid ); 
      Evolution::PDF DqHa( hadron, pid );

      cout 
	<< setw(6)  << "Q2"
	<< setw(16) << "moment"
	<< setw(16) << "pion"
	<< setw(16) << "kaon"
	<< setw(16) << "rest"
	<< setw(16) << "hadron"
	<< setw(16) << "difference"
	<< endl;
      
      cout 
	<< setw(6)  << "------"
	<< setw(16) << "------------"
	<< setw(16) << "------------"
	<< setw(16) << "------------"
	<< setw(16) << "------------"
	<< setw(16) << "------------"
	<< setw(16) << "------------"
	<< endl;
      
      for( int i = 0; i < vQ2.size(); i++ ){
	
	double& Q2 = vQ2[ i ];
      
	pion->setQ2( Q2 );
	kaon->setQ2( Q2 );
	rest->setQ2( Q2 );
	hadron->setQ2( Q2 );
	
	complex< double > dqpi = DqPi( moment );
	complex< double > dqka = DqKa( moment );
	complex< double > dqre = DqRe( moment );
	complex< double > dqha = DqHa( moment );
	
	cout << setw(6)  << Q2     << flush;
	cout << setw(16) << moment << flush;
	cout << setw(16) << dqpi << flush;
	cout << setw(16) << dqka << flush;
	cout << setw(16) << dqre << flush;
	cout << setw(16) << dqha << flush;
	cout << setw(16) << dqha - dqpi - dqka - dqre<< flush;
	cout << endl;
	
      }
      cout << endl;
    }
    
    if( 1 ){
      cout 
	<< "#-------------------------------------------------------------\n" 
	<< "# check error matrix stored in the initial distributions \n"
	<< "# \n"
	<< endl;
      
      vector< QCD::PdfErrMatrix > vem(0);
      addem( vem, pion->initialDistribution() );
      addem( vem, kaon->initialDistribution() );
      addem( vem, rest->initialDistribution() );
      
      for( int i = 0; i < vem.size(); i++ ){
	if( vem.size() > 0 ){
	  cout << "#-------------------------------------------------------------" << endl;
	  cout << "#   Error matrix " << endl;
	  cout << "#" << endl;
	  cout << vem[ i ] << endl;
	}
      }
      
      cout << "#           end of error matrix check..." << endl;
    }
    
    delete hadron, pion, kaon, rest;
  }
  
  catch( int error ){
    cerr << "catch error " << endl;
    return 1;
  }


  
  cout << "********************************************************" << endl;
  cout << "***********              end            ****************" << endl;
  cout << "********************************************************" << endl;
  
  return 0;
}
