//#include "Utility/Config.hh"

extern "C" {
#include <libxml2/libxml/HTMLparser.h>
}

int main( int argc, char* argv[] ){
  
  char *url 
    = "http://durpdg.dur.ac.uk/cgi-hepdata/struct3/HERMES/0609039/g1pd";
  
  htmlDocPtr doc = htmlParseFile( url, "UTF-8" );

  return 0;
}
