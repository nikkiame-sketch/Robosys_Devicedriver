# Robosys_Devicedriver
2021年度ロボットシステム学 課題１
ラズベリーパイに接続された7セグメントLEDに文字を表示させる。a~hまでの表示に対応しています。

上田先生のプログラム
https://github.com/ryuichiueda/robosys_device_drivers
に必要な処理を追記しました。

動作環境:Raspberry pi 3b,ubuntu20.04LTS



使用前にソースコード内のsegment配列に、LEDが接続されているGPIOの番号を格納する必要があります。

文字を表示するには、表示させたいa~hまでの小文字を1文字デバイスファイルに格納します。

表示を消すには、小文字のxをデバイスファイルに格納します。
