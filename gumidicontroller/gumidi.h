#ifndef guMidi_H
#define guMidi_H

#include <stddef.h>
#include <MIDIUSB.h>
#include <Arduino.h>
#include "guhelpers.h"

constexpr byte Channel = 16;
constexpr byte On = 1;
constexpr byte Off = 0;
constexpr byte OnValue = 127;
constexpr byte OffValue = 0;
constexpr byte MaxVelocity = 127;
constexpr byte noteOnByte = 0x90;
constexpr byte noteOffByte = 0x80;
constexpr byte CcByte = 0xB0;

struct MidiValue {
  const byte value;
  const bool isnote;
  const char* abbr;
  
  constexpr bool isNote() const { return isnote; }
  String toString() const {  
    return abbr == NULL ? (isNote() ?  "\1" : "c") + String(value) : String(abbr);
  }

  constexpr midiEventPacket_t midiNoteOn(){ return { noteOnByte >> 4, noteOnByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiNoteOff(){ return { noteOffByte >> 4, noteOffByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiCc(byte v){ return { CcByte >> 4, CcByte | Channel, value, v}; }
  constexpr midiEventPacket_t getPacket(byte isOn = On){ return isOn == On ? turnOnPacket() : turnOffPacket(); }
  constexpr midiEventPacket_t turnOnPacket(){ return isnote ? midiNoteOn() : midiCc(OnValue); }
  constexpr midiEventPacket_t turnOffPacket(){ return isnote ? midiNoteOff() : midiCc(OffValue); }
};

constexpr MidiValue MidiNone() { return {0, false, NULL}; }
constexpr MidiValue MidiNote(byte v) { return {v, true, NULL}; }
constexpr MidiValue MidiCC(byte v) { return {v, false, NULL}; }
constexpr MidiValue MidiTransport(byte v, const char* abbr4char) { return {v, false, abbr4char}; }

struct MidiSender {
  void sendMidiOnce(const MidiValue& midi) {
    MidiUSB.sendMIDI(midi.getPacket());
    MidiUSB.flush();
  }

  void sendMidiToggle(const MidiValue& midi) {
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
    delay(1);
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
  }

  void sendMidiOffOnImpulse(const MidiValue& midi) {
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
    delay(20);
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
  }
};

#endif
