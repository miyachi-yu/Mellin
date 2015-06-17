#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main( int argc, char* argv[] ){

  double zmin, data, stat, sysp, sysm, frac, efrac;
  
  string hadron = ( argc == 1 ? "pion" : "kaon" );
  
  ifstream ifs1( ( hadron + "_zmin.dat" ).c_str() );
  ifstream ifs2( ( hadron + "_multi.dat" ).c_str() );
  ifstream ifs3( ( hadron + "_stat.dat" ).c_str()  );
  ifstream ifs4( ( hadron + "_sys+.dat" ).c_str()  );
  ifstream ifs5( ( hadron + "_sys-.dat" ).c_str()  );
  ifstream ifs6( ( hadron + "_strong.dat" ).c_str()  );
  ifstream ifs7( (hadron + "_strong_err.dat" ).c_str()  );
  
  
  while( ifs1 >> zmin && ifs2 >> data && ifs3 >> stat &&
	 ifs4 >> sysp && ifs5 >> sysm &&
	 ifs6 >> frac && ifs7 >> efrac ){
    
    cout 
      << setw(8) << zmin
      << setw(8) << zmin + 0.01
      << setw(12) << 10.52
      << setw(12) << data
      << setw(12) << ( sysp > sysm ? sysp : sysm )
      << setw(12) << frac
      << setw(12) << efrac
      << endl;
    
  }

  return 0;
}
