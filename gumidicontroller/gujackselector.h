#ifndef guJackSelector_H
#define guJackSelector_H

struct guJackSelector {
  const Pin relayPin;
  constexpr static byte inputA = 0;
  constexpr static byte inputB = 1;
  byte selectedInput;
  guJackSelector(Pin pin): relayPin(pin) {}
  void SelectA(){
    Serial.println("digitalWrite(relayPin,LOW);");
    selectedInput = inputA;
  }
  void SelectB(){
    Serial.println("digitalWrite(relayPin,HIGH);");
    selectedInput = inputB;
  }
  void init(){
    pinMode(relayPin, OUTPUT);
  }
};
#endif
