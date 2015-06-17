// $Id: Scheme.hh,v 1.6 2008/11/11 04:06:22 miyachi Exp $
/*!
  \file    Scheme.hh
  \version $Revision: 1.6 $
  \date    $Date: 2008/11/11 04:06:22 $
  \author  $Author: miyachi $
  \brief   Definition of QCD renormalization scheme class
 */

#ifndef _Scheme_hh_
#define _Scheme_hh_

#include <iostream>

namespace QCD {
  
  /*!
    \class Scheme   Scheme.hh    "QCD/Scheme.hh"
    \brief QCD Renormalization Scheme Class
    
    This is a class to handle QCD renormalization scheme.
    So far, this does not have much functionalities, 
    it will come later...
    
  */
  class Scheme {
  public:
    
    enum TYPE { MSbar = 1 };                     //!< scheme type
    enum ORDER { LO = 0, NLO = 1, NNLO = 2, NNNLO = 3 };    //!< QCD order
    
    Scheme( TYPE type = MSbar, ORDER order = NLO ); //!< a default constructor
    virtual ~Scheme();                              //!< destructor
    
    Scheme& operator=( const Scheme& sc ); //!< assignemnet operator
    bool operator!=( const Scheme& sc );   //!< not equal operator
    bool operator==( const Scheme& sc );   //!< equal operator
    
    TYPE type() const { return type_; } //!< get a value of scheme type
    TYPE& type() { return type_; }      //!< get a reference of scheme type
    
    ORDER order() const { return order_; } //!< get a value of QCD order
    ORDER& order() { return order_; }      //!< get a reference of QCD order
    
    //! dump information of scheme object to output stream
    friend std::ostream& operator <<( std::ostream& os, Scheme& scheme );
    
  private:
    //! scheme type
    TYPE type_; 
    //! QCD order
    ORDER order_;
    
  };
};
#endif //  _Scheme_hh_
