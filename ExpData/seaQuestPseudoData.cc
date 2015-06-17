#include "ExpData.hh"
#include "DataSet.hh"
#include "DataPoint.hh"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Plot: SeaQuestPseudo_d4x4y3
// ------------------------------------------------------------------- //
namespace SeaQuestPseudo {
  
  int numpoints = 11;
  
  double x2[] = {
    0.1168, 0.1504, 0.1840, 0.2160, 0.2496, 
    0.2832, 0.3168, 0.3504, 0.3840, 0.4176, 0.4504
  };
  
  double M[] = { 
    4.2, 4.7, 5.0, 5.3, 5.6, 
    5.7, 5.9, 6.0, 6.0, 6.0, 6.0 };
  
  // ------------------------------------------------------------------- //
  namespace dbar_ubar_ratio {
    
    double yval[] = 
      { 1.65855,  1.59112,  1.68452,  1.48792,  1.32466, 
	0.980175, 0.640127, 0.495886, 0.336266, 0.14156, 0.584586 };
    
    double ystat[] = 
      { 0.15,   0.0873, 0.0791, 0.0764, 0.0819, 
	0.0846,	0.0928, 0.1037, 0.131,  0.1801, 0.322 };
    
  };
  
};

int main(){
  
  Exp::DataSet data;
  data.info() = "E906,2015,xsec";
  data.ref()  = "Pseudo-Data";
  data.cite() = "E906Expectation";
  
  for( int i = 0; i < SeaQuestPseudo::numpoints; i++ ){
    Exp::DataPoint point;
    point.info() = "dbar/ubar";
    point.unit() = "";
    point.kine( SeaQuestPseudo::x2[ i ], "x",  "" );      // x2
    point.kine( SeaQuestPseudo::M[ i ],  "M",  "GeV" ); // invariant mass
    point.data() = SeaQuestPseudo::dbar_ubar_ratio::yval[ i ];
    point.stat() = SeaQuestPseudo::dbar_ubar_ratio::ystat[ i ];
    data.push_back( point );
  }

  Exp::ExpData expdata;
  expdata.data().push_back( data );

  ofstream ofs( "E906_Pseudo_pdf_ratio.xml" );
  ofs << expdata << endl;
  
  return 0;
}
