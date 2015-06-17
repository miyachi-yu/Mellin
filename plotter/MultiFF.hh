#ifndef _MultiFF_hh_
#define _MultiFF_hh_

#include "MultiPanel.hh"
#include <TH1.h>
#include <Fragmentation/Evolution.hh>

class MultiFF : public MultiPanel {
public:
  MultiFF( int argc, char* argv[] ) throw( int );
  virtual ~MultiFF();
  virtual void privateStyle();
  virtual void drawPad( const int& co, const int& ro, TH1 *frame );
  
protected:
  
private:
  Fragmentation::Evo *ff_;
  Fragmentation::Evo* ff() throw( int );

};
#endif // _MultiFF_hh_
