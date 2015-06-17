#include <iostream>
#include <iomanip>
#include <cmath>

#include <Utility/Arguments.hh>
#include <Tranform/CBeta.hh>

using namespace std;

/*
  Moment calculator.....
 */
int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  double a = args.get( "a", 1.0 );
  double b = args.get( "b", 1.0 );
  double c = args.get( "c", 1.0 );
  double d = args.get( "d", 1.0 );
  complex< double > x = args.get( "x", complex< double >( 1.0, 0.0 ) );
  double A = args.get( "A", 1.0 );

  Transform::CBeta b1( a - 1.0, b );
  Transform::CBeta b2( a - 0.5, b );
  Transform::CBeta b3( a,       b );
  
  cout << "xf(x) = x^a (1-x)b (1+c*sqrt(x)+d*x)" << endl;
  cout << "a = " << setw(14) << a << endl;
  cout << "b = " << setw(14) << b << endl;
  cout << "c = " << setw(14) << c << endl;
  cout << "d = " << setw(14) << d << endl;
  
  complex< double > mom( 0.0, 0.0 );
  cout << "f" << x << " = " 
       << ( mom = b1(x) + c * b2(x) + d * b3(x) ) << endl;
  
  cout << "eta = "
       << setw(14) << A 
       << " * " 
       << setw(14) << mom.real()
       << " = "
       << setw(14) << A * mom.real() << endl;

  return 0;
}
