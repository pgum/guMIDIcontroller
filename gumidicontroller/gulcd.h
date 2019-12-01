#ifndef guLCD_H
#define guLCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


using i2cAddress = byte;
using lcdDimention = byte;
using stringBufferSize = byte;
using lcdStayAwakeTimeMS = unsigned long;

class guLcd {
  public:
  constexpr static lcdStayAwakeTimeMS extendLcdBacklightMs = 20000;
  constexpr static lcdStayAwakeTimeMS lcdRedrawTooltipAfterMs = 2000; 
  unsigned long lcdBacklightTime;
  unsigned long lcdDelayPrintTime;
  String delayString;
  guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols);
  void intro(const String date, const String time, const String name, const String ver, byte buttons);
  void printMidiSend(const byte id, String text);
  void printProgramChange(const String status, const String tooltip);
  void printSecondLine(String text); 
  void printWithDelay(String text, lcdStayAwakeTimeMS timeMs = lcdRedrawTooltipAfterMs); 
  //bool isBacklight();
  void noBacklight();
  void backlight();
  void enableAlwaysOn();
  void disableAlwaysOn();
  void extendLcdBacklight(lcdStayAwakeTimeMS timeMs = extendLcdBacklightMs);
  void update();
  private:
  bool alwaysOn;
  void printFirstLine(String text);
  void printIntroScreen(const String first, const String second);
  //bool isBacklit;
  LiquidCrystal_I2C lcd;
};

#endif
