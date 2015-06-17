#include "ExpData.hh"
#include "DataPoint.hh"
#include "DataSet.hh"
#include "HERMES_MultiQ2.hh"
#include "HERMES_MultiZ.hh"

#include <Utility/Arguments.hh>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace Exp;

int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  try {

    if( args->hasOpt( "Q2bin" ) ){

      DataSet MultiQ2_p_pip = 
	HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), Exp::HADRON( 30 ) );
      ofstream ofs( "MultiQ2_p_pip.xml" );
      ofs << ExpData::xmlHeader() << endl;
      ofs << "<expdata version=\"1.0\" >" << endl;
      ofs << MultiQ2_p_pip << endl;
      ofs << "</expdata>" << endl;      

      DataSet MultiQ2_p_pim =
	HERMES_MultiQ2::Multi( Exp::TARGET( 1 ), Exp::HADRON( 31  ) );
      ofstream ofs2( "MultiQ2_p_pim.xml" );
      ofs2 << ExpData::xmlHeader() << endl;
      ofs2 << "<expdata version=\"1.0\" >" << endl;
      ofs2 << MultiQ2_p_pim << endl;
      ofs2 << "</expdata>" << endl;
	
      DataSet MultiQ2_d_pip = 
	HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), Exp::HADRON( 30 ) );
      ofstream ofs3( "MultiQ2_d_pip.xml" );
      ofs3 << ExpData::xmlHeader() << endl;
      ofs3 << "<expdata version=\"1.0\" >" << endl;
      ofs3 << MultiQ2_d_pip << endl;
      ofs3 << "</expdata>" << endl;
      
      DataSet MultiQ2_d_pim =
	HERMES_MultiQ2::Multi( Exp::TARGET( 3 ), Exp::HADRON( 31 ) );
      ofstream ofs4( "MultiQ2_d_pim.xml" );
      ofs4 << ExpData::xmlHeader() << endl;
      ofs4 << "<expdata version=\"1.0\" >" << endl;
      ofs4 << MultiQ2_d_pim << endl;
      ofs4 << "</expdata>" << endl;      
      
    } else if ( args->hasOpt( "Zbin" ) ){

      DataSet MultiZ_p_pip = 
	HERMES_MultiZ::Multi( Exp::TARGET( 1 ), Exp::HADRON( 30 ) );
      ofstream ofs( "MultiZ_p_pip.xml" );
      ofs << ExpData::xmlHeader() << endl;
      ofs << "<expdata version=\"1.0\" >" << endl;
      ofs << MultiZ_p_pip << endl;
      ofs << "</expdata>" << endl;

      DataSet MultiZ_p_pim =
	HERMES_MultiZ::Multi( Exp::TARGET( 1 ), Exp::HADRON( 31  ) );
      ofstream ofs2( "MultiZ_p_pim.xml" );
      ofs2 << ExpData::xmlHeader() << endl;
      ofs2 << "<expdata version=\"1.0\" >" << endl;
      ofs2 << MultiZ_p_pim << endl;
      ofs2 << "</expdata>" << endl;

      ofstream ofs_fit( "MultiZ_p_pipm.xml" );
      ofs_fit << ExpData::xmlHeader() << endl;
      ofs_fit << "<expdata version=\"1.0\" >" << endl;
      ofs_fit << MultiZ_p_pip << endl;
      ofs_fit << MultiZ_p_pim << endl;
      ofs_fit << "</expdata>" << endl;

      //      /*
      DataSet MultiZ_d_pip = 
	HERMES_MultiZ::Multi( Exp::TARGET( 3 ), Exp::HADRON( 30 ) );
      ofstream ofs3( "MultiZ_d_pip.xml" );
      ofs3 << ExpData::xmlHeader() << endl;
      ofs3 << "<expdata version=\"1.0\" >" << endl;
      ofs3 << MultiZ_d_pip << endl;
      ofs3 << "</expdata>" << endl;

      DataSet MultiZ_d_pim =
	HERMES_MultiZ::Multi( Exp::TARGET( 3 ), Exp::HADRON( 31 ) );      
      ofstream ofs4( "MultiZ_d_pim.xml" );
      ofs4 << ExpData::xmlHeader() << endl;
      ofs4 << "<expdata version=\"1.0\" >" << endl;
      ofs4 << MultiZ_d_pim << endl;
      ofs4 << "</expdata>" << endl;
      //      */      

      ofstream ofs_fitf( "MultiZ_pd_pipm.xml" );
      ofs_fitf << ExpData::xmlHeader() << endl;
      ofs_fitf << "<expdata version=\"1.0\" >" << endl;
      ofs_fitf << MultiZ_p_pip << endl;
      ofs_fitf << MultiZ_p_pim << endl;
      ofs_fitf << MultiZ_d_pip << endl;
      ofs_fitf << MultiZ_d_pim << endl;
      ofs_fitf << "</expdata>" << endl;

    }

    
  }
  
  catch( int err ){
  }

  return 0;
}
