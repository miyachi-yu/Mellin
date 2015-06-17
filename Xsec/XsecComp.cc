#include <Evolution/PDF.hh>

#include "XsecComp.hh"
#include "Xsec.hh"
#include "CKernelBase.hh"
#include "CKernel.hh"

#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace Xsection;

XsecComp::XsecComp() : 
  XsecCompBase(), 
  q2_( 1.0 ),
  rescaleFcn_( 0 ),
  localPdfs_( 0 )
{
}

XsecComp::~XsecComp(){
  for( vector< ComplexFunction* >::iterator itr = localPdfs_.begin();
       itr != localPdfs_.end();
       itr++ ){
    delete *itr;
  }
}

void XsecComp::setQ2( const double& q2 ){
  
  // set Q2
  q2_ = q2;

  // set scales for evolution kernels with rescaling 
  // if rescale funcion is given.
  for( int i = 0; i < vevo_.size(); i++ ) {
    double ratio = 1.0;
    if( ( i < rescaleFcn_.size() ) && 
	( rescaleFcn_[ i ] != NULL ) ) {
      RealFunction& f = *( rescaleFcn_[ i ] );
      ratio = f( q2_ );
    }
    vevo_[ i ]->setQ2( q2_ / ratio );
  }
  // set scale for coefficient functions
  coeff_->setQ2( q2_ );
  this->update();
}

vector< vector< complex< double > > >& XsecComp::vCoeff() {
  return coeff_->vCoeff();
}

vector< vector< complex< double > > >& XsecComp::vCoeff2() {
  return coeff_->vCoeff2();
}

vector< vector< complex< double > > >& XsecComp::vCoeff3() {
  return coeff_->vCoeff3();
}

void XsecComp::coeff( CKernelBase* ck ){
  coeff_ = ck;
  coeff_->dimension( vevo_.size() );
}

void XsecComp::update(){  
}

void XsecComp::setOrder( const Scheme::ORDER& order ){
  for( int i = 0; i < vevo_.size(); i++ ) {
    vevo_[i]->alpha().scheme().order() = order;
  }
}

Scheme::ORDER XsecComp::order() {
  if( vevo_.size() > 0 ){
    return vevo_[ 0 ]->alpha().scheme().order();
  }
  return Scheme::NLO;
}

bool XsecComp::insert( KernelBase* kernel, const Flavor::HADRON& hadron ){
  
  // add new KernelBase pointer at the end of vevo_
  vevo_.push_back( kernel );
  
  // prepare PDF pointer container
  vector< ComplexFunction* > vpdf( 0 );
  
  // add new PDF pointers to the end of the container object
  for( int i = -6; i <= 6; i++ ){
    PDF *pdf = new PDF( kernel, Flavor::PARTON( hadron > 0 ? i : -i ) );
    localPdfs_.push_back( pdf );
    vpdf.push_back( pdf );
  }
  
  // add the container object to vpdf_
  vpdf_.push_back( vpdf );
  
  // set dimension if CKernelBase pointer is already assinged
  if( coeff_ ) coeff_->dimension( vevo_.size() );
  
  // at the moment just return true.
  // It should be implemented to return false in case any failuer
  return true;
}

ostream& Xsection::operator<<( ostream& os, XsecComp& xc ){
  
  os << "Xsecion::XsecComp information" << endl;
  
  for( int i = 0; i < xc.vevo().size(); i++ ){
    os << "Evolution::KernelBase "<< setw(4) << i << endl;
    os << *( xc.vevo()[ i ] ) << endl;
    
    os << "PDF information:" << endl;
    for( int j = 0; j < xc.vpdf()[ i ].size(); j++ ){
      os << *( dynamic_cast< Evolution::PDF* >( xc.vpdf()[ i ][ j ] ) ) 
	 << flush;
    }
    
  }
  
  if( dynamic_cast< Xsection::CKernel* >( xc.coeff() ) ) 
    os << *( dynamic_cast< Xsection::CKernel* >( xc.coeff() ) ) << endl;
  
  return os;
}
