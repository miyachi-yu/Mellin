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

namespace Tmpcalc{

  // ------ for full complex calc ------ //
  class A0calc: public Transform::ComplexFunction{
  public:
    A0calc( Evolution::KernelBase* evo ){
      pdf_ = new Evolution::PDF( evo, QCD::Flavor::PARTON( 0 ) );
    };
    
    virtual ~A0calc(){ delete pdf_; }
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n ){
      complex< double > a0( 0.0, 0.0 );
      
      for( int i = -3; i <= 3; i++ ){
	if( i == 0 ) continue;
	pdf_->getPID() = QCD::Flavor::PARTON( i );
	a0 += (*pdf_)( n );
      }
      return a0;
      
    };

  private:
    Evolution::PDF* pdf_;
  };

  class A3calc: public Transform::ComplexFunction{
  public:
    A3calc( Evolution::KernelBase* evo ){
      pdf_ = new Evolution::PDF( evo, QCD::Flavor::PARTON( 0 ) );
    };
    
    virtual ~A3calc(){ delete pdf_; }
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n ){
      complex< double > a3( 0.0, 0.0 );
      
      for( int i = -2; i <= 2; i++ ){
	if( i == 0 ) continue;
	pdf_->getPID() = QCD::Flavor::PARTON( i );
	if( abs( i ) == 1 ) a3 += (*pdf_)( n );
	if( abs( i ) == 2 ) a3 -= (*pdf_)( n );
      }
      return a3;
      
    };
    
  private:
    Evolution::PDF* pdf_;
  };
    
  class A8calc: public Transform::ComplexFunction{
  public:
    A8calc( Evolution::KernelBase* evo ){
      pdf_ = new Evolution::PDF( evo, QCD::Flavor::PARTON( 0 ) );
    };
    
    virtual ~A8calc(){ delete pdf_; }
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n ){
      complex< double > a8( 0.0, 0.0 );
      
      for( int i = -3; i <= 3; i++ ){
	if( i == 0 ) continue;
	pdf_->getPID() = QCD::Flavor::PARTON( i );
	if( abs( i ) == 1 ) a8 += (*pdf_)( n );
	if( abs( i ) == 2 ) a8 += (*pdf_)( n );
	if( abs( i ) == 3 ) a8 -= 2.0 * (*pdf_)( n );
      }
      return a8;
      
    };
    
  private:
    Evolution::PDF* pdf_;
  };

  // ------ for partial real calc ------ //
  class A0calcR: public Transform::RealFunction{
  public:
    A0calcR( Evolution::KernelBase* evo ){
      pdfx_ = new Evolution::PDFx( evo, QCD::Flavor::PARTON( 0 ),
				   4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
      //				   4, 6, 1.0E-4, 12.0, 2.0, 0.55 );
    };
    
    virtual ~A0calcR(){ delete pdfx_; }
    
    virtual double operator()( const double& x ){ return 0.0; }

    virtual double operator()( const std::vector< double >& vx ){
      double a0( 0.0 );
      
      for( int i = -3; i <= 3; i++ ){
	if( i == 0 ) continue;
	pdfx_->pdf().getPID() = QCD::Flavor::PARTON( i );
	a0 += (*pdfx_)( vx );
      }
      return a0;      
    };

    Evolution::PDFx* getPDFx(){ return pdfx_; }

  private:
    Evolution::PDFx* pdfx_;
  };
 
  class A3calcR: public Transform::RealFunction{
  public:
    A3calcR( Evolution::KernelBase* evo ){
      pdfx_ = new Evolution::PDFx( evo, QCD::Flavor::PARTON( 0 ),
				   4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
      //				   4, 6, 1.0E-4, 12.0, 2.0, 0.55 );
    };
    
    virtual ~A3calcR(){ delete pdfx_; }
    
    virtual double operator()( const double& x ){ return 0.0; }

    virtual double operator()( const std::vector< double >& vx ){
      double a3( 0.0 );
      
      for( int i = -2; i <= 2; i++ ){
	if( i == 0 ) continue;
	pdfx_->pdf().getPID() = QCD::Flavor::PARTON( i );
	if( abs( i ) == 1 ) a3 += (*pdfx_)( vx );
	if( abs( i ) == 2 ) a3 -= (*pdfx_)( vx );
      }
      return a3;
    };

    Evolution::PDFx* getPDFx(){ return pdfx_; }

  private:
    Evolution::PDFx* pdfx_;
  };

  class A8calcR: public Transform::RealFunction{
  public:
    A8calcR( Evolution::KernelBase* evo ){
      pdfx_ = new Evolution::PDFx( evo, QCD::Flavor::PARTON( 0 ),
				   4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
      //x				   4, 6, 1.0E-4, 12.0, 2.0, 0.55 );
    };
    
    virtual ~A8calcR(){ delete pdfx_; }

    virtual double operator()( const double& x ){ return 0.0; }
    
    virtual double operator()( const std::vector< double >& vx ){
      double a8( 0.0 );
      
      for( int i = -3; i <= 3; i++ ){
	if( i == 0 ) continue;
	pdfx_->pdf().getPID() = QCD::Flavor::PARTON( i );
	if( abs( i ) == 1 ) a8 += (*pdfx_)( vx );
	if( abs( i ) == 2 ) a8 += (*pdfx_)( vx );
	if( abs( i ) == 3 ) a8 -= 2.0 * (*pdfx_)( vx );
      }
      return a8;
    };

    Evolution::PDFx* getPDFx(){ return pdfx_; }

  private:
    Evolution::PDFx* pdfx_;
  };

  void Usage(){
    Utility::Arguments& args = Utility::Arguments::ref();
    args.usage( "help",  "", "show this help" );
    args.usage( "FF,polPDF,unpolPDF", "", "set xmls with ordinary options" );
    args.usage( "fixQ2",      "1.0", "output only at given fixed Q2" );
    args.usage( "moment",     "1.0", "set moment value" );
    args.usage( "partial", "", "take the partial monent in the range of figures separated by ','; e.g. --partial=0.05,0.8" );
    args.usage( "imax",      "3", "set maximum flavor id" );
    args.usage( "sigfig",      "4", "set significant digits" );
    args.usage( "werror",    "", "calc moments with error if possible" );
    args.usage( "wcombs",    "", "calc additional a0, a3, a8 combinations" );
  }

};

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  if( args.hasOpt( "help" ) ){
    Tmpcalc::Usage();
    return 0;
  }

  try{

    //! ----- plot Q2 ------ //
    vector< double > q2( 0 );
    if( ! args.hasOpt( "fixQ2" ) ){
      q2.push_back(    1.0 );
      q2.push_back(    2.0 );
      q2.push_back(    4.0 );
      q2.push_back(   10.0 );
      q2.push_back(   20.0 );
      q2.push_back(   30.0 );
      q2.push_back(  100.0 );
      q2.push_back(  200.0 );
      q2.push_back(  300.0 );
    } else {
      q2.push_back( args.get( "fixQ2", 1.0 ) );
    }

    //!----------------- evolution set --------------------//
    KernelBase* evo_( NULL );
    
    vector< string > polpdf_opts = polPDF::Evo::options();
    vector< string > unpolpdf_opts = unpolPDF::Evo::options();
    vector< string > ff_opts( 0 );

    string pdfname_( "" );

    if( args.hasOpt( polpdf_opts ) != polpdf_opts.end() ){
      evo_ = new polPDF::Evo( args ); // polpdf
      pdfname_ = *args.hasOpt( polpdf_opts );
    } else if ( args.hasOpt( unpolpdf_opts ) != unpolpdf_opts.end() ){
      evo_ = new unpolPDF::Evo( args ); // unpolpdf
      pdfname_ = *args.hasOpt( unpolpdf_opts );
    } else {
      for( int i = 1; i < 3; i++ ){ // pi+,ka+,pr+ ff
	ff_opts = Fragmentation::Evo::options( Flavor::HADRON( i ) );
	if( args.hasOpt( ff_opts ) != ff_opts.end() ){
	  evo_ = new Fragmentation::Evo( args, Flavor::HADRON( i ) );
	  pdfname_ = *args.hasOpt( ff_opts );
	}
      }
    }
    
    if( ! evo_ ){
      cerr << "options for polpdf, unpolpdf, or " << flush;
      cerr << "ff were not found" << endl;
      return 0; 
    }

    //!----------------- errcalc set --------------------//
    ErrorCalc::ErrCalc ecal( evo_ ); // errorMX assign

    //! ------------- moment params ------------------//
    double mom = args.get( "moment", 1.0 );

    complex< double > cmom_( mom, 0.0 ); // for complex calc
    vector< double > vx_ 
      = args.get( "partial", ",", vector< double >( 0 ) );
    if( vx_.size() && vx_.size() != 2 ) return 0;
    // for real calc

    int imax_ = args.get( "imax", 3 );

    //! ------------- set filename ---------------//
    ostringstream ost; ost << pdfname_ << "_" << mom << "_";
    if( vx_.size() == 2 ) ost << vx_[ 0 ] << "-" << vx_[ 1 ];
    if( ! vx_.size() ) ost << "full";
    ost << "_scale.dat";
    
    ofstream outfile( ost.str().c_str() );
    int sigf = args.get( "sigfig", 4 );
    outfile << setprecision( sigf );

    //! ------------- out process ---------------//
    if ( outfile ){

      int space = ( args.hasOpt( "werror" ) ? 2 * sigf + 12 : sigf + 4 );
      outfile << setw( 8 ) << "Q2" << flush;
      for( int ii = -3; ii <= imax_; ii++ ){
	outfile << setw( space ) << ii << flush;
      }

      if( args.hasOpt( "wcombs" ) ){
	outfile << setw( space ) << "a0" << flush;
	outfile << setw( space ) << "a3" << flush;
	outfile << setw( space ) << "a8" << flush;
      }
      outfile << endl;

      //!------- q2 loop --------//      
      for( int kk = 0; kk < q2.size(); kk++ ){
	
	double qq2 = q2[ kk ];
	outfile << setw( 8 ) << qq2 << flush;

	evo_->setQ2( qq2 );

	//!------- flavor loop --------//      
	for( int ii = -3; ii <= imax_; ii++ ){

	  //!---------- set pdfs and error calc -----------//    	  
	  if( ! args.hasOpt( "partial" ) ){

	    Evolution::PDF pdf( evo_, Flavor::PARTON( ii ) );
	    //	    ecal.setPID( Flavor::PARTON( ii ) );
	    
	    ecal.cfunccalc( true );
	    ecal.setPDFcalc( false );

	    ecal.setCompFunc( &pdf );
	    outfile << setw( sigf + 4 ) << pdf( cmom_ ).real() << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( cmom_ ).real() ) << flush;

	    ecal.cfunccalc( false );
	    ecal.setPDFcalc( true );

	  } else {

	    Evolution::PDFx pdfx( evo_, Flavor::PARTON( ii ),
				  4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
	    //				   4, 6, 1.0E-4, 12.0, 2.0, 0.55 );
	    pdfx.setMomParam( vector< double >( 1, mom ) );

	    //	    ecal.setPID( Flavor::PARTON( ii ) );
	    //	    ecal.getPDFx()->setMomParam( vector< double >( 1, mom ) );

	    ecal.rfunccalc( true );
	    ecal.setPDFcalc( false );

	    ecal.setRealFunc( &pdfx );
	    outfile << setw( sigf + 4 ) << pdfx( vx_ ) << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( vx_ ) ) << flush;

	    ecal.rfunccalc( false );
	    ecal.setPDFcalc( true );

	  }

	}

	if( args.hasOpt( "wcombs" ) ){

	  if( ! args.hasOpt( "partial" ) ){

	    Tmpcalc::A0calc a0( evo_ );
	    Tmpcalc::A3calc a3( evo_ );
	    Tmpcalc::A8calc a8( evo_ );
	    
	    ecal.cfunccalc( true );
	    ecal.setPDFcalc( false );
	    
	    ecal.setCompFunc( &a0 );
	    outfile << setw( sigf + 4 ) << a0( cmom_ ).real() << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( cmom_ ).real() ) << flush;
	    
	    ecal.setCompFunc( &a3 );
	    outfile << setw( sigf + 4 ) << a3( cmom_ ).real() << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( cmom_ ).real() ) << flush;
	    
	    ecal.setCompFunc( &a8 );
	    outfile << setw( sigf + 4 ) << a8( cmom_ ).real() << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( cmom_ ).real() ) << flush;
	    
	    ecal.cfunccalc( false );
	    ecal.setPDFcalc( true );
	    
	  } else {
	    
	    Tmpcalc::A0calcR a0r( evo_ );
	    Tmpcalc::A3calcR a3r( evo_ );
	    Tmpcalc::A8calcR a8r( evo_ );
	    a0r.getPDFx()->setMomParam( vector< double >( 1, mom ) );
	    a3r.getPDFx()->setMomParam( vector< double >( 1, mom ) );
	    a8r.getPDFx()->setMomParam( vector< double >( 1, mom ) );
	    
	    ecal.rfunccalc( true );
	    ecal.setPDFcalc( false );
	    
	    ecal.setRealFunc( &a0r );
	    outfile << setw( sigf + 4 ) << a0r( vx_ ) << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( vx_ ) ) << flush;
	    
	    ecal.setRealFunc( &a3r );
	    outfile << setw( sigf + 4 ) << a3r( vx_ ) << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( vx_ ) ) << flush;
	    
	    ecal.setRealFunc( &a8r );
	    outfile << setw( sigf + 4 ) << a8r( vx_ ) << flush;
	    if( args.hasOpt( "werror" ) )
	      outfile << " +- " << setw( sigf + 4 ) 
		      << sqrt( ecal( vx_ ) ) << flush;
	    
	    ecal.rfunccalc( false );
	    ecal.setPDFcalc( true );
	    
	  }
	  
	}

	outfile << endl;
	  
      }// q2 loop
	
    }// file

  }// try
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }
  
  return 0;
}
