// $Id: Flavor.hh,v 1.28 2013/04/05 09:02:13 miyachi Exp $
/*!
  \file    Flavor.hh
  \version $Revision: 1.28 $
  \date    $Date: 2013/04/05 09:02:13 $
  \author  $Author: miyachi $
  \brief   Definition of quark flavor information class
 */
#ifndef _Flavor_hh_
#define _Flavor_hh_

#include <string>
#include <vector>

#include <Tranform/RealFunction.hh>

namespace QCD {

  /*!
    \class  Flavor   Flavor.hh   "QCD/Flavor.hh"
    \brief  Quark information class. 
    
    This can be used to handle information of Quark Flavor.
    So far, it is implemented to get number of flavor at the given energy 
    scale..
    
  */
  class Flavor : public Transform::RealFunction {
  public:
    
    /*!
      \brief  parton flavor information
    */
    enum PARTON {
      tbar = -6, bbar = -5, cbar = -4, sbar = -3, dbar = -2, ubar = -1,
      g =  0,
      u   =  1, d   =  2, s   =  3, c   =  4, b   =  5, t   =  6,
      uv  = 11, dv  = 12, sv  = 13, cv  = 14, bv  = 15, tv  = 16,
      uns = 21, dns = 22, sns = 23, cns = 24, bns = 25, tns = 26,
      U   = 31, D   = 32, S   = 33, C   = 34, B   = 35, T   = 36,
      ubarubar =-11
    };
    
    /*!
      \brief  combination type of quarks, such as flavor singlet and non-singlet
    */
    enum TYPE {
      a3m = -1, a8m = -3, a15m = -4, a24m = -5, a35m = -6,
      a0 = 0,
      a3 = 1, a8 = 3, a15 = 4, a24 = 5, a35 = 6,
      NSp = 7, NSm = 8, NSv =9,
      DELTA = 100, SEA = 101,    // for MRST parameterization
      DURATIO = 102, DUSUM = 103 // for CTEQ6 parameterization
    };
    
    //!  hadron type
    enum HADRON {
      PIp  =   1, PIm  = -1,    PI0  = 0,
      KAp  =   2, KAm  = -2,
      PRp  =   3, PRm  = -3,
      CHp  =   4, CHm  = -4,
      REp  =  10, REm  = -10,
      HAp  = 100, HAm  = -100
    };
    
    //! structure function type
    enum SFNCTYPE {
      F1p =  1, F1d =  2, F1n =  3,
      g1p = -1, g1d = -2, g1n = -3
    };

    //! color id
    enum COLOR_ID {
      Black       =  1, 
      Red         =  2,
      BlightGreen =  3,
      BlightBlue  =  4,
      Yellow      =  5,
      Magenta     =  6,
      Cyan        =  7,
      Green       =  8,
      Blue        =  9,
      White       = 10,
      Gray        = 11
    };

    //! style id
    enum STYLE_ID {
      SOLID       =  1, 
      DASH        =  2,
      DOTTED      =  3,
      DASHDOT     =  4,
      LDASHDOT    =  5,
      LDASHDDDOT  =  6,
      LDASH       =  7,
      LDASHDDOT   =  8,
      LLDASH      =  9,
      LLDASHDOT   = 10,
    };

    static int color( const Flavor::PARTON& p ); 
    static int style( const Flavor::PARTON& p ); 
    
    Flavor();                                //!< a default constructor
    virtual ~Flavor();                       //!< a destructor
    
    /*!
      calcualte number of the flvator at the given scale Q.
      At the thereshold value, the new flavor is not taken into account.
      (i.e., at the strange quark mass scale it returns 2, instead of 3.)
    */
    virtual double operator() ( const double& Q );
    
    static std::string name( const PARTON& q );    
    //!< name of the given parton type
    static std::string name( const TYPE&   q );    
    //!< name of the given parton type
    static std::string name( const SFNCTYPE& q );  
    //!< name of the given function type
    static std::string name( const HADRON& q, 
			     const bool& texflag = false );  
    //!< name of the given hadron type
    
    static int  id( const std::string& name );   //!< get id of the given name
    
    //! true if the given name specify quark flavor
    static bool isParton( const std::string& name ); 
    
    static std::vector< double > mass;           //!< quark masses
    static void setMassValue(); //!< set quark mass
    
  private:
    
  };
  
};
#endif //  _Flavor_hh_
