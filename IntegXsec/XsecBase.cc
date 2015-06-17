#include "XsecBase.hh"
#include "XsecIntegrand.hh"
#include "CKernel.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace IntegXsec;

XsecBase::XsecBase() : 
  Xsection::CKernelBase(), XInteg_( NULL ),
  cache_(), cache2_(),
  cdata_( 0 ),
  X_( 0.0 ), Z_( 0.0 ),
  ncount_( 0 ),
  acti_( 0 ), actj_( 0 ) // actwc1(2)_ : created as zero map implicitly
{
}

void XsecBase::setQ2( const double& q2 ){
  XInteg_->setQ2( q2 );
}

double XsecBase::q2(){
  return XInteg_->q2();
}

double XsecBase::nf(){
  return XInteg_->coeff()->nf();
}

ChargeBase* XsecBase::charge(){
  return XInteg_->coeff()->charge();
}

void XsecBase::constNf( const bool& frag ){
  return XInteg_->coeff()->constNf( frag );
}

Alpha& XsecBase::alpha(){
  return XInteg_->coeff()->alpha();
}

vector< vector< RealFunction* > >& XsecBase::vpdfx(){
  return XInteg_->vpdfx();
}

void XsecBase::update(){
  XInteg_->update();
}

void XsecBase::setOrder( const Scheme::ORDER& order ){
  XInteg_->setOrder( order );
}

//Scheme::ORDER& XsecBase::order(){ // obj -> disappear after "}". never ref.
//const Scheme::ORDER& ok??
Scheme::ORDER XsecBase::order(){
  return  XInteg_->order();
}

void XsecBase::setIndiv( const QCD::Flavor::PARTON& q ){
  XInteg_->setIndiv( q );
}

void XsecBase::setIndiv( const bool& status ){
  XInteg_->setIndiv( status );
}

ostream& IntegXsec::operator<<( ostream& os, XsecBase& xs ){
  
  os << "#----------------------------------------------------#" << endl;
  os << " IntegXsec::XsecBase Information " << endl;
  os << "   Q2 = " << setw(6) << xs.q2() << endl;
  
  
  if( xs.xsecInteg() ) os << *(  xs.xsecInteg() ) << endl;
  
  os << "#----------------------------------------------------#" << endl;
  
  return os;
}

