#include "ConfigDataSet.hh"
#include "DataSet.hh"

#include <cstdlib>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Exp;
using namespace Utility;

ConfigDataSet::ConfigDataSet() : 
  Config(), version_( "not-initiated" ), lambda_( 1.0 )
{
}

ConfigDataSet::ConfigDataSet( Arguments& args, const std::string& option ) : 
  Config( args, option ), version_( "" ), lambda_( 1.0 )
{
  this->loadRootNodeInfo();
}

ConfigDataSet::ConfigDataSet( const string& path ) : 
  Config( path ), version_( "" ), lambda_( 1.0 )
{
  this->loadRootNodeInfo();
}

ConfigDataSet::~ConfigDataSet(){
}

void ConfigDataSet::loadRootNodeInfo(){
  xmlpp::Element *expdata = this->rootNode();
  xmlpp::Element::AttributeList alist = expdata->get_attributes();
  for( xmlpp::Element::AttributeList::iterator itr = alist.begin();
       itr != alist.end(); itr++ ){
    if( (*itr)->get_name() == "version" )
      version_ = (*itr)->get_value().data();
    if( (*itr)->get_name() == "lambda" )
      lambda_ = atof( (*itr)->get_value().data() );
  }
}

DataSet ConfigDataSet::dataSet( xmlpp::Element* elm ){
  
  DataSet set;
  
  if( xmlpp::Attribute* info = elm->get_attribute("info") ){
    set.info() = info->get_value();
  }
  
  if( xmlpp::Attribute* info = elm->get_attribute("stat") ){
    string val = info->get_value();
    set.enable() = ( val == "enable" );
  }
  
  if( xmlpp::Attribute* cite = elm->get_attribute("cite") ){
    set.cite() = cite->get_value();
  }
  
  if( xmlpp::Attribute* ref = elm->get_attribute("ref") ){
    set.ref() = ref->get_value();
  }
  
  if( xmlpp::Attribute* norm = elm->get_attribute("norm") ){
    set.norm() = atof( norm->get_value().data() );
  }
  
  if( xmlpp::Attribute* ncoef = elm->get_attribute("ncoef") ){
    set.ncoef() = atof( ncoef->get_value().data() );
  }
  
  if( xmlpp::Attribute* ncerr = elm->get_attribute("ncerr") ){
    set.ncerr() = atof( ncerr->get_value().data() );
  }
  
  if( xmlpp::Attribute* nstat = elm->get_attribute("nstat") ){
    set.ncstat() = nstat->get_value();
  }
  
  // error matrix ID
  if( elm->get_attribute( "emid" ) ) {
    
    string emid = elm->get_attribute( "emid" )->get_value();
    int pos = emid.find( "-" );
    if( pos == string::npos ){
      set.emNumber() = 0;
      set.emid() = atoi( emid.c_str() );
    } else {
      set.emNumber() = atoi( emid.substr( 0, pos - 1 ).c_str() );
      set.emid()     = atoi( emid.substr( pos + 1 ).c_str() );
    }
    
  } else {
    set.emNumber() = -1;
    set.emid() = -1;
  }
  
  // get error matrix elements
  xmlpp::Node::NodeList emslist = elm->get_children( "ems" );
  set.ems().resize( emslist.size() );
  
  for( xmlpp::Node::NodeList::iterator jtr = emslist.begin();
       jtr != emslist.end(); jtr++ ){
    xmlpp::Element* emsElm = dynamic_cast< xmlpp::Element* >( *jtr );
    if( emsElm ){
      int emid = 
	atoi( emsElm->get_attribute( "emid" )->get_value().data()  );
      
      set.ems()[ emid ] = 
	atof( emsElm->get_attribute( "value" )->get_value().data()  );
    }
  }
  
  // get data point object
  xmlpp::Node::NodeList points = elm->get_children( "point" );
  for( xmlpp::Node::NodeList::iterator itr = points.begin();
       itr != points.end(); itr++ ){
    
    if( xmlpp::Element* poElm = 
	dynamic_cast< xmlpp::Element* >( *itr ) ){
      
      DataPoint dp;
      
      if( poElm->get_attribute( "stat" ) ){
	string val = poElm->get_attribute( "stat" )->get_value().data();
	if( val == "fitted" ) dp.fitted() = true;
      }
      
      // error matrix ID handling
      if( poElm->get_attribute( "emid" ) ) {
	
	string emid = poElm->get_attribute( "emid" )->get_value();
	int pos = emid.find( "-" );
	if( pos == string::npos ){
	  dp.emNumber() = 0;
	  dp.emid()     = atoi( emid.c_str() );
	} else {
	  dp.emNumber() = atoi( emid.substr( 0, pos - 1 ).c_str() );
	  dp.emid()     = atoi( emid.substr( pos + 1 ).c_str() );
	}
	
      } else {
	dp.emNumber() = -1;
	dp.emid()     = -1;
      }
      
      xmlpp::Node::NodeList datas = poElm->get_children( "data" );
      for( xmlpp::Node::NodeList::iterator jtr = datas.begin();
	   jtr != datas.end(); jtr++ ){
	if( xmlpp::Element* daElm = 
	    dynamic_cast< xmlpp::Element* >( *jtr ) ){
	  
	  string type = daElm->get_attribute("type")->get_value().data();

	  string info = daElm->get_attribute("info")->get_value().data();

	  string unit = 
	    ( daElm->get_attribute("unit") ? 
	      daElm->get_attribute("unit")->get_value().data() : "" );

	  double valu = 
	    atof( daElm->get_attribute("value")->get_value().data() );
	  
	  if( type == "data" ) dp.data( valu, info, unit );
	  else if( type == "stat" ) dp.stat() = valu;
	  else if( type == "syst" ) dp.syst( valu, info );
	  else if( type == "kine" ) dp.kine( valu, info, unit );
	  else if( type == "theo" ) dp.model() = valu;
	  else if( type == "terr" ) dp.modelError() = valu;
	  else if( type == "scale1" ) dp.scale1() = valu;
	  else if( type == "scale2" ) dp.scale2() = valu;
	  else if( type == "chi2" ) dp.chiSquare() = valu; 
	}
      }
      
      // get error matrix elements
      xmlpp::Node::NodeList emslist = poElm->get_children( "ems" );
      dp.ems().resize( emslist.size() );
      
      for( xmlpp::Node::NodeList::iterator jtr = emslist.begin();
	   jtr != emslist.end(); jtr++ ){
	xmlpp::Element* emsElm = dynamic_cast< xmlpp::Element* >( *jtr );
	if( emsElm ){
	  int emid = 
	    atoi( emsElm->get_attribute( "emid" )->get_value().data()  );
	  
	  dp.ems()[ emid ] = 
	    atof( emsElm->get_attribute( "value" )->get_value().data()  );
	}
      }
      
      set.push_back( dp );
    }
  }
  
  return set;
}

void ConfigDataSet::dataSet( vector< DataSet >& data ) {

  xmlpp::Node::NodeList dSetNodes = 
    this->rootNode()->get_children( DataSet::xmlTag );
  
  for( xmlpp::Node::NodeList::iterator itr = dSetNodes.begin();
       itr != dSetNodes.end(); itr++ ){
    xmlpp::Element* dSetElms = dynamic_cast< xmlpp::Element* >( *itr );
    if( dSetElms ) data.push_back( this->dataSet( dSetElms ) );
  }
  
}
