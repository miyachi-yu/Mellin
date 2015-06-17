C *********************************************************************
C  aac08.f  Version 1.2                                2008/AUG/05
C
C  [Package for the AAC polarized PDFs]
C  Reference: 
C         Determination of gluon polarization 
C             from deep inelastic scattering and collider data
C         M. Hirai, and S. Kumano
C         arXiv:0808.0413.
C
C *********************************************************************
C ---------------------------------------------------------------------
C  SUBROUTINE AAC08PDF(Q2,X,ISET,XPPDF,GRAD):
C
C   Subroutine AAC08PDF returns the values of polarized PDFs
C   and their gradient terms at specified Q^2 and x point 
C   by interpolating the grid data.
C   [ Log(Q^2): LINEAR INTERPOLATION, x: CUBIC SPLINE INTERPOLATION ]
C
C   INPUT:
C     Q2, X ... Q^2 and x values at which the functions are calculated. 
C               Available range: 10^-9 <= X <= 1.0,
C                           1.0 GeV^2 <= Q^2 <= 10^8 GeV^2.
C     ISET=1: Positive type PDFs and their gradient terms of Set-B
C          2: Node type PDFs and their gradient terms
C
C   OUTPUT: Arrays XPPDF(-3:3) & GRAD(I,J)
C
C     XPPDF(I) --> AAC08 polarized PDFs.
C      I = -3 ... s-bar quark
C          -2 ... d-bar quark 
C          -1 ... u-bar quark 
C           0 ... gluon D_g(x)
C           1 ... u quark 
C           2 ... d quark 
C           3 ... s quark 
C
C     GRAD(I,J) --> Gradient terms of AAC08 PDFs
C      I is the same index as the one in XPPDF(I).
C      J indicates the parameter index for a gradient term dXPPDF(I)/da_J
C      (a_J = parameter).
C
C      J= 1..4: g   (delta, nu, kappa, mu)
C         5, 6: d_v (delta, mu)
C         7, 8: u_v (delta, mu)
C        9..11: qb  (delta, nu, kappa)
C
C   For example, the above J=1 indicates d XPPDF_g/d delta_g.
C
C   NOTE: The returned values are PPDF multiplied by x.
C
C      *  Error matrix can be used by declaring a common block:
C         COMMON/ERRM/EM(11,11). This matrix is defined as
C         the inverse matrix of Hessian multiplied by Delta chi^2:
C         EM(i,j)=Delta chi^2*H_ij^-1.
C         The values of Delta chi^2 is 12.647
C *********************************************************************
      SUBROUTINE AAC08PDF(Q2,X,ISET,XPPDF,GRAD)
C ---------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      PARAMETER (NQ=33, NX=117, ND=74, NPDF=6, NSET=3, IFILE=20)
      DIMENSION IREAD(2), QG(NQ), XG(NX),PDFJ1(ND), PDFJ2(ND)
     +         ,XPPDF(-3:3), GRADFF(6,11), GRAD(-3:3,11)
     +         ,BXG(NX,NQ,ND), CXG(NX,NQ,ND), DXG(NX,NQ,ND)
     +         ,PDFG(NX,NQ,ND), EMI(11,11,2)

      COMMON/ERRM/EM(11,11)
      SAVE IREAD, NPAR, BXG, CXG, DXG 
      DATA IREAD /1, 1/ 
      DATA EPS/1.D-12/

      INCLUDE 'EM.inc'

C Q2 AND X GRID.
      DATA QG /
     +  1.000000D+00, 1.467799D+00, 2.154435D+00,
     +  3.162278D+00, 4.641589D+00, 6.812921D+00,
     +  1.000000D+01, 1.467799D+01, 2.154435D+01,
     +  3.162278D+01, 4.641589D+01, 6.812921D+01,
     +  1.000000D+02, 1.778279D+02, 3.162278D+02, 5.623413D+02,
     +  1.000000D+03, 1.778279D+03, 3.162278D+03, 5.623413D+03,
     +  1.000000D+04, 1.778279D+04, 3.162278D+04, 5.623413D+04,
     +  1.000000D+05, 1.778279D+05, 3.162278D+05, 5.623413D+05,
     +  1.000000D+06, 4.641589D+06, 
     +  1.000000D+07, 4.641589D+07,  
     +  1.000000D+08  /

      DATA XG / 
     +  1.000000D-09, 1.333521D-09, 1.778279D-09, 2.371374D-09,
     +  3.162278D-09, 4.216965D-09, 5.623413D-09, 7.498942D-09,
     +  1.000000D-08, 1.333521D-08, 1.778279D-08, 2.371374D-08,
     +  3.162278D-08, 4.216965D-08, 5.623413D-08, 7.498942D-08,
     +  1.000000D-07, 1.333521D-07, 1.778279D-07, 2.371374D-07,
     +  3.162278D-07, 4.216965D-07, 5.623413D-07, 7.498942D-07,
     +  1.000000D-06, 1.333521D-06, 1.778279D-06, 2.371374D-06,
     +  3.162278D-06, 4.216965D-06, 5.623413D-06, 7.498942D-06,
     +  1.000000D-05, 1.333521D-05, 1.778279D-05, 2.371374D-05,
     +  3.162278D-05, 4.216965D-05, 5.623413D-05, 7.498942D-05,
     +  1.000000D-04, 1.333521D-04, 1.778279D-04, 2.371374D-04,
     +  3.162278D-04, 4.216965D-04, 5.623413D-04, 7.498942D-04,
     +  1.000000D-03, 1.154782D-03, 1.333521D-03, 1.539927D-03,
     +  1.778279D-03, 2.053525D-03, 2.371374D-03, 2.738420D-03,
     +  3.162278D-03, 3.651741D-03, 4.216965D-03, 4.869675D-03,
     +  5.623413D-03, 6.493816D-03, 7.498942D-03, 8.659643D-03,
     +  1.000000D-02, 1.154782D-02, 1.333521D-02, 1.539927D-02,
     +  1.778279D-02, 2.053525D-02, 2.371374D-02, 2.738420D-02,
     +  3.162278D-02, 3.651741D-02, 4.216965D-02, 4.869675D-02,
     +  5.623413D-02, 6.493816D-02, 7.498942D-02, 8.659643D-02,
     +  1.000000D-1, 1.250000D-1, 1.500000D-1, 1.750000D-1,
     +  2.000000D-1, 2.250000D-1, 2.500000D-1, 2.750000D-1,
     +  3.000000D-1, 3.250000D-1, 3.500000D-1, 3.750000D-1,
     +  4.000000D-1, 4.250000D-1, 4.500000D-1, 4.750000D-1, 
     +  5.000000D-1, 5.250000D-1, 5.500000D-1, 5.750000D-1,
     +  6.000000D-1, 6.250000D-1, 6.500000D-1, 6.750000D-1,
     +  7.000000D-1, 7.250000D-1, 7.500000D-1, 7.750000D-1,
     +  8.000000D-1, 8.250000D-1, 8.500000D-1, 8.750000D-1,
     +  9.000000D-1, 9.250000D-1, 9.500000D-1, 9.750000D-1,
     +  1.000000D+0 /

C CALCULATE SPLINE COEFFICIENTS.
      IF((IREAD(1).NE.1).AND.(IREAD(2).EQ.ISET)) GO TO 20

C READ GRID DATA AND CALCULATE SPLINE COEFFICIENTS. 
      IF(ISET.EQ.1) THEN
        OPEN(UNIT=IFILE,FILE='aacnlo08_posi.grd',STATUS='OLD')
        OPEN(UNIT=IFILE+1,FILE='grad_posi.grd',STATUS='OLD')
        NPAR=11

      ELSE IF(ISET.EQ.2) THEN
        OPEN(UNIT=IFILE,FILE='aacnlo08_node.grd',STATUS='OLD')
        OPEN(UNIT=IFILE+1,FILE='grad_node.grd',STATUS='OLD')
        NPAR=11

      ELSE
        WRITE(*,1010) ISET
 1010   FORMAT(' ','AAC08PDF ERROR: ISET =', I3)
        STOP
      END IF

      DO I=1,11
        DO J=1,11
          EM(I,J)=EMI(I,J,ISET)
        ENDDO
      ENDDO

      DO J=1,NQ
        DO K=1,NX-1
          READ(IFILE,1025) (PDFG(K,J,I), I=1,6)
          DO NR=1,NPAR
            NI=8+NPDF*(NR-1)
            READ(IFILE+1,1025) (PDFG(K,J,I),I=NI,NI+NPDF-1)
          ENDDO
        ENDDO
      ENDDO

 1025 FORMAT(1X,6(1PE14.5))
      CLOSE(IFILE)
      CLOSE(IFILE+1)

      DO I=1,ND
        DO J=1,NQ
          PDFG(NX,J,I)=0.D0 ! x=1 XPPDF=0.D0
          CALL LSPLINE(NX,XG,PDFG,BXG,CXG,DXG,ISET,I,J)
        ENDDO
      ENDDO

      IREAD(1)=2
      IREAD(2)=ISET
   20 CONTINUE

      DO I=1,7
        XPPDF(I-4)=0.D0
      END DO

      DO J=1,11
        DO I=1,6
          GRADFF(I,J)=0.D0
        ENDDO
      ENDDO

C CHECK X AND Q2 VALUES.
      IF((X.LT.1.D-9).OR.(X.GT.1.D0)) THEN
        WRITE(*,1030) X
 1030   FORMAT (' ','WARNING: OUT OF RANGE --> X =', 1PE12.3)
        STOP
      ENDIF
      IF((Q2.LT.1.D0).OR.(Q2.GT.1.D8)) THEN
        WRITE(*,1040) Q2
 1040   FORMAT (' ','WARNING: OUT OF RANGE --> Q2 =', 1PE12.3)
        STOP
      ENDIF

C INTERPOLATION.
C X: CUBIC SPLINE INTERPOLATION, LOG(Q2): LINEAR INTERPOLATION.
      J=ISERCH(NQ,QG,Q2)
      IF(J.EQ.NQ) J=NQ-1
      K=ISERCH(NX,XG,X)
      DO I=1,ND
        DX=X-XG(K)
        PDFJ1(I)=PDFG(K,J,I)
     >       +DX*(BXG(K,J,I)+DX*(CXG(K,J,I)+DX*DXG(K,J,I)))
        PDFJ2(I)=PDFG(K,J+1,I)
     >       +DX*(BXG(K,J+1,I)+DX*(CXG(K,J+1,I)+DX*DXG(K,J+1,I)))
      ENDDO
 
C -- Polarized PDFs --
      T=(DLOG(Q2)-DLOG(QG(J)))/(DLOG(QG(J+1))-DLOG(QG(J)))
      DO I=1,3
        XPPDF(I-1)=(1.D0-T)*PDFJ1(I)+T*PDFJ2(I)     ! g, u, d
        XPPDF(-I)=(1.D0-T)*PDFJ1(I+3)+T*PDFJ2(I+3)  ! ub, db, sb
      ENDDO
      XPPDF(3)=XPPDF(-3)

C -- Gradient terms of parameters for polarized PDFs --
      DO J=1,NPAR
        DO I=1,NPDF
          NI=7+NPDF*(J-1)
          GRADFF(I,J)=(1.D0-T)*PDFJ1(NI+I)+T*PDFJ2(NI+I)
        ENDDO
      ENDDO

      DO J=1,NPAR
        DO I=1,3
          GRAD(I-1,J)=GRADFF(I,J)   ! g, u, d
          GRAD(-I,J)=GRADFF(I+3,J)  ! ub, db, sb
        ENDDO
        GRAD(3,J)=GRAD(-3,J) ! s=sb
      ENDDO

      RETURN
      END
C ---------------------------------------------------------------------
      SUBROUTINE LSPLINE(N,X,Y,B,C,D,ISET,I,J)
C ---------------------------------------------------------------------
C CALCULATE THE COEFFICIENTS B,C,D IN A CUBIC SPLINE INTERPOLATION.
C INTERPOLATION SUBROUTINES ARE TAKEN FROM
C G.E. FORSYTHE, M.A. MALCOLM AND C.B. MOLER,
C COMPUTER METHODS FOR MATHEMATICAL COMPUTATIONS (PRENTICE-HALL, 1977).
      IMPLICIT REAL*8(A-H,O-Z)
      PARAMETER (NQ=33, NX=117, ND=74)
      DIMENSION Y(NX,NQ,ND),B(NX,NQ,ND),C(NX,NQ,ND),D(NX,NQ,ND)
     1         ,X(NX) 
      NM1=N-1
      IF(N.LT.2) RETURN
      IF(N.LT.3) GO TO 250
      D(1,J,I)=X(2)-X(1)
      C(2,J,I)=(Y(2,J,I)-Y(1,J,I))/D(1,J,I)
      DO 210 K=2,NM1
        D(K,J,I)=X(K+1)-X(K)
        B(K,J,I)=2.0D0*(D(K-1,J,I)+D(K,J,I))
        C(K+1,J,I)=(Y(K+1,J,I)-Y(K,J,I))/D(K,J,I)
        C(K,J,I)=C(K+1,J,I)-C(K,J,I)
  210 CONTINUE
      B(1,J,I)=-D(1,J,I)
      B(N,J,I)=-D(N-1,J,I)
      C(1,J,I)=0.0D0
      C(N,J,I)=0.0D0
      IF(N.EQ.3) GO TO 215
      C(1,J,I)=C(3,J,I)/(X(4)-X(2))-C(2,J,I)/(X(3)-X(1))
      C(N,J,I)=C(N-1,J,I)/(X(N)-X(N-2))-C(N-2,J,I)/(X(N-1)-X(N-3))
      C(1,J,I)=C(1,J,I)*D(1,J,I)**2.0D0/(X(4)-X(1))
      C(N,J,I)=-C(N,J,I)*D(N-1,J,I)**2.0D0/(X(N)-X(N-3))
  215 CONTINUE
      DO 220 K=2,N
        T=D(K-1,J,I)/B(K-1,J,I)
        B(K,J,I)=B(K,J,I)-T*D(K-1,J,I)
        C(K,J,I)=C(K,J,I)-T*C(K-1,J,I)
  220 CONTINUE
      C(N,J,I)=C(N,J,I)/B(N,J,I)
      DO 230 IB=1,NM1
        K=N-IB
        C(K,J,I)=(C(K,J,I)-D(K,J,I)*C(K+1,J,I))/B(K,J,I)
  230 CONTINUE
      B(N,J,I)=(Y(N,J,I)-Y(NM1,J,I))/D(NM1,J,I)
     1        +D(NM1,J,I)*(C(NM1,J,I)+2.0D0*C(N,J,I))
      DO 240 K=1,NM1
        B(K,J,I)=(Y(K+1,J,I)-Y(K,J,I))/D(K,J,I)
     1          -D(K,J,I)*(C(K+1,J,I)+2.0D0*C(K,J,I))
        D(K,J,I)=(C(K+1,J,I)-C(K,J,I))/D(K,J,I)
        C(K,J,I)=3.0D0*C(K,J,I)
  240 CONTINUE
      C(N,J,I)=3.0D0*C(N,J,I)
      D(N,J,I)=D(N-1,J,I)
      RETURN
  250 CONTINUE
      B(1,J,I)=(Y(2,J,I)-Y(1,J,I))/(X(2)-X(1))
      C(1,J,I)=0.0D0
      D(1,J,I)=0.0D0
      B(2,J,I)=B(1,J,I)
      C(2,J,I)=0.0D0
      D(2,J,I)=0.0D0
      RETURN
      END
C ---------------------------------------------------------------------
      INTEGER FUNCTION ISERCH(N,X,Y)
C ---------------------------------------------------------------------
C THIS FUNCTION SEARCHES "I" WHICH SATISFIES THE RELATION
C X(I) <= Y < X(I+1) BY USING A BINARY SEARCH.
      IMPLICIT REAL*8(A-H,O-Z)
      DIMENSION X(118)

      MIN=1
      MAX=N+1

   10 CONTINUE
      MID=(MIN+MAX)/2
      IF(Y.LT.X(MID)) THEN
        MAX=MID
      ELSE
        MIN=MID
      END IF
      IF((MAX-MIN).GT.1) GO TO 10

      ISERCH=MIN

      RETURN
      END
C *********************************************************************
C THE END OF THE PROGRAM.
C *********************************************************************
