#ifndef guJackSelector_H
#define guJackSelector_H
#include "guhelpers.h"

struct guJackSelector {
  const Pin InputAPin;
  const Pin InputBPin;
  constexpr static InputId inputA = 0;
  constexpr static InputId inputB = 1;
  InputId selectedInput;
  guJackSelector(Pin inA, Pin inB): InputAPin(inA), InputBPin(inB) {}
  void Select(const InputId inputId){
    inputId == inputA ? SelectA() : SelectB();
  }
  void SelectA(){
    digitalWrite(InputAPin,HIGH);
    digitalWrite(InputBPin,LOW);
    selectedInput = inputA;
  }
  void SelectB(){
    digitalWrite(InputAPin,LOW);
    digitalWrite(InputBPin,HIGH);
    selectedInput = inputB;
  }
  void init(){
    pinMode(InputAPin, OUTPUT);
    pinMode(InputBPin, OUTPUT);
  }
};
#endif
