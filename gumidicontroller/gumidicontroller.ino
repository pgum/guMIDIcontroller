#include <AceButton.h>
#include <ButtonConfig.h>
#include "helpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "guhwapi.h"
#include "guprogramscfg.h"

namespace {
const char* ProductName = "GuMIDI mk.3";
const char* ProductVersion = "0.9125";
constexpr byte numberOfPrograms = 7;
constexpr byte numberOfUserButtons = 4;
constexpr byte numberOfControlButtons = 2;

constexpr inputPin btn0pin6 = 6;
constexpr inputPin btn1pin7 = 7;
constexpr inputPin btn2pin8 = 8;
constexpr inputPin btn3pin9 = 9;
constexpr inputPin ctrlBtn0Pin10 = 10;
constexpr inputPin ctrlBtn1Pin16 = 16;

constexpr inputPin outputSelectorPin18 = 18;

constexpr eepromAddress eepromAddrLastProgram = 0;

constexpr i2cAddress lcdI2C = 0x27;
constexpr lcdDimention lcdDimentionY = 16;
constexpr stringBufferSize lcdStringSize = lcdDimentionY + sizeof("");
constexpr lcdDimention lcdDimentionX = 2;
constexpr lcdStayAwakeTimeMS stayAwakeTime = 2000;
constexpr SerialBaudRate serialBaud = 115200;
constexpr lcdStayAwakeTimeMS extendLcdBacklightMs = 20000;
constexpr lcdStayAwakeTimeMS lcdRedrawTooltipAfterMs = 2000; 

constexpr guHwConfig <numberOfUserButtons, numberOfControlButtons> hwCfg {
  { { CfgId::Id0, btn0pin6 },
    { CfgId::Id1, btn1pin7 },
    { CfgId::Id2, btn2pin8 },
    { CfgId::Id3, btn3pin9 } },
  { { CfgId::Id0, ctrlBtn0Pin10 },
    { CfgId::Id1, ctrlBtn1Pin16 } },
};

constexpr guProgramConfig<numberOfUserButtons, Action, lcdStringSize> programsConfigs[numberOfPrograms] {
  { CfgId::Id0, { Note(36), Note(37), Note(38), Note(39) },"1/7  S8 ch. 1,2 ", "36 37 <\1\1> 38 39" },
  { CfgId::Id1, { Note(41), Note(42), Note(43), Note(44) },"2/7  S8 ch. 3,4 ", "41 42 <\1\1> 43 44" },
  { CfgId::Id2, { Note(45), Note(46), Note(38), Note(49) },"3/7  S8 ch. 5,6 ", "45 46 <\1\1> 48 49" },
  { CfgId::Id3, { Note(50), Note(51), Note(52), Note(53) },"4/7  S8 ch. 7,8 ", "50 51 <\1\1> 53 54" },
  { CfgId::Id4, { Stop(), Play(), Loop(), Record() }, "5/7  Transport  ", "Stp Pla Loop Rec" },
  { CfgId::Id5, { CC(21), CC(22), CC(23), CC(24) }, "6/7  CC 21 - 24 ", "21 22 <cc> 23 24" },
  { CfgId::Id6, { InputA(), NotUsed(), NotUsed(),InputB()},"7/7  A/B -> Out ", "\1A -- <\1\1> -- \1B" }
};

guLcd lcd(lcdI2C, lcdDimentionY, lcdDimentionX); 
guHwApi<numberOfUserButtons, numberOfControlButtons> hwApi(hwCfg);

guProgramsCfg<numberOfPrograms,
              numberOfUserButtons, 
              Action, 
              lcdStringSize> programs(programsConfigs, eepromAddrLastProgram);
}

void handleMidiEvent(const MidiValue& midi) {
    if(midi.isNote()){ sendMidiToggle(midi); }
    else{ sendMidiOnce(midi); }
}
void handleUserEvent(const UserValue& user) {
    lcd.printSecondLine(user.toString());
    if(user.value == UserValue::UserInputA) selectInputA();
    if(user.value == UserValue::UserInputB) selectInputB();
}
void handleUserButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  lcd.extendLcdBacklight(extendLcdBacklightMs);
  //Serial.println(programs.tooltip());
  lcd.printWithDelay(lcdRedrawTooltipAfterMs, programs.tooltip());
  const auto id = button->getId();
  const auto currentAction = programs.get(id);
  const auto currentActionType = currentAction.type;
  if(eventType == AceButton::kEventPressed) {
    //Serial.println(currentActionType);
    //Serial.println(currentAction.toString());
    if(currentActionType == Action::ActionTypeMidi) {
      lcd.printMidiSend(id, currentAction.toString()); 
      handleMidiEvent(currentAction.mv);
    }
    else if(currentActionType == Action::ActionTypeUser){
      lcd.printSecondLine(currentAction.toString()); 
      handleUserEvent(currentAction.uv);
    }
  }
}

void handleControlButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  lcd.extendLcdBacklight(extendLcdBacklightMs);
  const auto id = button->getId();
  if(eventType == AceButton::kEventPressed) {
    if(id == Id2Byte(CfgId::Id0)){ programs.prev(); lcd.printProgramChange(programs.header(), programs.tooltip());}
    if(id == Id2Byte(CfgId::Id1)){ programs.next(); lcd.printProgramChange(programs.header(), programs.tooltip());}

    if(id == Id2Byte(CfgId::Id2)){ lcd.enableAlwaysOn(); }

  }else if(eventType == AceButton::kEventReleased) {
    if(id == Id2Byte(CfgId::Id2)){ lcd.disableAlwaysOn(); }
  }
}

void selectInputA(){
  Serial.println("digitalWrite(outputSelectorPin18,HIGH);");
}
void selectInputB(){
  Serial.println("digitalWrite(outputSelectorPin18,LOW);");
}

void setup() {
 
  Serial.begin(serialBaud);
  pinMode(outputSelectorPin18, OUTPUT);
  lcd.intro(__DATE__, __TIME__, ProductName, ProductVersion, numberOfUserButtons);
  hwApi.userButtonsEventHandler = handleUserButtonsEvent;
  hwApi.ctrlButtonsEventHandler = handleControlButtonsEvent;
  hwApi.init();
  programs.loadProgramIndex();
  lcd.printProgramChange(programs.header(), programs.tooltip());
  lcd.extendLcdBacklight(extendLcdBacklightMs);
}

void loop() {
  hwApi.update();
  lcd.update();
}
