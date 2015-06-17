// $Id: Browser.hh,v 1.9 2008/07/01 10:10:42 miyachi Exp $
/*!
  \file   Browser.hh
  \brief  TRint application 
 */
#ifndef _Browser_hh_
#define _Browser_hh_

#include <TRint.h>

namespace Fragmentation {
  class Evo;
}

namespace Xsection {
  class Xsec;
}

class FragXsecPanel;
class G1Panel;

namespace Exp {
  class ExpData;
};

#include "PdfPanel.hh"

/*!
  \brief TRint application for browsing PDF and Structure functions
  
  This is a class which inherits TRint ROOT class to provide
  browsing method for PDF and Xsections.

  \par
  Since this inherits TRint, so that user must create object in
  a main routine and call Run method.
  On the mellin prompt, one can access to the Browser singleton 
  pointer by Broswer::instance() method. Using the singleton
  pointer, user can for example draw the structure functions...

 */
class Browser : public TRint {
public:
  
  /*!
    \param[in] argc is a number of arguments given.
    \param[in] argv is a arguments list
    \return  a singleton pointer of Browser object
    \brief initiate singleton Browser object and get the pointer.

    This initiate Brower object only once from the given arguments list.
    On creation of the singleton object, this register void function to
    be call at the end of the program with atexit system call.
    
  */
  static Browser* instance( int argc, char* argv[] );

  static Browser* instance();         //!< get a singleton object pointer
  virtual ~Browser();                 //!< a destructor
  
  //  Xsection::Xsec* xsec() { return xsec_; } //!< get xsection model to be drawn
  
  void pdfPanel();                         //!< display PdfPanel object 
  void draw();                             //!< draw the structure functions....
  void update(); //!< clear cache data

private:

  static Browser *instance_;
  
  Browser( int argc, char* argv[] );
  
  Fragmentation::Evo *ff_;

  PdfPanel       *panel_;
  FragXsecPanel  *view_;
  Exp::ExpData   *expr_;
  
  G1Panel        *g1view_;
  
  ClassDef( Browser, 1 )
};

#endif // _Browser_hh_

