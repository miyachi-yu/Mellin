#include <iostream>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
     Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
     if( args.hasOpt( "expdata" ) ) { 
	  
	  Exp::ExpData ed_  = Exp::ExpData( args );
	  for( int i = 0; i < ed_.data().size(); i++){
	       for( int j = 0; j < ed_.data()[i].size(); j++){
		    Exp::DataPoint pt_ = ed_.data()[i][j];
		    cout << pt_.data() << " " << pt_.kine("zmin")<< endl;
	       }	       
	  }

	  
     }
     return 0;
}
