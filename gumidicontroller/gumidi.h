/*
Gu::Midi implements MidiValue class that sends midi data thru USB.
It adds Midi related actions to Gu::Actions::Midi.
*/
#pragma once
#ifndef guMidi_H
#define guMidi_H

#include <stddef.h>
#include <MIDIUSB.h>
#include <Arduino.h>
#include "guaction.h"
#include "guhelpers.h"

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
  static void sendMidiOnce(MidiPacket midi) {
    MidiUSB.sendMIDI(midi.getPacket());
    MidiUSB.flush();
  }
  static void sendMidiToggle(MidiPacket midi) {
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
    delay(1);
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
  }
  static void sendMidiOffOnImpulse(MidiPacket midi) {
    MidiUSB.sendMIDI(midi.turnOffPacket());
    MidiUSB.flush();
    delay(20);
    MidiUSB.sendMIDI(midi.turnOnPacket());
    MidiUSB.flush();
  }
};
} //namespace Gu::Midi

namespace Gu::Actions::Midi {
  using namespace Gu::Midi;
  static Action Note(byte v) {  return { [&](){ auto midi= MidiNote(v); MidiSender::sendMidiToggle(midi); }, String("\1"+String(v)).c_str() }; };
  static Action CC(byte v) {    return { [&](){ auto midi= MidiCC(v); MidiSender::sendMidiOffOnImpulse(midi);}, String("c"+String(v)).c_str() }; };
  static Action Rewind() {      return { [&](){ auto midi= MidiCC(116); MidiSender::sendMidiOnce(midi);}, "Rew" }; };
  static Action FastForward() { return { [&](){ auto midi= MidiCC(117); MidiSender::sendMidiOnce(midi);}, "FFw" }; };
  static Action Stop() {        return { [&](){ auto midi= MidiCC(118); MidiSender::sendMidiOnce(midi);}, "Stp" }; };
  static Action Play() {        return { [&](){ auto midi= MidiCC(119); MidiSender::sendMidiOnce(midi);}, "Pla" }; };
  static Action Loop() {        return { [&](){ auto midi= MidiCC(115); MidiSender::sendMidiOnce(midi);}, "Lop" }; };
  static Action Record() {      return { [&](){ auto midi= MidiCC(114); MidiSender::sendMidiOnce(midi);}, "Rec" }; };
} //namespace Gu::Actions::Midi

#endif
