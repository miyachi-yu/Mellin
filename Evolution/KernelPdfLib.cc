#include "KernelPdfLib.hh"

#include <PDFLIB/Package.hh>

#include <Tranform/Mellin.hh>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

KernelPdfLib::KernelPdfLib( const string& name ) : 
  KernelBase(),
  name_( name ),
  pdflib_( NULL ), m_( NULL ),
  fkey_( 0 ), fdata_( 0 ), fcache_( 0 )
{
  
  // create Mellin transformation object
  m_ = 
    new Mellin( new GridIntegration( new LegQuadrature( 4 ),
				     new LegQuadrature( 6 ),
				     4, 1.0E-4 ) );
  
  // set Mellin integration range
  // PDF is only available ( 0 < x < 1 ).
  m_->lower( 0.0 );
  m_->upper( 1.0 ); 
  
  // open file base cache DB
  this->openDB();
}

KernelPdfLib::~KernelPdfLib(){
  
  delete dynamic_cast< GridIntegration* >( m_->integration() )->method1();
  delete dynamic_cast< GridIntegration* >( m_->integration() )->method2();
  delete m_->integration();
  delete m_;
  
  this->closeDB();
  
}

/*
  At the moment, 
  file base cache will be initialized for 
  quarks and anti-quarks ( u, d, s, c, b ) and gluon.
  In total 11.
*/
void KernelPdfLib::openDB(){
  
  const int nDB = 11;
  fkey_.resize( nDB, DataCache::Grid() );
  fdata_.resize( nDB, complex< double >( 0.0, 0.0 ) );
  fcache_.resize( nDB, 
		  ( Utility::LookupTable< DataCache::Grid, complex< double > >* ) NULL );
  
  for( int i = 0; i < nDB; i++ ){
    Flavor::PARTON q = static_cast< Flavor::PARTON >( i - 5 );
    string name = name_ + "-" + Flavor::name( q );
    fcache_[ i ] = 
      new Utility::LookupTable< DataCache::Grid, complex< double > >( fkey_[ i ], 
							     fdata_[ i ], 
							     name ); 
  }
}

void KernelPdfLib::closeDB(){
  
  for( int i = 0; i < fcache_.size(); i++ )
    if( fcache_[ i ] ) delete fcache_[ i ];
  
  fkey_.resize( 0 );
  fdata_.resize( 0 );
  fcache_.resize( 0 );
  
}

void KernelPdfLib::pdflib( PDFLIB::Package *p ){
  if( p == NULL || p == pdflib_ ) return;
  m_->transform( pdflib_ = p );
}

void KernelPdfLib::setQ2( const double& q2 ){
  if( pdflib_ == NULL ) return;
  pdflib_->setQ2( q2 );
}

complex< double > KernelPdfLib::q( const Flavor::PARTON& q ){
  
  if( pdflib_ == NULL ) return complex< double >( 0.0, 0.0 );
  
  pdflib_->setPID( q );
  
  int i = 5 + static_cast< int >( q );
  fkey_[ i ] = DataCache::Grid( this->n(), this->Q2() ); 
  
  if( ! fcache_[ i ]->get( fkey_[ i ], fdata_[ i ] ) ){
    fdata_[ i ] = (*m_)( this->n() );
    fcache_[ i ]->put();
  }
  
  /*
  cout 
    << "(" << setw(18) << this->n()
    << ", " << setw(12) << this->Q2()
    << ") = " << setw(18) << fdata_[ i ]
    << endl;
  */

  return fdata_[ i ] ;
}

complex< double > KernelPdfLib::q( const Flavor::TYPE& q ){
  
  if( pdflib_ == NULL ) return complex< double >( 0.0, 0.0 );
  
  pdflib_->setPID( q );
  return (*m_)( this->n() );
}

double KernelPdfLib::Q2(){
  return ( pdflib_ ? pdflib_->q2() : 0.0 );
}

void KernelPdfLib::setN( const std::complex< double >& n ){
  this->updateN( n );
}
