/*
Gu::Options holds all hardware specific constants and configs
*/
#pragma once
#ifndef guOptions_H
#define guOptions_H
#include "guhelpers.h"
#include "guhwapi.h"
#include "gujackselector.h"
#include "gulcd.h"
#include "gumidi.h"
#include "guprogramscfg.h"

namespace Gu::Options {

const char* ProductName = "GuMIDI mk.3";
const char* ProductVersion = "0.2020.11.10";
constexpr byte numberOfUserButtons = 4;
constexpr byte numberOfControlButtons = 5;

constexpr eepromAddress eepromAddrLastProgram = 0;
constexpr eepromAddress eepromAddrOutputSelector = 1;

constexpr SerialBaudRate serialBaud = 115200;
constexpr Gu::Lcd::i2cAddress lcdI2C = 0x27;
constexpr Gu::Lcd::lcdDimention lcdCols = Gu::Lcd::LcdCols16;
constexpr Gu::Lcd::stringBufferSize lcdStringSize = lcdCols + sizeof("");
constexpr Gu::Lcd::lcdDimention lcdRows = Gu::Lcd::LcdRows2;
constexpr timeMilliSeconds extendLcdBacklightTimeout = oneMinute;
constexpr timeMilliSeconds lcdTimeoutPrintTime = oneSecond; 

constexpr Pin outputSelectorOutputAPin = Pin04;
constexpr Pin outputSelectorOutputBPin = Pin05;

constexpr Gu::HWApi::guHwConfig <numberOfUserButtons, numberOfControlButtons> hwCfg {
  { { UserBtnId(0), Pin06 },
    { UserBtnId(1), Pin07 },
    { UserBtnId(2), Pin08 },
    { UserBtnId(3), Pin09 } },

  { { CtrlBtnId(0), Pin10 },
    { CtrlBtnId(1), Pin16 },
    { CtrlBtnId(2), Pin18 },
    { CtrlBtnId(3), Pin19 },
    { CtrlBtnId(4), Pin20 } },
};



}

#endif
