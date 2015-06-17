/*!
  \file    InverseMellinTest.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/19 09:43:59 $
  \author  $Author: miyachi $
  \brief   test program for libTransform.so

  This is an example for libTransform.so library.

  One must set enviroment variable LD_LIBRARY_PATH to include the
  directory which contains libTranform.so.

 */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <complex>

using namespace std;

#include "Tranform/RealFunction.hh"
#include "Tranfrom/ComplexFunction.hh"

/*!
  \class Fhat
  \brief g(z) = PI/sin(PI*z) 

  This is a test ComplexFunction chaild class.  The function
  g(z) = PI/sin(PI*z)
  is Mellin inversed to 1/(1+x).

 */
class Fhat : public ComplexFunction
{
	public:

  		virtual complex< double > operator()( complex< double > z );
  		virtual complex< double > operator() ( double x )
  			{ return (*this)( complex< double >( x, 0 ) ); }
  
	private:
};

complex< double > Fhat::operator()( complex< double > z )
	{ return M_PI/sin(M_PI*z); }


#include "Tranform/Mellin.hh"
#include "Tranfrom/InverseMellin.hh"
#include "Tranfrom/CBeta.hh"
#include "Tranfrom/Trapezoid.hh"
#include "Tranfrom/Rectangle.hh"
#include "Tranform/LegQuadrature.hh"
#include "Tranform/GridIntegration.hh"

int main( int argc, char* argv[] ){
 
  Fhat g;
  
  LegQuadrature meth1(4);  // Gauss Legendre with 4 inter points
  LegQuadrature meth2(6);  // Gauss Legendre with 6 inter points
  
  GridIntegration grid( &meth1, &meth2, 4, 0.001 );
  
  InverseMellin IM( &grid );
  IM.setParameter(0.1,0.49*M_PI);
  IM.upper(10.);
  
  RealFunction& f = IM.transform( &g ); 
 
	double A,B,rel,abs;
	double h=pow(10,0.1);

  std::cout 
    << std::setw(12) << "x\t"
    << std::setw(12) << "f(x)\t"
    << std::setw(12) << "M-1{g(z)} \t"
    << std::setw(12) << "rel.err \t"
    << std::setw(12) << "abs.err"
    << std::endl;

	for(double x=1E-5;x<1.;x*=h){

		A=1./(1.+x);
		B=f(x);
		rel=1-fabs(B/A);
		abs=fabs(B-A);

    std::cout 
      << std::setw(12) << x << "\t"
      << std::setw(12) << A << "\t"
      << std::setw(12) << B << "\t"
      << std::setw(12) << rel << "\t"
      << std::setw(12) << abs
      << std::endl;

	}
  
  return 0;
}
