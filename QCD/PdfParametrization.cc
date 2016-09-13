// $Id: PdfParametrization.cc,v 1.41 2010/03/03 03:04:38 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "PdfParametrization.hh"
#include "PdfBase.hh"
#include "FFDisFavor.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace QCD;

string PdfParametrization::tagName = "set";

PdfParametrization::PdfParametrization() : 
  n_( 1.0, 0.0 ), name_(""), scale_( 1.0 ), pdfs_( 0 ),
  g_( NULL ), u_( NULL ), d_( NULL ), s_( NULL ), c_( NULL ), b_( NULL ),
  ubar_( NULL ), dbar_( NULL ), sbar_( NULL ), cbar_( NULL ), bbar_( NULL ),
  uv_( NULL ), dv_( NULL ), sv_( NULL ),
  a0_( NULL ), a3_( NULL ), a8_( NULL ),
  a3m_( NULL ), a8m_( NULL ),
  sea_( NULL ), delta_( NULL ), duratio_( NULL ), dusum_( NULL ),
  path_( "" ), 
  U_( NULL ), D_( NULL ), S_( NULL ), C_( NULL ), B_( NULL ),
  flavors_( true ),
  hadron_( Flavor::PIp ),
  toBeDeleted_( false )
{
}

PdfParametrization::PdfParametrization( PdfParametrization& conf ) : 
  n_( conf.n_ ), name_( conf.name_), 
  scale_( conf.scale_ ), pdfs_( 0 ),
  g_( conf.g_ ), u_( conf.u_ ), d_( conf.d_ ), s_( conf.s_ ),
  c_( conf.c_ ), b_( conf.b_ ),
  ubar_( conf.ubar_ ), dbar_( conf.dbar_ ), sbar_( conf.sbar_ ),
  cbar_( conf.cbar_ ), bbar_( conf.bbar_ ),
  uv_( conf.uv_ ), dv_( conf.dv_ ), sv_( conf.sv_ ),
  a0_( conf.a0_ ), a3_( conf.a3_ ), a8_( conf.a8_ ),
  a3m_( conf.a3m_ ), a8m_( conf.a8m_ ), 
  sea_( conf.sea_ ), delta_( conf.delta_ ),
  duratio_( conf.duratio_ ), dusum_( conf.dusum_ ),
  path_( conf.path_ ), 
  U_( conf.U_ ), D_( conf.D_ ), S_( conf.S_ ), C_( conf.D_ ), B_( conf.B_ ),
  flavors_( conf.flavors_ ),
  hadron_( conf.hadron_ ),
  toBeDeleted_( false )
{
}

PdfParametrization::PdfParametrization( ConfigPdfBase& conf ) : 
  n_( 1.0, 0.0 ), name_( conf.name() ), 
  scale_( conf.scale() ), pdfs_( 0 ),
  g_( NULL ), u_( NULL ), d_( NULL ), s_( NULL ), c_( NULL ), b_( NULL ),
  ubar_( NULL ), dbar_( NULL ), sbar_( NULL ), cbar_( NULL ), bbar_( NULL ),
  uv_( NULL ), dv_( NULL ), sv_( NULL ),
  a0_( NULL ), a3_( NULL ), a8_( NULL ),
  a3m_( NULL ), a8m_( NULL ),
  sea_( NULL ), delta_( NULL ), duratio_( NULL ), dusum_( NULL ),
  path_( conf.path() ), 
  U_( NULL ), D_( NULL ), S_( NULL ), C_( NULL ), B_( NULL ),
  flavors_( true ),
  hadron_( Flavor::PIp ),
  toBeDeleted_( false )
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

PdfParametrization::PdfParametrization( ConfigFFbase& conf ) : 
  n_( 1.0, 0.0 ), name_( conf.name() ), 
  scale_( conf.scale() ), pdfs_( 0 ),
  g_( NULL ), u_( NULL ), d_( NULL ), s_( NULL ), c_( NULL ), b_( NULL ),
  ubar_( NULL ), dbar_( NULL ), sbar_( NULL ), cbar_( NULL ), bbar_( NULL ),
  uv_( NULL ), dv_( NULL ), sv_( NULL ),
  a0_( NULL ), a3_( NULL ), a8_( NULL ),
  a3m_( NULL ), a8m_( NULL ),
  sea_( NULL ), delta_( NULL ), duratio_( NULL ), dusum_( NULL ),
  path_( conf.path() ), 
  U_( NULL ), D_( NULL ), S_( NULL ), C_( NULL ), B_( NULL ),
  flavors_( true ),
  hadron_( Flavor::PIp ),
  toBeDeleted_( false )
{

  this->pdfs( conf.pdfs() );
}

PdfParametrization::~PdfParametrization() {
  // delete pdf object created....
  if( toBeDeleted_ ) for( int i = 0; i < pdfs_.size(); i++ ) delete pdfs_[ i ];
}

void PdfParametrization::setN( const complex< double >& n ){
  n_ = n;
}

complex< double > PdfParametrization::q( const Flavor::PARTON& q ) {
  
  static double c3 = 1.0 / 3.0;
  static double c6 = 1.0 / 6.0;
  
  // ---------------  gluon and heavy flavors --------------- //
  switch( q ){
  case Flavor::g : return (*g_)( n_ );
    
  case Flavor::c : return ( c_ ? (*c_)( n_ ) :
			    complex< double >( 0.0, 0.0 ) );
  case Flavor::b : return ( b_ ? (*b_)( n_ ) :
			    complex< double >( 0.0, 0.0 ) );
  case Flavor::cbar : return ( cbar_ ? (*cbar_)( n_ ) :
			       complex< double >( 0.0, 0.0 ) );
  case Flavor::bbar : return ( bbar_ ? (*bbar_)( n_ ) :
			       complex< double >( 0.0, 0.0 ) );
  case Flavor::cv: return this->q( Flavor::c ) - this->q( Flavor::cbar );
  case Flavor::bv: return this->q( Flavor::b ) - this->q( Flavor::bbar );
  }
  
  // ---------------  light flavors --------------- //
  if( flavors_ ){
    
    // --- valence distribution
    switch( q ){
    case Flavor::uv: return this->q( Flavor::u ) - this->q( Flavor::ubar );
    case Flavor::dv: return this->q( Flavor::d ) - this->q( Flavor::dbar );
    case Flavor::sv: return this->q( Flavor::s ) - this->q( Flavor::sbar );
    }    
    
    // --- non-singlet distribution
    switch( q ){
    case Flavor::uns: return this->q( Flavor::u ) + this->q( Flavor::ubar ) - c3 * this->q( Flavor::a0 );
    case Flavor::dns: return this->q( Flavor::d ) + this->q( Flavor::dbar ) - c3 * this->q( Flavor::a0 );
    case Flavor::sns: return this->q( Flavor::s ) + this->q( Flavor::sbar ) - c3 * this->q( Flavor::a0 );
    }      
    
    switch( q ){
      
    case Flavor::u:    
      if( u_ ) return (*u_)( n_ );
      if( U_ && ubar_ ) return (*U_)( n_ ) - (*ubar_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    case Flavor::ubar: 
      if( ubar_ ) return (*ubar_)( n_ );
      if( U_ && u_ ) return (*U_)( n_ ) - (*u_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    case Flavor::d:    
      if( d_ ) return (*d_)( n_ );
      if( D_ && dbar_ ) return (*D_)( n_ ) - (*dbar_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    case Flavor::dbar:
      if( dbar_ ) return (*dbar_)( n_ );
      if( D_ && d_ ) return (*D_)( n_ ) - (*d_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    case Flavor::s:
      if( s_ ) return (*s_)( n_ );
      if( S_ && sbar_ ) return (*S_)( n_ ) - (*sbar_)( n_ );
      if( S_ ) return 0.5 * (*S_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    case Flavor::sbar:
      if( sbar_ ) return (*sbar_)( n_ );
      if( S_ && s_ ) return (*S_)( n_ ) - (*s_)( n_ );
      if( S_ ) return 0.5 * (*S_)( n_ );
      return complex< double >( 0.0, 0.0 );
      
    }
    
  } else {
    
    complex< double > a0 = ( *a0_ )( n_ );
    complex< double > a3 = ( *a3_ )( n_ );
    complex< double > a8 = ( *a8_ )( n_ );
    
    complex< double > U = c3 * a0 + 0.5 * a3 + c6 * a8;
    complex< double > D = c3 * a0 - 0.5 * a3 + c6 * a8;
    complex< double > S = c3 * ( a0 - a8 );
    
    switch( q ){
    case Flavor::u : return U - this->q( Flavor::ubar );
    case Flavor::d : return D - this->q( Flavor::dbar );
    case Flavor::s : 
    case Flavor::sbar :  return 0.5 * S;
      
    case Flavor::ubar :
      return ( a3m_ && a8m_ ? 
	       0.25 * ( a3  - (*a3m_)( n_ ) + a8 - (*a8m_)( n_ ) ) :
	       0.5 * S );
      
    case Flavor::dbar :
      return ( a3m_ && a8m_ ? 
	       0.25 * ( - a3 + (*a3m_)( n_ ) + a8 - (*a8m_)( n_ ) ) :
	       0.5 * S );
      
    case Flavor::uns: return U - c3 * a0;
    case Flavor::dns: return D - c3 * a0;
    case Flavor::sns: return S - c3 * a0;
      
    case Flavor::uv: return U - 2.0 * this->q( Flavor::ubar );
    case Flavor::dv: return D - 2.0 * this->q( Flavor::dbar );
    case Flavor::sv: return complex< double >( 0.0, 0.0 );

    }
  }
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > PdfParametrization::q( const Flavor::TYPE& q ) {
  
  if( flavors_ ){
    switch( q ){
    case Flavor::a0: 
      return 
	(*u_)( n_ ) + (*ubar_)( n_ ) + 
	(*d_)( n_ ) + (*dbar_)( n_ ) + 
	(*s_)( n_ ) + (*sbar_)( n_ );
      
    case Flavor::a3m:
      return 
	( (*u_)( n_ ) - (*ubar_)( n_ ) ) -
	( (*d_)( n_ ) - (*dbar_)( n_ ) );
      
    case Flavor::a3:
      return 
	( (*u_)( n_ ) + (*ubar_)( n_ ) ) -
	( (*d_)( n_ ) + (*dbar_)( n_ ) );
      
    case Flavor::a8: 
      return 
	( (*u_)( n_ ) + (*ubar_)( n_ ) ) + 
	( (*d_)( n_ ) + (*dbar_)( n_ ) ) + 
	- 2.0 * ( (*s_)( n_ ) + (*sbar_)( n_ ) );
      
    case Flavor::a8m: 
      return 
	( (*u_)( n_ ) - (*ubar_)( n_ ) ) + 
	( (*d_)( n_ ) - (*dbar_)( n_ ) ) + 
	- 2.0 * ( (*s_)( n_ ) - (*sbar_)( n_ ) );
    } 

  } else {
    
    switch( q ){
    case Flavor::a0: return (*a0_)( n_ ); 
      
    case Flavor::a3: return (*a3_)( n_ ); 
    case Flavor::a8: return (*a8_)( n_ ); 

    case Flavor::a3m: return ( a3m_ ? (*a3m_)( n_ ) : this->q( Flavor::a3 ) );
    case Flavor::a8m: return ( a8m_ ? (*a8m_)( n_ ) : this->q( Flavor::a8 ) );

    } 
  }
  
  return complex< double >( 0.0, 0.0 );
}


void PdfParametrization::assignPdf( PdfBase *pdf, PdfBase *ref ){
  
  if( ref == NULL ) ref = pdf;               // register itsef
  
  if( pdf->type() == PdfBase::parton ){
    
    switch( pdf->id() ){
    case Flavor::g :    g_ = ref;    break;
    case Flavor::u :    u_ = ref;    break;
    case Flavor::d :    d_ = ref;    break;
    case Flavor::s :    s_ = ref;    break;
    case Flavor::c :    c_ = ref;    break;
    case Flavor::b :    b_ = ref;    break;
    case Flavor::ubar : ubar_ = ref; break;
    case Flavor::dbar : dbar_ = ref; break;
    case Flavor::sbar : sbar_ = ref; break;
    case Flavor::cbar : cbar_ = ref; break;
    case Flavor::bbar : bbar_ = ref; break;
    case Flavor::uv : uv_ = ref;     break;
    case Flavor::dv : dv_ = ref;     break;
    case Flavor::sv : sv_ = ref;     break;
    case Flavor::U : U_ = ref; break;
    case Flavor::D : D_ = ref; break;
    case Flavor::S : S_ = ref; break;
    case Flavor::C : C_ = ref; break;
    case Flavor::B : B_ = ref; break;
    }
    
  } else {
    
    switch( pdf->id() ){
    case Flavor::a0  : a0_  = ref; break;
    case Flavor::a3  : a3_  = ref; break;
    case Flavor::a8  : a8_  = ref; break;
    case Flavor::a3m : a3m_ = ref; break;
    case Flavor::a8m : a8m_ = ref; break;
    case Flavor::DELTA : delta_ = ref; break;
    case Flavor::SEA   : sea_   = ref; break;
    case Flavor::DURATIO : duratio_ = ref; break;
    case Flavor::DUSUM : dusum_ = ref; break;
    }
  }
}

void PdfParametrization::assign(){

  vector< PdfBase* > qs = this->pdfs();
  for( int i = 0; i < qs.size(); i++ ){
    PdfBase *ref = qs[ i ];     // set current object as refered obj..
    qs[ i ]->parametrization( this ); // set this pointer to PdfBase

    // does the pdf refer other pdf?
    if( qs[ i ]->src() != "" )
      for( int j = 0; j < qs.size(); j++ )
	if( qs[ i ]->src() == qs[ j ]->pid() ){
	  qs[ i ]->reference( qs[ j ] );
	  // if there is no parameter defined, it is just a red copy...
	  if( qs[ i ]->size() == 0 ) ref = qs[ j ]; 
	}
    
    // assign ref
    qs[ i ]->update();
    this->assignPdf( qs[ i ], ref );
  }
  
}

bool PdfParametrization::check(){
  
  this->assign();
  
  // gluon distribution is always required....
  if( ! g_ ){
    cerr << __FILE__ << ":" << __LINE__
	 << "\tgluon is missing" << endl;
    return false;
  }
  
  // check individual quark flavor
  flavors_ = false;
  if( u_ && d_ && s_ && ubar_ && dbar_ && sbar_ ) {
    flavors_ = true;
  } else if( U_ && ubar_ && D_ && dbar_ && s_ && sbar_ ) {
    flavors_ = true;
  } else if( U_ && ubar_ && D_ && dbar_ && S_ ) {
    flavors_ = true;
  }
  
  if( ! flavors_ && !( a0_ && a3_ && a8_ ) ){
    cerr 
      << "PdfParameterization requires either \n"
      << "\t quarks: u, d, s, ubar, dbar, and sbar\n"
      << "\t SU(3):  a0, a3, a8\n"
      << "Please check your XML file if these pdfs are properly configured.\n"
      << "u:\t" << ( u_ ? "OK" : "missing" ) << "\n"
      << "u-bar:\t" << ( ubar_ ? "OK" : "missing" ) << "\n"
      << "d:\t" << ( d_ ? "OK" : "missing" ) << "\n"
      << "d-bar:\t" << ( dbar_ ? "OK" : "missing" ) << "\n"
      << "s:\t" << ( s_ ? "OK" : "missing" ) << "\n"
      << "s-bar:\t" << ( sbar_ ? "OK" : "missing" ) << "\n"
      << "a0:\t" << ( a0_ ? "OK" : "missing" ) << "\n"
      << "a3:\t" << ( a3_ ? "OK" : "missing" ) << "\n"
      << "a8:\t" << ( a8_ ? "OK" : "missing" ) << "\n"
      << endl;
    return false;
  }
  
  return true;
}

string PdfParametrization::baseName() {
  return basename( const_cast< char* >( path_.c_str() ) );
}

void PdfParametrization::pdfs( vector< PdfBase* > v ) { 
  
  // clear all existing pointers
  g_ = NULL;
  u_ = NULL;
  d_ = NULL;
  s_ = NULL;
  c_ = NULL;
  b_ = NULL;

  ubar_ = NULL;
  dbar_ = NULL;
  sbar_ = NULL;
  cbar_ = NULL;
  bbar_ = NULL;
  
  uv_ = NULL;
  dv_ = NULL;
  sv_ = NULL;

  U_ = NULL;
  D_ = NULL;
  S_ = NULL;
  C_ = NULL;
  B_ = NULL;

  a0_ = NULL;
  a3_ = NULL;
  a8_ = NULL;

  sea_ = NULL;
  delta_ = NULL;

  duratio_ = NULL;
  dusum_   = NULL;

  // if I have already created objects, delete them
  if( toBeDeleted_ ) 
    for( int i = 0; i < pdfs_.size(); i++ ) if( pdfs_[i] ) delete pdfs_[i];
  
  // assign to manage with this 
  pdfs_ = v ; 

  // these object must be deleted by myself
  toBeDeleted_ = true;
  
} 

PdfParametrization& PdfParametrization::operator<<( ConfigPdfBase& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}

string PdfParametrization::xmlHeader(){
  
  ostringstream ostr;
  ostr << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << endl;
  ostr << "<!DOCTYPE " << PdfParametrization::tagName << " [" << endl;
  ostr << "<!ELEMENT " << PdfParametrization::tagName << " (pdf*)>" << endl;
  ostr << "<!ATTLIST set" << endl;
  ostr << "  scale CDATA #REQUIRED" << endl;
  ostr << "  name  CDATA #REQUIRED >" << endl;
  ostr << "<!ELEMENT pdf (parameter*) >" << endl;
  ostr << "<!ATTLIST pdf" << endl;
  ostr << "  name CDATA #REQUIRED" << endl;
  ostr << "  pid  ID    #IMPLIED" << endl;
  ostr << "  src  IDREF #IMPLIED" << endl;
  ostr << "  dfav IDREF #IMPLIED" << endl;
  ostr << "  fav  IDREF #IMPLIED" << endl;
  ostr << ">" << endl;
  ostr << "<!ELEMENT parameter (ems*) >" << endl;
  ostr << "<!ATTLIST parameter" << endl;
  ostr << "  index  CDATA #REQUIRED" << endl;
  ostr << "  srcid  CDATA #IMPLIED" << endl;
  ostr << "  name   CDATA #IMPLIED" << endl;
  ostr << "  value  CDATA #REQUIRED" << endl;
  ostr << "  error  CDATA #IMPLIED" << endl;
  ostr << "  state  (fixed|free|add|multi) #REQUIRED" << endl;
  ostr << "  upper  CDATA #IMPLIED" << endl;
  ostr << "  lower  CDATA #IMPLIED" << endl;
  ostr << "  emid   CDATA #IMPLIED" << endl;
  ostr << ">" << endl;
  ostr << "<!ELEMENT ems EMPTY >" << endl;
  ostr << "<!ATTLIST ems" << endl;
  ostr << "  emid   CDATA  #REQUIRED" << endl;
  ostr << "  value  CDATA  #REQUIRED" << endl;
  ostr << ">" << endl;
  ostr << "]>" << endl;		      
  return ostr.str();
}

namespace QCD {
  ostream& operator<<( ostream& os, const PdfParametrization& param ){
    os << PdfParametrization::xmlHeader() << endl;
    os << "<" << PdfParametrization::tagName 
       << " scale=\"" << param.scale() 
       << "\" name=\"" << param.name() << "\">" << endl;
    
    vector< PdfBase* > pdfs = param.pdfs();
    for( int i = 0; i < pdfs.size(); i++ ) os << *pdfs[ i ] << endl;
    
    os << "</" << PdfParametrization::tagName << ">" << flush;
    return os;
  }
}

PdfErrMatrix PdfParametrization::errorMatrix() {
  return PdfErrMatrix( this );
}

void PdfParametrization::errorMatrix( PdfErrMatrix& em ) {
  for( int i = 0; i < pdfs_.size(); i++ ){
    PdfBase *pdf = pdfs_[ i ];
    for( PdfBase::iterator itr = pdf->begin(); itr != pdf->end(); itr++ ){
      Parameter& par = itr->second;
      if( par.emid() >= 0 ) {
	if( em.size() != par.ems().size() ) em.resize( par.ems().size() );
	em[ par.emid() ] = par.ems(); 
      }
    }
  }
  
  // fill with 0.0 if line has different size from the row.
  for( int i = 0; i < em.size(); i++ ){
    if( em[ i ].size() != em.size() ) 
      em[ i ] = vector< double >( em.size(), 0.0 );
  }
  
  em.ID() = this->emNumber();
  
}

// return the emNumber of the first Parameter, assuming 
// all emNumbers are the same.
int PdfParametrization::emNumber() {
  for( int i = 0; i < pdfs_.size(); i++ ){
    PdfBase *pdf = pdfs_[ i ];
    for( PdfBase::iterator itr = pdf->begin(); itr != pdf->end(); itr++ ){
      Parameter& par = itr->second;
      if( par.emNumber() != -1 && par.emid() != -1 ) return par.emNumber();
    }
  }
  
  // no Parameter object with error matrix is found. 
  // return negative value for the error matrix ID,
  // which means there are no errorMatrix defined.
  return -1;
}

PdfBase*   PdfParametrization::find( const std::string& name ){
  for( int i = 0; i < pdfs_.size(); i++ ){
    if( pdfs_[ i ]->name() == name ) return pdfs_[ i ];
  }
  return NULL;
}

Parameter* PdfParametrization::find( const std::string& name, 
				     const int& index ) {
  PdfBase *base = this->find( name );
  if( base == NULL ) return NULL;
  PdfBase::iterator itr = base->find( index );
  return ( itr != base->end() ? &( itr->second ) : NULL );
}

void PdfParametrization::remove( const std::string& name ){
  vector< PdfBase* >::iterator itr = pdfs_.begin();
  while( itr != pdfs_.end() ){
    if( ( *itr )->name() == name ) { 
      itr = pdfs_.erase( itr );
    } else itr++;
  }
}
