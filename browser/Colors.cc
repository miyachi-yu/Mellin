#include "Colors.hh"

#include <Rtypes.h>

using namespace QCD;

namespace Colors {
  
  int id( const QCD::Flavor::PARTON& p ){
    
    switch( p ){
      
    case Flavor::g: return kGreen+2;

    case Flavor::u: return kRed;
    case Flavor::d: return kMagenta;
    case Flavor::s: return kBlue;
    case Flavor::c: return kOrange+2;
    case Flavor::b: return kAzure+10;
      
    case Flavor::ubar: return kRed-10;
    case Flavor::dbar: return kMagenta-10;      
    case Flavor::sbar: return kBlue-10;
    case Flavor::cbar: return kOrange+1;
    case Flavor::bbar: return kAzure+1;
      
    case Flavor::uv: return kRed-1;
    case Flavor::dv: return kMagenta-1;
    case Flavor::sv: return kBlue-1;
    case Flavor::cv: return kOrange+3;
    case Flavor::bv: return kAzure+3;
      
    case Flavor::uns: return 46;
    case Flavor::dns: return 36;
    case Flavor::sns: return 49;
    case Flavor::cns: return 32;
    case Flavor::bns: return 43;
      
    case Flavor::U: return 196;
    case Flavor::D: return 186;
    case Flavor::S: return 199;
    case Flavor::C: return 182;
    case Flavor::B: return 193;
      
    case Flavor::T:
    case Flavor::tv:
    case Flavor::tbar:
    case Flavor::tns:
    case Flavor::t: return 1;
      
    }
    return kBlack;
  }
  
  int id( const QCD::Flavor::TYPE& p ){
    switch( p ){
    }
    
    return kBlack;
  }
}
