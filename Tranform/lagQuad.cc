#include "LaguerreQuadrature.hh"
#include "LegQuadrature.hh"
#include "GridIntegration.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
using namespace Transform;

class F1 : public RealFunction {
public:
  
  F1( const double& a1 = 1.0, const double& b1 = 1.0 ) : a( a1 ), b( b1 ), xp( 0 ) {}
  virtual ~F1(){}
  
  double operator()( const double& x ){
    xp.push_back( x );
    // return sin(x) / x;
    return pow( x, a ) * exp( - b * x );
  }
  
  double a;
  double b;
  
  vector< double > xp;
  
};

class F2 : public RealFunction {
public:
  
  F2( const double& a = 0.0 ) : alpha( a ){}
  virtual ~F2(){}
  
  double operator()( const double& x ){
    return exp( x ) * pow( x, -alpha ) * (*f)( x );
  }
  
  double alpha;
  RealFunction* f;
  
};

class F3 : public RealFunction {
public:
  
  F3( const double& a = 0.0 ) : alpha( a ){}
  virtual ~F3(){}
  
  double operator()( const double& x ){
    return exp( -x ) * pow( x, alpha ) * (*f)( x );
  }
  
  double alpha;
  RealFunction* f;
  
};

void dump( const string& remark, Integration& integ, RealFunction& func, 
	   F1& f1,
	   const double& upper ){
  f1.xp.resize( 0 );
  cout
    << setw(50) << remark
    << setw(12) << integ.integral( 0.0, upper, func ) << flush;
  
  cout << setw(6)  << f1.xp.size() << endl;
}


int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  cout << endl;

  cout
    << "+-------------------------------------------------------------------+\n" 
    << "+-------------------------------------------------------------------+\n" 
    << "|                    Test of Laguerre Quadrature                    |\n"
    << "+-------------------------------------------------------------------+\n" 
    << "+-------------------------------------------------------------------+\n" 
    << endl;
  
  cout 
    << "Usage: " << argv[0] << " accepts following options\n" 
    << endl;
  
  cout 
    << "\t--a=1.0          Test function: x^{a} exp^{-bx}\n" 
    << "\t--b=1.0          Test function: x^{a} exp^{-bx}\n" << endl;
  double a = args.get( "a", 1.0 );
  double b = args.get( "b", 1.0 );
  F1 f1( a, b );
  
  cout 
    << "\t--n=10           Laguerre Quadrature: L_{n}^{alpha}(x)\n"
    << "\t--alpha=0.0      Laguerre Quadrature: L_{n}^{alpha}(x)\n"
    << endl;
  double alpha = args.get( "alpha", 0.0 );
  int    n     = args.get( "n", 10 );
  LaguerreQuadrature lag( n, alpha );
  
  cout 
    << "\t--upper=100.0    Upper limit of numerical integration\n"
    << endl;
  double upper = args.get( "upper", 100.0 );
  
  cout 
    << "\t--prec=1.0E-3    precision of grid integration\n"
    << endl;
  double prec = args.get( "prec", 1.0E-3 );
  
  //           Preparation of standard grid integration           //
  LegQuadrature leg4( 4 );
  LegQuadrature leg5( 5 );
  GridIntegration grid( &leg4, &leg5, 2, prec );
  
  ostringstream ostr;
  
  cout << endl;

  cout << "  Test function:  f(x) = x ^{" << a << "} * e^{-" << b << "x}\n" << endl;

  {
    F3 f3;
    f3.alpha = alpha;
    f3.f     = &f1;
    
    ostr << "int_" << 0 << "^" << upper << " dx x^{" << alpha << "} e^{-x} f(x)" ;
    dump( ostr.str(), grid, f3, f1, upper );
    
    ostr.str("");
    ostr << "Laguerre Quadrature: L_" << n  << "^" << alpha << "(x)";
    dump( ostr.str(), lag, f1, f1, upper );
  }

  {
    F2 f2;
    f2.alpha = alpha;
    f2.f     = &f1;
    
    ostr.str("");
    ostr << "int _" << 0 << "^" << upper << " dx f(x)";
    dump( ostr.str(), grid, f1, f1, upper );
    
    ostr.str("");
    ostr << "Lagerre Quadrature of x^{" << -alpha << "}e^{x} f(x)" ;
    dump( ostr.str(), lag, f2, f1, upper );
  }

  cout << endl;

  return 0;
}

