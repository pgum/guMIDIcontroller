#include <EEPROM.h>
#include <AceButton.h>
//#include <ButtonConfig.h>
#include "guprogramscfg.h"
#include "guhelpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "guhwapi.h"
#include "gujackselector.h"

namespace {
const char* ProductName = "GuMIDI mk.3";
const char* ProductVersion = "0.2020.11.10";
constexpr byte numberOfUserButtons = 4;
constexpr byte numberOfControlButtons = 5;

constexpr eepromAddress eepromAddrLastProgram = 0;
constexpr eepromAddress eepromAddrOutputSelector = 1;

constexpr SerialBaudRate serialBaud = 115200;
constexpr i2cAddress lcdI2C = 0x27;
constexpr lcdDimention lcdCols = LcdCols16;
constexpr stringBufferSize lcdStringSize = lcdCols + sizeof("");
constexpr lcdDimention lcdRows = LcdRows2;
constexpr timeMilliSeconds extendLcdBacklightTimeout = oneMinute;
constexpr timeMilliSeconds lcdTimeoutPrintTime = oneSecond; 

constexpr Pin outputSelectorOutputAPin = Pin04;
constexpr Pin outputSelectorOutputBPin = Pin05;

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

constexpr guProgramConfig<numberOfUserButtons> programsConfigs[] {
  { { Note(36), Note(37), Note(38), Note(39) },"S8 ch1,2"},
  { { Note(41), Note(42), Note(43), Note(44) },"S8 ch3,4"},
  { { Note(45), Note(46), Note(48), Note(49) },"S8 ch5,6"},
  { { Note(50), Note(51), Note(52), Note(53) },"S8 ch7,8"},
  { { Stop(), Play(), Loop(), Record() },      "Transprt"},
  { { CC(21), CC(22), CC(23), CC(24) },        " Reaper "},
  { { CC(80), CC(81), CC(82), CC(83) },        "BOSS 202"},
  { { CC(80), CC(81), NotUsed(), CC(84) },       "BOSS 202"},
  { { InputA(), NotUsed(), NotUsed(),InputB()},"A/B-> \1\1"}
};

constexpr byte numberOfPrograms = sizeof(programsConfigs)/sizeof(guProgramConfig<numberOfUserButtons>);

guLcd lcd(lcdI2C, lcdRows, lcdCols); 
guHwApi<numberOfUserButtons, numberOfControlButtons> hwApi(hwCfg);
guProgramsCfg<numberOfPrograms, numberOfUserButtons, lcdStringSize> programs(programsConfigs);
guJackSelector jackSelector(outputSelectorOutputAPin, outputSelectorOutputBPin);
MidiSender midiController;
}

void handleMidiEvent(const MidiValue& midi) {
    if(midi.isNote()){ midiController.sendMidiToggle(midi); }
    else{ midiController.sendMidiOffOnImpulse(midi); }
}

void handleUserEvent(const UserAction& user) {
    lcd.printWithTimeout(user.toString());
    if(user.value == UserAction::UserInputA) jackSelector.SelectA();
    if(user.value == UserAction::UserInputB) jackSelector.SelectB();
    EEPROM.update(eepromAddrOutputSelector, jackSelector.selectedInput); 
}

void handleUserButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto buttonId = button->getId();
  const auto currentAction = programs.getActionForButton(buttonId);
  const auto currentActionType = currentAction.type;
  if(eventType == AceButton::kEventPressed) {
  lcd.extendLcdBacklight();
    if(currentActionType == Action::ActionTypeMidi) {
      handleMidiEvent(currentAction.mv);
    }
    else if(currentActionType == Action::ActionTypeUser){
      handleUserEvent(currentAction.uv);
    }
    lcd.printWithTimeout(currentAction.toString()); 
  }
}

void handleControlSwitch(CtrlBtnId id){
  if(id == CtrlBtnId(2)){ 
    lcd.toggleAlwaysOn();     
  }
  if(id == CtrlBtnId(3)){ 
    lcd.toggleDisplay();
    lcd.printProgram(programs.getCurrentProgram());     
  }
}

void handleControlButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto ctrlBtnId = button->getId();
  if(eventType == AceButton::kEventPressed) {
    if(ctrlBtnId > CtrlBtnId(1)) handleControlSwitch(ctrlBtnId);
    if(lcd.isBacklight()){
      if(ctrlBtnId == CtrlBtnId(0)){ programs.prev(); lcd.printProgram(programs.getCurrentProgram());}
      if(ctrlBtnId == CtrlBtnId(1)){ programs.next(); lcd.printProgram(programs.getCurrentProgram());}
      EEPROM.update(eepromAddrLastProgram, programs.getProgramId()); 
    }else{
      lcd.extendLcdBacklight();
    }
  }
}

void setup() {
  Serial.begin(serialBaud);
  lcd.init();
  hwApi.userButtonsEventHandler = handleUserButtonsEvent;
  hwApi.ctrlButtonsEventHandler = handleControlButtonsEvent;
  hwApi.init();

  jackSelector.init();
  lcd.intro(__DATE__, __TIME__, ProductName, ProductVersion, programs.toString() + " " + hwApi.toString());

  jackSelector.Select(EEPROM.read(eepromAddrOutputSelector));
  programs.setProgram(EEPROM.read(eepromAddrLastProgram));
  lcd.printProgram(programs.getCurrentProgram());
}

void loop() {
  hwApi.update();
  lcd.update();
}
