#include "PdfCteq6.hh"
#include <cmath>

#include <Tranform/GridIntegration.hh>
#include <Tranform/MCInt.hh>

using namespace std;
using namespace PDFSET;

double PdfCteq6::Form::operator()( const double& x ){
  if( x >= 1.0 || x <= 0.0 ) return 0.0;
  double v( a );
  
  if( ratio_ ){
    v *= pow( x, b );
    v *= pow( 1.0 - x, c );
    v += ( 1.0 + d * x ) * pow( 1.0 - x, e );
    return v;
  }
  
  if( v == 0.0 ) return v;
  v *= pow( x, b - 1.0 );
  v *= pow( 1.0 - x, c );
  v *= exp( d * x );
  v *= pow( 1.0 + exp( e ) * x, f);
  return v;
}

PdfCteq6::PdfCteq6() :
  QCD::PdfBase(), Transform::Mellin( NULL ), f_()
{
  Transform::MCInt *mc1 = new Transform::MCInt( 10 );
  Transform::MCInt *mc2 = new Transform::MCInt( 15 );
  this->integration( new Transform::GridIntegration( mc1, mc2, 4, 1.0E-4 ) );
  this->transform( &f_ );
  this->upper( 1.0 );
  this->lower( 0.0 );
}

PdfCteq6::~PdfCteq6(){
  if( this->integration() ) {
    Transform::GridIntegration* grid = 
      dynamic_cast< Transform::GridIntegration* >( this->integration() );
    if( grid ) {
      delete grid->method1();
      delete grid->method2();
    }
    delete this->integration();
  }
}

double PdfCteq6::x( const double& x ){
  return f_( x );
}

void PdfCteq6::update(){
  f_.ratio_ = ( dynamic_cast< QCD::PdfBase* >( this )->name() == "duRatio" ); 
  f_.a = this->getValue( 0 );
  f_.b = this->getValue( 1 );
  f_.c = this->getValue( 2 );
  f_.d = this->getValue( 3 );
  f_.e = this->getValue( 4 );
  if( ! f_.ratio_ ) f_.f = this->getValue( 5 );
}

