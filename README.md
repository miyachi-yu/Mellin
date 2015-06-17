$Id: README,v 1.5 2008/09/25 08:37:43 miyachi Exp $
*****************************************************************************
****                      README for Mellin Package                      ****
****   $Revision: 1.5 $
****   $Date: 2008/09/25 08:37:43 $
****   $Author: miyachi $
****                                                                     ****
*****************************************************************************

* $B%G%#%l%/%H%j$N9=B$(B

$B!!%i%$%V%i%jKh$K%5%V%G%#%l%/%H%j$,$D$/$i$l$F$$$k!#(B
$B!!(BTranform       Mellin$B$*$h$S5U(BMellin$BJQ49%k!<%A%s$H$=$l$KIU?o$9$k4XO"(B
                 $B%k!<%A%s!#(B

  example        $B%/%i%9%i%$%V%i%j$N;HMQNc(B
  
  doc            $B4XO">pJs!"%j%U%!%l%s%9%^%K%e%"%kEy(B

  include/lib    $B%Q%C%1!<%8$N%S%k%I;~$K<+F0E*$K@8@.$5$l$k(B

* $B:G=i$K$9$k;v(B

  -- $BI,MW$J%Q%C%1!<%8$N=`Hw(B

$B!!(B- aclocal, autoheader, autoconf, automake, make, pkg-config, libxml++-2.6
    doxygen, cernlib ( cfortran ), db4-dev, libtool $BEy(B
    
$B!!4D6-@_Dj$K(B aclocal$B!"(Bautoheader$B!"(Bautomake$B!"(Bautoconf$B!"(Blibtool $BEy$r;H$C$F$$$^$9!#(B
$B!!$^$?%j%U%!%l%s%9%^%K%e%"%k$O(Bdoxygen$B$r;H$$$^$9!#K:$l$:$K%$%s%9%H!<%k(B
$B!!$7$^$7$g$&!#(B

  Utility/Config.{hh,cc} $B$O(Blibxml++-2.6$B$rMxMQ$7$F$$$^$9!#(B
$B!!(Blibxml++-2.6 libxml++-2.6-dev $BEy$r%$%s%9%H!<%k$7$F$*$-$^$7$g$&!#(B

$B!!(Bexample/mellin$B$N%3%s%Q%$%k$K(Bcfortran$B%Q%C%1!<%8$,I,MW$G$9!#(B
$B!!7W;;%N!<%I$K$O%$%s%9%H!<%k$5$l$F$$$^$9$,!"8D?MMxMQ$N(BPC$B$G$O(B
$B!!DL>o%$%s%9%H!<%k$5$l$^$;$s!#(BDeabin$B!"(BFedora$B6&$K:G6a$O%Q%C%1!<%8$,(B
$B!!MQ0U$5$l$F$$$k$N$G!"(BCERNlib$B4XO"$N$b$N$r$H$j$"$($:%$%s%9%H!<%k(B
$B!!$7$F$*$-$^$7$g$&!#(Byum$B$d(Bapt$B$G(Bcernlib$B$r8!:w$7$F!"(Bdevel$BHG$b$U$/$a$F(B
$B!!$H$j$"$($:%$%s%9%H!<%k$7$F$/$@$5$$!#(B

	
  -- $B4D6-@_Dj(B
  $ aclocal
  $ libtoolize --force --copy
  $ autoheader
  $ automake --add-missing --copy --force-missing
$B!!(B$ autoconf --force
$B!!(B$ ./configure

$B!!$G$9!#%3%s%Q%$%k$9$k4D6-$,$+$o$C$?$i!"K:$l$:$K!#(B

  Mac OS $B$G$O(B Fink$B$rMxMQ$7$F!"%3%s%Q%$%iEy(B(gcc-4$B!"(Bg++-4$B!K$rCV$-49$($^$9!#(B
  $ CPPFLAGS=-I/sw/include LDFLAGS=-L/sw/lib CC=gcc-4 CXX=g++-4 ./configure
  $B$H$7$F$/$@$5$$!#(B
  
* $B%3%s%Q%$%k$N$7$+$?(B

  $  make
  $B$G(Binclude/lib$B$NCf$K%X%C%@!<%U%!%$%k!"6&M-%i%$%V%i%j$,%$%s%9%H!<%k(B
$B!!$5$l$^$9!#(B

* $B%W%m%0%i%`<B9T$N$7$+$?(B

  example$B%G%#%l%/%H%j$K$G$-$k<B9T%U%!%$%k$rNc$K$H$C$F@bL@$7$^$9!#(B
  make $B$r$9$k$H!"(Bexample/mellin$B$,$G$-$^$9!#$3$l$O(BTransform$B%i%$%V%i%j(B
$B!!$N%5%s%W%k%W%m%0%i%`$G$9!#Nc$($P(B
  $  ./example/mellin
$B!!$H%3%^%s%I$rF~NO$7$F$_$F$/$@$5$$!#$b$7%i%$%V%i%j$,$J$$$h$H$$$&(B
$B!!%(%i!<$,I=<($5$l$?$i!"6&M-%i%$%V%i%j$N$"$k%G%#%l%/%H%j$,(B
  LD_LIBRARY_PATH
$B!!$KF~$C$F$$$J$$$N$,860x$G$9!#<+F0E*$K@_Dj$9$k%9%/%j%W%H(B
$B!!(Bsetup.sh
  $B$,$"$j$^$9!#FI$_9~$_$^$7$g$&!#(B
$B!!(B$  source setup.sh

  LD_LIBRARY_PATH $B$,@5$7$/@_Dj$5$l$F$$$l$P$"$H$O%3%^%s%I$r;XDj$7$F(B
$B!!<B9T$9$l$P$h$$$G$7$g$&!#(B

* $B%j%U%!%l%s%9%^%K%e%"%k(B

  $  make doc
  $B$G%j%U%!%l%s%9%^%K%e%"%k$,@8@.$5$l$^$9!#%j%U%!%l%s%9%^%K%e%"%k$r:n$k(B
$B!!$K$O(B doxygen $B$,I,MW$G$9!#(B


$B!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v(B
$B!v!!!!!!!!!!!!!!!!!!!!!!!!!!!!%W%m%0%i%`3+H/!!(B
$B!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v!v(B

$B!v!!?75,%i%$%V%i%j$N9=C[J}K!(B (Makefile.in) $B$N=q$-J}(B

$B!!(B-- Makefile.in $B$N@bL@(B

$B!!%i%$%V%i%j:n@.$N$?$a$N6&DL%k!<%k$O(B Rules.mk.in$B!"(BLibs.mk.in $B$K4^$^$l$F$$$k(B
$B!!$N$G!"3F%i%$%V%i%j$G$O(Bmake$B$N4D6-JQ?t$rE,@Z$K@_Dj$9$k$@$1$G!"%i%$%V%i%j$N(B
$B!!9=C[$,$G$-$k$h$&$K$J$C$F$$$^$9!#Nc$H$7$F(B Tranform/Makefile.in $B$r8+$F$/$@(B
$B!!$5$$!#(B
$B!!(B
$B!!!{!!%i%$%V%i%j9=C[MQ4D6-JQ?t(B

$B!!(BLIBRARY$B!!!!!!(B $B!!%i%$%V%i%j$NL>A0!#@_DjCM$r;H$C$F(B lib$(LIBRARY).so
$B!!!!!!!!!!!!!!!!!!$,$D$/$i$l$k!#(B
$B!!(BLIB_OBJS $B!!!!!!(B $B%i%$%V%i%j$K4^$`$Y$-%*%V%8%'%/%H%U%!%$%k%j%9%H!#(B
$B!!!!!!!!!!!!!!!!!!$3$NJQ?t$KNs5-$5$l$?%*%V%8%'%/%H$,%3%s%Q%$%k$5$l!"(B
$B!!!!!!!!!!!!!!!!!!%i%$%V%i%j$KEPO?$5$l$k!#(B
$B!!(BEXTRA_INC$B!!!!!!(B $BI8=`0J30$N%3%s%Q%$%k%*%W%7%g%s!#(B
$B!!!!!!!!!!!!!!!!!!I,MW$J%$%s%/%k!<%I%G%#%l%/%H%jEy$,$"$l$P@_Dj$9$k!#(B
$B!!(BINTERFACE$B!!!!!!(B LIB_OBJS$B$K$"$k$b$N$K$D$$$F$O!"BP1~$9$k%X%C%@%U%!%$%k(B
$B!!!!!!!!!!!!!!!!!!$O<+F0E*$K(Binclude$B%G%#%l%/%H%j$K%$%s%9%H!<%k$5$l$k!#(B
$B!!!!!!!!!!!!!!!!!!$=$l0J30!"Nc$($P(Bcc$B%U%!%$%k$,$J$$$b$NEy!"$G%$%s%9%H!<%k(B
$B!!!!!!!!!!!!!!!!!!$,I,MW$J$b$N$O$3$3$K2C$($k!#(B

$B!!!{!!%F%9%HMQ%W%m%0%i%`:n@.MQJQ?t(B

$B!!(BUSE_*$B!!!!!!!!!!(B $BNc$($P(BlibQCD.so $B$,I,MW$G$"$l$P(B USE_QCD = yes $B$H$9$k!#(B
$B!!!!!!!!!!!!!!!!!!$=$l$>$l$N%i%$%V%i%j$GI,MW$H$5$l$kDI2C%i%$%V%i%jEy$O(B
$B!!!!!!!!!!!!!!!!!!<+F0E*$K@_Dj$5$l$^$9!#(B
$B!!(BTARGET$B!!!!!!!!!!:n@.$9$k<B9T%U%!%$%k0lMw(B
$B!!(BOBJS$B!!!!!!!!!!!!%i%$%V%i%j$K4^$a$J$$%*%V%8%'%/%H0lMw(B
$B!!(BEXTRA_LIB$B!!!!!!(B $BI,MW$JDI2C%i%$%V%i%j$N$?$a$N%j%s%+%*%W%7%g%s(B


$B!v!!?75,%i%$%V%i%j$NEPO?(B

$B!!!{!!(Bconfigure.ac $B$K(B Makefile$B$rDI2C(B

$B!!(Bconfigure.ac $B$N2<It$K(B configure $B%9%/%j%W%H$G:n$i$l$k%U%!%$%k$N0lMw(B
$B!!$,$"$j$^$9!#$=$3$K$"$i$?$K(BMakefile$B$rDI2C$7$^$9!#(B

$B!!!{!!(BLibs.mk.in $B$NJT=8(B

$B!!:n@.$7$?%i%$%V%i%j$rB>$+$iMxMQ$G$-$k$h$&$K$9$k$?$a$K!"0MB8%k!<%k$r(B
$B!!@_Dj$7$^$9!#B>$N$b$N$r;29M$KDI2C$7$F$/$@$5$$!#0MB8@-$N9b$$$b$N$r=i$a$K(B
$B!!0MB8@-$NDc$$$b$N$r8e$NJ}$KDI2C$7$^$9!#(B

$B!!I,MW$H$9$k%i%$%V%i%j$,$"$l$P!"B>$r;29M$K@_Dj$7$^$9!#(B
