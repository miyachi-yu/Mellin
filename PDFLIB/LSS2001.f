






*********************************************************************
*                                                                   *
*         POLARIZED LO AND NLO PARTON DENSITIES                     *
*                                                                   *
*         E. LEADER, A.V. SIDOROV AND D.B. STAMENOV                 *
*                                                                   *
*                 (hep-ph/0111267)                                  *
*                                                                   *
*         PROBLEMS/QUESTIONS TO sidorov@thsun1.jinr.ru              *
*         OR TO stamenov@inrne.bas.bg                               *
*                                                                   *
*   Polarized NLO parton densities determined in the JET and        *
*   MS-bar factorization schemes as well as LO polarized parton     *   
*   densities are presented.                                        *         
*                                                                   *
*   Heavy quark thresholds Q(H)=M(H) in the BETA function:          *
*              M(c) = 1.5 GeV,   M(b) = 4.5 GeV.                    *
*   Corresponding LAMBDA(Nf) values in GeV for Q**2 > M(H)**2:      *
*       LO:  LAMBDA(3) = 0.204,     LAMBDA(4) = 0.174               *
*            LAMBDA(5) = 0.131.                                     *
*      NLO:  LAMBDA(3) = 0.353,     LAMBDA(4) = 0.300,              *
*            LAMBDA(5) = 0.206.                                     *
*                                                                   *
*   The number of active quarks flavours is Nf=3 everywhere except  *
*   in the BETA function.                                           *
*                                                                   *
*   INPUT:   ISET = number of the parton set :                      *
*            ISET = 1   NEXT-TO-LEADING ORDER (JET)                 *
*                      (DATA FILE 'JET.grid' UNIT=11, TO BE         *
*                       DEFINED BY THE USER )                       *
*            ISET = 2   NEXT-TO-LEADING ORDER (MS-bar)              *
*                      (DATA FILE 'MS.grid' UNIT=22, TO BE          *
*                       DEFINED BY THE USER )                       *
*            ISET = 3   LEADING ORDER                               *
*                      (DATA FILE 'LO.grid' UNIT=33, TO BE          *
*                       DEFINED BY THE USER )                       *
*                                                                   *
*            X  = Bjorken-x       (between  1.E-5  and  1)          *
*            Q2 = scale in GeV**2 (between  1.0 and 0.58E6)         *
*                                                                   *
*   OUTPUT:  UUB = x *(DELTA u + DELTA ubar)                        *
*            DDB = x *(DELTA d + DELTA dbar)                        *
*            SSB = x *(DELTA s + DELTA sbar)                        *
*            GL  = x * DELTA GLUON                                  *
*            UV  = x * DELTA uv                                     *
*            DV  = x * DELTA dv                                     *
*            UB  = x * DELTA ubar                                   *
*            DB  = x * DELTA dbar                                   *
*            ST  = x * DELTA sbar                                   *
*                                                                   *
*          (  For the parton distributions always x times           *
*                   the distribution is returned   )                *
*                                                                   *
*            The sets are the result of a combined fit to           *
*            data on the spin asymmetries A_1 (p,n,d)               *
*                                                                   *
*          NOTE: The valence parts DELTA uv, DELTA dv               *
*                                                                   *
*             DELTA uv = (DELTA u + DELTA ubar) - 2*DELTA ubar      *  
*             DELTA dv = (DELTA d + DELTA dbar) - 2*DELTA dbar      *
*                                                                   *
*          as well as DELTA ubar, DELTA dbar, DELTA sbar have       * 
*          been extracted using the assumption about the flavour    * 
*          symmetric sea                                            * 
*               DELTA ubar = DELTA dbar = DELTA sbar                *
*          and                                                      *
*               DELTA sbar = DELTA s.                               *
*                                                                   *
*   COMMON:  The main program or the calling routine has to have    *
*            a common block  COMMON / INTINI / IINI , and  IINI     *
*            has always to be zero when PARPOL is called for the    *
*            first time or when 'ISET' has been changed.            *
*                                                                   *
*********************************************************************

      SUBROUTINE LSS2001 (ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (NPART=9, NX=48, NQ=28, NARG=2)
      DIMENSION XUUBF(NX,NQ), XDDBF(NX,NQ),XSSBF(NX,NQ),
     1	XUVF(NX,NQ), XDVF(NX,NQ), XUBF(NX,NQ), XDBF(NX,NQ),
     1          XSF(NX,NQ), XGF(NX,NQ), PARTON (NPART,NQ,NX-1),
     2          QS(NQ), XB(NX), XT(NARG), NA(NARG), ARRF(NX+NQ)
      COMMON / INTINI / IINI
      SAVE XUUBF,XDDBF,XSSBF, XUVF, XDVF, XUBF, XDBF, XSF, XGF, NA, ARRF


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
*                                               ( FIRST NUMBER IN THE
*   INPUT:   ISET = number of the parton set :
*            ISET = 1   NEXT-TO-LEADING ORDER (JET)
*  FILE - NO. = 11                                 2.2714E-04
*
*            ISET = 2   NEXT-TO-LEADING ORDER (MS-bar)
*   FILE - NO. = 22                                1.4314E-04
*
*            ISET = 3   LEADING ORDER
*   FILE - NO. = 33
*                                                  2.1499E-03


      IF (IINI.NE.0) GOTO 16
      IF (ISET.EQ.1) THEN
       IIREAD=11
       OPEN(UNIT=11,FILE='JET.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.2) THEN
       IIREAD=22
       OPEN(UNIT=22,FILE='MS.grid',STATUS='OLD')
      ELSE IF (ISET.EQ.3) THEN
       IIREAD=33
       OPEN(UNIT=33,FILE='LO.grid',STATUS='OLD')

      ELSE
        WRITE(6,93)
  93    FORMAT (2X,'PARTON INTERPOLATION: ISET OUT OF RANGE')
        GOTO 60
      END IF
C
       DO 15 M = 1, NX-1
       DO 15 N = 1, NQ
       READ(IIREAD,90) PARTON(1,N,M), PARTON(2,N,M), PARTON(3,N,M),
     1                 PARTON(4,N,M), PARTON(5,N,M), PARTON(6,N,M),
     1                 PARTON(7,N,M), PARTON(8,N,M), PARTON(9,N,M)
  90   FORMAT (9(1PE12.4))
c  90   FORMAT (6(1E12.4))
  15   CONTINUE
C
      IINI = 1
*....ARRAYS FOR THE INTERPOLATION SUBROUTINE :
      DO 10 IQ = 1, NQ
      DO 20 IX = 1, NX-1

        XB0 = XB(IX)
        XB1 = 1.D0-XB(IX)
        XUUBF(iX,iQ) = PARTON(1,IQ,IX) / (XB1**8 * XB0**0.5)
        XDDBF(iX,iQ) = PARTON(2,IQ,IX) / (XB1**8 * XB0**0.5)
        XSSBF(iX,iQ) = PARTON(3,IQ,IX) / (XB1**8 * XB0**0.5)

        XGF(IX,IQ)  = PARTON(4,IQ,IX) / (XB1**5 * XB0**2.)
        XUVF(IX,IQ) = PARTON(5,IQ,IX) / (XB1**3 * XB0)
        XDVF(IX,IQ) = PARTON(6,IQ,IX) / (XB1**4 * XB0)
        XUBF(IX,IQ) = PARTON(7,IQ,IX) / (XB1**8 * XB0**0.5)
        XDBF(IX,IQ) = PARTON(8,IQ,IX) / (XB1**8 * XB0**0.5)
        XSF(IX,IQ)  = PARTON(9,IQ,IX) / (XB1**8 * XB0**0.5)

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
      UUB = DFINT(NARG,XT,NA,ARRF,XUUBF)  * (1.D0-X)**8 * X**0.5
      DDB = DFINT(NARG,XT,NA,ARRF,XDDBF)  * (1.D0-X)**8 * X**0.5
      SSB = DFINT(NARG,XT,NA,ARRF,XSSBF)  * (1.D0-X)**8 * X**0.5
      GL = DFINT(NARG,XT,NA,ARRF,XGF)  * (1.D0-X)**5 * X**2.
      UV = DFINT(NARG,XT,NA,ARRF,XUVF) * (1.D0-X)**3 * X
      DV = DFINT(NARG,XT,NA,ARRF,XDVF) * (1.D0-X)**4 * X
      UB = DFINT(NARG,XT,NA,ARRF,XUBF) * (1.D0-X)**8 * X**0.5
      DB = DFINT(NARG,XT,NA,ARRF,XDBF) * (1.D0-X)**8 * X**0.5
      ST = DFINT(NARG,XT,NA,ARRF,XSF)  * (1.D0-X)**8 * X**0.5

 60   RETURN
      END








