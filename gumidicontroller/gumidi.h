#ifndef guMidi_H
#define guMidi_H

#include <stddef.h>
#include <MIDIUSB.h>
#include <Arduino.h>
#include "guhelpers.h"
#include "guaction.h"

namespace Gu::Midi {
constexpr byte Channel = 16;
constexpr byte On = 1;
constexpr byte Off = 0;
constexpr byte OnValue = 127;
constexpr byte OffValue = 0;
constexpr byte MaxVelocity = 127;
constexpr byte noteOnByte = 0x90;
constexpr byte noteOffByte = 0x80;
constexpr byte CcByte = 0xB0;

template<byte value>
struct MidiValue {
  const bool isnote;
  constexpr bool isNote() const { return isnote; }
  void sendMidiOnce() const {
    MidiUSB.sendMIDI(getPacket());
    MidiUSB.flush();
  }
  void sendMidiToggle() const {
    MidiUSB.sendMIDI(turnOnPacket());
    MidiUSB.flush();
    delay(1);
    MidiUSB.sendMIDI(turnOffPacket());
    MidiUSB.flush();
  }
  void sendMidiOffOnImpulse() const {
    MidiUSB.sendMIDI(turnOffPacket());
    MidiUSB.flush();
    delay(20);
    MidiUSB.sendMIDI(turnOnPacket());
    MidiUSB.flush();
  }

  constexpr midiEventPacket_t midiNoteOn(){ return { noteOnByte >> 4, noteOnByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiNoteOff(){ return { noteOffByte >> 4, noteOffByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiCc(byte cc_value){ return { CcByte >> 4, CcByte | Channel, value, cc_value}; }
  constexpr midiEventPacket_t getPacket(byte isOn = On){ return isOn == On ? turnOnPacket() : turnOffPacket(); }
  constexpr midiEventPacket_t turnOnPacket(){ return isnote ? midiNoteOn() : midiCc(OnValue); }
  constexpr midiEventPacket_t turnOffPacket(){ return isnote ? midiNoteOff() : midiCc(OffValue); }
};
template<byte value>
constexpr MidiValue MidiNote() { return {true}; }
template<byte value>
constexpr MidiValue MidiCC() { return {false}; }
} //namespace Gu::Midi

namespace Gu::Actions::Midi {
  constexpr Action Note(byte v) {  return { [](){ Gu::Midi::MidiNote<v> midi; midi.sendMidiToggle(); }, "\1"+String(v) }; };
  constexpr Action CC(byte v) {    return { [](){ Gu::Midi::MidiCC<v> midi; midi.sendMidiOffOnImpulse();}, "c"+String(v) }; };
  constexpr Action Rewind() {      return { [](){ Gu::Midi::MidiCC<116> midi); midi.sendMidiOnce();}, "Rew" }; };
  constexpr Action FastForward() { return { [](){ Gu::Midi::MidiCC<117> midi); midi.sendMidiOnce();}, "FFw" }; };
  constexpr Action Stop() {        return { [](){ Gu::Midi::MidiCC<118> midi; midi.sendMidiOnce();}, "Stp" }; };
  constexpr Action Play() {        return { [](){ Gu::Midi::MidiCC<119> midi; midi.sendMidiOnce();}, "Pla" }; };
  //error: 'MidiCC' is not a member of 'Gu::Midi'
  constexpr Action Loop() {        return { [](){ Gu::Midi::MidiCC<115> midi; midi.sendMidiOnce();}, "Lop" }; };
  constexpr Action Record() {      return { [](){ Gu::Midi::MidiCC<114> midi; midi.sendMidiOnce();}, "Rec" }; };
}

#endif
