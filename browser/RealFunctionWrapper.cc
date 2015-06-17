//
// RealFunctionWrapper.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Oct 17 15:34:06 2008 Yoshiyuki Miyachi
// Started on  Fri Oct 17 15:34:06 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>

#include <Xsec/Xsec.hh>
#include "RealFunctionWrapper.hh"

using namespace std;

RealFunctionWrapper::RealFunctionWrapper() :
  xsec_( NULL ), x_( 0 ), z_( 0 ), 
  mode_( RealFunctionWrapper::LengthInX )
{
}

RealFunctionWrapper::~RealFunctionWrapper() {
}

void RealFunctionWrapper::Q2( const double& q2 ){
  if( xsec_ ) xsec_->moment()->setQ2( q2 );
}

double RealFunctionWrapper::operator()( const double& x ){
  
  if( xsec_ == NULL )  return 0.0;
  if( x_.size() == 0 ) return 0.0;
  
  vector< double > angle  = xsec_->angle();
  vector< double > length = xsec_->length();
  vector< double > offset = xsec_->offset();
  
  switch( mode_ ){
  case LengthInX: length[ 0 ] = x; xsec_->upper( length ); break;
  case LengthInZ: length[ 1 ] = x; xsec_->upper( length ); break;
  case AngleInX:  angle[ 0 ] = x;  xsec_->setParameter( offset, angle ); break;
  case AngleInZ:  angle[ 1 ] = x;  xsec_->setParameter( offset, angle ); break;
  case OffsetInX: offset[ 0 ] = x; xsec_->setParameter( offset, angle ); break;
  case OffsetInZ: offset[ 1 ] = x; xsec_->setParameter( offset, angle ); break;
  }
  
  /*
  cout << endl;
  cout << "Angle:  " << flush; 
  for( int i = 0; i < angle.size(); i++ ) 
    cout << setw(8) << angle[ i ] << flush;
  cout << endl;

  cout << "Length: " << flush; 
  for( int i = 0; i < length.size(); i++ ) 
    cout << setw(8)<< length[ i ] << flush;
  cout << endl;

  cout << "Offset: " << flush; 
  for( int i = 0; i < offset.size(); i++ )
    cout << setw(8) << offset[ i ] << flush;
  cout << endl;
  */

  return (*xsec_)( x_, z_ );
}


