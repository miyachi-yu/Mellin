//
// viewExpData.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Sep 26 15:52:13 2008 Yoshiyuki Miyachi
// Started on  Fri Sep 26 15:52:13 2008 Yoshiyuki Miyachi
//

#include <iostream>

using namespace std;

#include <TApplication.h>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>

#include "ExpDataPanel.hh"

class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) :
    TApplication( "MyApplication", &argc, argv ),
    ed_( NULL ), panel_( NULL )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref();
    if( args.hasOpt( "expdata" ) ) { 

      ed_    = new Exp::ExpData( args );
      panel_ = new ExpDataPanel( *ed_, "data", 
				 args.get( "kine", "zmid" ) );
      
      if( args.hasOpt( "logx" ) ) panel_->SetLogx();
      if( args.hasOpt( "logy" ) ) panel_->SetLogy();

    }
    
  }
  
  virtual ~MyApplication() {
    if( ed_ ) delete ed_;
  }
  
  virtual void Run( Bool_t retrn = kFALSE ){
    Utility::Arguments& args = Utility::Arguments::ref();
    if( panel_ ) panel_->draw( args.get( "output", "ExpDataPanel.png" ) );
  }
  
  Exp::ExpData *ed_;
  ExpDataPanel *panel_;

};
  
int main( int argc, char* argv[] ){

  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}
