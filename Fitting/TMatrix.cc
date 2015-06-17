#include "TMatrix.hh"

using namespace std;
using namespace Fit;

TMatrix::TMatrix():  
  TMatrixD(), style_( TMatrix::TEXT ), precision_( 2 )
{}

TMatrix::TMatrix( const QCD::PdfErrMatrix& pem ) throw( int ):
  TMatrixD(), style_( TMatrix::TEXT ), precision_( 2 )
{
  if( ! this->setMX( pem ) ){ 
    throw( 1 );
  };
}

TMatrix::TMatrix( const TMatrixD& tmxd ) :
  TMatrixD( tmxd ), style_( TMatrix::TEXT ), precision_( 2 )
{
}

TMatrix::TMatrix( const int& row, const int& column ):  
  TMatrixD( row, column ), style_( TMatrix::TEXT ), precision_( 2 )
{}

bool TMatrix::setMX( const QCD::PdfErrMatrix& pem ){

  // square matrix
  if( pem.size() != pem[ 0 ].size() ){
    ARG_ERR << "# TMatrix (square matrix) construction fail" << endl;    
    return false;
  }

  if( this->GetNrows() != pem.size() )
    this->ResizeTo( pem.size(), pem.size() );

  for( int i = 0; i < this->GetNrows(); i++ )
    for( int j = 0; j < this->GetNcols(); j++ )
      (*this)[ i ][ j ] = pem[ i ][ j ];

  return true;
}


namespace Fit {
  ostream& operator<<( ostream& os, const TMatrix& mx ){
    
    int n = mx.GetNrows();
    int m = mx.GetNcols();
    
    /*
      TMatrixD tmpmx( mx );
      for( int i = 0; i < n; i++ ){
      TVectorD v = TMatrixDRow( tmpmx, i );
      for( int j = 0; j < m; j++ )
      os << v( j ) << " " << flush;
      os << endl;
      }
    */
    
    for( int i = 0; i < n; i++ ){
      for( int j = 0; j < m; j++ )
	os << mx[ i ][ j ] << " " << flush;
      os << endl;
    }
    
    return os;
  }  
}

string TMatrix::suffix(){
  switch( style_ ){
  case TEXT: return ".txt";
  case TEX:  return ".tex";
  }

  return ".dat";
}

void TMatrix::print( std::ostream& os, const std::vector< string >& hrow, 
		     const std::vector< string >& hcol ){
  if( this->GetNrows() != hrow.size() ||
      this->GetNcols() != hcol.size() ){
    os << "inconsistency between matrix size and header size" << endl;
    return;
  }

  this->header( os, hcol );

  switch( style_ ){

  case TEXT:

    for( int i = 0; i < this->GetNrows(); i++ ){
      //      os << setw( 10 ) << hrow[ i ] << flush;
      if( ! i ) os << setw( 10 ) << hrow[ 0 ] << flush;
      if( i ) os << setw( 10 ) << i << flush;
      for( int j = 0; j < this->GetNcols(); j++ ){
	os << setw( precision_ + 8 ) << flush;
	if( (*this)[ i ][ j ] < pow( 10.0, - precision_ ) ){
	  ostringstream ost; ost << "--";
	  for( int k = 0; k < precision_; k++ ) ost << "-" << flush;	  
	  os << ost.str() << flush;
	} else {
	  os << rint( (*this)[ i ][ j ] * pow( 10.0, precision_ ) )
	    / pow( 10.0, precision_ ) << flush;
	}
      }
      os << endl;
    }

    break;

  case TEX:

    for( int i = 0; i < this->GetNrows(); i++ ){
      if( ! i ) os << hrow[ 0 ] << " & " << flush;
      if( i ) os << "$" << i << "$ & " << flush;
      for( int j = 0; j < this->GetNcols(); j++ ){
	if( (*this)[ i ][ j ] < pow( 10.0, - precision_ ) ){
	  os << "--" << flush;
	  for( int k = 0; k < precision_; k++ ) os << "-" << flush;
	} else {
	  os << rint( (*this)[ i ][ j ] * pow( 10.0, precision_ ) )
	    / pow( 10.0, precision_ ) << flush;
	}
	os << ( j != this->GetNcols() - 1 ? " & " : " \\\\" ) << flush;
      }
      os << endl;
      if( ! i )  os << "\\hline \\hline " << endl;
    }

    break;

  }

  this->footer( os );

}

void TMatrix::header( ostream& os, const vector< string >& head ){
  
  switch( style_ ){
    
  case TEXT:
    
    os << setw( 10 ) << "E.V. No." << flush;
    for( int i = 0; i < head.size(); i++ )
      os << setw( precision_ + 8 ) << head[ i ] << flush;
    os << endl;

    break;
    
  case TEX:
    
    os << "\\documentclass{article}" << endl;
    os << "\\begin{document}" << endl;
    os << "\\begin{tabular}{c|" << flush;
    for( int i = 0; i < head.size(); i++ )
      os << "r" << ( i != head.size() - 1 ? "|" : "}" ) << flush;
    os << endl;

    os << "E.V. No. & " << flush;
    for( int i = 0; i < head.size(); i++ )
      os << "$" << head[ i ] << "$" 
	 << ( i != head.size() - 1 ? " & " : " \\\\" ) 
	 << flush;
    os << endl;
    
    os << "\\hline " << endl;
    break;
  }
  
}

void TMatrix::footer( ostream& os ){
  switch( style_ ){
    
  case TEXT:
    break;
    
  case TEX:
    os << "\\hline \\hline " << endl;
    os << "\\end{tabular}" << endl;
    os << "\\end{document}" << endl;   
    break;
  }
}

