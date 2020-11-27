/*
Gu::Midi implements MidiValue class that sends midi data thru USB.
It adds Midi related actions to Gu::Actions::Midi.
*/
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

using midiValue = byte;
struct MidiPacket {
  const bool isnote;
  midiValue value;
  constexpr bool isNote() const { return isnote; }

  constexpr midiEventPacket_t midiNoteOn(){ return { noteOnByte >> 4, noteOnByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiNoteOff(){ return { noteOffByte >> 4, noteOffByte | Channel, value, MaxVelocity }; }
  constexpr midiEventPacket_t midiCc(byte cc_value){ return { CcByte >> 4, CcByte | Channel, value, cc_value}; }
  constexpr midiEventPacket_t getPacket(byte isOn = On){ return isOn == On ? turnOnPacket() : turnOffPacket(); }
  constexpr midiEventPacket_t turnOnPacket(){ return isnote ? midiNoteOn() : midiCc(OnValue); }
  constexpr midiEventPacket_t turnOffPacket(){ return isnote ? midiNoteOff() : midiCc(OffValue); }
};
constexpr MidiPacket MidiNote(midiValue value) { return {true, value}; }
constexpr MidiPacket MidiCC(midiValue value) { return {false, value}; }

struct MidiSender {
  static void sendMidiOnce(MidiPacket midi) const {
    MidiUSB.sendMIDI(midi.getPacket());
    MidiUSB.flush();
  }
  static void sendMidiToggle(MidiPacket midi) const {
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
    delay(1);
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
  }
  static void sendMidiOffOnImpulse(MidiPacket midi) const {
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
    delay(20);
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
  }
}
} //namespace Gu::Midi

namespace Gu::Actions::Midi {
  //error: 'v' is not captured
  //note: the lambda has no capture-default
  //error: 'midi' was not declared in this scope
  //error: could not convert '{<lambda closure object>Gu::Actions::Midi::Note(byte)::<lambda()>{}, operator+(StringSumHelper(((const char*)"\001")), String(v, 10))}' from '<brace-enclosed initializer list>' to 'Gu::Actions::Action'
  Action Note(byte v) {  return { [&](){ auto midi= Gu::Midi::MidiNote(v); MidiSender::sendMidiToggle(midi); }, "\1"+String(v) }; };
  Action CC(byte v) {    return { [&](){ auto midi= Gu::Midi::MidiCC(v); MidiSender::sendMidiOffOnImpulse(midi);}, "c"+String(v) }; };
  Action Rewind() {      return { [&](){ auto midi= Gu::Midi::MidiCC(116); MidiSender::sendMidiOnce(midi);}, "Rew" }; };
  Action FastForward() { return { [&](){ auto midi= Gu::Midi::MidiCC(117); MidiSender::sendMidiOnce(midi);}, "FFw" }; };
  Action Stop() {        return { [&](){ auto midi= Gu::Midi::MidiCC(118); MidiSender::sendMidiOnce(midi);}, "Stp" }; };
  Action Play() {        return { [&](){ auto midi= Gu::Midi::MidiCC(119); MidiSender::sendMidiOnce(midi);}, "Pla" }; };
  Action Loop() {        return { [&](){ Gu::Midi::MidiCC<115> midi; MidiSender::sendMidiOnce(midi);}, "Lop" }; };
  Action Record() {      return { [&](){ Gu::Midi::MidiCC<114> midi; MidiSender::sendMidiOnce(midi);}, "Rec" }; };
}

#endif
