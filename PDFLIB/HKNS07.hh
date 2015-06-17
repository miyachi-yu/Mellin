#ifndef _PDFLIB_HKNS07_hh_
#define _PDFLIB_HKNS07_hh_

#include "Package.hh"

#include <string>

namespace PDFLIB {
  
  class HKNS07 : public Package {
  public:
    
    HKNS07( const int& iset = 2,
	    const int& icharge = 1, 
	    std::string path = "hkns07_table" );
    virtual ~HKNS07();
    
    virtual double parton();
    virtual double type();

  protected:
    
    virtual void loadGrid() ;
    
  private:
    /*!
      \brief set id

     ISET=1: Pion LO Fragmentation functions and their gradient terms 
          2: Pion NLO
          3: Kaon LO
          4: Kaon NLO
          5: Proton LO
          6: Proton NLO
     */
    int    set_; 

    /*!
      \brief charge selection

     ICHARGE=1: pi^+, K^+, or proton
     ICHARGE=2: pi^-, K^-, or neutron
     ICHARGE=3: pi^0=[pi^+ + pi^-]/2, [K^0+K^0b]/2, or [p+pb]/2
        If you want to obtain the fragmentaion functions for each
        K0, K0b, pb, or nb, you may use the relations in Appendix of
        hep-ph/0702250.

     */
    int    charge_;

    /*!
C     FF(I) --> HKNS fragmentation functions (FFs).
C      I = -5 ... b-bar quark (D_b-bar = D_b)
C          -4 ... c-bar quark (D_c-bar = D_c)
C          -3 ... s-bar quark
C          -2 ... d-bar quark 
C          -1 ... u-bar quark 
C           0 ... gluon D_g(x)
C           1 ... u quark 
C           2 ... d quark 
C           3 ... s quark 
C           4 ... c quark 
C           5 ... b quark 
     */
    double ff_[ 11 ];


    /*!
C     GRAD(I,J) --> Gradient terms of HKSN FFs
C      I is the same index as the one in FF(I).
C      J indicates the parameter index for a gradient term dFF(I)/da_J
C      (a_J = parameter).
C
C Pion,J= 1..2: g (2ndM, alpha)           2ndM = second moment
C         3..5: u (2ndM, alpha, beta)
C         6..8: d (2ndM, alpha, beta)
C        9..11: c (2ndM, alpha, beta)
C       12..14: b (2ndM, alpha, beta)
C   For example, the above J=14 indicates d D_b^{pi^+}/d beta_b^{pi^+}.
C
C Kaon,J= 1..2: g (2ndM, beta)
C         3..5: u (2ndM, alpha, beta)
C         6..8: d (2ndM, alpha, beta)
C        9..11: sb(2ndM, alpha, beta)
C       12..14: c (2ndM, alpha, beta)
C       15..17: b (2ndM, alpha, beta)
C
CProton,J=1..2: g (2ndM, beta)
C         3..5: u (2nsM, alpha, beta)
C         6..8: qb(2ndM, alpha, beta)
C        9..11: c (2ndM, alpha, beta)
C       11..13: b (2ndM, alpha, beta)
C
C   NOTE: The returned values are not multiplied by x.
C
C      *  Error matrix can be used by declaring a common block:
C         COMMON/ERRM/EM(17,17). This matrix is defined as
C         the inverse matrix of Hessian multiplied by Delta chi^2:
C         EM(i,j)=Delta chi^2*H_ij^-1.
C         The values of Delta chi^2 are as follows:
C         15.9359730(pion), 19.1977555(kaon), 14.8470228(proton).
C *******************************************************************
     */
    double grad_[ 11 ][ 17 ];
    
  };
};

#endif // _HKNS07_hh_
