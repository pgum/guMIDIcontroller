#include <EEPROM.h>
#include <AceButton.h>
//#include <ButtonConfig.h>
#include "guaction.h"
#include "guprogramscfg.h"
#include "guhelpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "guhwapi.h"
#include "gujackselector.h"
#include "guoptions.h"

namespace {
using namespace Gu::Options;
using namespace Gu::HWApi;
using namespace Gu::Actions;
using namespace Gu::Actions::Midi;
using namespace Gu::Actions::JackSelector;
using namespace Gu::Programs;

Gu::HWApi::guHwApi<numberOfUserButtons, numberOfControlButtons> hwApi(hwCfg);
Gu::Lcd::guLcd lcd(lcdI2C, lcdRows, lcdCols); 
guJackSelector jackSelector(outputSelectorOutputAPin, outputSelectorOutputBPin, eepromAddrOutputSelector);
Gu::Programs::BankController<numberOfUserButtons> programs;

BankSetting<numberOfUserButtons> programsConfigs[] {
  { { Note(36), Note(37), Note(38), Note(39) }, "S8 ch1,2"},
  { { Note(41), Note(42), Note(43), Note(44) }, "S8 ch3,4"},
  { { Note(45), Note(46), Note(48), Note(49) }, "S8 ch5,6"},
  { { Note(50), Note(51), Note(52), Note(53) }, "S8 ch7,8"},
  { { Stop(), Play(), Loop(), Record() },       "Transprt"},
  { { CC(21), CC(22), CC(23), CC(24) },         " Reaper "},
  { { CC(80), CC(81), CC(82), CC(83) },         "BOSS 202"},
  { { CC(80), CC(81), CC(83), CC(84) },         "BOSS 202"},
  { { InputA(jackSelector), None(), None(),InputB(jackSelector)},       "A/B>\1Swt"}
};

}

void handleButtonsEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto buttonId = button->getId();
  if(eventType == AceButton::kEventPressed) {
    lcd.extendLcdBacklight();
    programs.triggerAction(buttonId);
    lcd.printWithTimeout(programs.getCurrentProgram()->actionsList[buttonId].name); 
  }
}

void handleControlSwitch(CtrlBtnId id){
  if(id == CtrlBtnId(1)){ 
    lcd.toggleAlwaysOn();     
  }
  if(id == CtrlBtnId(2)){ 
    lcd.toggleDisplay();
    lcd.printBothLines(programs.printProgramName(), programs.printProgramDescription());
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
