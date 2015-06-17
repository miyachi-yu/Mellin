#include "Wilsons.hh"

using namespace std;
using namespace IntegXsec;

Wilsons::Wilsons() :
  vector< vector< vector< ConvIntegrand* > > >
  ( 3, vector< vector< ConvIntegrand* > >
    ( 3, vector< ConvIntegrand* >( 2, ( ConvIntegrand* ) NULL ) ) )
{
}

Wilsons::~Wilsons(){
}

