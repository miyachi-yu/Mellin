//
// HadronParam.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Sun Oct 26 13:57:18 2008 Yoshiyuki Miyachi
// Started on  Sun Oct 26 13:57:18 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "HadronParam.hh"

#include <Utility/Arguments.hh>

using namespace std;
using namespace QCD;

HadronParam::HadronParam( PdfParametrization* pion,
			  PdfParametrization* kaon,
			  PdfParametrization* rest ) :
  PdfParametrization(),
  pion_( pion ), kaon_( kaon ), rest_( rest ) {
  
  pdfs_.insert( pdfs_.end(), pion_->pdfs().begin(), pion_->pdfs().end() );
  pdfs_.insert( pdfs_.end(), kaon_->pdfs().begin(), kaon_->pdfs().end() );
  pdfs_.insert( pdfs_.end(), rest_->pdfs().begin(), rest_->pdfs().end() );
  
}

HadronParam::~HadronParam(){
}

bool HadronParam::check(){
  
  if( pion_ == NULL || ! pion_->check() ){
    ARG_ERR << "check pion fragmentation function" << endl;
    return false;
  }

  if( kaon_ == NULL || ! kaon_->check() ){
    ARG_ERR << "check kaon fragmentation function" << endl;
    return false;
  }

  if( rest_ == NULL || ! rest_->check() ){
    ARG_ERR << "check rest fragmentation function" << endl;
    return false;
  }

  return true;
}

complex< double > HadronParam::q( const Flavor::PARTON& q ){
  return pion_->q( q ) + kaon_->q( q ) + rest_->q( q );
}

complex< double > HadronParam::q( const Flavor::TYPE& q ){
  return pion_->q( q ) + kaon_->q( q ) + rest_->q( q );
}


string HadronParam::path(){
  return 
    "pion:" + pion_->path() + 
    ", kaon:" + kaon_->path() +
    ", rest:" + rest_->path();
}
