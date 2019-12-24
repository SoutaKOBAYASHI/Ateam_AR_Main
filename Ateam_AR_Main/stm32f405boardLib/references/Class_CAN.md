# ControlAreaNetworkクラス

# 概要
Control Area Network (以下CAN) の機能を提供します。CANは弊部内ではモータドライバの通信、マイコン間の通信に利用されます。この規格の特徴としては、

- 最高1Mbpsの通信速度
- 差動通信
- デイジーチェーン(数珠つなぎ)可能
- アドレス指定可能

などがあげられます。

# 目次
- [列挙型](#列挙型)
    - [初期化用](##初期化用)
- [クラス宣言](#クラス宣言)
- [メンバ関数](#メンバ関数)
    - [コンストラクタ](##コンストラクタ)
    - [snedData関数](##sendData関数)
    - [sendRemote関数](##sendRemote関数)

# 列挙型
## 初期化用
- CAN_TimeQuanta
    - CANのビットセグメント長を表します。
    ``` control_area_network.hpp
    enum class CAN_TimeQuanta : uint8_t
    {
        tq1		= 0x00,
        tq2		= 0x01,
        tq3		= 0x02,
        tq4		= 0x03,
        tq5		= 0x04,
        tq6		= 0x05,
        tq7		= 0x06,
        tq8		= 0x07,
        tq9		= 0x08,
        tq10	= 0x09,
        tq11	= 0x0A,
        tq12	= 0x0B,
        tq13	= 0x0C,
        tq14	= 0x0D,
        tq15	= 0x0E,
        tq16	= 0x0F
    };
    ```

# クラス宣言
``` control_area_network.hpp
template<uint8_t setAddress, CAN_TimeQuanta BS1_timeQuanta = CAN_TimeQuanta::tq4, CAN_TimeQuanta BS2_timeQuanta = CAN_TimeQuanta::tq2, uint8_t prescaler = 6>
class ControlAreaNetwork
{
    ...
}
```

## テンプレート引数
- setAddress
    - 自身のアドレスを設定します。ここで設定されたアドレスのデータのみを受信します。

- BS1_timeQuanta
    - BS1のビットセグメント長を設定します。デフォルトで ```CAN_TimeQuanta::tq4``` に初期化されており、 ```PCLK = 42MHz``` の時は変更不要です。

- BS2_timeQuanta
    - BS2のビットセグメント長を設定します。デフォルトで ```CAN_TimeQuanta::tq2``` に初期化されており、 ```PCLK = 42MHz``` の時は変更不要です。

- prescaler
    - CANに供給するクロックの分周比を設定します。デフォルト ```prescaler = 6``` に初期化されており、 ```PCLK = 42MHz``` の時は変更不要です。

# メンバ関数
## コンストラクタ
``` control_area_network.hpp
ControlAreaNetwork() { ... };
```
ControlAreaNetworkクラスのデフォルトコンストラクタです。テンプレート引数を用いてピンの設定およびCANコントローラの初期化を行います。

- サンプルコード
    ``` sample.cpp
    #include "control_area_network.hpp"

    int main(void)
    {
        /*クロック初期化関数の後にインスタンス化すること*/
        ControlAreaNetwork<0x01, CAN_TimeQuanta::tq1, CAN_TimeQuanta::tq2, 4> can;

        return 0;
    }
    ```
また、不要なテンプレート引数を省略すると以下のように書けます。
- サンプルコード(一部テンプレート引数省略)
    ```sample.cpp
    #include "control_area_network.hpp"

    int main(void)
    {
        /*クロック初期化関数の後にインスタンス化すること*/
        ControlAreaNetwork<0x01> can;

        return 0;
    }
    ```
## sendData関数
``` control_area_network.hpp
void sendData(uint8_t *Data, uint8_t DataLenge, uint8_t Address)
{
    ...
}

template<size_t S>
void sendData(const std::array<uint8_t, S> &SendDataArray, uint8_t Address)
{
    static_assert( !(S > 8), "Size of SendDataArray has to be less than eight.");
    ...
}
```
- オーバーロード
    - void sendData(uint8_t *data, uint8_t dataLenge, uint8_t sendAddress);
        - C言語likeな配列を使用するオーバーロードです。
            - uint8_t *data
                - 送信する配列の先頭のポインタを入力してください。
            - uint8_t dataLenge
                - 送信する配列の配列長を入力してください。
            - uint8_t sendAddress
                - 送信先のアドレスを指定してください。

        - サンプルコード
        ```sample.cpp
        #include "control_area_network.hpp"

        int main(void)
        {
            ControlAreaNetwork<0x00> can;

            constexpr uint8_t sendAddress = 0x10;
            uint8_t sendDataArray[5] = {0x00, 0x10, 0x20, 0x40, 0x80};
            can.sendData(sendDataArray, sizeof(sendDataArray), sendAddress);
            return 0;
        }
        ```
    - void sendData(const std::array<uint8_t, S> &SendDataArray, uint8_t Address)
        - arrayクラスを使用するオーバーロードです。
            - const std::array<uint8_t, S>& sendDataArray
                - 送信するarrayを入力してください。ただし、arrayクラスのサイズは8byte以下としてください。8byteより大きなarrayを入力された場合、この関数はstatic_assartを返しコンパイルエラーとなります。
            - uint8_t sendAddress
                - 送信先のアドレスを指定してください。
        
        - サンプルコード
        ```sample.cpp
        #include <array>
        #include "control_area_network.hpp"

        int main(void)
        {
            ControlAreaNetwork<0x00> can;

            constexpr uint8_t sendAddress = 0x10;
            std::array<uint8_t, 5>sendDataArray = {0x00, 0x10, 0x20, 0x40, 0x50};
            can.sendData(sendDataArray, sendAddress);
             
             /*compile error!!*/
            //std::array<uint8_t, 10>sendDataArray_big
            //     = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00};
            //can.sendData(sendDataArray_big, sendAddress);
            //error: static assertion failed: Size of SendDataArray has to be less than eight.

            return 0;
        }
        ```
## sendRemote関数
```Control_area_network.hpp
void sendRemote(uint8_t Address)
{
    ...
}
```
引数で指定されたアドレスにリモートフレームを送信します。
- 引数
    - address 
        - リモートフレームを送信するアドレスを指定します。

- サンプルコード
    ``` sample.cpp
    #include "control_area_network.hpp"

    int main(void)
    {
        ControlAreaNetwork<0x00> can;

        constexpr uint8_t sendAddress = 0x10;
        can.sendRemote(sendAddress);

        return 0;
    }
    ```