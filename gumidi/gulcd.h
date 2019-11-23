 #ifndef guLCD_H
#define guLCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "gumidi.h"

class guLcd {
  public:
 
  guLcd(byte i2cAddr, byte rows, byte cols);
  void intro(String date, String time, String name, String ver, int buttons);
  void printSendToggleMidi(const byte id, const MidiValue m);
  void printSendOnceMidi(const byte id, const MidiValue m);
  void printProgramChange(String status, String tooltip);
  private:
  void printSecondLine(String text);
  void printFirstLine(String text);
  void printIntroScreen(const String first, const String second);
  LiquidCrystal_I2C lcd;
};

#endif
