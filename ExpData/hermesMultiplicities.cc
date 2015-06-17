#include "HERMES_multiplicities.hh"
#include "ExpData.hh"

#include <Utility/Arguments.hh>

#include <fstream>

using namespace std;

int main( int argc, char *argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  Exp::DataSet pip_p = Exp::HERMES_multiplicities::multiplicities( Exp::PRO, Exp:: PIp );
  Exp::DataSet pim_p = Exp::HERMES_multiplicities::multiplicities( Exp::PRO, Exp:: PIm );
  Exp::DataSet kap_p = Exp::HERMES_multiplicities::multiplicities( Exp::PRO, Exp:: Kp );
  Exp::DataSet kam_p = Exp::HERMES_multiplicities::multiplicities( Exp::PRO, Exp:: Km );
  
  Exp::DataSet pip_d = Exp::HERMES_multiplicities::multiplicities( Exp::DEU, Exp:: PIp );
  Exp::DataSet pim_d = Exp::HERMES_multiplicities::multiplicities( Exp::DEU, Exp:: PIm );
  Exp::DataSet kap_d = Exp::HERMES_multiplicities::multiplicities( Exp::DEU, Exp:: Kp );
  Exp::DataSet kam_d = Exp::HERMES_multiplicities::multiplicities( Exp::DEU, Exp:: Km );
  
  Exp::ExpData edata;
  edata.data().push_back( pip_p );
  edata.data().push_back( pim_p );
  edata.data().push_back( kap_p );
  edata.data().push_back( kam_p );
  edata.data().push_back( pip_d );
  edata.data().push_back( pim_d );
  edata.data().push_back( kap_d );
  edata.data().push_back( kam_d );
  
  ofstream ofs( args.get( "expdata", "hermes_multiplicities.xml" ).c_str() ); 
  ofs << edata << endl;
  
  return 0;
}
