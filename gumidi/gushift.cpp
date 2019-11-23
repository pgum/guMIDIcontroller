#include "gushift.h"

byte guShift::Mode() const { return mode; }
void guShift::nextMode() { mode = (mode + 1) % numOfModes; saveProgramIndex(); }
void guShift::prevMode() { mode = (mode == 0 ? mode = numOfModes - 1 : mode - 1); saveProgramIndex(); }
void guShift::check(){
    shiftButtonPrev.check();
    shiftButtonNext.check();
}
void guShift::init(ButtonConfig::EventHandler handler, byte prevPin, byte nextPin) {
  shiftConfig.setEventHandler(handler);
  shiftConfig.setFeature(ButtonConfig::kFeatureSuppressAll);
  initializeButtons(prevPin, nextPin);
  loadProgramIndex();
}
String guShift::status(){ 
    switch(mode){
      case 0: return String("1/7  S8 ch. 1,2 ");
      case 1: return String("2/7  S8 ch. 3,4 ");
      case 2: return String("3/7  S8 ch. 5,6 ");
      case 3: return String("4/7  S8 ch. 7,8 ");
      case 4: return String("5/7  Transport  ");
      case 5: return String("6/7  CC 21 - 24 ");
      case 6: return String("7/7  CC 25 - 28 ");
      default: return String("Woot?!  Woot?!  ");
    }
}
String guShift::tooltip(){
    switch(mode){
      case 0: return String("36 37 <\1\1> 38 39");
      case 1: return String("41 42 <\1\1> 43 44");
      case 2: return String("45 46 <\1\1> 48 49");
      case 3: return String("50 51 <\1\1> 53 54");
      case 4: return String("Stp Pla Loop Rec");
      case 5: return String("21 22 <cc> 23 24");
      case 6: return String("25 26 <cc> 27 28");
      default: return String("Woot?!    Woot?!");
    }
}
void guShift::initializeButtons(byte prevPin, byte nextPin) {
    pinMode(prevPin, INPUT_PULLUP); 
    shiftButtonPrev.setButtonConfig(&shiftConfig);
    shiftButtonPrev.init(prevPin, HIGH, Id2Byte(CfgId::Id0));
    pinMode(nextPin, INPUT_PULLUP); 
    shiftButtonNext.setButtonConfig(&shiftConfig);
    shiftButtonNext.init(nextPin, HIGH, Id2Byte(CfgId::Id1));
}
void guShift::saveProgramIndex(){
  EEPROM.update(eepromAddrLastProgram, mode);
}
void guShift::loadProgramIndex(){
  mode = EEPROM.read(eepromAddrLastProgram) % numOfModes;
}
