//
// FF.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:08:13 2008 Yoshiyuki Miyachi
// Last update Thu Oct  2 18:36:18 2008 Yoshiyuki Miyachi
//

#ifndef _PDFLIB_FF_HH_
#define _PDFLIB_FF_HH_

#include "Package.hh"

namespace PDFLIB {
  
  class FF : public Package {
  public:
    
    /*!
      \brief a constructor

      * ffset = 1,2,3 means K, KKP, BFGW                                     *
* Details are to be found in the corresponding references:             *
* S.Kretzer, Phys.Rev.D62, 054001 (2000)                               *
* B.A.Kniehl, G.Kramer, B.Potter, Nucl.Phys.B582, 514 (2000)           *
* L.Bourhis, M.Fontannaz, J.P.Guillet, M.Werlen,Eur.Phys.J.C19,89(2001)*
*                                                                      *
* fforder=0,1 is LO, NLO(MSbar)                                        *
* no LO set for BFGW                                                   *
*                                                                      *
* ihadron=1,2,3,4,5 is pi,K,h,p,n                                      *
* no pi,K sets for BFGW                                                *
* no p,n sets for K, BFGW                                              *
*                                                                      *
* icharge=0,1,2,3 is 0,+,-,+&-                                         *
* note: 3 = +&- = charge sum (NOT average) in this library             *
* for the neutral (icharge=0) particles K^0, n:                        *
* icp = 1,2,3 chooses between particle, anti-particle or sum of both   *
* icp is inactive for pi,h,p and charged Kaons                         *
*                                                                      *
* Following KKP, FFs into {pi^0;(anti-)K^0} and into {pi^+/-;K^+/-}    *
* are related by isospin.                                              *
*                                                                      *
* ipi = 1,2,3 is a flag for BFGW; inactive for K, KKP:                 *
* ipi = 1: best fit     (formula (8))  of BFGW                         *
* ipi = 2: large Ng set (formula (9))  of BFGW                         *
* ipi = 3: low Ng set   (formula (10)) of BFGW                         *
************************************************************************

     */
    FF( const int & ffset,
	const std::string& path = "ff_tables",
	const int& hadron = 1,
	const int& charge = 1,
	const int& order = 1 );
    
    virtual ~FF();

    virtual double parton();
    
  protected:
    virtual void loadGrid() ;
    
  private:

    double dff_[11];
    int ffset_;
    int fforder_;
    int ihadron_;
    int icharge_;
    int icp_;
    int ipi_;

  };
};

#endif	    /* !FF_HH_ */
