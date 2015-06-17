//
// TableMaker.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Tue Nov 18 11:09:23 2008 Yoshiyuki Miyachi
// Started on  Tue Nov 18 11:09:23 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TableMaker.hh"

#include <QCD/PdfParametrization.hh>
#include <QCD/PdfErrMatrix.hh>

#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>

using namespace std;

TableMaker::TableMaker() : 
  params_( 0 ), style_( TEXT ), level_( SIMPLE ), precision_( 3 ),
  withError_( true )
{
}

TableMaker::~TableMaker() {
}

void TableMaker::print( ostream& os, 
			QCD::PdfBase* pdf,
			map< int, string >& head ) const {
  
  if( level_ == SIMPLE && pdf->size() == 0 ) return;
  
  map< int, string >::iterator last = head.end();
  last--;

  os << setprecision( precision_ );
  
  switch( style_ ){
    
  case TEXT:
    
    os << setw( 6 ) << pdf->name() << flush;
    
    for( map< int, string >::iterator htr = head.begin();
	 htr != head.end();
	 htr++ ){
      
      QCD::PdfBase::iterator itr;
      
      for( itr = pdf->begin(); itr != pdf->end(); itr++ ) {
	if( htr->second == itr->second.name() ) break;
      }
      
      if( itr != pdf->end() ){

	if( itr->second.checkSrcid() ){

	  os << setw( precision_ - 1 );
	  string rname_( (*pdf)[ itr->second.srcid() ].name() );
	  os << "(" << ( rname_.size() == 1 ? rname_ : "r" ) << ")";

	  if( itr->second.add() ){
	    os << setw( 4 ) << itr->second.value() << "+";
	  } else if( itr->second.multi() ) {
	    os << setw( 4 ) << itr->second.value() << "*";
	  } else {
	    os << setw( 5 ) << "";
	    //	    os << (*pdf)[ itr->second.srcid() ].value();
	  }

	} else if( itr->second.add() ){
	  os << setw( precision_ + 5 ) << itr->second.value() << "+";
	} else if( itr->second.multi() ){
	  os << setw( precision_ + 5 ) << itr->second.value() << "*";
	} else {
	  os << setw( precision_ + 6 ) << itr->second.value();
	}

	if( withError_ ){
	  if( itr->second.fixed() || itr->second.checkSrcid() ){
	    os << setw( precision_ + 10 ) << "(fixed)";
	  } else {
	    os << " +- ";
	    os << setw( precision_ + 6 ) << itr->second.error() ;
	  }
	}
      } else {
	os << setw( precision_ + 6 ) << "";
	if( withError_ ) os << setw( precision_ + 10 ) << "";
      }
    }
    
    os << endl;
    
    break;
    
  case TEX:
    
    os << pdf->name() << " & " << flush;
    
    for( map< int, string >::iterator htr = head.begin();
	 htr != head.end();
	 htr++ ){
      
      QCD::PdfBase::iterator itr;
      
      for( itr = pdf->begin(); itr != pdf->end(); itr++ ) {
	if( htr->second == itr->second.name() ) break;
      }
      
      if( itr != pdf->end() ){
	os << itr->second.value();
	if( itr->second.fixed() ){
	  os << " (fixed)";
	} else {
	  os << " $\\pm$ " << itr->second.error() ;
	}
      }
      os << ( htr->first != last->first  ? " & " : " \\\\" );
    
    }
    
    os << endl;
    
    break;
      
  }
  
}

void TableMaker::header( ostream& os, map< int, string >& head ) const {
  
  map< int, string >::iterator last = head.end();
  last--;
  
  switch( style_ ){
    
  case TEXT:
    
    os << setw(6) << "name" << flush;
    for( map< int, string >::iterator htr = head.begin();
	 htr != head.end();
	 htr++ ){
      os << setw( withError_ ? 2 * precision_ + 16 : precision_ + 6 ) << htr->second << flush;
    }
    os << endl;
    break;
    
  case TEX:
    
    os << "\\documentclass{article}" << endl;
    os << "\\begin{document}" << endl;
    os << "\\begin{tabular}{c|" << flush;
    for( int i = 0; i < head.size(); i++ ){
      os << "r" << ( i != head.size() - 1 ? "|" : "}" ) << flush;
    }
    os << endl;
    os << "name & " << flush;
    for( map< int, string >::iterator itr = head.begin();
	 itr != head.end();
	 itr++ ){ 
      os << "$" << itr->second << "$" <<
	( itr->first != last->first ? " & " : " \\\\" );
    }
    os << endl;
    
    os << "\\hline \\hline " << endl;
    break;
  }
  
}

void TableMaker::footer( ostream& os ) const {
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

void TableMaker::print( ostream& os, 
			QCD::PdfErrMatrix* em ) const {
  
  // error matrix does not have any error
  if( withError_ ){
    cerr << "Error Matrix should be wrote without error" << endl;
    return;
  }
  
  os << setprecision( precision_ );

  map< int, string > header = this->header( em );
  this->header( os, header );
  
  for( int i = 0; i < em->size(); i++ ){
    
    switch( style_ ){
    case TEXT:
      
      os << setw(6) << header[ i ].substr( 0, 6 ) << flush;
      for( int j = 0; j < (*em)[ i ].size(); j++ ){
	os << setw( precision_ + 6 ) << (*em)[ i ][ j ] << flush;
      }
      os << endl;

      break;
    case TEX:
      
      os << "$" <<  header[ i ] << "$ & "<< flush;
      for( int j = 0; j < (*em)[ i ].size(); j++ ){
	os << (*em)[ i ][ j ] << 
	  ( j != (*em)[ i ].size() - 1 ? " & " : "\\\\" ) << flush;
      }
      os << endl;
      
      break;
    }
    
  }

  this->footer( os );  
}

void TableMaker::print( ostream& os, 
			QCD::PdfParametrization* param ) const {
  
  map< int, string >    header = this->header( param );
  vector< QCD::PdfBase* > pdfs = param->pdfs();
  
  this->header( os, header );
  
  for( int i = 0; i < pdfs.size(); i++ ) {
    this->print( os, pdfs[ i ], header );
  }
  
  this->footer( os );
  
}

map< int, string > TableMaker::header( QCD::PdfParametrization* param ) const {
  map< int, string > head;
  vector< QCD::PdfBase* > pdfs = param->pdfs();
  for( int i = 0; i < pdfs.size(); i++ ){
    for( QCD::PdfBase::iterator itr = pdfs[ i ]->begin();
	 itr != pdfs[ i ]->end();
	 itr++ ){      
      head[ itr->first ] = itr->second.name();
    }
  }
  return head;
}

map< int, string > TableMaker::header( QCD::PdfErrMatrix* em ) const {
  map< int, string > head;
  for( int i = 0; i < em->size(); i++ ){
    head[ i ] = em->info()[ i ];
    this->update_name( head[ i ] );
  }
  return head;
}

ostream& operator<<( ostream& os, 
		     const TableMaker& maker ){
  return os;
}

string TableMaker::suffix(){
  switch( style_ ){
  case TEXT: return ".txt";
  case TEX:  return ".tex";
  case HTML: return ".html";
  }

  return ".dat";
}

void TableMaker::update_name( string& name ) const {
  if( style_ == TEX ){
    int pos;
    while( ( pos = name.find( "_" ) ) != string::npos ){
      name.replace( pos, 1, " " );
    }
  }
}

void TableMaker::print( ostream& os, Exp::DataSet* dset ) const {
  
  double q2 = 0.0;
  for( int i = 0; i < dset->size(); i++ ) {
    double q2val = (*dset)[ i ].kine( "Q^2") ;
    if( q2val == 0.0 ) q2val = (*dset)[ i ].kine( "Q^2 ave") ;
    q2 += q2val / dset->size();
  }
  
  string name = dset->info();
  this->update_name( name );
  
  switch( style_ ){
  case TEXT: 
    
    os << setw( 35 ) << dset->info() << flush;
    os << setw( 10 )  << sqrt( q2 ) << flush;
    os << setw( 5 )  << dset->fittedDataPoint() << flush;
    os << "/" << flush;
    os << setw( 4 )  << dset->size() << flush;
    os << setw( 10 ) << dset->chiSquare() << flush;
    os << setw( 6 )  << dset->norm() << flush;
    os << setw( 10 ) << dset->normalization() << flush;
    if( withError_ && dset->normalizationError() != 0.0 ){
      os << " +- " 
	 << setw( 10 ) << dset->normalizationError() << flush;
    }
    os << flush;
    
    break;
    
  case TEX:
    
    
    os << name
       << "\\cite{"<< dset->cite() << "}"
       << " & " << flush;
    os << sqrt( q2 ) << " & " << flush;
    os << dset->fittedDataPoint() 
       << "/"
       << dset->size() << " & " << flush;
    os << dset->chiSquare() << " & " << flush;
    os << dset->norm() << " & " << flush;
    os << dset->normalization() << flush;
    if( withError_ && dset->normalizationError() != 0.0 ){
      os << " +- " 
	 << dset->normalizationError() << flush;
    }
    os << "\\\\" << flush;
    
    break;
    
  case HTML:
    
    break;
  }
  
  os << endl;
  
}

void TableMaker::print( ostream& os, Exp::ExpData* data ) const {

  vector< Exp::DataSet > dset = data->data();

  // header
  switch( style_ ){
  case TEXT:
    
    os << setw( 35 ) << "NAME"   << flush;
    os << setw( 10 ) << "Q"      << flush;
    os << setw( 10 )  << "#data"  << flush;
    os << setw( 10 ) << "chi^2"  << flush;
    os << setw( 6 )  << "norm"   << flush;
    os << setw( 10 ) << "normal." << flush;
    os << endl;

    break;

  case TEX:  
    os << "\\documentclass{article}" << endl;
    os << "\\begin{document}" << endl;
    os << "\\begin{tabular}{c|rrrrrr}" << endl;
    os << "\\hline \\hline" << endl;

    os << "NAME & ";
    os << "$Q$ (${\\rm GeV}^2$) &";
    os << "No. of data & ";
    os << "$\\chi^2$ & ";
    os << " & Normalization \\\\ ";
    os << "\\hline " << endl;

    break;
  case HTML: break;
  }
  
  for( int i = 0; i < dset.size(); i++ ){
    this->print( os, &( dset[ i ] ) );
  }
  
  // footer
  switch( style_ ){
  case TEXT: 
    
    for( int i = 0; i < 80; i++ ) os << "-" << flush;
    os << endl;
    os << setw( 35 ) << "Total"   << flush;
    os << setw( 10 ) << ""        << flush;
    os << setw( 5 )  << data->fittedDataPoint() << flush;
    os << "/" << flush;
    os << setw( 4 )  << data->dataPoint() << flush;
    os << setw( 10 ) << data->chiSquare()  << flush;
    os << endl;
    os << setw( 35 ) << "Reduced_Chi2" << flush;
    os << setw( 10 ) << ""        << flush;
    os << setw( 5 )  << data->fittedDataPoint() << flush;
    os << setw( 5 )  << "" << flush;
    os << setw( 10 ) << data->chiSquare() / data->fittedDataPoint() << flush;
    os << endl;

    break;

  case TEX:  
    os << "\\hline " << endl;
    os << " Total & & ";
    os << data->fittedDataPoint() << flush;
    os << "/" << flush;
    os << data->dataPoint() << " & ";
    os << data->chiSquare() << " & ";
    os << " & ";
    os << "\\\\ ";
    os << endl;

    os << "\\hline " << endl;
    os << " Reduced Chi2 & & ";
    os << data->fittedDataPoint() << flush;
    os << " & ";
    os << data->chiSquare() / data->fittedDataPoint() << " & ";
    os << " & ";
    os << "\\\\ ";
    os << endl;

    os << "\\hline \\hline" << endl;
    os << "\\end{tabular}" << endl;
    os << "\\end{document}" << endl;
    break;
  case HTML: break;
  }
  
}
