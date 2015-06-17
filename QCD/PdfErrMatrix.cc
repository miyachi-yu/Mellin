//
// PdfErrMatrix.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Wed Oct 29 15:57:39 2008 Yoshiyuki Miyachi
// Started on  Wed Oct 29 15:57:39 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>

#include "PdfErrMatrix.hh"
#include "PdfParametrization.hh"

#include <Utility/Arguments.hh>

using namespace std;
using namespace QCD;

PdfErrMatrix::PdfErrMatrix() :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
}

PdfErrMatrix::PdfErrMatrix( const int& id ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( id )
{
}

PdfErrMatrix::PdfErrMatrix( Exp::ExpData* edata ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  if( edata ) this->load( edata->data() );
}

PdfErrMatrix::PdfErrMatrix( Exp::DataSet& data ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  this->load( data );
}

PdfErrMatrix::PdfErrMatrix( Exp::DataPoint& data ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  this->load( data );
}

PdfErrMatrix::PdfErrMatrix( vector< Exp::DataSet >& data ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  this->load( data );
}

PdfErrMatrix::PdfErrMatrix( PdfParametrization* pdf ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  if( pdf ) this->load( pdf );
}

PdfErrMatrix::PdfErrMatrix( vector< PdfParametrization* >& pdfs ) :
  vector< vector< double > >( 0 ), info_( 0 ), normflag_( false ),
  prec_( 3 ), ID_( 0 )
{
  for( int i = 0; i < pdfs.size(); i++ ) 
    if( pdfs[ i ] ) this->load( pdfs[ i ] );
}

PdfErrMatrix::PdfErrMatrix( const PdfErrMatrix& pdf ) : 
  vector< vector< double > >( pdf ), info_( pdf.info_ ),
  normflag_( pdf.normflag_ ), prec_( pdf.prec_ ), ID_( pdf.ID_ )
{
}

PdfErrMatrix::~PdfErrMatrix(){
}

PdfErrMatrix& PdfErrMatrix::operator+=( const PdfErrMatrix& pdf ){
  //  if( ID_ == pdf.ID_ && info_ == pdf.info_ ){
  if( ID_ == pdf.ID_ ){
    PdfErrMatrix tmpem = *this;
    this->resetMatrixSize( pdf.size() );
    for( int i = 0; i < pdf.size(); i++ ){
      if( tmpem.size() == 0 ){
	(*this)[ i ] = pdf[ i ];
      } else if ( tmpem[ i ][ 0 ] == 0.0 ){
	(*this)[ i ] = pdf[ i ];
      } else {
	(*this)[ i ] = tmpem[ i ];
      }    
    }
    //    ID_   = pdf.ID_;
    info_ = pdf.info_;
  } else {
    ARG_ERR << "Error Matrix ID does not match. Do nothing" << endl;
  }
  return *this;
}

PdfErrMatrix& PdfErrMatrix::operator=( const PdfErrMatrix& pdf ){
  if( this != & pdf ){
    PdfErrMatrix tmpem = *this;
    this->resetMatrixSize( pdf.size() );
    for( int i = 0; i < pdf.size(); i++ ) (*this)[ i ] = pdf[ i ];
    ID_ = pdf.ID_;
    info_ = pdf.info_;
  }
  return *this;
}

void PdfErrMatrix::load( PdfParametrization* pdf ){
  
  if( pdf == NULL ) {
    ARG_ERR << "The given pointer is NULL" << endl;
    return;
  }
  
  if( pdf->emNumber() < 0 ){ // no error matrix in the parameterization
    return;
  }
  
  /*
    if this has no elements, try to copy the given information
  */
  if( this->size() == 0 ) ID_ = pdf->emNumber();
  
  if( pdf->emNumber() != ID_ ){
    ARG_ERR << "Skip: ID is different:" 
	    << setw(12) << pdf->emNumber()
	    << setw(12) << ID_
	    << endl;
    return;
  }
  
  vector< PdfBase* > pdfs = pdf->pdfs();
  
  for( int i = 0; i < pdfs.size(); i++ ){
    PdfBase& pp = *( pdfs[ i ] );
    for( PdfBase::iterator itr = pp.begin(); itr != pp.end(); itr++ ){
      
      Parameter& par = itr->second;
      
      if( par.emid() < 0 ) continue; // skip negative matrix index
      
      if( this->size() == 0 ) this->resetMatrixSize( par.ems().size() );
      
      if( this->size() != par.ems().size() ) continue;
      
      (*this)[ par.emid() ] = par.ems(); 
      info_[ par.emid() ]   = pp.name() + "-" + par.name();
      
    }
  }
}

void PdfErrMatrix::load( Exp::ExpData* edata ){
  if( edata ) this->load( edata->data() );
}

void PdfErrMatrix::load( std::vector< Exp::DataSet >& data ){
  for( int i = 0; i < data.size(); i++ ) this->load( data[ i ] );
}

void PdfErrMatrix::load( Exp::DataPoint& data ){

  int emID = data.emNumber();
  
  if( emID < 0 ) return; // skip the given DataSet
  
  if( data.emid() < 0 ){
    ARG_ERR << "Skip: negative matrix index is defined :" 
	    << setw(12) << data.emid()
	    << endl;
    return;
  }
  
  // copy the given error matrix
  if( this->size() == 0 ) { 
    this->resetMatrixSize( data.ems().size() );
    ID_ = data.emNumber();
  } 
  
  if( data.emNumber() != ID_ ){
    ARG_ERR << "Skip: ID is different:" 
	    << setw(12) << data.emNumber()
	    << setw(12) << ID_
	    << endl;
    return;
  }
  
  if( this->size() != data.ems().size() ) {
    ARG_ERR << "Skip: Matrix size is different from number of elements in the given DataSet :" 
	    << setw(12) << this->size()
	    << setw(12) << data.ems().size()
	    << endl;
    return;
  }
  
  (*this)[ data.emid() ] = data.ems();  // set error matrix elements
  info_[ data.emid() ]   = data.info(); // set data information 
  
  return;
}


void PdfErrMatrix::load( Exp::DataSet& data ){
  
  int emID = data.emNumber();
  
  if( emID < 0 ) return; // skip the given DataSet
  
  if( data.emid() < 0 ){
    ARG_ERR << "Skip: negative matrix index is defined :" 
	    << setw(12) << data.emid()
	    << endl;
    return;
  }
  
  // copy the given error matrix
  if( this->size() == 0 ) { 
    this->resetMatrixSize( data.ems().size() );
    ID_ = data.emNumber();
  } 
  
  if( data.emNumber() != ID_ ){
    ARG_ERR << "Skip: ID is different:" 
	    << setw(12) << data.emNumber()
	    << setw(12) << ID_
	    << endl;
    return;
  }
  
  if( this->size() != data.ems().size() ) {
    ARG_ERR << "Skip: Matrix size is different from number of elements in the given DataSet :" 
	    << setw(12) << this->size()
	    << setw(12) << data.ems().size()
	    << endl;
    return;
  }
  
  (*this)[ data.emid() ] = data.ems();  // set error matrix elements
  info_[ data.emid() ]   = data.info(); // set data information 
  
  return;
}

bool PdfErrMatrix::operator==( const PdfErrMatrix& em ){
  return ID_ == em.ID_;
}

bool PdfErrMatrix::operator<( const PdfErrMatrix& em ){
  return ID_ < em.ID_;
}

bool PdfErrMatrix::operator>( const PdfErrMatrix& em ){
  return ID_ > em.ID_;
}

namespace QCD {
  ostream& operator<<( ostream& os, const PdfErrMatrix& em ){
    
    vector< double > diagval_( 0 );

    if( em.getnflag() ){
      for( int i = 0; i < em.size(); i++ )
	diagval_.push_back( sqrt( em[ i ][ i ] ) );
    }

    os << "ID=" << setw( 12 ) << em.ID_ << endl;
    
    os << setw( 10 ) << "********" << flush;
    for( int i = 0; i < em.info().size(); i++ ){
      if( em.info()[ i ].size() > 9 ){
	os << setw( 10 ) << em.info()[ i ].substr( 0, 9 ) << flush;
      } else {
	os << setw( 10 ) << em.info()[ i ] << flush;
      }
    }
    os << endl;
    
    for( int i = 0; i < em.size(); i++ ){
      if( em.info()[ i ].size() > 9 ){
	os << setw( 10 ) << em.info()[ i ].substr( 0, 9 ) << flush;
      } else {
	os << setw( 10 ) << em.info()[ i ] << flush;
      }
      for( int j = 0; j < em[ i ].size(); j++ ){
	os << setw( 10 ) 
	   << rint( ( em.getnflag() ? 
		      em[ i ][ j ] / diagval_[ i ] / diagval_[ j ] 
		      : em[ i ][ j ] ) * pow( 10.0, em.getprec() ) )
	  / pow( 10.0, em.getprec() ) << flush;
      }
      os << endl;
    }

    return os;
  }
}

void PdfErrMatrix::resetMatrixSize( const int& n ){
  this->resize( n, vector< double >( n, 0.0 ) );
  this->info_.resize( n, "" );
}


std::vector< std::vector< double > > PdfErrMatrix::normalized() {
  vector< vector< double > > nm = *this;
  vector< double > sig( nm.size(), 1.0 );
  for( int i = 0; i < nm.size(); i++ )
    if( nm[ i ][ i ] > 0.0 ) sig[ i ] = sqrt( nm[ i ][ i ] );
  
  for( int i = 0; i < nm.size(); i++ )
    for( int j = 0; j < nm[ i ].size(); j++ )
      nm[ i ][ j ] /= sig[ i ] * sig[ j ];
  
  return nm;
}
