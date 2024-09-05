# Hello World

環境: MacOS + VSCode + STM32CubeMX

## Build environment

中国語なんだけど一応これを参照した：https://zhuanlan.zhihu.com/p/690345624

1. **VSCodeのインストール**
    
    ここで省略する。ただし、PlatformIO IDEというpluginのインストールは忘れずに。

2. **STM32CubeMXのインストール**

    ダウンロード先：https://www.st.com/zh/development-tools/stm32cubemx.html

    全てデフォルトで大丈夫。

3. **プロジェクトの基本設定**

    - File -> New Project
    - Commercial Part NumberのところにSTM32F446REと入れたら、右下のところのいくつかの選択肢の中に、Nucleoっぽい選択肢があるはず。そこでクリック。
    - System CoreのSysをクリックし、DebugをSerial Wireにする。
    - Project Managerで自分のプロジェクトの名前やパスを設定する。
    - Code Generatorでcopy only the necessary library filesを選択する。
    - Generate Code!

4. **VSCodeの設定**

    - 左のバーのところのアリのアイコンをクリックし、PIO HomeのOpenをクリックした後、New Projectをクリックする。
    - 色々設定。
    - そしてこのリポジトリに似ているコードのストラクチャーになる。
    
    重要：platformio.iniファイルに、以下の内容を一番下に追加する
    >; 烧录协议 默认未 stlink 根据自己具体的烧录工具进行选择\
    >; 参考：https://docs.platformio.org/en/latest/boards/ststm32/black_f407ve.html \
    >upload_protocol = stlink
    >
    >; 配置 源文件(src) 和 头文件(inc) 位置\
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