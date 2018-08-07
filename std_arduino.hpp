/*
 * std_arduino.hpp
 * ArduinoをC/C++で実装するに当たって必須のヘッダファイル
 */
#ifndef M5STACK_H
#define M5STACK_H
#include <M5Stack.h>
#endif

#ifndef STD_ARDUINO_HPP
#define STD_ARDUINO_HPP

#include "Arduino.h"    // ここでArduino.hをインクルードする

// Arduinoで必須な関数をここで宣言しておく。
void setup();
void loop();

#endif //STD_ARDUINO_HPP
