#include <EEPROM.h>
#include <AceButton.h>
//#include <ButtonConfig.h>
#include "guprogramscfg.h"
#include "guhelpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "guhwapi.h"
//#include "gujackselector.h"
#include "guoptions.h"

namespace {
using namespace Gu::Options;
guHwApi<numberOfUserButtons, numberOfControlButtons> hwApi(hwCfg);
Gu::Lcd::guLcd lcd(lcdI2C, lcdRows, lcdCols); 
//guJackSelector jackSelector(outputSelectorOutputAPin, outputSelectorOutputBPin, eepromAddrOutputSelector);
Gu::Programs::guProgramsCfg<numberOfUserButtons> programs;
}

void handleButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto buttonId = button->getId();
  if(eventType == AceButton::kEventPressed) {
    lcd.extendLcdBacklight();
    programs.triggerAction(buttonId);
    lcd.printWithTimeout(programs.getCurrentProgram().actionsList[buttonId].name); 
  }
}

void handleControlSwitch(CtrlBtnId id){
  if(id == CtrlBtnId(1)){ 
    lcd.toggleAlwaysOn();     
  }
  if(id == CtrlBtnId(2)){ 
    lcd.toggleDisplay();
    lcd.print(programs.getCurrentProgram(),1);     
  }
}

void setup() {
  Serial.begin(serialBaud);
  lcd.init();
  hwApi.userButtonsEventHandler = handleButtonsEvent;
  hwApi.ctrlButtonsEventHandler = handleControlSwitch;
  hwApi.init();
  //jackSelector.init();
  lcd.intro(__DATE__, __TIME__, ProductName, ProductVersion, programs.signature() + " " + hwApi.signature());
  programs.loadFromEeprom();
  lcd.printBothLines(programs.printProgramName(), programs.printProgramDescription());
}

void loop() {
  hwApi.update();
  lcd.update();
}
