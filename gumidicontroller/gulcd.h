#ifndef guLcd_H
#define guLcd_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "guhelpers.h"
#include "guprogramscfg.h"
#include "guaction.h"

namespace Gu::Lcd {

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
  
  private:
  String prepareLineToPrint(const String& source);
  void updateTimeoutPrint(timeMilliSeconds now);
  void updateBacklight(timeMilliSeconds now);
  void printWithProgramHalt(const String& first, const String& second, timeMilliSeconds duration = twoSeconds);
  void printBothLines(const String& first, const String& second, timeMilliSeconds duration = 0);
  timeMilliSeconds lcdBacklightTime;
  timeMilliSeconds lcdTimeoutPrintTime;
  bool alwaysOn;
  bool isdisplay;
  bool isbacklight;
  LiquidCrystal_I2C lcd;
  lcdDimention rows, cols;
};
} // namespace Gu::Lcd
/*
//TODO: TO nie działa
namespace Gu::Actions::Lcd {
  constexpr Action EnableAlwaysOn(const Gu::Lcd::guLcd& lcd) { return { [=](){ lcd.enableAlwaysOn(); }, "LAE" }; };
  constexpr Action DisableAlwaysOn(const Gu::Lcd::guLcd& lcd) { return { [=](){ lcd.disableAlwaysOn(); }, "LAD" }; };
  constexpr Action ToggleAlwaysOn(const Gu::Lcd::guLcd& lcd) { return { [=](){ lcd.toggleAlwaysOn(); }, "LAT" }; };
  constexpr Action print2ndLineWithTimeout(const Gu::Lcd::guLcd& lcd, const String& text) { return { [=](){ lcd.print(text); }, "LPT" }; };
  constexpr Action ExtendLcdBacklight(const Gu::Lcd::guLcd& lcd, timeMilliSeconds timeMs = oneMinute) { return { [=](){ lcd.extendLcdBacklight(timeMs); }, "LEB" }; };
}
*/
#endif
