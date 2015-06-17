#include <Utility/Timer.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/Xsec.hh>
#include "Evolution/PDFx.hh"

#include <iostream>
#include <iomanip>//setw

#include "StrFunc.hh"
#include "XsecComp.hh"

class Yoshi : public Xsection::Xsec {
public:
     Yoshi() {};
     virtual ~Yoshi(){};
     
     virtual std::complex< double > 
     operator()( const std::complex< double >& n, 
		 const std::complex< double >& m )
//	  { return 1.0 / ( n + 1.0 ) / ( m + 1.0 ); };
//	  { return 1.0 / ( n ) / ( m ); };
//	  { return 1.0 / ( n ) / ( m + 1.0 ); };
	  { return 1.0 / ( n + 1.0 ) / ( m ); };
     
     virtual std::complex< double > 
     operator()( const std::complex< double >& n )
	  { return 1.0 / ( n + 1.0 ); };
};

using namespace std;

int main() {

     Yoshi yoshi;
//     Xsection::xzXsec yxz( &yoshi, 4, 6, 1.0E-4, 100.0, 0.0, 0.99999);
     Xsection::xzXsec yxz( &yoshi, 4, 6, 1.0E-4, 100.0, 1.0, 0.99999);

//    std::vector< double > mom( 2, 0.0 );
//    mom[ 1 ] = 1.0;
//    yxz.SetMomParam( mom );
//     std::vector< double > xx_( 1, 0.5 ), zz_( 1, 0.5 );//zz_( 2, 0.2 );
//     std::vector< double > xx_( 2, 0.0 ), zz_( 2, 0.0 );//zz_( 2, 0.2 );
//     std::vector< double > xx_( 2, 0.0 ), zz_( 1, 0.0 );//zz_( 2, 0.2 );
     std::vector< double > xx_( 1, 0.0 ), zz_( 2, 0.0 );//zz_( 2, 0.2 );

//     double xmin = - 2.0; double xmax = 2.0;
     double xmin = 0.01; double xmax = 1.01;
     int nx = 100;
     double dlx = ( xmax - xmin ) / nx;

     
     for( int i = 0; i <= nx; i++ ){
	  double x = xmin + i * dlx;
	  xx_[ 0 ] = x; zz_[ 0 ] = x;
//	  cout << x << "   " << yxz( xx_, xx_ ) << endl;
	  cout << x << "   " << yxz( xx_, zz_ ) 
//	       * ( xx_[ 0 ] - xx_[ 1 ] ) 
	       * ( zz_[ 0 ] - zz_[ 1 ] ) 
	       << endl;
//	  cout << yxz( xx_, xx_ ) << endl;
     }

}
