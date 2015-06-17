#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Evolution/PDFx.hh>
#include <Evolution/Kernel.hh>
#include <Evolution/KernelSum.hh>
#include <polPDF/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <ErrorCalc/ErrCalc.hh>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

// -------------------------------------------------------------------
//                         PDF parameterizations
// -------------------------------------------------------------------

#include <PDFLIB/GRSV2000.hh>
//#include <PDFLIB/LSS.hh>
#include <PDFLIB/AAC.hh>
#include <PDFLIB/BB.hh>

using namespace std;
using namespace Utility;
using namespace Evolution;
using namespace QCD;
using namespace ErrorCalc;

bool q2judge( const double& q2 );
int swapID( const int& ii );

//! for pdf output
void set_pdf_( const int& ii, const double& qq2, const double& x );
//! for error output
void set_epdf_( const int& ii, const double& qq2, const double& x );

string output;
string SORT;
// pdf values
double pdf_q;
// errors of pdfs ( note sigma^2 )
double epdf_q;
vector< double > vepdf_q;

PDFx* xpdf_PP;
PDFx* xpdf_TT;
ErrCalc* eCalc;

vector< double > q2( 0 );
double qq2max( 0.0 );
double qq2tmp( 0.0 );

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  Kernel *pion = NULL;
  Kernel *kaon = NULL;
  Kernel *proton = NULL;
  Kernel *rest = NULL;
  Kernel *evo = NULL;
  PDFLIB::Package *packs_ = NULL;
  
  try{
    
    //! ----- plot Q2 ------ //
    vector< double > q2tmp( 0 );
    if( ! args.hasOpt( "fixQ2" ) ){
      //      q2tmp.push_back(    0.4 );
      q2tmp.push_back(    1.0 );
      q2tmp.push_back(    2.0 );
      q2tmp.push_back(    4.0 );
      q2tmp.push_back(   10.0 );
      q2tmp.push_back(   20.0 );
      q2tmp.push_back(  100.0 );
      q2tmp.push_back(  200.0 );
      /*
	q2tmp.push_back( 1000.0 );
      */
    } else {
      q2tmp.push_back( args.get( "fixQ2", 1.0 ) );
    }
    
    q2 = q2tmp;
    
    //!----------------- evolution set --------------------//
    bool pack_on( false );
    
    if( ! args.hasOpt( "dtype" ) ) throw( 1 );
    SORT = args.get( "dtype", "FF" );
    output = args.get( "otype", "org" );    
    
    if ( SORT == "FF" || SORT == "KretzerFF" || SORT == "DSSFF" )
      { evo = new Fragmentation::Evo ( args, Flavor::PIp, SORT );}
    else if ( SORT == "FFK" || SORT == "KretzerFFK" || SORT == "DSSFFK" )
      { evo = new Fragmentation::Evo ( args, Flavor::KAp, SORT );}
    else if ( SORT == "FFP" || SORT == "KretzerFFP" || SORT == "DSSFFP" )
      { evo = new Fragmentation::Evo ( args, Flavor::PRp, SORT );}
    else if ( SORT == "FFR" || SORT == "KretzerFFR" || SORT == "DSSFFR" )
      { evo = new Fragmentation::Evo ( args, Flavor::REp, SORT );}
    else if( SORT == "polPDF" || SORT == "DSSV" || SORT == "BB" )
      { evo = new polPDF::Evo ( args, SORT );}
    else if( SORT == "unpolPDF" || SORT == "MRST" )
      { evo = new unpolPDF::Evo ( args );}
    else if( SORT == "GRSV00" )
      { 
	packs_ = new PDFLIB::GRSV2000( PDFLIB::GRSV2000::STANDARD_NLO,
				       args.getOpt( "GRSV2000" ) ); 
	evo = new polPDF::Evo ( args, "DSSV" );
	pack_on = true;
      }
    else if( SORT == "hadronFF" ){
      
      vector< string > optPi = Fragmentation::Evo::options( Flavor::PIp );
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
      pion = new Fragmentation::Evo( args, QCD::Flavor::PIp, *itrPi);
      
      
      vector< string > optKa = Fragmentation::Evo::options( Flavor::KAp );
      vector< string >::iterator itrKa = args.hasOpt( optKa );
      if( itrKa == optKa.end() ){
	
	ARG_ERR 
	  << args.command() 
	  << " requires one of the follwing option for pion" 
	  << endl;
	
	for( int i = 0; i < optKa.size(); i++ ){
	  ARG_ERR << "\t--" << optKa[ i ] << endl;
	}
	
	return 1;
      }
      kaon = new Fragmentation::Evo( args, QCD::Flavor::KAp, *itrKa);
      
      vector< string > optRe = Fragmentation::Evo::options( Flavor::REp );
      vector< string >::iterator itrRe = args.hasOpt( optRe );
      if( itrRe == optRe.end() ){
	
	ARG_WARN
	  << args.command() 
	  << " requires one of the follwing option for pion" 
	  << endl;
	
	for( int i = 0; i < optRe.size(); i++ ){
	  ARG_ERR << "\t--" << optRe[ i ] << endl;
	}
	
      } else rest = new Fragmentation::Evo( args, QCD::Flavor::REp, *itrRe);
      
      vector< string > optPr = Fragmentation::Evo::options( Flavor::PRp );
      vector< string >::iterator itrPr = args.hasOpt( optPr );
      if( itrPr == optPr.end() ){
	
	ARG_WARN 
	  << args.command() 
	  << " requires one of the follwing option for pion" 
	  << endl;
	
	for( int i = 0; i < optPr.size(); i++ ){
	  ARG_ERR << "\t--" << optPr[ i ] << endl;
	}
	
      } else proton = new Fragmentation::Evo( args, QCD::Flavor::PRp, *itrPr);
      
      if( pion == NULL ){
	ARG_ERR << "fail to create pion fragmentation evolution objects"
		<< endl;
	return 2;
      }
      
      if( kaon == NULL ) {
	  ARG_ERR << "fail to create kaon fragmentation evolution objects"
		  << endl;
	  return 2;
      }
      
      Evolution::KernelSum *hadron = new Evolution::KernelSum();
      hadron->add( pion );
      hadron->add( kaon );
      if( rest )   hadron->add( rest );
      if( proton ) hadron->add( proton );
      
      evo = hadron;


    }

    else return 0;
    
    //    cout << evo->initialScale() << endl;

    int nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;

    //! ------------- pdf output ------------------//
    
    //! ------------- set filename ---------------//
    string filename;
    
    filename = "distout" + args.get( "order", "NLO" );

    string sort( SORT );
    if( SORT == "hadronFF" && args.hasOpt( "DSSH" ) )
      sort = "DSS" + SORT;
    if ( SORT == "hadronFF" && args.hasOpt( "KretzerH" ) ) 
      sort = "Kretzer" + SORT;

    filename += sort;

    if ( ( output == "singlet" ) || ( output == "valence" )){
      filename += "_" + output;
    }
    if( args.hasOpt( "rescaleFF" ) ) 
      filename += "_r" + args.get( "rescaleFF", "1.0" );
    if( args.hasOpt( "wError" ) ) filename += "_werr";
    if( args.hasOpt( "kret" ) ) filename += "_kret";
    if( args.hasOpt( "Dchi2" ) ){ 
      double tmpval_ = args.get( "Dchi2", 1.0 );
      ostringstream ost;
      ost << "_Dchi2_" << tmpval_;
      filename += ost.str(); 
    }
    filename += ".dat";	 
    
    //  ofstream outfile( filename.data() );
    ofstream outfile( filename.c_str() );

    //!---------- set pdfxs -----------//    
    PDFx xpdf_p( evo, Flavor::PARTON( 0 ), 4, 6, 
		 args.get( "precision", 1.0E-4 ) ); 
    PDFx xpdf_t( evo, Flavor::TYPE( 0 ), 4, 6, 
		 args.get( "precision", 1.0E-4 ) ); 
    
    double offset = args.get( "offset", 3.0  );  
    double angle  = args.get( "angle",  0.55  );  
    double length = args.get( "length", 15.0 );  
    
    xpdf_p.setParameter( offset, angle );
    xpdf_p.upper( length );
    xpdf_t.setParameter( offset, angle );
    xpdf_t.upper( length );
    
    xpdf_PP = &xpdf_p;
    xpdf_TT = &xpdf_t;
    
    //!---------- set error calc -----------//    
    ErrCalc ecal( evo );
    ecal.setDchi2( args.get( "Dchi2", 1.0 ) );
    eCalc = &ecal;
    
    if( args.hasOpt( "comp" ) ){
      vector< double > tmp( nx + 1, 0.0 );
      for( int k = 0; k <= nx; k++ )
	tmp[ k ] = xmin * pow( 10.0, k * dx );
      eCalc->setvX( tmp );
    }

    if ( outfile ){

      //!------- q2 loop --------//      
      for( int kk = 0; kk < q2.size(); kk++ ){
	
	double qq2 = q2[ kk ];

	//	evo->clearCache();
	( pack_on ? packs_->setQ2( qq2 ) : evo->setQ2( qq2 ) ); 
	  	  
	  for( int ii = -6; ii <= 6; ii++ ){
	    
	    if( ! pack_on && args.hasOpt( "wError") && args.hasOpt( "comp" ) )
	      {
		eCalc->setPID( Flavor::PARTON( swapID( ii ) ) );	      
		vepdf_q = (*eCalc)();
	      }
	    
	    if( pack_on ) packs_->setPID( Flavor::PARTON( ii ) );

	    for( int jj = 0; jj <= nx; jj ++ ){
	      
	      double x = xmin * pow( 10.0, jj * dx );
	      
	      if( ! pack_on ){
		set_pdf_( ii, qq2, x );

		if( args.hasOpt( "wError") ) if( !args.hasOpt( "comp" ) )
		  if( !( jj % 4 ) ) set_epdf_( ii, qq2, x );
	      }

	      outfile << setw(16) << x << setw(16) << qq2 
		      << setw(16) << flush;

	      if( ! pack_on ){
		outfile << ii << setw(16) << x * pdf_q << setw(16) 
			<< flush;
	      } else {
		outfile << ii << setw(16) << x * (*packs_)( x ) << setw(16) 
			<< flush;
	      }
	      
	      if( ! pack_on && args.hasOpt( "wError") ){
		if( !args.hasOpt( "comp" ) ){
		  if( !( jj % 4 ) ) outfile << x * sqrt( epdf_q ) 
					    << setw(16) << flush; // error
		} else {
		  outfile << x * sqrt( vepdf_q[ jj ] ) << setw(16) << flush;
		}
	      }
	      outfile << endl;
	      
	    } // x loop
	  } // id loop
	  
      }	// q2 loop
      
    } // outfile
    
  } // try
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }


  if( pion ) delete pion;
  if( kaon ) delete kaon;
  if( rest ) delete rest;
  if( proton ) delete proton;
  if( evo ) delete evo;
  if( packs_ ) delete packs_ ;

  
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

int swapID( const int& ii ){
    return ii;
}

void set_pdf_( const int& ii, const double& qq2, const double& x ){

  if( ii == 0 ){
    if( output == "valence" ){
      pdf_q = 0.0;
      return;
    }
    xpdf_PP->pdf().getPID() = Flavor::PARTON( swapID( ii ) );
    pdf_q = ( q2judge( qq2 ) ? (*xpdf_PP)( x ) : 0.0 );
    return;
  } 
  
  if( output == "singlet" ){
    xpdf_TT->pdf().getTID() = Flavor::TYPE( 0 );
    pdf_q = ( q2judge( qq2 ) ? (*xpdf_TT)( x ) : 0.0 );
    return;
  }
  
  int iq = swapID( ii );
  if( output == "valence" ){
    iq = abs( swapID( ii ) ) + 10;
    if( ii < 0 ) iq = - iq;
  }  
  
  xpdf_PP->pdf().getPID() = Flavor::PARTON( iq );
  pdf_q = ( q2judge( qq2 ) ? (*xpdf_PP)( x ) : 0.0 );

}

void set_epdf_( const int& ii, const double& qq2, const double& x ){
  
  if( output == "valence" || output == "singlet" ){
    epdf_q = 0.0;
    return;
  }

  eCalc->setPID( Flavor::PARTON( swapID( ii ) ) );
  epdf_q = ( q2judge( qq2 ) ? (*eCalc)( x ) : 0.0 );

}
