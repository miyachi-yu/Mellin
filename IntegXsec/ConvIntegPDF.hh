#ifndef _IntegXsec_ConvIntegPDF_hh_
#define _IntegXsec_ConvIntegPDF_hh_

#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <vector>

namespace IntegXsec{
     
  class ConvIntegPDF : public Transform::RealFunction, 
		       public Transform::ComplexFunction {
  public:
    ConvIntegPDF( const std::vector< std::complex< double > >& param ):
      xpdf_( ( Transform::RealFunction* ) NULL ), 
      cpdf_( ( Transform::ComplexFunction* ) NULL ), 
      y_( param ){};
    virtual ~ConvIntegPDF(){};
    
    // assume integ range y_[0] < x < 1.0 //
    virtual double operator()( const double& x ){
      return y_[ 0 ].real() / x * (*xpdf_)( y_[ 0 ].real() / x ); 
      // y / x * f( y / x ) dx
      //      return (*xpdf_)( y_[ 0 ] / x );
    };

    virtual std::complex< double > operator()
      ( const std::complex< double >& n ){
      return 1.0 / n * pow( n.real() / y_[ 0 ].real(), y_[ 1 ] ); 
      // 1 / y * ( y / x )^n dy; 
      /*
      return 1.0 / n * pow( n.real(), y_[ 1 ] ); 
      // 1 / y * ( y )^n dy; ( x^{-n} -> inverseMellin ) 
      */
    };
    
    void setxPDF( Transform::RealFunction* xpdf ){ xpdf_ = xpdf; }
    Transform::RealFunction* getxPDF(){ return xpdf_; }

    void setcPDF( Transform::ComplexFunction* cpdf ){ cpdf_ = cpdf; }
    Transform::ComplexFunction* getcPDF(){ return cpdf_; } 

    std::vector< std::complex< double > > getparam(){ return y_; } 
    
  private:
    Transform::RealFunction* xpdf_;
    Transform::ComplexFunction* cpdf_;
    const std::vector< std::complex< double > >& y_;

  };
};

#endif //  _IntegXsec_ConvIntegPDF_hh_
