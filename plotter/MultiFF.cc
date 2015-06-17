#include "MultiFF.hh"

using namespace std;

MultiFF::MultiFF( int argc, char* argv[] ) throw( int ) : 
  MultiPanel( argc, argv ),
  ff_( NULL )
{
  Utility::Arguments& args = Utility::Arguments::ref();
  args.log() << __FILE__ << ":" << __LINE__ << endl;  
  try {
    ff_ = this->ff();
  } catch( int e ) {
    args.err() << "catch error (" << e << ")" << endl;
    throw e;
  }
  // style
  this->style();
}

MultiFF::~MultiFF() {
  if( ff_ ) delete ff_;
}

void MultiFF::privateStyle(){
  Utility::Arguments& args = Utility::Arguments::ref();
  args.log() << __FILE__ << ":" << __LINE__ << endl;
  row_    = 3;
  column_ = 1;
  xtitle_ = "z";
  ytitle_ = "z D(z, Q^{2})";
}

void MultiFF::drawPad( const int& co, const int& ro, TH1 *frame ){
}

Fragmentation::Evo* MultiFF::ff() throw( int ) {
  
  vector< string > optPI = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string > optKA = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string > optPR = Fragmentation::Evo::options( QCD::Flavor::PRp );
  vector< string > optRE = Fragmentation::Evo::options( QCD::Flavor::REp );
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string >::iterator itrPI = args.hasOpt( optPI );
  vector< string >::iterator itrKA = args.hasOpt( optKA );
  vector< string >::iterator itrPR = args.hasOpt( optPR );
  vector< string >::iterator itrRE = args.hasOpt( optRE );
  
  if( itrPI == optPI.end() && itrKA == optKA.end() &&
      itrPR == optPR.end() && itrRE == optRE.end() ){
    args.err() << "you have to specify fragmentation function...." 
	       << endl;
    return NULL;
  }
  
  QCD::Flavor::HADRON h;
  string opt;
  if( itrRE != optRE.end() ){ h = QCD::Flavor::REp; opt = *itrRE; }    
  if( itrPR != optPR.end() ){ h = QCD::Flavor::PRp; opt = *itrPR; }    
  if( itrKA != optKA.end() ){ h = QCD::Flavor::KAp; opt = *itrKA; }    
  if( itrPI != optPI.end() ){ h = QCD::Flavor::PIp; opt = *itrPI; }    
  
  try {
    return new Fragmentation::Evo( args, h, opt );
  } catch( int e ){
    throw e;
  }

  return NULL;
}
