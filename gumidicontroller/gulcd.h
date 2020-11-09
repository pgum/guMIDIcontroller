#ifndef guLcd_H
#define guLcd_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "guhelpers.h"
#include "guprogramscfg.h"

using stringLength = byte;
using i2cAddress = byte;
using lcdDimention = byte;
using stringBufferSize = byte;
using lcdRowIndex = byte;

constexpr lcdDimention LcdRows2 = 2;
constexpr lcdDimention LcdCols16 = 16;

class guLcd {
  static constexpr lcdRowIndex firstLine = 0;
  static constexpr lcdRowIndex secondLine = 1;
  static constexpr lcdRowIndex maxRows = 2;
  
  public:
  String lastPermanentLine[maxRows];
  guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols);
  void init();
  void intro(const String& date, const String& time, const String& name, const String& ver, const String& ProgramsAndButtons);
  void print(const String& text, lcdRowIndex line = secondLine, timeMilliSeconds duration = 0); 
  void printWithTimeout(const String& text, timeMilliSeconds timeMs = oneSecond, lcdRowIndex line = secondLine); 
  void toggleDisplay();
  bool isDisplay();
  void noDisplay();
  void display();
  bool isBacklight();
  void noBacklight();
  void backlight();
  void enableAlwaysOn();
  void disableAlwaysOn();
  void toggleAlwaysOn();
  void restoreLine(lcdRowIndex line = secondLine);
  void extendLcdBacklight(timeMilliSeconds timeMs = oneMinute);
  void update();
  void printProgram(const guProgramConfig<4>* program);
  
  private:
  String prepareLineToPrint(const String& source);
  void updateTimeoutPrint(timeMilliSeconds now);
  void updateBacklight(timeMilliSeconds now);
  void printWithProgramHalt(const String& first, const String& second, timeMilliSeconds duration = twoSeconds);
  timeMilliSeconds lcdBacklightTime;
  timeMilliSeconds lcdTimeoutPrintTime;
  bool alwaysOn;
  bool isdisplay;
  bool isbacklight;
  LiquidCrystal_I2C lcd;
  lcdDimention rows, cols;
};

#endif
