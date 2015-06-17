#include "RealFunction.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Transform;

RealFunction::RealFunction() : 
  precision_( 1.0E-8 ){
}

RealFunction::~RealFunction(){
}

double RealFunction::differential( const double& x, 
				   const int& n,
				   const double& dx ){
  
  if( dx == 0.0 || n < 0 ) return 0.0;
  double h( 0.5 * dx );
  return
    ( n == 0 ? 
      (*this)( x ) :
      ( this->differential( x + h, n - 1, dx ) - 
	this->differential( x - h, n - 1, dx )    ) / dx );
  
}

double RealFunction::minimum( const double& x, const double& dx ){


  /*
    cout 
    << "Try to find minimum: " 
    << setw(12) << x 
    << " -> "
    << setw(12) << dx
    << endl;
  */
  if( dx < 1.0E-6 ) return (*this)( x );
  
  double df1 = this->differential( x, 1, dx );
  double df2 = this->differential( x, 2, dx );
  //  cout << setw(12) << df1 << setw(12) << df2 << endl;
  
  if( fabs( df1 ) < precision_ && df2 > 0.0 ) {
    return x; 
  }
  
  double step = ( df1 < 0 ? dx : -dx );
  double x1 = x;
  double fx;
  while( ( fx = (*this)( x1 ) ) > (*this)( x1 + step ) ){
    /*
    cout 
    << "\t"
      << setw(12) << x1 
      << setw(12) << fx
      << setw(12) << x1 + step
      << setw(12) <<  (*this)( x1 + step )
      << endl;
    */
    x1 += step;
  }
  
  //  cout << endl;
  
  double x2;
  return ( x1 != ( x2 = this->minimum( x1 + step, - 0.2 * dx ) ) ? 
	   x1 : this->minimum( x2, 0.1 * dx ) );
  
  /*
    
    cout << setw(12) << df1 << flush;
    cout << setw(12) << df2 << flush;
  cout << setw(12) << ( df2 != 0.0 ? - df1 / df2 : 0.0 ) << flush;
  
  
  double next = x + ( df1 < 0.0 ? 1.0 : -1.0 ) * dx  ;
  if( df2 > 0.0 ) 
  if( fabs( - df1 / df2 - x ) < fabs( dx ) ) next = - df1 / df2;
  
  /*  
      if( df2 > 0.0 ) {
    double r = df1 / df2;
    next = - r;
    double d = (*this)( x ) - 0.5 * df2 * pow( x + r, 2 ) - (*this)( next );
    if( fabs( d ) > precision_ ) next = x + 0.5 * ( next - x );
  } else {
  next = x + ( df1 < 0.0 ? 1.0 : -1.0 ) * dx  ; 
  }
  cout << setw(12) << next << endl;
  return this->minimum( next, dx );
  */
}


#define MOV3(a,b,c,d,e,f)  (a)=(d);(b)=(e);(c)=(f);

double RealFunction::minimum( const double& x,  const double& z, 
			      const double& dx, const double& dz  ){
  
  double dfx = this->differential( x, z, 1, 0, dx, dz );
  double dfz = this->differential( x, z, 0, 1, dx, dz );


  return 0.0;
}

double RealFunction::differential( const double& x, 
				   const double& z, 
				   const int& nx,
				   const int& nz,
				   const double& dx,
				   const double& dz ){
  
  if( dx == 0.0 || nx < 0 ) return 0.0;
  if( dz == 0.0 || nz < 0 ) return 0.0;
  
  if( nx == 0 && nz == 0 ) return ( *this )( x, z );
  
  if( nx == 0 ){
    double hz( 0.5 * dz );
    return 
      ( this->differential( x, z + hz, 0, nz - 1, dx, dz ) -
	this->differential( x, z - hz, 0, nz - 1, dx, dz ) ) / dz;
  }
  
  double hx( 0.5 * dx );
  return
    ( this->differential( x + hx, z, nx - 1, nz, dx, dz ) - 
      this->differential( x - hx, z, nx - 1, nz, dx, dz ) ) / dx ;
  
}

double RealFunction::minimum( const vector< double >& xv,
			      const double& tol ){
  
  if( xv.size() != 3 ) {
    cerr << __FILE__ << ":" << __LINE__ << "\t"
	 << "range must has 3 elements" << endl;
    return 0.0;
  }
  
  // some constant values
  static const int max_iteration = 1000;
  static const double zeps       = 1.0E-10;
  
  // set lower and upper bound correctly
  double lower = ( xv[0] < xv[2] ? xv[0] : xv[1] );
  double upper = ( xv[0] > xv[2] ? xv[0] : xv[1] );
  
  // initialization with the middle point xv[1]
  double w  = xv[1],                   v  = w,  x  = w;
  double fw = (*this)( x ),            fv = fw, fx = fw;
  double dw = this->differential( x ), dv = dw, dx = dw;
  
  double u( 0.0 ), fu( 0.0 ), du( 0.0 );
  
  double e( 0.0 ), olde( 0.0 ), d( 0.0 );
  
  for( int iter = 0; iter < max_iteration; iter++ ){
    
    double middle = 0.5 * ( lower + upper );
    double tol1   = tol * fabs( x ) + zeps;
    double tol2   = 2.0 * tol1;
    
    if( fabs( x - middle ) <= tol2 - 0.5 * ( upper - lower ) ) return x;
    
    if( fabs( e ) > tol ){
      
      // set d1 and d2 to outside of the interested range.....
      double d1 = 2.0 * ( upper - lower );
      double d2 = d1;
      
      // apply secant method for both points
      if( dw != dx ) d1 = dx * ( w - x ) / ( dx - dw );
      if( dv != dx ) d2 = dx * ( v - x ) / ( dx - dv );
      double u1 = x + d1;
      double u2 = x + d2;
      
      // if these new point are inside the interested range
      bool ok1 = ( lower - u1 ) * ( u1 - upper ) > 0.0 && dx * d1 <= 0.0 ;
      bool ok2 = ( lower - u2 ) * ( u2 - upper ) > 0.0 && dx * d2 <= 0.0 ;
      
      // keep previously calculated value.....
      olde = e;
      e = d;
      
      if( ok1 || ok2 ){
	
	d = ( ok1 && ok2 ?
	      ( fabs( d1 ) < fabs( d2 ) ? d1 : d2 ) :
	      ( ok1 ? d1 : d2 ) );
	
	if( fabs( d ) <= fabs( 0.5 * olde ) ){
	  u = x + d;
	  if( u - lower < tol2 || upper - u < tol2 ){ 
	    d = ( middle - x > 0.0 ? 1.0 : - 1.0 ) * fabs( tol1 );
	  }
	} else {
	  d = 0.5 * ( e = ( dx >= 0.0 ? lower - x : upper - x ) );
	}
      } else {
	d = 0.5 * ( e = ( dx >= 0.0 ? lower - x : upper - x ) );
      }
      
      
      if( fabs( d ) >= tol1 ){
	u =  x + d;
	fu = (*this)( u );
      } else {
	u = x + ( d > 0.0 ? fabs( tol1 ) : - fabs( tol1 ) );
	fu = (*this)( u );
	if( fu > fx ) return x;
      }
      
      du = this->differential( u );
      if( fu <= fx ){
	if( u >= x ) lower = x; else upper = x;
	MOV3(v,fv,dv,w,fw,dw);
	MOV3(w,fw,dw,x,fx,dx);
	MOV3(x,fx,dx,u,fu,du);
      } else {
	if( u < x ) lower = u; else upper = u;
	if( fu <= fw || w == x ){
	  MOV3(v,fv,dv,w,fw,dw);
	  MOV3(w,fw,dw,u,fu,du);
	} else if( fu < fv || v == x || v == w ){
	  MOV3(v,fv,dv,u,fu,du);
	}
      }
      
    }
    
  }


  cerr << __FILE__ << ":" << __LINE__ << "\t"
       << "reach the maximum iteration " << endl;
  
  return 0.0;
}
