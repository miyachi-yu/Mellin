//
// HadronParam.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Sun Oct 26 13:57:18 2008 Yoshiyuki Miyachi
// Last update Sun Oct 26 15:21:48 2008 Yoshiyuki Miyachi
//

#ifndef _QCD_HADRONPARAM_HH_
#define _QCD_HADRONPARAM_HH_

#include "PdfParametrization.hh"

namespace QCD {

  class HadronParam : public PdfParametrization {
  public:
    
    HadronParam( PdfParametrization* pion,
		 PdfParametrization* kaon,
		 PdfParametrization* rest );

    virtual ~HadronParam();

    /*!
      \par [in] q is a PARTON type
      \return value of PDF for the given q in the mellin space at n
      \brief  calculate the q(n) ( in the Mellin space )
      
      This calculte the PDF value for parton q at complex number n 
      in the mellin space. n shuld be pre-defined with setN method.
    */
    virtual std::complex< double > q( const Flavor::PARTON& q );
    
    //!< see q( const Flavor::TYPE&   q );
    virtual std::complex< double > q( const Flavor::TYPE& q );

    //! check if the information in config is enough or not
    virtual bool check();

    virtual PdfParametrization* pion() { return pion_; }
    virtual PdfParametrization* kaon() { return kaon_; }
    virtual PdfParametrization* rest() { return rest_; }

    virtual std::string path();

  private:

    PdfParametrization* pion_;  //!< Pion fragmentation
    PdfParametrization* kaon_;  //!< Kaon fragmentation
    PdfParametrization* rest_;  //!< Rest fragmentttion


  };
}

#endif	    /* !HADRONPARAM_HH_ */
