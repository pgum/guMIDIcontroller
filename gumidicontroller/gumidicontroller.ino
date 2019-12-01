#include <EEPROM.h>
#include <AceButton.h>
#include <ButtonConfig.h>
#include "guhelpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "guhwapi.h"
#include "gujackselector.h"
#include "guprogramscfg.h"

namespace {
const char* ProductName = "GuMIDI mk.3";
const char* ProductVersion = "0.9150";
constexpr byte numberOfPrograms = 7;
constexpr byte numberOfUserButtons = 4;
constexpr byte numberOfControlButtons = 2;

constexpr eepromAddress eepromAddrLastProgram = 0;
constexpr eepromAddress eepromAddrOutputSelector = 1;

constexpr SerialBaudRate serialBaud = 115200;
constexpr i2cAddress lcdI2C = 0x27;
constexpr lcdDimention lcdDimentionY = 16;
constexpr stringBufferSize lcdStringSize = lcdDimentionY + sizeof("");
constexpr lcdDimention lcdDimentionX = 2;
constexpr timeMilliSeconds extendLcdBacklightTimeout = 30 * seconds;
constexpr timeMilliSeconds lcdRedrawTooltipTimeout = 2 * seconds; 

constexpr Pin outputSelectorPin = Pin18;

constexpr guHwConfig <numberOfUserButtons, numberOfControlButtons> hwCfg {
  { { UserBtnId(0), Pin06 },
    { UserBtnId(1), Pin07 },
    { UserBtnId(2), Pin08 },
    { UserBtnId(3), Pin09 } },
  { { CtrlBtnId(0), Pin10 },
    { CtrlBtnId(1), Pin16 } },
};
constexpr guProgramConfig<numberOfUserButtons, lcdStringSize> programsConfigs[numberOfPrograms] {
  { ProgramId(0), { Note(36), Note(37), Note(38), Note(39) },"1/7  S8 ch. 1,2 ", "36 37 <\1\1> 38 39" },
  { ProgramId(1), { Note(41), Note(42), Note(43), Note(44) },"2/7  S8 ch. 3,4 ", "41 42 <\1\1> 43 44" },
  { ProgramId(2), { Note(45), Note(46), Note(38), Note(49) },"3/7  S8 ch. 5,6 ", "45 46 <\1\1> 48 49" },
  { ProgramId(3), { Note(50), Note(51), Note(52), Note(53) },"4/7  S8 ch. 7,8 ", "50 51 <\1\1> 53 54" },
  { ProgramId(4), { Stop(), Play(), Loop(), Record() }, "5/7  Transport  ", "Stp Pla Loop Rec" },
  { ProgramId(5), { CC(21), CC(22), CC(23), CC(24) }, "6/7  CC 21 - 24 ", "21 22 <cc> 23 24" },
  { ProgramId(6), { InputA(), NotUsed(), NotUsed(),InputB()},"7/7  A/B -> Out ", "\1A -- <\1\1> -- \1B" }
};

guLcd lcd(lcdI2C, lcdDimentionY, lcdDimentionX); 
guHwApi<numberOfUserButtons, numberOfControlButtons> hwApi(hwCfg);
guProgramsCfg<numberOfPrograms, numberOfUserButtons, lcdStringSize> programs(programsConfigs);
guJackSelector jackSelector(outputSelectorPin);
}

void handleMidiEvent(const MidiValue& midi) {
    if(midi.isNote()){ sendMidiToggle(midi); }
    else{ sendMidiOnce(midi); }
}

void handleUserEvent(const UserAction& user) {
    lcd.printSecondLine(user.toString());
    if(user.value == UserAction::UserInputA) jackSelector.SelectA();
    if(user.value == UserAction::UserInputB) jackSelector.SelectB();
    EEPROM.update(eepromAddrOutputSelector, jackSelector.selectedInput); 
}

void handleUserButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  lcd.extendLcdBacklight();
  lcd.printWithDelay(programs.tooltip());
  const auto buttonId = button->getId();
  const auto currentAction = programs.getActionForButton(buttonId);
  const auto currentActionType = currentAction.type;
  if(eventType == AceButton::kEventPressed) {
    if(currentActionType == Action::ActionTypeMidi) {
      handleMidiEvent(currentAction.mv);
    }
    else if(currentActionType == Action::ActionTypeUser){
      handleUserEvent(currentAction.uv);
    }
    lcd.printSecondLine(currentAction.toString()); 
  }
}

void handleControlButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  lcd.extendLcdBacklight();
  const auto ctrlBtnId = button->getId();
  if(eventType == AceButton::kEventPressed) {
    if(ctrlBtnId == CtrlBtnId(0)){ programs.prev(); lcd.printProgramChange(programs.header(), programs.tooltip());}
    if(ctrlBtnId == CtrlBtnId(1)){ programs.next(); lcd.printProgramChange(programs.header(), programs.tooltip());}

    if(ctrlBtnId == CtrlBtnId(2)){ lcd.enableAlwaysOn(); }
    EEPROM.update(eepromAddrLastProgram, programs.getProgram()); 
  }else if(eventType == AceButton::kEventReleased) {
    if(ctrlBtnId == CtrlBtnId(2)){ lcd.disableAlwaysOn(); }
  }
}

void setup() {
  Serial.begin(serialBaud);
  hwApi.userButtonsEventHandler = handleUserButtonsEvent;
  hwApi.ctrlButtonsEventHandler = handleControlButtonsEvent;
  hwApi.init();
  lcd.intro(__DATE__, __TIME__, ProductName, ProductVersion, numberOfUserButtons);
  lcd.setExtendLcdBacklightMs(extendLcdBacklightTimeout);
  lcd.setLcdRedrawTooltipAfterMs(lcdRedrawTooltipTimeout);
  jackSelector.selectedInput= EEPROM.read(eepromAddrOutputSelector);
  programs.setProgram(EEPROM.read(eepromAddrLastProgram));
  lcd.printProgramChange(programs.header(), programs.tooltip());
  Serial.print("Input Selector read from EEPROM: ");
  Serial.println(jackSelector.selectedInput);
}

void loop() {
  hwApi.update();
  lcd.update();
}
