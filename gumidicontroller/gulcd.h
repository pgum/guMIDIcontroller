#ifndef guLCD_H
#define guLCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "guhelpers.h"

using i2cAddress = byte;
using lcdDimention = byte;
using stringBufferSize = byte;

class guLcd {
  public:

  String delayString;
  guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols);
  void init();
  void intro(const String date, const String time, const String name, const String ver, byte buttons);
  void printMidiSend(const byte id, String text);
  void printProgramChange(const String status, const String tooltip);
  void printSecondLine(String text); 
  void printWithDelay(String text, timeMilliSeconds timeMs = 0); 
  bool isBacklight();
  void noBacklight();
  void backlight();
  void enableAlwaysOn();
  void disableAlwaysOn();
  void extendLcdBacklight(timeMilliSeconds timeMs = 0);
  void updateDelayPrint(timeMilliSeconds now);
  void updateBacklight(timeMilliSeconds now);
  void update();
  void setExtendLcdBacklightMs(timeMilliSeconds timeMs);
  void setLcdRedrawTooltipAfterMs(timeMilliSeconds timeMs);
  private:
  unsigned long lcdBacklightTime;
  unsigned long lcdDelayPrintTime;
  timeMilliSeconds extendLcdBacklightMs;
  timeMilliSeconds lcdRedrawTooltipAfterMs;
  bool alwaysOn;
  bool isbacklight;
  void printFirstLine(String text);
  void printIntroScreen(const String first, const String second);
  
  LiquidCrystal_I2C lcd;
};

#endif
