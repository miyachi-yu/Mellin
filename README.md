$Id: README,v 1.5 2008/09/25 08:37:43 miyachi Exp $
*****************************************************************************
****                      README for Mellin Package                      ****
****   $Revision: 1.5 $
****   $Date: 2008/09/25 08:37:43 $
****   $Author: miyachi $
****                                                                     ****
*****************************************************************************

* ディレクトリの構造

　ライブラリ毎にサブディレクトリがつくられている。
　Tranform       Mellinおよび逆Mellin変換ルーチンとそれに付随する関連
                 ルーチン。

  example        クラスライブラリの使用例
  
  doc            関連情報、リファレンスマニュアル等

  include/lib    パッケージのビルド時に自動的に生成される

* 最初にする事

  -- 必要なパッケージの準備

　- aclocal, autoheader, autoconf, automake, make, pkg-config, libxml++-2.6
    doxygen, cernlib ( cfortran ), db4-dev, libtool 等
    
　環境設定に aclocal、autoheader、automake、autoconf、libtool 等を使っています。
　またリファレンスマニュアルはdoxygenを使います。忘れずにインストール
　しましょう。

  Utility/Config.{hh,cc} はlibxml++-2.6を利用しています。
　libxml++-2.6 libxml++-2.6-dev 等をインストールしておきましょう。

　example/mellinのコンパイルにcfortranパッケージが必要です。
　計算ノードにはインストールされていますが、個人利用のPCでは
　通常インストールされません。Deabin、Fedora共に最近はパッケージが
　用意されているので、CERNlib関連のものをとりあえずインストール
　しておきましょう。yumやaptでcernlibを検索して、devel版もふくめて
　とりあえずインストールしてください。

	
  -- 環境設定
  $ aclocal
  $ libtoolize --force --copy
  $ autoheader
  $ automake --add-missing --copy --force-missing
　$ autoconf --force
　$ ./configure

　です。コンパイルする環境がかわったら、忘れずに。

  Mac OS では Finkを利用して、コンパイラ等(gcc-4、g++-4）を置き換えます。
  $ CPPFLAGS=-I/sw/include LDFLAGS=-L/sw/lib CC=gcc-4 CXX=g++-4 ./configure
  としてください。
  
* コンパイルのしかた

  $  make
  でinclude/libの中にヘッダーファイル、共有ライブラリがインストール
　されます。

* プログラム実行のしかた

  exampleディレクトリにできる実行ファイルを例にとって説明します。
  make をすると、example/mellinができます。これはTransformライブラリ
　のサンプルプログラムです。例えば
  $  ./example/mellin
　とコマンドを入力してみてください。もしライブラリがないよという
　エラーが表示されたら、共有ライブラリのあるディレクトリが
  LD_LIBRARY_PATH
　に入っていないのが原因です。自動的に設定するスクリプト
　setup.sh
  があります。読み込みましょう。
　$  source setup.sh

  LD_LIBRARY_PATH が正しく設定されていればあとはコマンドを指定して
　実行すればよいでしょう。

* リファレンスマニュアル

  $  make doc
  でリファレンスマニュアルが生成されます。リファレンスマニュアルを作る
　には doxygen が必要です。


＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
＊　　　　　　　　　　　　　　プログラム開発　
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊

＊　新規ライブラリの構築方法 (Makefile.in) の書き方

　-- Makefile.in の説明

　ライブラリ作成のための共通ルールは Rules.mk.in、Libs.mk.in に含まれている
　ので、各ライブラリではmakeの環境変数を適切に設定するだけで、ライブラリの
　構築ができるようになっています。例として Tranform/Makefile.in を見てくだ
　さい。
　
　○　ライブラリ構築用環境変数

　LIBRARY　　　 　ライブラリの名前。設定値を使って lib$(LIBRARY).so
　　　　　　　　　がつくられる。
　LIB_OBJS 　　　 ライブラリに含むべきオブジェクトファイルリスト。
　　　　　　　　　この変数に列記されたオブジェクトがコンパイルされ、
　　　　　　　　　ライブラリに登録される。
　EXTRA_INC　　　 標準以外のコンパイルオプション。
　　　　　　　　　必要なインクルードディレクトリ等があれば設定する。
　INTERFACE　　　 LIB_OBJSにあるものについては、対応するヘッダファイル
　　　　　　　　　は自動的にincludeディレクトリにインストールされる。
　　　　　　　　　それ以外、例えばccファイルがないもの等、でインストール
　　　　　　　　　が必要なものはここに加える。

　○　テスト用プログラム作成用変数

　USE_*　　　　　 例えばlibQCD.so が必要であれば USE_QCD = yes とする。
　　　　　　　　　それぞれのライブラリで必要とされる追加ライブラリ等は
　　　　　　　　　自動的に設定されます。
　TARGET　　　　　作成する実行ファイル一覧
　OBJS　　　　　　ライブラリに含めないオブジェクト一覧
　EXTRA_LIB　　　 必要な追加ライブラリのためのリンカオプション


＊　新規ライブラリの登録

　○　configure.ac に Makefileを追加

　configure.ac の下部に configure スクリプトで作られるファイルの一覧
　があります。そこにあらたにMakefileを追加します。

　○　Libs.mk.in の編集

　作成したライブラリを他から利用できるようにするために、依存ルールを
　設定します。他のものを参考に追加してください。依存性の高いものを初めに
　依存性の低いものを後の方に追加します。

　必要とするライブラリがあれば、他を参考に設定します。
