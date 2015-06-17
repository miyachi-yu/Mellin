//
// XFragXsec.hh
// $Id: XFragXsec.hh,v 1.1 2008/09/25 02:10:14 miyachi Exp $
/*!
  \brief  class definition of hadron crossection in z space
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Wed Sep 24 17:30:42 2008 Yoshiyuki Miyachi
// Last update Thu Sep 25 11:08:58 2008 Yoshiyuki Miyachi
//
*/

#ifndef  _FragXsec_XFRAGXSEC_HH_
#define  _FragXsec_XFRAGXSEC_HH_

#include <Xsec/xXsec.hh>
#include <QCD/Flavor.hh>

namespace Fragmentation {
  class Evo;
}

namespace FragXsec_T {

  /*!
    \brief  hadron production cross section in z space
   */
  class XFragXsec : public Xsection::xXsec {
  public:

    //! a default constructor
    XFragXsec( const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int ) ;

    //! constructur with a given Fragmentation function evolution kernel
    XFragXsec( Fragmentation::Evo *evo, 
	       const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int ) ;

    virtual ~XFragXsec();  //!< a destructor
    
  protected:

    //! calculate hadron production cross section at the givne kinematic point
    virtual double eval( Exp::DataPoint& dp );

  private:
    
  };
  
};

#endif	    /* !XFRAGXSEC_HH_ */
