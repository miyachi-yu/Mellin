**********************************************************************
*                                                                    *
*         POLARIZED LO AND NLO QCD PARTON DENSITIES                  *
*                                                                    *
*         E. LEADER, A.V. SIDOROV AND D.B. STAMENOV                  *
*                                                                    *
*         Phys. Rev. D73 (2006) 034023 [hep-ph/0512114]              *
*                                                                    *
*         PROBLEMS/QUESTIONS TO sidorov@thsun1.jinr.ru               *
*         OR TO stamenov@inrne.bas.bg                                *
*                                                                    *
*   Two sets of polarized NLO parton densities determined in the     *
*   JET and  MS-bar factorization schemes, as well as LO polarized   *
*   parton  densities are presented. The coresponding values of g_1  *                                
*   are also given.                                                  *                       
*                                                                    *
*   The sets of PPDs are obtained using two different methods to     *
*   fit the world data on the ratio g_1/F_1 and the spin asymmetries *
*   A_1 (p,n,d). In the case of Set 2 PPDs, the corresponding        *
*   values of g1 are given only in the experimental x region         *                                       
*   (0.0048 < x < 0.751) of the present data because it is not known *
*   how to approximate the higher twist corrections to g1 outside    *
*   this region. For more details see the paper.                     * 
*                                                                    *
*                                                                    *
*   Heavy quark thresholds Q(H)=M(H) in the BETA function:           *
*              M(c) = 1.5 GeV,   M(b) = 4.5 GeV.                     *
*       LO:  LAMBDA(3) = 0.204,     LAMBDA(4) = 0.174                *
*            LAMBDA(5) = 0.131.                                      *
*      NLO:  LAMBDA(3) = 0.353,     LAMBDA(4) = 0.300,               *
*            LAMBDA(5) = 0.206.                                      *
*                                                                    *
*   in the BETA function.                                            *
*                                                                    *
*   INPUT:   ISET = number of the parton set :                       *
*            ISET = 1   Set 1 NEXT-TO-LEADING ORDER (MS-bar)         *
*                      (DATA FILE 'Set1_NLO_MS.grid' UNIT=11,        *
*                       TO BE DEFINED BY THE USER )                  *
*            ISET = 2   Set 1 NEXT-TO-LEADING ORDER (JET)            *
*                       TO BE DEFINED BY THE USER )                  *
*            ISET = 3   Set 2 LEADING ORDER                          *
*                      (DATA FILE 'Set2_LO.grid' UNIT=33,            *
*            ISET = 4   Set 2 NEXT-TO-LEADING ORDER (MS-bar)         *
*                      (DATA FILE 'Set2_NLO_MS.grid' UNIT=44,        *
*                       TO BE DEFINED BY THE USER )                  *
*            ISET = 5   Set 2 NEXT-TO-LEADING ORDER (JET)            *
*                      (DATA FILE 'Set2_NLO_JET.grid' UNIT=55,       *
*                       TO BE DEFINED BY THE USER )                  *
*                                                                    *
*            X  = Bjorken-x       (between  1.E-5  and  1)           *
*            Q2 = scale in GeV**2 (between  1.0 and 0.58E6)          *
*                                                                    *
*   OUTPUT:  UUB = x *(DELTA u + DELTA ubar)                         *
*            DDB = x *(DELTA d + DELTA dbar)                         *
*            SSB = x *(DELTA s + DELTA sbar)                         *
*            GL  = x * DELTA GLUON                                   *
*            UV  = x * DELTA uv                                      *
*            DV  = x * DELTA dv                                      *
*            UB  = x * DELTA ubar                                    *
*            DB  = x * DELTA dbar                                    *
*            ST  = x * DELTA sbar                                    *
*      Set 1 case:                                                   *
*            g1p = x*g_1^proton = x*g1p_LT= x*g1p_NLO                *
*            g1n = x*g_1^neutron= x*g1n_LT= x*g1n_NLO                *
*            g1pLT = g1p                                             *
*            g1nLT = g1n                                             *
*                                                                    *
*      Set 2 case:                                                   *
*            g1p = x*g_1^proton = x*g1p_LT + x*g1p_HT                *
*            g1pLT = x*g1p_{LO/NLO+TMC}                              *
*                                                                    *
*            g1n = x*g_1^neutron = x*g1n_LT + x*g1n_HT               *
*            g1nLT = x*g1n_{LO/NLO+TMC}                              *
*                                                                    *
*          (  For the parton distributions and g1(p,n) always        *
*             x times the distribution and g1 is returned   )        *
*                                                                    *
*                                                                    *
*          NOTE: The valence parts DELTA uv, DELTA dv                *
*                                                                    *
*             DELTA uv = (DELTA u + DELTA ubar) - 2*DELTA ubar       *  
*             DELTA dv = (DELTA d + DELTA dbar) - 2*DELTA dbar       *
*                                                                    *
*          as well as DELTA ubar, DELTA dbar, DELTA sbar have        * 
*          been extracted using the convention of the flavour        * 
*          symmetric sea at Q**2 = 1 GeV**2                          * 
*               DELTA ubar = DELTA dbar = DELTA sbar                 *
*          and                                                       *
*               DELTA sbar = DELTA s.                                *
*                                                                    *
*   COMMON:  The main program or the calling routine has to have     *
*            a common block  COMMON / INTINI / IINI , and  IINI      *
*            has always to be zero when LSS2005 is called for the    *
*            first time or when 'ISET' has been changed.             *
*                                                                    *
********************************************************************** 

      SUBROUTINE 
     1  LSS2005(ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST
     1          ,g1pLT,g1p,g1nLT,g1n)

      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (NPART=13, NX=48, NQ=28, NARG=2)
      DIMENSION XUUBF(NX,NQ), XDDBF(NX,NQ),XSSBF(NX,NQ),
     1	XUVF(NX,NQ), XDVF(NX,NQ), XUBF(NX,NQ), XDBF(NX,NQ),
     1          XSF(NX,NQ), XGF(NX,NQ), 
     1 Xg1pLTf(nX,nQ),Xg1pf(nX,nQ),Xg1nLTf(nX,nQ),Xg1nf(nX,nQ),              
     1PARTON (NPART,NQ,NX),
     2      QS(NQ), XB(NX), XT(NARG), NA(NARG), ARRF(NX+NQ)
      COMMON / INTINI / IINI
	      CHARACTER*2  STAR
      SAVE XUUBF,XDDBF,XSSBF, XUVF, XDVF, XUBF, XDBF, XSF, XGF,
     1  Xg1pf,Xg1nf
     1 	 NA, ARRF


*...BJORKEN-X AND Q**2 VALUES OF THE GRID :
       DATA QS / 1.0D0, 1.25D0, 1.5D0, 2.D0, 2.5D0,
     1           4.0D0, 6.4D0, 1.0D1, 1.5D1, 2.5D1, 4.0D1, 6.4D1,
     2           1.0D2, 1.8D2, 3.2D2, 5.8D2, 1.0D3, 1.8D3,
     3           3.2D3, 5.8D3, 1.0D4, 1.8D4, 3.2D4, 5.8D4,
     4           1.0D5, 1.8D5, 3.2D5, 5.8D5 /
       DATA XB /
     1           1.D-5, 1.5D-5, 2.2D-5, 3.2D-5, 4.8D-5, 7.D-5,
     2           1.D-4, 1.5D-4, 2.2D-4, 3.2D-4, 4.8D-4, 7.D-4,
     3           1.D-3, 1.5D-3, 2.2D-3, 3.2D-3, 4.8D-3, 7.D-3,
     4           1.D-2, 1.5D-2, 2.2D-2, 3.2D-2, 5.0D-2, 7.5D-2,
     5           0.1, 0.125, 0.15, 0.175, 0.2, 0.225, 0.25, 0.275,
     6           0.3, 0.325, 0.35, 0.375, 0.4, 0.45,  0.5, 0.55,
     7           0.6, 0.65,  0.7,  0.75,  0.8, 0.85,  0.9, 1.0 /



*...CHECK OF X AND Q2 VALUES :

       IF ( (X.LT.1.0D-5) .OR. (X.GT.1.0D0) ) THEN
           WRITE(6,91)
  91       FORMAT (2X,'PARTON INTERPOLATION: X OUT OF RANGE')
           STOP
       ENDIF
       IF ( (Q2.LT.1.D0) .OR. (Q2.GT.5.8D5) ) THEN
           WRITE(6,92)
  92       FORMAT (2X,'PARTON INTERPOLATION: Q2 OUT OF RANGE')
           STOP
       ENDIF
*...INITIALIZATION :
*    SELECTION AND READING OF THE GRID :
*                                  ( third NUMBER IN THE grid)
*   INPUT:   ISET = number of the parton set 
*            ISET = 1 Set 1 NEXT-TO-LEADING ORDER (MS-bar)  
*   FILE - NO. = 11                               -4.0752E-04
*
*            ISET = 2 Set 1 NEXT-TO-LEADING ORDER (JET) 
*   FILE - NO. = 22:                              -5.1426E-04
*            ISET = 3 Set 2 LEADING ORDER 
*   FILE - NO. = 33                                3.9098E-03
*
*            ISET = 4 Set 2 NEXT-TO-LEADING ORDER (MS-bar) 
*   FILE - NO. = 44                               -5.2304E-04
*
*            ISET = 5 Set 2 NEXT-TO-LEADING ORDER (JET) 
*   FILE - NO. = 55                               -3.5331E-04


      IF (IINI.NE.0) GOTO 16
      IF (ISET.EQ.1) THEN

       IIREAD=11
       OPEN(UNIT=11,FILE='Set1_NLO_MS.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.2) THEN
       IIREAD=22
       OPEN(UNIT=22,FILE='Set1_NLO_JET.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.3) THEN
       IIREAD=33
       OPEN(UNIT=33,FILE='Set2_LO.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.4) THEN
       IIREAD=44
       OPEN(UNIT=44,FILE='Set2_NLO_MS.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.5) THEN
       IIREAD=55
       OPEN(UNIT=55,FILE='Set2_NLO_JET.grid',STATUS='OLD')

      ELSE
        WRITE(6,93)
  93    FORMAT (2X,'PARTON INTERPOLATION: ISET OUT OF RANGE')
        GOTO 60
      END IF
C
       
      READ(IIREAD,2004) STAR
 2004 FORMAT (A2)	 
	 DO 15 N = 1, NQ

       DO 15 M = 1, NX

c             Set 1
      if(Iset.eq.1.or.Iset.eq.2) then
       READ(IIREAD,90) q2gri, xgri,
     1            	 PARTON( 1,N,M), PARTON( 2,N,M), PARTON( 3,N,M),
     1                 PARTON( 4,N,M), PARTON( 5,N,M), PARTON( 6,N,M),
     1                 PARTON( 7,N,M), PARTON( 8,N,M), PARTON( 9,N,M),
     1                 PARTON(10,N,M),                 PARTON(12,N,M)
       PARTON(11,N,M) = PARTON(10,N,M)
       PARTON(13,N,M) = PARTON(12,N,M)

  90   FORMAT (2d9.3,11(1pd12.4))
           endif


c             Set 2

      if(Iset.eq.3.or.Iset.eq.4.or.Iset.eq.5) then
       READ(IIREAD,190) q2gri, xgri,
     1            	 PARTON( 1,N,M), PARTON( 2,N,M), PARTON( 3,N,M),
     1                 PARTON( 4,N,M), PARTON( 5,N,M), PARTON( 6,N,M),
     1                 PARTON( 7,N,M), PARTON( 8,N,M), PARTON( 9,N,M),
     1                 PARTON(10,N,M), PARTON(11,N,M),
     1                 PARTON(12,N,M), PARTON(13,N,M)

 190   FORMAT (2d9.3,13(1pd12.4))

            endif
  15   CONTINUE
C
      IINI = 1
*....ARRAYS FOR THE INTERPOLATION SUBROUTINE :
      DO 10 IQ = 1, NQ
      DO 20 IX = 1, NX-1


        XB0 = XB(IX)
        XB1 = 1.D0-XB(IX)

        XUUBF(iX,iQ) = PARTON(1,IQ,IX) / (XB1**3 * XB0**0.5)
        XDDBF(iX,iQ) = PARTON(2,IQ,IX) / (XB1**3 * XB0**0.5)
        XSSBF(iX,iQ) = PARTON(3,IQ,IX) / (XB1**7 * XB0**0.5)

        XGF(IX,IQ)  = PARTON(4,IQ,IX) / (XB1**6 * XB0**3.)
        XUVF(IX,IQ) = PARTON(5,IQ,IX) / (XB1**3 * XB0**0.5)
        XDVF(IX,IQ) = PARTON(6,IQ,IX) / (XB1**3 * XB0**0.5)
        XUBF(IX,IQ) = PARTON(7,IQ,IX) / (XB1**7 * XB0**0.5)
        XDBF(IX,IQ) = PARTON(8,IQ,IX) / (XB1**7 * XB0**0.5)
        XSF(IX,IQ)  = PARTON(9,IQ,IX) / (XB1**7 * XB0**0.5)


       Xg1pLTf(IX,IQ) = PARTON(10,IQ,IX) / (XB1**3 * XB0**0.5)
       Xg1pf  (IX,IQ) = PARTON(11,IQ,IX) / (XB1**3 * XB0**0.5)
       Xg1nLTf(IX,IQ) = PARTON(12,IQ,IX) / (XB1**3 * XB0**0.5)
       Xg1nf  (IX,IQ) = PARTON(13,IQ,IX) / (XB1**3 * XB0**0.5)

  20  CONTINUE
        XUUBF(nX,iQ) =0.d0
        XDDBF(nX,iQ) =0.d0
        XSSBF(nX,iQ) =0.d0
        XUVF(NX,IQ) = 0.D0
        XDVF(NX,IQ) = 0.D0
        XUBF(NX,IQ) = 0.D0
        XDBF(NX,IQ) = 0.D0
        XSF(NX,IQ)  = 0.D0
        XGF(NX,IQ)  = 0.D0

        Xg1pLTf(NX,IQ) = 0.d0
        Xg1pf  (NX,IQ) = 0.d0
        Xg1nLTf(NX,IQ) = 0.d0
        Xg1nf  (NX,IQ) = 0.d0

  10  CONTINUE
      NA(1) = NX
      NA(2) = NQ
      DO 30 IX = 1, NX
        ARRF(IX) = DLOG(XB(IX))
  30  CONTINUE
      DO 40 IQ = 1, NQ
        ARRF(NX+IQ) = DLOG(QS(IQ))
  40  CONTINUE
  16  CONTINUE
*...INTERPOLATION :
      XT(1) = DLOG(X)
      XT(2) = DLOG(Q2)
      UUB = DFINT(NARG,XT,NA,ARRF,XUUBF)  * (1.D0-X)**3 * X**0.5
      DDB = DFINT(NARG,XT,NA,ARRF,XDDBF)  * (1.D0-X)**3 * X**0.5
      SSB = DFINT(NARG,XT,NA,ARRF,XSSBF)  * (1.D0-X)**7 * X**0.5
      GL = DFINT(NARG,XT,NA,ARRF,XGF)  * (1.D0-X)**6 * X**3.
      UV = DFINT(NARG,XT,NA,ARRF,XUVF) * (1.D0-X)**3 * X**0.5
      DV = DFINT(NARG,XT,NA,ARRF,XDVF) * (1.D0-X)**3 * X**0.5
      UB = DFINT(NARG,XT,NA,ARRF,XUBF) * (1.D0-X)**7 * X**0.5
      DB = DFINT(NARG,XT,NA,ARRF,XDBF) * (1.D0-X)**7 * X**0.5
      ST = DFINT(NARG,XT,NA,ARRF,XSF)  * (1.D0-X)**7 * X**0.5

c     In the Set 2 case the values of g1p and g1n are given only in
c     the present experimental x region !

      g1pLT = DFINT(NARG,XT,NA,ARRF,Xg1pLTf) * (1.D0-X)**3 * X**0.5
      g1p   = DFINT(NARG,XT,NA,ARRF,Xg1pf  ) * (1.D0-X)**3 * X**0.5
      g1nLT = DFINT(NARG,XT,NA,ARRF,Xg1nLTf) * (1.D0-X)**3 * X**0.5
      g1n   = DFINT(NARG,XT,NA,ARRF,Xg1nf  ) * (1.D0-X)**3 * X**0.5


 60   RETURN
      END









