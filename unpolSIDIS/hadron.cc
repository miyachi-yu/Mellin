//
// hadron.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Fri Nov  7 16:25:29 2008 Yoshiyuki Miyachi
// Started on  Fri Nov  7 16:25:29 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>

#include <Utility/Arguments.hh>
#include <Evolution/KernelSum.hh>
#include <Evolution/PDF.hh>
#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Xsec/xXsec.hh>

#include "StrFunc.hh"

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  vector< string > optPi = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string > optKa = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string > optRe = Fragmentation::Evo::options( QCD::Flavor::REp );
  vector< string > optPDF = unpolPDF::Evo::options();
  
  vector< string >::iterator itrPi = args.hasOpt( optPi );
  vector< string >::iterator itrKa = args.hasOpt( optKa );
  vector< string >::iterator itrRe = args.hasOpt( optRe );
  vector< string >::iterator itrPDF = args.hasOpt( optPDF );
  
  if( itrPi == optPi.end() ){
    ARG_ERR << "This requires pion fragmentation funtion." << endl;
    return 1;
  }
  
  if( itrKa == optKa.end() ){
    ARG_ERR << "This requires kaon fragmentation funtion." << endl;
    return 2;
  }
  
  if( itrRe == optRe.end() ){
    ARG_ERR << "This requires rest fragmentation funtion." << endl;
    return 3;
  }
  
  /*
  if( itrPDF == optPDF.end() ){
    ARG_ERR << "This requires unpolarized parton distribution funtion." << endl;
    return 3;
  }
  */

  ARG_LOG << args << endl;
  
  vector< double > q2( 1, 1.0 );    // get Q2 values
  vector< double > N( 1, 4.0 ), M( 1, 4.0 );
  vector< double >  angle( 2, 0.55 );
  vector< double >  offset( 2, 3.0 );
  vector< double >  length( 2, 8.0 ); 
  vector< int > leg( 2, 6 ); leg[1] = 8;
  double  prec = 5.0E-3;
  int     grid = 2;
  vector< double > z( 2 ); z[ 0 ] = 0.2; z[ 1 ] = 0.8;
  vector< double > x( 0 );
  x.push_back( 0.01 );
  x.push_back( 0.03 );
  x.push_back( 0.07 );
  x.push_back( 0.1 );
  x.push_back( 0.3 );
  x.push_back( 0.7 );
  
  N      = args.get( "N", ",",  N );
  M      = args.get( "M", ",",  M );
  q2     = args.get( "Q2",     ",", q2 );
  angle  = args.get( "angle",  ",", angle );
  offset = args.get( "offset", ",", offset );
  length = args.get( "length", ",", length );
  leg    = args.get( "leg",    ",", leg );
  prec   = args.get( "precision", prec );
  grid   = args.get( "grid", grid );
  x      = args.get( "x", ",", x );
  z      = args.get( "z", ",", z );
  
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
  
  try{
    
    Fragmentation::Evo *pion = 
      new Fragmentation::Evo( args, QCD::Flavor::PIp, *itrPi );
    pion->constNf( false );
    
    Fragmentation::Evo *kaon = 
      new Fragmentation::Evo( args, QCD::Flavor::KAp, *itrKa );
    kaon->constNf( false );
    
    Fragmentation::Evo *rest = 
      new Fragmentation::Evo( args, QCD::Flavor::REp, *itrRe );
    rest->constNf( false );
    
    Evolution::KernelSum *hadron = new Evolution::KernelSum();
    hadron->add( pion );
    hadron->add( kaon );
    hadron->add( rest );
    
    Evolution::PDF *DqPi = ( isParton ? 
			     new Evolution::PDF( pion, pid ) : 
			     new Evolution::PDF( pion, type ) );
    
    Evolution::PDF *DqKa = ( isParton ? 
			     new Evolution::PDF( kaon, pid ) : 
			     new Evolution::PDF( kaon, type ) );
    
    Evolution::PDF *DqRe = ( isParton ? 
			     new Evolution::PDF( rest, pid ) : 
			     new Evolution::PDF( rest, type ) );
    
    Evolution::PDF *DqHa = ( isParton ? 
			     new Evolution::PDF( hadron, pid ) : 
			     new Evolution::PDF( hadron, type ) );
    
    unpolPDF::Evo *pdf = NULL;
    unpolSIDIS::StrFunc *f1pi = NULL;
    unpolSIDIS::StrFunc *f1ka = NULL;
    unpolSIDIS::StrFunc *f1re = NULL;
    unpolSIDIS::StrFunc *f1ha = NULL;

    Xsection::xXsec *f1xpi = NULL;
    Xsection::xXsec *f1xka = NULL;
    Xsection::xXsec *f1xre = NULL;
    Xsection::xXsec *f1xha = NULL;

    if( itrPDF != optPDF.end() ){

      pdf = new unpolPDF::Evo( args, *itrPDF );
      
      f1pi =
	new unpolSIDIS::StrFunc( pdf, pion, 
				 Xsection::Charge::p, QCD::Flavor::PIp );
      
      f1ka = 
	new unpolSIDIS::StrFunc( pdf, kaon, 
				 Xsection::Charge::p, QCD::Flavor::KAp );
      
      f1re = 
	new unpolSIDIS::StrFunc( pdf, rest, 
				 Xsection::Charge::p, QCD::Flavor::REp );
      
      f1ha = 
	new unpolSIDIS::StrFunc( pdf, hadron, 
				 Xsection::Charge::p, QCD::Flavor::HAp );

      f1xpi = new Xsection::xXsec( f1pi, leg[ 0 ], leg[ 1 ], prec );
      f1xka = new Xsection::xXsec( f1ka, leg[ 0 ], leg[ 1 ], prec );
      f1xre = new Xsection::xXsec( f1re, leg[ 0 ], leg[ 1 ], prec );
      f1xha = new Xsection::xXsec( f1ha, leg[ 0 ], leg[ 1 ], prec );
      
      f1xpi->setGngrid( grid );
      f1xpi->setParameter( offset, angle );
      f1xpi->upper( length );

      f1xka->setGngrid( grid );
      f1xka->setParameter( offset, angle );
      f1xka->upper( length );

      f1xre->setGngrid( grid );
      f1xre->setParameter( offset, angle );
      f1xre->upper( length );

      f1xha->setGngrid( grid );
      f1xha->setParameter( offset, angle );
      f1xha->upper( length );

    }
    
    if( 1 ){
      
      ARG_LOG 
	<< "------------------------------------------------------------\n"
	<< "-                        moment                            -\n"
	<< "------------------------------------------------------------\n"
	<< endl;
  
      for( int iN = 0; iN < N.size(); iN++ ){
	for( int iM = 0; iM < M.size(); iM++ ){
	  
	  ARG_LOG << "( N, M )  = ( " 
		  << setw( 6 ) << N[ iN ] << ","
		  << setw( 6 ) << M[ iM ] << endl;
	  
	  ARG_LOG << setw( 6 ) << "Q2"
		  << setw( 26 ) << "D^pi(M)"
		  << setw( 26 ) << "D^ka(M)"
		  << setw( 26 ) << "D^re(M)"
		  << setw( 26 ) << "D^ha(M)"
		  << endl;
	  
	  ARG_LOG << setw( 6 ) << "-----"
		  << setw( 26 ) << "------------------------"
		  << setw( 26 ) << "------------------------"
		  << setw( 26 ) << "------------------------"
		  << setw( 26 ) << "------------------------"
		  << endl;
	  
	  complex< double > n( N[ iN ], 0.0 );
	  complex< double > m( M[ iM ], 0.0 );
	  
	  for( int i = 0; i < q2.size(); i++ ){
	    
	    pion->setQ2( q2[ i ] );
	    kaon->setQ2( q2[ i ] );
	    rest->setQ2( q2[ i ] );
	    hadron->setQ2( q2[ i ] );
	    
	    complex< double > p = (*DqPi)( m ) ;
	    complex< double > k = (*DqKa)( m ) ;
	    complex< double > r = (*DqRe)( m ) ;
	    complex< double > h = (*DqHa)( m ) ;
	    
	    ARG_LOG << setw( 6  )  << q2[ i ] << flush;
	    ARG_LOG << setw( 26 ) << p << flush;
	    ARG_LOG << setw( 26 ) << k << flush;
	    ARG_LOG << setw( 26 ) << r << flush;
	    ARG_LOG << setw( 26 ) << h << flush;
	    ARG_LOG << setw( 26 ) << h - p - k - r << flush;
	    ARG_LOG << endl;
	  }
	  
	  ARG_LOG << endl;

	  if( pdf ){
	    
	    ARG_LOG << setw( 6 ) << "Q2"
		    << setw( 26 ) << "F1^pi(N,M)"
		    << setw( 26 ) << "F1^ka(N,M)"
		    << setw( 26 ) << "F1^re(N,M)"
		    << setw( 26 ) << "F1^ha(N,M)"
		    << endl;
	    
	    ARG_LOG << setw( 6 ) << "-----"
		    << setw( 26 ) << "------------------------"
		    << setw( 26 ) << "------------------------"
		    << setw( 26 ) << "------------------------"
		    << setw( 26 ) << "------------------------"
		    << endl;
	    
	    for( int i = 0; i < q2.size(); i++ ){
	      
	      
	      f1pi->setQ2( q2[ i ] );
	      f1ka->setQ2( q2[ i ] );
	      f1re->setQ2( q2[ i ] );
	      f1ha->setQ2( q2[ i ] );
	      
	      complex< double > p = (*f1pi)( n, m );
	      complex< double > k = (*f1ka)( n, m );
	      complex< double > r = (*f1re)( n, m );
	      complex< double > h = (*f1ha)( n, m );
	      
	      ARG_LOG
		<< setw(6)  << q2[ i ]
		<< setw(26) << p
		<< setw(26) << k
		<< setw(26) << r
		<< setw(26) << h
		<< setw(26) << h - p - k - r 
		<< endl;
	      
	    }
	  }
	}

	ARG_LOG << endl;
      }

      ARG_LOG << setw(6) << "x"
	      << setw(6) << "Q2"
	      << setw(18) << "F1^PIp( x, z )"
	      << setw(18) << "F1^KAp( x, z )"
	      << setw(18) << "F1^REp( x, z )"
	      << setw(18) << "F1^HAp( x, z )"
	      << endl;
      
      ARG_LOG << setw(6) << "----"
	      << setw(6) << "----"
	      << setw(18) << "---------------"
	      << setw(18) << "---------------"
	      << setw(18) << "---------------"
	      << setw(18) << "---------------"
	      << endl;
      
      for( int i = 0; i < q2.size(); i++ ){
	for( int j = 0; j < x.size(); j++ ){
	  
	  f1pi->setQ2( q2[ i ] );
	  f1ka->setQ2( q2[ i ] );
	  f1re->setQ2( q2[ i ] );
	  f1ha->setQ2( q2[ i ] );
	  
	  vector< double > xv( 1, x[ j ] );

	  double p = (*f1xpi)( xv, z );
	  double k = (*f1xka)( xv, z );
	  double r = (*f1xre)( xv, z );
	  double h = (*f1xha)( xv, z );
	  
	  ARG_LOG << setw(6) << x[ j ]
		  << setw(6) << q2[ i ]
		  << setw(18) << p
		  << setw(18) << k
		  << setw(18) << r
		  << setw(18) << h
		  << setw(18) << h - p - k - r
		  << endl;
	  
	}
      }
    }
    
    delete DqPi, DqKa, DqRe, DqHa;
    delete pion, kaon, rest, hadron;
    if( pdf != NULL ) {
      delete pdf, f1pi, f1ka, f1re, f1ha ;
      delete f1xpi, f1xka, f1xre, f1xha ;
    }
    
  }

  
  catch( int err ){
    ARG_ERR << "catch error (" << err << ")" << endl;
    return err;
  }

  return 0;
}
