/*
Gu::Programs contains class related to connecting available Actions.
It organizes Actions into banks of programs able to be triggered by physical buttons.
If implements its won Actions in namespace Gu::Actions::Programs to iterate over programs.
*/
#pragma once
#ifndef guProgramsCfg_H
#define guProgramsCfg_H
#include <Arduino.h>
#include <EEPROM.h>
#include "guhelpers.h"
#include "gumidi.h"
#include "guaction.h"

namespace Gu::Programs {

template <byte numberOfUserButtons>
struct BankSetting { 
  const Gu::Actions::Action actionsList[numberOfUserButtons];
  const char* bankName;
};

template <byte numberOfUserButtons, 
          byte defaultProgram = 0>
struct BankController { 
  using programId = byte;
  using bankSetting = BankSetting<numberOfUserButtons>;
  bankSetting *c;
  uint8_t numberOfPrograms;
  programId currentProgramId;
  byte eepromAddr;
  BankController(const bankSetting* config= NULL):c(config) { currentProgramId= defaultProgram; numberOfPrograms= sizeof(config)/sizeof(BankSetting<numberOfUserButtons>); }
  void loadConfig(const bankSetting* config){ c= config; numberOfPrograms= sizeof(config)/sizeof(BankSetting<numberOfUserButtons>); }
  Gu::Actions::Action triggerAction(BtnId buttonId) const {  (c[currentProgramId].actionsList[buttonId].callback)(); }
  void setProgram(programId programNumber) { currentProgramId = programNumber % numberOfPrograms; }
  byte getProgramId() { return currentProgramId % numberOfPrograms; }
  bankSetting* getCurrentProgram() const { return c+currentProgramId; }
  void updateEeprom() { EEPROM.update(eepromAddr, currentProgramId); }
  void loadFromEeprom() { setProgram(EEPROM.read(eepromAddr)); }
  void next() { currentProgramId = (currentProgramId + 1) % numberOfPrograms; updateEeprom(); }
  void prev() { currentProgramId = (currentProgramId == 0 ? numberOfPrograms - 1 : currentProgramId - 1); updateEeprom(); }
  String signature() const {
    return "Pr" + String(numberOfPrograms);
  }
  String toString() const {
    return String(currentProgramId) + String("/") + String(numberOfPrograms);
  }
  String printProgramName() const {
    auto currentConfig= getCurrentProgram();
    String programNameRaw= String(currentConfig->bankName);
    return String("\2"+getProgramId()-1 % numberOfPrograms) + "  " + programNameRaw + "  " + String(getProgramId()+1 % numberOfPrograms)+String("\3");
  }
  String printProgramDescription() const {
    auto currentConfig= getCurrentProgram();
    String desc = "";
    for(int i=0; i< numberOfUserButtons; ++i){
      desc+=String(currentConfig->actionsList[i].name)+String(" ");
    }
    return desc + emptyLine64Width;
  }
};

} //namespace Gu::Programs
/*
namespace Gu::Actions::Programs {
  using namespace Gu::Programs;
  static Action NextProgram(const BankController& programsConfigs) { return { [&]()->auto{ programsConfigs.next(); }, "Nxt" }; }
  static Action PrevProgram(const BankController& programsConfigs) { return { [&]()->auto{ programsConfigs.prev(); }, "Prv" }; }
  static Action ZeroProgram(const BankController& programsConfigs) { return { [&]()->auto{ programsConfigs.setProgram(0); }, "P0" }; }
} //namespace Gu::Actions::Programs
*/
#endif
