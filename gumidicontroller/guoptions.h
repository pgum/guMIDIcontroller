/*
Gu::Options holds all hardware specific constants and configs
*/
#ifndef guOptions_H
#define guOptions_H
#include "guhelpers.h"
#include "gumidi.h"
#include "gulcd.h"

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

//TODO: constexpr guHwConfig <> hwCfg {
constexpr guHwConfig <numberOfUserButtons, numberOfControlButtons> hwCfg {
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

using namespace Gu::Actions::Midi;
using namespace Gu::Programs;
//constexpr byte numberOfPrograms = 9; //sizeof(programsConfigs)/sizeof(guProgramConfig<numberOfUserButtons>);

//error: 'constexpr Gu::Actions::Action Gu::Actions::Midi::CC(byte)' called in a constant expression
//error: 'constexpr Gu::Actions::Action Gu::Actions::Midi::CC(byte)' called in a constant expression
//to powinno pewnie pójść do *.ino
constexpr guProgramConfig<numberOfUserButtons> programsConfigs[] {
  { { Note(36), Note(37), Note(38), Note(39) }, "S8 ch1,2"},
  { { Note(41), Note(42), Note(43), Note(44) }, "S8 ch3,4"},
  { { Note(45), Note(46), Note(48), Note(49) }, "S8 ch5,6"},
  { { Note(50), Note(51), Note(52), Note(53) }, "S8 ch7,8"},
  { { Stop(), Play(), Loop(), Record() },       "Transprt"},
  { { CC(21), CC(22), CC(23), CC(24) },         " Reaper "},
  { { CC(80), CC(81), CC(82), CC(83) },         "BOSS 202"},
  { { CC(80), CC(81), CC(83), CC(84) },         "BOSS 202"}
  //{ { InputA(), None(), None(),InputB()},       "A/B>\1Swt"}
};

}

#endif
