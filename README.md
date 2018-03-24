# THETA S Remote Shutter on M5Stack (ESP32/Arduino)

制作中
work in progress.

M5Stack で動作する THETA S 用のリモートシャッターです。

## THETA S を見つける

事前に THETA S の電源をいれて、WiFi をオンにしてください。
FIND(ボタンB)を押すと THETA S を探しにいきます。
見つけた THETA S の SSID は M5Stack の EEPROM に保存されるので、
次回起動時からはこの SSID が使われます。

## THETA S と接続する

CONN.(ボタンB)を押すと、THETA S と接続しにいきます。
接続が終ると OK とメッセージが表示されます。

## 撮影する

THETA S と接続後、THETA(ボタンB)を押すと THETA S のシャッターを切ります。
THETA S 側の処理を待って、保存された画像のファイル名が表示されれば撮影完了です。

DISCN.(ボタンA)を押すと、THETA S との接続を切ります。
