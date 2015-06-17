#include "FFDisFavor.hh"

using namespace std;
using namespace QCD;

FFDisFavor::FFDisFavor() :
  PdfBase()
{
}

FFDisFavor::~FFDisFavor(){
}

complex< double > FFDisFavor::operator() ( const complex< double >& z ){
  if( this->reference() == NULL ) return complex< double >( 0.0, 0.0 );
  PdfBase& fav = *( this->reference() );
  return fav( z ) - fav( z + 1.0 );
}

double FFDisFavor::x( const double& x ) {
  return ( this->reference() ? ( 1.0 - x ) * this->reference()->x( x ) : 0.0 );
}

void FFDisFavor::update(){
}

//! get value for the given i-th parameter
double FFDisFavor::getValue( const int& i ){
  if( i == 0 ) { // return 2nd moment
    return (*this)( 2.0 ).real();
  }
  return 
    ( i == 2 ? 1.0 : 0.0 ) +
    ( this->reference() ? this->reference()->getValue( i ) : 0.0 );
}

ostream& operator<<( ostream& os, FFDisFavor& ff ){
  
  os << "<pdf name=\"" << ff.name() << "\""<< flush;
  if( ff.pid() != "" )  os << " pid=\""  << ff.pid() << "\"" << flush;
  os << " dfav=\"" << ff.dfav() << "\"" << flush;
  os  << ">" << flush; 
  
  os << "<!--  This is a dis-favored fragmentation function:"
     << " N = " << ff.getValue( 0 ) 
     << ", a = " << ff.getValue( 1 ) 
     << ", b = " << ff.getValue( 2 ) 
     << " -->" << flush;

  os << "</pdf>" << flush;
}
