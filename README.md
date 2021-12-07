# Robosys_Devicedriver
2021年度ロボットシステム学 課題１
ラズベリーパイに接続された7セグメントLEDに文字を表示させる。a~hまでの表示に対応しています。

上田先生のプログラム
https://github.com/ryuichiueda/robosys_device_drivers
に必要な処理を追記しました。

動作環境:Raspberry pi 3b,ubuntu20.04LTS

実行方法
""
1.ソースコード内のsegment配列に、LEDが接続されているGPIOの番号を格納する。

2.ソースコードをビルドし、myled.koを作成する。

3.sudo insmod myled.ko

4.sudo mknod /dev/myled0 c メジャー番号　0

5.sudo chmod 666 /dev/myled0

6.echo 表示させる文字　> /dev/myled0
""


文字を表示するには、表示させたいa~hまでの小文字を1文字デバイスファイルに格納します。

表示を消すには、小文字のxをデバイスファイルに格納します。
