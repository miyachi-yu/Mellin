// $Id: StrFunc.hh,v 1.7 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file  StrFunc.hh
*/
#ifndef _polSIDIS_StrFunc_hh_
#define _polSIDIS_StrFunc_hh_

#include <iostream>
#include <vector>

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/Charge.hh>

namespace polSIDIS {
  
  /*!
    \brief  a class for polarized structure function in SIDIS 
    
    This is a class for calculating \f$ g_1^h( N, Q^2 ) \f$.
    

   */
  class StrFunc : public Xsection::Xsec {
  public:
    
    /*!
      \param[in] arg is an Utility::Argument object
      \param[in] n is a target nucleon type
      \param[in] h is a fragmenting hadron type
      \brief  a  constructor
    */
    StrFunc( Utility::Arguments& arg, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    /*!
      \param[in] pdf is a pointer to a polPDF::Evo object
      \param[in] ff is a pointer to a Fragmentation::Evo object
      \param[in] n is a target nucleon type
      \param[in] h is a fragmenting hadron type
    */
    StrFunc( Evolution::Kernel *pdf,
	     Evolution::Kernel *ff,
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    virtual ~StrFunc();                          //!< a destructor
    
    /*!
      \param[in] n is a moment for PDF
      \param[in] m is a moment for FF
      \return  \f$ g_{1~{\rm N}}^{\rm h}( n, m )\f$
      \brief  returns polarized structure function in SIDIS, \f$ g_{1~{\rm N}}^{\rm h}( n, m )\f$
     */
    virtual std::complex< double > 
    operator()( const std::complex< double >& n, 
		const std::complex< double >& m );
    
  private:
    
  };
  
};

#endif // _polSIDIS_StrFunc_hh_

