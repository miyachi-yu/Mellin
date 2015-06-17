// $Id: ErrCalc.cc,v 1.24 2013/06/13 01:52:07 miyachi Exp $
/*!
  \brief class implemenation of Error Calculation 
 */
#include "ErrCalc.hh"

#include <iomanip>

#include <Utility/Arguments.hh>

#include <Evolution/KernelSum.hh>

#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <IntegXsec/xXsec.hh>
#include <IntegXsec/xzXsec.hh>
#include <IntegXsec/XsecIntegrand.hh>

using namespace std;
using namespace Evolution;
using namespace Xsection;
using namespace QCD;
using namespace Transform;
using namespace ErrorCalc;

ErrCalc::ErrCalc( KernelBase* evo ):
  xsec_( NULL ), xxsec_( NULL ), xzxsec_( NULL ), 
  ixxsec_( NULL ), ixzxsec_( NULL ), 
  vevo_( 1, evo ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), 
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( evo ){
    this->setparamvec( vevo_ );
    pdfcalc_ = true;
    this->createpdfs();
  }
}

ErrCalc::ErrCalc( Xsec* xsec ):
  xsec_( xsec ), xxsec_( NULL ), xzxsec_( NULL ), 
  ixxsec_( NULL ), ixzxsec_( NULL ), 
  vevo_( 0 ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), isFequalG_( false ),
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( xsec_ ){
    vevo_ = xsec_->xsecComp()->vevo();
    this->setparamvec( vevo_ );
  }
}

ErrCalc::ErrCalc( xXsec* xxsec ):
  xsec_( NULL ), xxsec_( xxsec ), xzxsec_( NULL ), 
  ixxsec_( NULL ), ixzxsec_( NULL ), 
  vevo_( 0 ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), isFequalG_( false ),
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( xxsec_ ){
    vevo_ = xxsec_->moment()->xsecComp()->vevo();
    xsec_ = xxsec_->moment();
    this->setparamvec( vevo_ );
  }
}

ErrCalc::ErrCalc( xzXsec* xzxsec ):
  xsec_( NULL ), xxsec_( NULL ), xzxsec_( xzxsec ), 
  ixxsec_( NULL ), ixzxsec_( NULL ), 
  vevo_( 0 ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), isFequalG_( false ), 
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( xzxsec_ ){
    vevo_ = xzxsec_->moment()->xsecComp()->vevo();
    xsec_ = xzxsec_->moment();
    this->setparamvec( vevo_ );
  }
}

ErrCalc::ErrCalc( IntegXsec::xXsec* ixxsec ):
  xsec_( NULL ), xxsec_( NULL ), xzxsec_( NULL ), 
  ixxsec_( ixxsec ), ixzxsec_( NULL ), 
  vevo_( 0 ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), isFequalG_( false ),
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( ixxsec_ ){
    if( dynamic_cast< XsecCompBase* >( ixxsec_->xsecInteg() ) != 
	ixxsec_->xsecInteg()->getvevoXC() ) 
      throw( 1 );
    vevo_ = ixxsec_->xsecInteg()->vevo(); // = ->vvevo()
    this->setparamvec( vevo_ );
  }
}

ErrCalc::ErrCalc( IntegXsec::xzXsec* ixzxsec ):
  xsec_( NULL ), xxsec_( NULL ), xzxsec_( NULL ), 
  ixxsec_( NULL ), ixzxsec_( ixzxsec ), 
  vevo_( 0 ), vpdfbase_( 0 ),
  pdfcalc_( false ), pdfdirect_( false ), ematrices_(),
  par_( 0 ), dpar_( 1.0E-4 ), ivevo_( 0 ), N_( complex< double >( 0.0, 0.0 ) ),
  Np_( N_ ), M_( N_ ), Mp_( M_ ), 
  X_( 0.0 ), Z_( X_ ), Xp_( X_ ), Zp_( Z_ ),
  vX_( 0 ), vZ_( 0 ), vXp_( 0 ), vZp_( 0 ),
  vvX_( 0 ), vvZ_( 0 ), vcalc_( false ),
  cfuncF_( NULL ), cfuncG_( NULL ), 
  rfuncF_( NULL ), rfuncG_( NULL ), isFequalG_( false ), 
  ccalc_( false ), dcalc_( false ), vectvar_( false ),
  pdf_( NULL ), pdfx_( NULL ), 
  parton_( Flavor::PARTON( 0 ) ), dataset_( NULL ), 
  datanorm_( 1.0 ), dataStEr_( 0.0 ), dataID_( 0 ), emxID_( 0 ),
  cfunc_( NULL ), rfunc_( NULL ),
  dchi2_( 1.0 )
{
  dpar_ = Utility::Arguments::ref().get( "Dprec", dpar_ );
  if( ixzxsec_ ){
    if( dynamic_cast< XsecCompBase* >( ixzxsec_->xsecInteg() ) != 
	ixzxsec_->xsecInteg()->getvevoXC() ) 
      throw( 1 );
    vevo_ = ixzxsec_->xsecInteg()->vevo(); // = ->vvevo()
    this->setparamvec( vevo_ );
  }
}

ErrCalc::~ErrCalc(){
  if( pdf_ ) delete pdf_;
  if( pdfx_ ) delete pdfx_;
}

void ErrCalc::setparam( QCD::PdfParametrization *p ){
  
  int emID = p->emNumber();
  
  if( emID < 0 ) return; // errorMatrix ID shoud not be netative 
  
  if( ematrices_.find( emID ) == ematrices_.end() ){
    ematrices_[ emID ] = PdfErrMatrix( p );
  } else {
    ematrices_[ emID ].load( p );
  }
  
  vector< PdfBase* > pdfs = p->pdfs();
  
  for( int j = 0; j < pdfs.size(); j++ ){
    PdfBase* pdfb = pdfs[ j ];
    vpdfbase_.push_back( pdfb );
    
    for( PdfBase::iterator k = pdfb->begin(); k != pdfb->end(); k++ ){
      if( k->second.emid()     == -1    || // no    error matrix assigned
	  k->second.emNumber() != emID     // wrong error matrix assinged
	  ) continue;
      par_.push_back( &( k->second ) );    // register Parameter
    }
  }
}

void ErrCalc::setparamvec( vector< KernelBase* > vevo_ ){
  
  for( int i = 0; i < vevo_.size(); i++ ){
    
    PdfParametrization *param = vevo_[ i ]->initialDistribution();
    
    Evolution::KernelSum::ParamSum *psum =
      dynamic_cast< Evolution::KernelSum::ParamSum* >( param ); 
    
    if( psum ){
      for( int ip = 0; ip < psum->size(); ip++ )
	this->setparam( (*psum)[ ip ] );
    } else {
      this->setparam( param );
    }

  }
}

complex< double > ErrCalc::operator()( const complex< double >& n ){

  ccalc_   = true;
  dcalc_   = false;
  vectvar_ = false;

  N_ = n;
  if( pdfcalc_ ) return this->pdfcalcC();
  if( cfcalc_ && cfunc_ != NULL )  
    return this->calcerror( cfunc_, NULL );
  return this->calcerror( xsec_, NULL );
}

complex< double > ErrCalc::operator()( const complex< double >& n, 
				       const complex< double >& np ){
  ccalc_   = true;
  dcalc_   = true;
  vectvar_ = false;
  
  N_ = n; Np_ = np;
  if( pdfcalc_ ) return this->pdfcalcC();
  return this->calcerror( xsec_, NULL );
}

double ErrCalc::operator()( const double& x ){
  
  ccalc_   = false; // real   variable
  dcalc_   = false; // single variable
  vectvar_ = false; // scalar variable
  
  X_ = x;
  if( pdfcalc_ ) return this->pdfcalcR();
  if( rfcalc_ && rfunc_ != NULL )  
    return this->calcerror( rfunc_, NULL );
  if( xxsec_ ) return this->calcerror( xxsec_, NULL );
  if( ixxsec_ ) return this->calcerror( dynamic_cast< RealFunction* >
					( ixxsec_), NULL );

  return 0.0;
}

double ErrCalc::operator()( const double& x, const double& z ){

  ccalc_   = false; // real variable
  dcalc_   = true;  // two variables
  vectvar_ = false; // scalar variables

  X_ = x;
  Xp_ = z;

  if( pdfcalc_ ) return this->pdfcalcR();
  if( xzxsec_ )  return this->calcerror( xzxsec_, NULL );
  if( ixzxsec_ )  return this->calcerror( dynamic_cast< RealFunction* >
					  ( ixzxsec_), NULL );
  if( xxsec_ )   return this->calcerror( xxsec_,  NULL );
  
  return 0.0;
}

double ErrCalc::operator()( const vector< double >& x,
			    const vector< double >& xp ){

  ccalc_   = false; // real variable
  dcalc_   = true;  // two variables
  vectvar_ = true;  // vector variables
  
  vX_ = x;
  vXp_ = xp;
  if( pdfcalc_ ) return this->pdfcalcR();
  if( xzxsec_ )  return this->calcerror( xzxsec_, NULL );
  if( xxsec_ )   return this->calcerror( xxsec_,  NULL );
  
  return 0.0;
}

double ErrCalc::operator()( const vector< double >& x ){
  
  if( x.size() == 1 ) return (*this)( x[ 0 ] );
  
  ccalc_   = false; // real   variable
  dcalc_   = false; // single variables
  vectvar_ = true;  // vector variables
  
  vX_ = x;
  if( pdfcalc_ ) return this->pdfcalcR();
  if( rfcalc_ && rfunc_ != NULL )  
    return this->calcerror( rfunc_, NULL );
  if( xxsec_ )   return this->calcerror( xxsec_,  NULL );
  
  return 0.0;
}

vector< double > ErrCalc::operator()(){
  if( vvX_.size() == 0 ) throw( 1 );
  return this->vpdfcalcR();
}

complex< double > ErrCalc::pdfcalcC(){
  if( parton_ != pdf_->getPID() ) pdf_->getPID() = parton_ ; 
  if( pdfdirect_ ) return (*pdf_)( N_ );
  
  dcalc_ = false;
  return this->calcerror( pdf_, NULL );
}

double ErrCalc::pdfcalcR(){
  if( parton_ != pdfx_->pdf().getPID() ) pdfx_->pdf().getPID() = parton_ ; 
  if( pdfdirect_ ) return (*pdfx_)( X_ );
  
  dcalc_ = false;
  return this->calcerror( pdfx_, NULL );
}

vector< double > ErrCalc::vpdfcalcR(){
  if( parton_ != pdfx_->pdf().getPID() ) pdfx_->pdf().getPID() = parton_ ; 
  
  dcalc_ = false;
  return this->vcalcerror( pdfx_, NULL );
}

void ErrCalc::setF_G( ComplexFunction* f, ComplexFunction* g ) throw( int ) {
  
  if( f == NULL && g == NULL ) {
    ARG_ERR << "At least one function, F or G, has to be given." << endl;
    throw( 0 );
  }
  isFequalG_ = ( ( f == g && this->valconst() ) || f == NULL || g == NULL );
  // in case f == g, argument consistency also must be checked
  // beause of possible case of correlation calc for f(x) vs f(x')
  ccalc_ = true;                        // complex function
  cfuncF_ = f; cfuncG_ = g;             // set pointers
}

void ErrCalc::setF_G( RealFunction* f, RealFunction* g ) throw( int ) {
  
  if( f == NULL && g == NULL ) {
    ARG_ERR << "At least one function, F or G, has to be given." << endl;
    throw( 0 );
  }
  isFequalG_ = ( ( f == g && this->valconst() ) || f == NULL || g == NULL );
  // in case f == g, argument consistency also must be checked
  // beause of possible case of correlation calc for f(x) vs f(x')
  ccalc_ = false;                       // real function
  rfuncF_ = f; rfuncG_ = g;             // set pointers
}

complex< double > ErrCalc::calcerror( ComplexFunction* f, 
				      ComplexFunction* g ) throw( int ){
  
  complex< double > err( 0.0, 0.0 );
  
  try {
    this->setF_G( f, g );   // set functions F and G
    this->eManipul( err );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << endl;
    throw( error );
  }
  
  return err;
}

double ErrCalc::calcerror( RealFunction* f, RealFunction* g ) throw( int ){
  
  double err( 0.0 );
  
  try {
    this->setF_G( f, g );
    this->eManipul( err );
  } 
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << endl;
    throw( error );
  }
  
  return err;
}

vector< double > ErrCalc::vcalcerror( RealFunction* f, RealFunction* g
				      ) throw( int ){
  
  vector< complex< double > > err( vvX_.size(), 
				   complex< double >( 0.0, 0.0 ) );
  try {
    this->setF_G( f, g );
    this->eManipul( err );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << endl;
    throw( error );
  }
  
  // comvert calculated complex value to real value
  vector< double > tmp( err.size(), 0.0 );
  for( int k = 0; k < err.size(); k++ ){
    if( err[ k ].imag() != 0.0 ) {
      ARG_ERR << "calculated error has non zero imaginary part."
	      << endl;
      throw( 1 );
    }
    tmp[ k ] = err[ k ].real();
  }
  return tmp;
}

void ErrCalc::pdfupdate(){
  for( int k = 0; k < vpdfbase_.size(); k++ ) vpdfbase_[ k ]->update();
  for( int k = 0; k < vevo_.size(); k++ ) vevo_[ k ]->clearCache();
  if( xsec_ ) xsec_->clearCache();
  if( xxsec_ ) xxsec_->moment()->clearCache();
  if( xzxsec_ ) xzxsec_->moment()->clearCache();
}

void ErrCalc::eManipul( double& err ) throw( int ){
  complex< double > errc( 0.0, 0.0 );
  this->eManipul( errc );
  if( errc.imag() != 0.0 ) throw( 2 );
  err = errc.real();
}

void ErrCalc::eManipul( complex< double >& err ) throw( int ){
  
  // initialize with zero
  err = complex< double >( 0.0, 0.0 ); 

  complex< double > orgF = this->getFvalue();
  complex< double > orgG = this->getGvalue();
  
  for( int i = 0; i < par_.size(); i++ ){
    
    Parameter& pi = *( par_[ i ] );
    
    if( pi.emid() == -1 ) continue; // no error matrix assigned
    
    // check if the correspond error matrix is already loaded or not.
    if( ematrices_.find( pi.emNumber() ) == ematrices_.end() ){
      ARG_LOG << "No error matrix (" << pi.emNumber() << ") is loaded." 
	      << endl; 
      continue;
    }
    
    // set error matrix reference
    PdfErrMatrix& em = ematrices_[ pi.emNumber() ];
    
    if( em[ pi.emid() ][ pi.emid() ] == 0.0 ) continue;
    
    // shift parameter by its standard deviation
    pi.value() += dpar_;
    this->pdfupdate();
    
    // calculate with shifted paraemter
    complex< double > diff_f1 = ( this->getFvalue() - orgF ) / dpar_;
    
    // shift back to the original value for off-diagonal elements
    pi.value() -= dpar_;
    
    for( int j = ( isFequalG_ ? i : 0 ); j < par_.size(); j++ ){
      
      Parameter& pj = *( par_[ j ] );
      
      if( pj.emid() == -1 ||                  // no error matrix assigned
	  pi.emNumber() != pj.emNumber()  ||  // belong different matrix
	  em[ pj.emid() ][ pj.emid() ] == 0.0 // no error value
	  ) continue; 
      
      // shift 2nd parameter with its standard deviation
      pj.value() += dpar_;
      this->pdfupdate();
      
      // calcualte with shifted parameters
      complex< double > diff_f2 = ( this->getGvalue() - orgG ) / dpar_;
      
      err += 
	( isFequalG_ == true && i != j ? 2.0 : 1.0 ) * 
	em[ pi.emid() ][ pj.emid() ] * diff_f1 * diff_f2;
      
      // shift 2nd parameter back to the original value 
      pj.value() -= dpar_;
    }
  }
  
  this->pdfupdate();

  // manipulation for data normalization
  if( dataset_ != NULL ){

    //    cout << "norm start" << endl;

    err *= datanorm_ * datanorm_;

    for( int j = 0; j < par_.size(); j++ ){
      
      Parameter& pj = *( par_[ j ] );
      
      // set error matrix reference
      PdfErrMatrix& em = ematrices_[ pj.emNumber() ];
    
      //      if( em[ dataID_ ][ dataID_ ] == 0.0 ) continue;
    
      if( pj.emid() == -1 ||                  // no error matrix assigned
	  dataset_->emNumber() != pj.emNumber() || // belong different matrix
	  em[ pj.emid() ][ pj.emid() ] == 0.0 // no error value
	  ) continue; 
      
      // shift 2nd parameter with its standard deviation
      pj.value() += dpar_;
      this->pdfupdate();
      
      // calcualte with shifted parameters
      complex< double > diff_f2 = ( this->getGvalue() - orgG ) / dpar_;
      
      err += 2.0 * em[ dataID_ ][ pj.emid() ] * dataStEr_ * orgF 
	* datanorm_ * diff_f2;
      
      // shift 2nd parameter back to the original value 
      pj.value() -= dpar_;
    }
    
    this->pdfupdate();
    
    // set error matrix reference
    PdfErrMatrix& em = ematrices_[ dataset_->emNumber() ];
    // dataset diagnal element
    err += em[ dataID_ ][ dataID_ ] * dataStEr_ * dataStEr_ * orgF * orgG;
    
    err /= datanorm_ * datanorm_;
  }

  //! for delta chi2 different from 1
  err *= dchi2_;

}

void ErrCalc::eManipul( vector< complex< double > >& err ) throw( int ){
  
  // calculate original value with function F
  vector< complex< double > > orgF = this->vgetFvalue();
  
  // check vector size for both error and original value
  if( orgF.size() != err.size() ){
    ARG_ERR 
      << "The given error vector size is not same to the calculated value"
      << endl;
    throw( 1 );
  }
  
  // calculate original value for function G
  vector< complex< double > > orgG = this->vgetGvalue();
  
  // initialization
  for( int i = 0; i < err.size(); i++ ) 
    err[ i ] = complex< double >( 0.0, 0.0 );
  
  for( int i = 0; i < par_.size(); i++ ){
    
    Parameter& pi = *( par_[ i ] );
    
    if( pi.emid() == -1 ) continue; // no error matrix element assinged
    
    // check if the correspond error matrix is already loaded or not.
    if( ematrices_.find( pi.emNumber() ) == ematrices_.end() ){
      ARG_LOG << "No error matrix (" << pi.emNumber() << ") is loaded." 
	      << endl; 
      continue;
    }
    
    // set error matrix reference
    PdfErrMatrix& em = ematrices_[ pi.emNumber() ];
    
    if( em[ pi.emid() ][ pi.emid() ] == 0.0 ) continue; // NULL error
    
    // shift parameter i with it error
    pi.value() += dpar_;
    this->pdfupdate();
    
    // calculate value of function F
    vector< complex< double > > tmpvalF = this->vgetFvalue();
    vector< complex< double > > diffF( tmpvalF.size(), 
				       complex< double >( 0.0, 0.0 ) );
    
    for( int k = 0; k < tmpvalF.size(); k++ )
      diffF[ k ] = ( tmpvalF[ k ] - orgF[ k ] ) / dpar_;
    
    // shift back to original value
    pi.value() -= dpar_;
    
    for( int j = ( isFequalG_ ? i : 0 ); j < par_.size(); j++ ){
      
      Parameter& pj = *( par_[ j ] );
      
      if( pj.emid() == -1 ||                  // no error matrix assigned
	  pi.emNumber() != pj.emNumber()  ||  // belong different matrix
	  em[ pj.emid() ][ pj.emid() ] == 0.0 // no error value
	  ) continue; 
      
      // shift parameter j with its error
      pj.value() += dpar_;
      this->pdfupdate();
      
      // get value of function G
      vector< complex< double > > tmpvalG = this->vgetGvalue();
      vector< complex< double > > diffG( tmpvalG.size(), 
					 complex< double >( 0.0, 0.0 ) );
      
      for( int k = 0; k < tmpvalG.size(); k++ )
	diffG[ k ] = ( tmpvalG[ k ] - orgG[ k ] ) / dpar_;
      
      for( int k = 0; k < err.size(); k++ )
	err[ k ] += 
	  ( isFequalG_ == true && i != j ? 2.0 : 1.0 ) * 
	  em[ pi.emid() ][ pj.emid() ] * diffF[ k ] * diffG[ k ];
      
      // shift back to original value
      pj.value() -= dpar_;
    }
  }
  
  this->pdfupdate();

  //! for delta chi2 different from 1
  for( int i = 0; i < err.size(); i++ ) 
    err[ i ] *= dchi2_;
  
}

complex< double > ErrCalc::getFvalue(){
  
  if( ccalc_ ) {
    if( cfuncF_ == NULL ) return this->getGvalue(); // use G instead
    return ( dcalc_ ? (*cfuncF_)( N_, Np_ ) : (*cfuncF_)( N_ ) );
  }
  
  if( rfuncF_ == NULL ) return this->getGvalue();   // use G instead
  
  double v( 0.0 );
  if( vectvar_ ){ // calculate with vector arguments
    if( dcalc_ )  v = (*rfuncF_)( vX_, vXp_ ); 
    else          v = (*rfuncF_)( vX_ ); 
  } else {        // calculate with scalar arguments
    if( dcalc_ ) v = (*rfuncF_)( X_,  Xp_ ); 
    else         v = (*rfuncF_)( X_ ); 
  }
  
  return complex< double >( v, 0.0 );
}

complex< double > ErrCalc::getGvalue(){
  if( ccalc_ ) {
    if( cfuncG_ == NULL ) return this->getFvalue(); // use F instead
    return ( dcalc_ ? (*cfuncG_)( M_, Mp_ ) : (*cfuncG_)( M_ ) );
  }
  
  if( rfuncG_ == NULL ) return this->getFvalue();   // use F instead
  
  double v( 0.0 );
  if( vectvar_ ){ // calculate with vector arguments
    if( dcalc_ )  v = (*rfuncG_)( vZ_, vZp_ ); 
    else          v = (*rfuncG_)( vZ_ ); 
  } else {        // calculate with scalar arguments
    if( dcalc_ ) v = (*rfuncG_)( Z_,  Zp_ ); 
    else         v = (*rfuncG_)( Z_ ); 
  }
  
  return complex< double >( v, 0.0 );
}

bool ErrCalc::valconst(){
  
  if( N_ == M_ && Np_ == Mp_ && X_ == Z_ && Xp_ == Zp_ && 
      vX_ == vZ_ && vXp_ == vZp_ && vvX_ == vvZ_ ) return true;
  
  return false;
}

vector< complex< double > > ErrCalc::vgetFvalue(){
  
  vector< complex< double > > tmpv( 0 );
  if( ccalc_ ){
    ARG_ERR << "Not yet implemented... Do nothing" << endl;
    return tmpv;
  }
  
  if( dcalc_ ) { 
    ARG_ERR << "Not yet implemented... Do nothing" << endl;
    return tmpv;
  }
  
  if( ! pdfcalc_ ){
    ARG_ERR << "vgetFvalue is only implemented for pdfcalc_" << endl;
    return tmpv;
  }
  
  if( ! vcalc_ ) {
    ARG_ERR << "vgetFvalue is only for vcalc_ mode." << endl;
    return tmpv;
  }
  
  if( rfuncF_ == NULL ) return this->vgetGvalue();
  
  tmpv.resize( vvX_.size(), complex< double >( 0.0, 0.0 ) );
  for( int k = 0; k < vvX_.size(); k++ )
    tmpv[ k ] = complex< double >( (*rfuncF_)( vvX_[ k ] ), 0.0 );
  
  return tmpv;
}

vector< complex< double > > ErrCalc::vgetGvalue(){
  
  vector< complex< double > > tmpv( 0 );
  if( ccalc_ ){
    ARG_ERR << "Not yet implemented... Do nothing" << endl;
    return tmpv;
  }
  
  if( dcalc_ ) { 
    ARG_ERR << "Not yet implemented... Do nothing" << endl;
    return tmpv;
  }
  
  if( ! pdfcalc_ ){
    ARG_ERR << "vgetFvalue is only implemented for pdfcalc_" << endl;
    return tmpv;
  }
  
  if( ! vcalc_ ) {
    ARG_ERR << "vgetFvalue is only for vcalc_ mode." << endl;
    return tmpv;
  }
  
  if( rfuncG_ == NULL ) return this->vgetFvalue();

  tmpv.resize( vvZ_.size(), 0.0 );
  for( int k = 0; k < vvZ_.size(); k++ )    
    tmpv[ k ] = complex< double >( (*rfuncG_)( vvZ_[ k ] ), 0.0 );
  
  return tmpv;
}

void ErrCalc::setErrMatrix( const QCD::PdfErrMatrix& ematrix ){
  ematrices_[ ematrix.ID() ] = ematrix;
}

void ErrCalc::addErrMatrix( const QCD::PdfErrMatrix& ematrix ){
  if( ematrices_.find( ematrix.ID() ) == ematrices_.end() ){
    this->setErrMatrix( ematrix );
  } else {
    ematrices_[ ematrix.ID() ] += ematrix;
  }
}

void ErrCalc::addErrMatrix( Exp::ExpData* edata ){
  
  if( edata == NULL ){
    ARG_ERR << "NULL pointer is given." << endl;
    return;
  }
  
  int emID = edata->emNumber();
  if( emID < 0 ) {
    ARG_LOG << "No Error Matrix found in the given ExpData." << endl;
    return;
  }
  
  if( ematrices_.find( emID ) == ematrices_.end() ){
    ematrices_[ emID ] = QCD::PdfErrMatrix( edata );
  } else {
    ematrices_[ emID ].load( edata );
  }
  
}

void ErrCalc::setDataSet( Exp::DataSet* dataset ){
  
  dataset_ = NULL;
  
  if( dataset == NULL ) {
    ARG_LOG << "DataSet pointer is NULL." << endl;
    return;
  }
  
  if( dataset->emid() == -1 ){
    //    ARG_LOG << "The given DataSet does not have error matrix." << endl;
    return;
  }
  
  if( ematrices_.find( dataset->emNumber() ) == ematrices_.end() ){
    ARG_LOG << "Error matrix (" << dataset->emNumber() << ") is not found." 
	    << endl; 
    return;
  }
  
  datanorm_  = dataset->normalization();
  dataStEr_  = dataset->norm();
  dataID_    = dataset->emid();
  emxID_     = dataset->emNumber();
  
  if( ematrices_[ emxID_ ][ dataID_ ][ dataID_ ] == 0.0 ){
    
    ARG_LOG 
      << "Zero em elements: " 
      << setw(35) << dataset->info() << endl;
    
    return;
  }
  
  dataset_ = dataset;
  
}

void ErrCalc::createpdfs(){
  if( pdf_ == NULL ) pdf_ = new PDF( vevo_[ ivevo_ ], parton_ );

  if( pdfx_ == NULL ) pdfx_ = new PDFx( vevo_[ ivevo_ ], parton_,
					4, 6, 1.0E-4, 10.0, 2.0, 0.55 );
}
