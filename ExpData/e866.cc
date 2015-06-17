#include "ExpData.hh"
#include "DataSet.hh"
#include "DataPoint.hh"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Plot: p5109_d4x4y3
// ------------------------------------------------------------------- //
namespace p5109 {
  
  int numpoints = 15;
  
  double x2[] =
    { 0.026, 0.038, 0.052, 0.067, 0.082, 0.097, 0.112, 0.127, 0.142, 
      0.161, 0.186, 0.211, 0.236, 0.269, 0.315 };
  
  double xF[] = 
    { 0.534, 0.415, 0.356, 0.326, 0.296, 0.261, 0.227, 0.199, 0.182, 
      0.164, 0.146, 0.133, 0.12, 0.097, 0.046 };
  
  double pT[] =
    { 1.004, 1.045, 1.076, 1.103, 1.122, 1.141, 1.156, 1.168, 1.161, 
      1.156, 1.146, 1.146, 1.178, 1.177, 1.078 };
  
  double M[] = { 4.6, 5.1, 5.6, 6.2, 6.8, 7.2, 7.5, 7.8, 8.2, 
		 8.7, 9.5, 10.3, 11.1, 12.0, 12.9 };
  
  namespace xsec_ratio { // x vs cross section ratio
    
    double yval[] = 
      { 1.038, 1.056, 1.081, 1.086, 1.118, 1.116, 1.115, 1.161, 1.132, 
	1.124, 1.144, 1.091, 1.039, 0.935, 0.729 };
    
    double ystat[] = 
      { 0.022, 0.011, 0.01, 0.011, 0.013, 0.015, 0.018, 0.023, 0.027, 
	0.027, 0.038, 0.047, 0.063, 0.067, 0.124 };
    
  };
  
  // ------------------------------------------------------------------- //
  namespace dbar_ubar_ratio {

    double yval[] = 
      { 1.085, 1.14, 1.215, 1.249, 1.355, 1.385, 1.419, 1.63, 1.625, 
	1.585, 1.709, 1.56, 1.419, 1.082, 0.346 };
    
    double ystat[] = 
      { 0.05, 0.027, 0.026, 0.028, 0.036, 0.046, 0.06, 0.085, 0.11, 
	0.111, 0.158, 0.194, 0.264, 0.256, 0.395 };

    double ysyst[] = 
      { 0.017, 0.018, 0.020, 0.021, 0.023, 0.025, 0.27, 0.031, 0.33, 
	0.032, 0.036, 0.034, 0.036, 0.032, 0.022 };
    
  };
  
};

int main(){

  Exp::DataSet data;
  data.info() = "E866,2001,xsec";
  data.ref()  = "Phys.Rev.D64(2001)052002";
  data.cite() = "Towell:2001nh";
  
  for( int i = 0; i < p5109::numpoints; i++ ){
    Exp::DataPoint point;
    point.info() = "dbar/ubar";
    point.unit() = "";
    point.kine( p5109::x2[ i ], "x",  "" );      // x2
    point.kine( p5109::xF[ i ], "xf", "" );      // x-feynman
    point.kine( p5109::pT[ i ], "pT", "GeV" );   // transverse momentum
    point.kine( p5109::M[ i ],  "M",  "GeV" ); // invariant mass
    point.data() = p5109::dbar_ubar_ratio::yval[ i ];
    point.stat() = p5109::dbar_ubar_ratio::ystat[ i ];
    point.syst( p5109::dbar_ubar_ratio::ysyst[ i ], "all" );
    data.push_back( point );
  }

  Exp::ExpData expdata;
  expdata.data().push_back( data );

  ofstream ofs( "E866_PRD64_052002_pdf_ratio.xml" );
  ofs << expdata << endl;
  
  return 0;
}
