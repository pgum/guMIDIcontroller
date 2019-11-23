#ifndef guShift_H
#define guShift_H

#include <Arduino.h>
#include <AceButton.h>
#include <ButtonConfig.h>
#include <EEPROM.h>
#include "helpers.h"

using namespace ace_button;

constexpr byte eepromAddrLastProgram = 0;

struct guShift {
  static constexpr byte numOfModes = 7;
  
  byte Mode() const;
  void nextMode();
  void prevMode();
  void check();
  void init(ButtonConfig::EventHandler handler, byte prevPin, byte nextPin);
  String status();
  String tooltip();

  private:
  byte mode;
  AceButton shiftButtonPrev;
  AceButton shiftButtonNext;
  ButtonConfig shiftConfig;
  
  void initializeButtons(byte prevPin, byte nextPin);
  void saveProgramIndex();
  void loadProgramIndex();
};

#endif
