#ifndef __UDratio_hh__
#define __UDratio_hh__

#include "Tranform/RealFunction.hh"
#include "unpolPDF/Evolution.hh"
#include "Evolution/PDFx.hh"

class UDratio : public Transform::RealFunction {
public: 
  
  UDratio( unpolPDF::Evo *pdf ) : 
    RealFunction(), pdf_( pdf ), ubar_( NULL ), dbar_( NULL )
  {
  }
  
  virtual ~UDratio() {
  }
  
  //! calculate f(x) at the given x
  virtual double operator() ( const double& x ) {
    return ( ubar_ && dbar_ ? (*dbar_)( x )/(*ubar_)(x) : 1.0 );
  }
  
  void ubar( Evolution::PDFx *pdfx ) { ubar_ = pdfx; }
  void dbar( Evolution::PDFx *pdfx ) { dbar_ = pdfx; }

  Evolution::PDFx* ubar(){ return ubar_; }
  Evolution::PDFx* dbar(){ return dbar_; }

  void pdf( unpolPDF::Evo *p ) { pdf_ = p; }
  unpolPDF::Evo* pdf() { return pdf_; }
  
  void setQ2( const double& q2 ) { pdf_->setQ2( q2 ); }
  
private:
  
  unpolPDF::Evo *pdf_;
  Evolution::PDFx* ubar_;
  Evolution::PDFx* dbar_;

};

#endif // __UDratio_hh__
