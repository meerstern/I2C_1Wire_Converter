# I2C-1Wireマスタ変換基板
I2C_1Wire_Converter with DS2485

## 概要
  * [DS2485][1]とレベル変換回路を搭載したI2C-1Wireマスタ変換基板です  
  * I2Cスレーブを1Wireマスタに変換します  
  * タイミングがシビアな1WireデバイスをI2Cコマンドで容易に操作することが可能です  
  * DS2485のレジスタを変更することで1Wire通信のタイミングを細かく調整することが可能です  
  * DS2485は126バイトのバッファを備えており、効率良く1Wire通信をすることが可能です  
  * 本変換基板にはレベル変換回路を搭載しているため、5VレベルのI2C及び1Wireに対応しています  
  * DS2485のGPIOは電源入力のPMOSに接続されているため、1Wireデバイスの強力プルアップを利用することが可能です
  * I2Cの接続はGrove互換コネクタを使用しています  
  * 1Wireの接続はVCC、1Wire、GNDの2.54mmピッチターミナルコネクタを使用しています  
  * 1Wireの電源及び信号電圧レベルはGroveコネクタから供給する電源電圧に一致します  
  * 電源電圧：3.3V~5V
  * 基板サイズ25mm x30mm、固定穴M3x2 幅19mm
  
  
<img src="https://github.com/meerstern/I2C_1Wire_Converter/blob/main/IMG/img1.jpg" width="360">
   
<img src="https://github.com/meerstern/I2C_1Wire_Converter/blob/main/IMG/img2.jpg" width="360">
  
[1]: https://www.maximintegrated.com/jp/products/ibutton-one-wire/memory-products/DS2485.html
  
  
  
