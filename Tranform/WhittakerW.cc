#include "WhittakerW.hh"
#include <cmath>

using namespace std;
using namespace Transform;

WhittakerW::WhittakerW( const double& k, const double& m ) :
  U_( 0.5 + m - k, 1.0 + 2.0 * m ), k_( k ), m_( m )
{
}

WhittakerW::~WhittakerW(){
}

void WhittakerW::set( const double& k, const double& m ){
  k_ = k;
  m_ = m;
  U_.set( 0.5 + m - k, 1.0 + 2.0 * m );
}

complex< double > WhittakerW::operator()( const complex< double >& z ){
  return exp( -0.5 * z ) * pow( z, m_ + 0.5 ) * U_( z );
}
