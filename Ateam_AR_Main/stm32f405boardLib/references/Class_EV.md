# class EV(電磁弁クラス)

# 目次
- [列挙型](#列挙型)
    - [初期化用](##初期化用)
    - [その他](##その他)
- [クラス宣言](#クラス宣言)
- [メンバ関数](#メンバ関数)
    - [コンストラクタ](##コンストラクタ)
    - [setNewState](##setNewState関数)

# 列挙型
## 初期化用
- EvName
    - 設定する電磁弁ポートの名前を格納している列挙型です。
    この中で列挙されている名前と基板上での名前は一致しています。

    - 2019年高専ロボコン用基板では回路設計ミスによりEvNameの中身を変更しています。      ```VERSION_2019K```をdefineして使用してください。

    ```electric_valve.hpp
    #ifdef VERSION_2019K
    enum class EvName : uint16_t
    {
        EV0     = (0x0001 << 7),
        EV1     = (0x0001 << 6),
        EV2     = (0x0001 << 5),
        EV3     = (0x0001 << 4),
        EV4     = (0x0001 << 3),
        EV5     = (0x0001 << 2),
        EV6     = (0x0001 << 1),
        EV7     = (0x0001 << 0),
        EV8     = (0x0001 << 15),
        EV9     = (0x0001 << 14),
        EV10    = (0x0001 << 13),
        EV11    = (0x0001 << 12),
        EV12    = (0x0001 << 11),
        EV13    = (0x0001 << 10),
        EV14    = (0x0001 << 9),
        EV15    = (0x0001 << 8),
    };
    #else
    enum class EvName : uint16_t
    {
        EV0     = (0x0001 << 0),
        EV1     = (0x0001 << 1),
        EV2     = (0x0001 << 2),
        EV3     = (0x0001 << 3),
        EV4     = (0x0001 << 4),
        EV5     = (0x0001 << 5),
        EV6     = (0x0001 << 6),
        EV7     = (0x0001 << 7),
        EV8     = (0x0001 << 8),
        EV9     = (0x0001 << 9),
        EV10    = (0x0001 << 10),
        EV11    = (0x0001 << 11),
        EV12    = (0x0001 << 12),
        EV13    = (0x0001 << 13),
        EV14    = (0x0001 << 14),
        EV15    = (0x0001 << 15),
    };
    #endif
    ```

## その他
- EvState
    - 電磁弁の状態を表す列挙型です
    ```electric_valve.hpp
    enum class EvState { Set, Reset };
    ```

# クラス宣言
```electric_valve.hpp
template<EvName useEvName>
class EV
{
    ...
}
```
## テンプレート引数
- useEvName
    - 使用する電磁弁を指定します。

## サンプルコード
```sample.cpp
#include "electric_valve.hpp"

EV<EvName::EV5> ev5;
```

# メンバ関数
## コンストラクタ
```electric_valve.hpp
EV(){ ... }
```
EVクラスのデフォルトコンストラクタです。テンプレート引数を用いてConfigを行います。
- サンプルコード
    ```sample.cpp
    #include "electric_valve.hpp"

    EV<EvName::EV5> ev5;
    ```

## setNewState関数
```electric_valve.hpp
void setNewState(EvState newState){ ... }
```
電磁弁をON/OFFします。
- 引数
    - newState 
        - 設定するON/OFFを与えます。
        - 同ヘッダの中にある列挙型"EvState"を使用してください
- サンプルコード
    ```sample.hpp
    #include "electric_valve.hpp"  

    int main(void)
    {
        EV<EvName::EV3> ev3;                //インスタンス化

        ev3.setNewState(EvState::Set);      //電磁弁をONします
        ev3.setNewState(EvState::Reset);    //電磁弁をOFFします
        return 0;
    }
    ``` 

## デストラクタ
```electric_valve.hpp
virtual ~EV(){}
```
EVクラスのデストラクタです。