# Hello World

環境: MacOS + VSCode + STM32CubeMX

## Build environment

中国語なんだけど一応これを参照した：https://zhuanlan.zhihu.com/p/690345624

1. **VSCodeのインストール**
    
    ここで省略する。ただし、PlatformIO IDEというpluginのインストールは忘れずに。

2. **STM32CubeMXのインストール**

    ダウンロード先：https://www.st.com/en/development-tools/stm32cubemx.html

   　ここでSTのアカウントを作ってない場合は作成する

   　（メールアドレスは任意）（アカウントなしでもダウンロード可能だがSTM32CubeMXで必要になる）

    ダウンロードの際の設定は全てデフォルトで大丈夫。

4. **プロジェクトの基本設定**

    - File -> New Project
    - Commercial Part NumberのところにSTM32F446REと入れたら、右下のところのいくつかの選択肢の中に、BoardのところにNucleo~~~とある選択肢があるはず。そこでクリック。
    - System CoreのSysをクリックし、DebugをSerial Wireにする。
    - Nucleoでテストするなら、ConnectivityでUSART2をAsynchronousにする。
    - Project Manager（上のバーをクリック）で自分のプロジェクトの名前やパスを設定する。Toolchain/IDEのところはMakefileを選択する。
    - Code Generatorでcopy only the necessary library filesを選択する。
    - Generate Code!（右上）

5. **VSCodeの設定**

    - (まずPlatformIOというextensionをインストールする。)
    - 左のバーのところのアリのアイコンをクリックし、PIO HomeのOpenをクリックした後、New Projectをクリックする。
    - name(任意),board→STM32F446RE,framework→STM32Cubeに設定し、LocationのUse default locationのチェックを外し、STM32CubeMXで作ったフォルダーのパスにしてFinishを押す。
    -　その後、STM32CubeMXで作ったフォルダーの中にさっきPlatformIOで入力した名前のファイルがあるため、中のファイルなどすべてをそのファイルからコピーして、一つ外に出す。
    不要となった空のフォルダーは削除する。
    - そしてこのリポジトリに似ているコードのストラクチャーになる。
    
    重要：platformio.iniファイルに、以下の内容を一番下に追加する
    >upload_protocol = stlink
    >
    >[platformio]\
    >src_dir = Core/Src\
    >include_dir = Core/Inc

## BMP390 Test

リポジトリの`Code/Inc/`の`bmp390.h`と、`Code/Src`の`bmp390.c`と`main.c`をコピペする。

そして、BMP390をNucleo F446REに接続する。このプログラムでは、SPIの各PINの対応関係は以下となる：

    - SCK -> PA5
    - SDO -> PA6
    - SDI -> PA7
    - CS -> PA9

プログラムを実行する前に、まずリポジトリの`main.py`というPythonファイルをコピペする。`main.py`の変数`port`を自分のUSBポートに変更した後に実行し、Nucleo側のprintfをターミナルでプリントすることができる。

そしてテストプログラムをNucleoに書き込み、ターミナルでBMP390からの情報がプリントされたかを確認する。
