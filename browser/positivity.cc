#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Evolution/PDFx.hh>
#include <Evolution/Kernel.hh>
#include <polPDF/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <ErrorCalc/ErrCalc.hh>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace Utility;
using namespace Evolution;
using namespace QCD;
using namespace ErrorCalc;

Evolution::Kernel* evo_;
Evolution::Kernel* evo2_;

bool q2judge( const double& q2 );
int swapID( const int& ii );

//! for parameter change and pdf set
void changeparam_( const int& flavi, const double& x, const int& iparam, 
const double& da ) throw( int );

void changeparam2_( const int& flavi, const double& x, const int& iparam, 
const double& da ) throw( int );

//! pick up appropriate PdfBase from PdfParam
PdfBase* select_flavor( const int& flavi, PdfParametrization* param );
//! update changes of parameters in PdfBase's
void pdfupdate();

void setN( const double& offset, const double& length, const double& angle );

vector< PdfBase* > vpdfs_;

complex< double > N_( 0.0, 0.0 );

string output;
string SORT;
string SORT2;
// pdf values
double pdf_q;
double pdf_q2;
double pdf_n_q;

PDFx* xpdf_PP;
PDFx* xpdf_PP2;

vector< double > q2( 0 );
double qq2max( 0.0 );
double qq2tmp( 0.0 );

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{

    //! ----- plot Q2 ------ //
    vector< double > q2tmp( 0 );
    if( ! args.hasOpt( "fixQ2" ) ){
      q2tmp.push_back(    1.0 );
    } else {
      q2tmp.push_back( args.get( "fixQ2", 1.0 ) );
    }

    q2 = q2tmp;

    //!----------------- evolution set --------------------//
    
    if( ! args.hasOpt( "dtype" ) ) throw( 1 );
    SORT = args.get( "dtype", "FF" );

    if ( SORT == "FF" || SORT == "KretzerFF" || SORT == "DSSFF" )
      { evo_ = new Fragmentation::Evo ( args, Flavor::PIp, SORT );}
    else if ( SORT == "FFK" || SORT == "KretzerFFK" || SORT == "DSSFFK" )
      { evo_ = new Fragmentation::Evo ( args, Flavor::KAp, SORT );}
    else if( SORT == "polPDF" || SORT == "DSSV" || SORT == "BB" )
      { evo_ = new polPDF::Evo ( args, SORT );}
    else if( SORT == "unpolPDF" || SORT == "MRST" )
      { evo_ = new unpolPDF::Evo ( args );}
    else return 0;

    //    if( ! args.hasOpt( "dtype2" ) ) throw( 1 );
    SORT2 = args.get( "dtype2", "unpolPDF" );

    if ( SORT2 == "FF" || SORT2 == "KretzerFF" || SORT2 == "DSSFF" )
      { evo2_ = new Fragmentation::Evo ( args, Flavor::PIp, SORT2 );}
    else if ( SORT2 == "FFK" || SORT2 == "KretzerFFK" || SORT2 == "DSSFFK" )
      { evo2_ = new Fragmentation::Evo ( args, Flavor::KAp, SORT2 );}
    else if( SORT2 == "polPDF" || SORT2 == "DSSV" || SORT2 == "BB" )
      { evo2_ = new polPDF::Evo ( args, SORT2 );}
    else if( SORT2 == "unpolPDF" || SORT2 == "MRST" )
      { evo2_ = new unpolPDF::Evo ( args );}
    else return 0;

    int nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;

    double neg = ( args.hasOpt( "negative" ) ? -1.0 : 1.0 );

    //! ------------- pdf output ------------------//
    
    //! ------------- set filename ---------------//
    string filename;
    
    filename = "positiv" + args.getOpt( "order" ) + SORT;
    filename += ".dat";	 
    
    //  ofstream outfile( filename.data() );
    ofstream outfile( filename.c_str() );

    //!---------- set pdfxs -----------//    
    PDFx xpdf_p( evo_, Flavor::PARTON( 0 ), 4, 6, 
		 args.get( "precision", 1.0E-4 ) ); 
    double offset = args.get( "offset", 3.0  );  
    double angle  = args.get( "angle",  0.55  );  
    double length = args.get( "length", 15.0 );  
    xpdf_p.setParameter( offset, angle );
    xpdf_p.upper( length );
    xpdf_PP = &xpdf_p;

    PDFx xpdf_p2( evo2_, Flavor::PARTON( 0 ), 4, 6, 
		 args.get( "precision", 1.0E-4 ) ); 
    offset = args.get( "offset", 3.0  );  
    angle  = args.get( "angle",  0.55  );  
    length = args.get( "length", 15.0 );  
    xpdf_p2.setParameter( offset, angle );
    xpdf_p2.upper( length );
    xpdf_PP2 = &xpdf_p2;

    setN( offset, length, angle );

    if ( outfile ){
      
      //!------- q2 loop --------//      
      for( int kk = 0; kk < q2.size(); kk++ ){
	
	double qq2 = q2[ kk ];	
	evo_->setQ2( qq2 );
	evo2_->setQ2( qq2 );
	
	vector< double > vpdf_org( 0 );
	vector< double > vpdf_n_org( 0 );

	for( int jj = 0; jj <= nx; jj ++ ){
	  
	  double x = xmin * pow( 10.0, jj * dx );
	  int flavi = args.get( "flav", 1 );
	  int iparam = args.get( "param", 1 );
	  double da = 0;
	  
	  changeparam_( flavi, x, iparam, da );
	  
	  vpdf_org.push_back( pdf_q );
	  vpdf_n_org.push_back( pdf_n_q );
	  
	}
	
	for( int ii = -1; ii <= 1; ii++ ){
	  for( int jj = 0; jj <= nx; jj ++ ){
	    
	    double x = xmin * pow( 10.0, jj * dx );
	    
	    int flavi = args.get( "flav", 1 );
	    int iparam = args.get( "param", 1 );
	    
	    double da = ii * args.get( "da", 0.1 );
	    
	    changeparam_( flavi, x, iparam, da );
	    
	    outfile << setw(16) << x << setw(16) << qq2 
		    << setw(16) << flush;
	    outfile << ii << setw(16) << x * neg * ( pdf_q )
		    << setw(16) << flush;
	    outfile << x * neg * ( pdf_q - pdf_n_q )
	      //	    outfile << x * ( pdf_n_q )
		    << setw(16) << flush;
	    /*
	    outfile << ii << setw(16) << x * ( pdf_q - vpdf_org[ jj ] )
		    << setw(16) << flush;
	    outfile << x * ( pdf_n_q - vpdf_n_org[ jj ] )
		    << setw(16) << flush;
	    */
	    outfile << endl;

	  } // x loop
	} // id loop	      


	for( int ii = 4; ii <= 4; ii++ ){
	  for( int jj = 0; jj <= nx; jj ++ ){
	    
	    double x = xmin * pow( 10.0, jj * dx );
	    
	    int flavi = args.get( "flav", 1 );
	    int iparam = args.get( "param", 1 );
	    
	    double da = ii * args.get( "da", 0.1 );
	    
	    changeparam2_( flavi, x, iparam, da );
	    
	    outfile << setw(16) << x << setw(16) << qq2 
		    << setw(16) << flush;
	    outfile << ii << setw(16) << x * ( pdf_q2 )
		    << setw(16) << flush;
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

void changeparam2_( const int& flavi, const double& x, const int& iparam, 
		   const double& da ) throw( int ){

  xpdf_PP2->pdf().getPID() = Flavor::PARTON( swapID( flavi ) );

  if( xpdf_PP2->pdf().getPID() != Flavor::PARTON( swapID( flavi ) ) )
    throw( 1 );

  pdf_q2 = (*xpdf_PP2)( x );

}

void changeparam_( const int& flavi, const double& x, const int& iparam, 
		   const double& da ) throw( int ){

  Arguments& args = Arguments::ref();
  
  xpdf_PP->pdf().getPID() = Flavor::PARTON( swapID( flavi ) );
  
  if( xpdf_PP->pdf().getPID() != Flavor::PARTON( swapID( flavi ) ) )
    throw( 1 );

  /*  
  //  Kernel* kernel_ = xpdf_PP->pdf().kernel();
  //  PdfParametrization* param = kernel_->initialDistribution();
  PdfParametrization* param = evo_->initialDistribution();
  vpdfs_ = param->pdfs();
  PdfBase* pdfb_ = select_flavor( flavi, param );
  
  //  for( PdfBase::iterator k; pdfb_->begin(); pdfb_->end(); k++ )
  PdfBase::iterator kk = pdfb_->find( iparam );
  if( kk == pdfb_->end() ) throw( 2 );
  
  // shift parameter by da
  kk->second.value() += da;
  // update the change to all pdfbase's
  pdfupdate();
  */

  pdf_q = (*xpdf_PP)( x );
  pdf_n_q = pdf_q;

  /*
  xpdf_PP->getIMIntegrand()->x( x );
  complex< double > tmp = (*xpdf_PP->getIMIntegrand())( N_ );
  */
  /*
  complex< double > tmp = xpdf_PP->pdf()( N_ );
  pdf_n_q = pow( abs( N_ ), args.get( "npow", 1 ) ) * abs( tmp );
  
  // return to the original value
  kk->second.value() -= da;
  // update the change to all pdfbase's
  pdfupdate();
  */

  
}

void pdfupdate(){
  for( int k = 0; k < vpdfs_.size(); k++ ) vpdfs_[ k ]->update();
  evo_->clearCache();
}

PdfBase* select_flavor( const int& flavi, PdfParametrization* param ){
  
  Flavor::PARTON q = Flavor::PARTON( swapID( flavi ) );
  
  switch( q ){
  case Flavor::g   : return param->g();
  case Flavor::u   : return param->u();
  case Flavor::ubar: return param->ubar();
  case Flavor::d   : return param->d();
  case Flavor::dbar: return param->dbar();
  case Flavor::s   : return param->s();
  case Flavor::sbar: return param->sbar();
  }
  return param->g();
}

void setN( const double& offset, const double& length, const double& angle ){

  Arguments& args = Arguments::ref();

  N_ = complex< double >( args.get( "nreal", 10.0 ), 
			  args.get( "nimag", 0.0 ) );

  double length_ = length + args.get( "addl", 0.0 );
  if( args.hasOpt( "straight" ) )
    // N_ = offset + length_  * exp( complex< double >( 0.0, angle * M_PI ));
    N_ = offset + length_ * complex< double >( cos( angle * M_PI ), 
					       sin( angle * M_PI ) );
}
