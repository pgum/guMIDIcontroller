/*
Gu::JackSelector namespace holds device interface to control relay switching between two jack outputs.
It adds Actions under Gu::Actions::JackSelector namespace.
*/
#ifndef guJackSelector_H
#define guJackSelector_H
#include "guhelpers.h"

namespace Gu::JackSelector{
struct guJackSelector {
  const Pin InputAPin;
  const Pin InputBPin;
  const eepromAddress eepromAddress;
  constexpr static InputId inputA = 0;
  constexpr static InputId inputB = 1;
  constexpr static InputId inputNone = 125;
  InputId selectedInput;
  guJackSelector(Pin inA, Pin inB): InputAPin(inA), InputBPin(inB) {}
  void Select(const InputId inputId){
    if(inputId == inputNone) return;
    inputId == inputA ? SelectA() : SelectB();
  }
  void updateEeprom(){
    EEPROM.update(eepromAddress, jackSelector.selectedInput); 
  }
  void loadFromEeprom(){
    Select(EEPROM.read(eepromAddress));
  }
  void SelectA(){
    digitalWrite(InputAPin,HIGH);
    digitalWrite(InputBPin,LOW);
    selectedInput = inputA;
    updateEeprom();
  }
  void SelectB(){
    digitalWrite(InputAPin,LOW);
    digitalWrite(InputBPin,HIGH);
    selectedInput = inputB;
    updateEeprom();
  }
  void SelectNone(){
    digitalWrite(InputAPin,LOW);
    digitalWrite(InputBPin,LOW);
    selectedInput = inputNone;
    updateEeprom();
  }
  void init(){
    pinMode(InputAPin, OUTPUT);
    pinMode(InputBPin, OUTPUT);
    loadFromEeprom();
  }
};
}

namespace Gu::Actions::JackSelector {
  constexpr Action InputA(const guJackSelector& jackSelector) { return { [&](){ jackSelector.SelectA(); }, "\1A"}; };
  constexpr Action InputB(const guJackSelector& jackSelector) { return { [&](){ jackSelector.SelectB(); }, "\1B" }; };
  constexpr Action InputOther(const guJackSelector& jackSelector) { return { [&](){ jackSelector.selectedInput ? SelectB() : SelectA(); }, "Otr" }; };
  constexpr Action InputNone(const guJackSelector& jackSelector) { return { [&](){ jackSelector.selectedInput ? SelectB() : SelectA(); }, "Otr" }; };
}

#endif
