#ifndef guProgramsCfg_H
#define guProgramsCfg_H
#include <Arduino.h>
#include "gumidi.h"

using eepromAddress = byte;
using ActionType = byte;

struct UserAction {
  using UserActionType = byte;
  const UserActionType value;
  const char* abbr;
  constexpr static UserActionType UserActionNone = 0;
  constexpr static UserActionType UserInputA = 10;
  constexpr static UserActionType UserInputB = 11;
  String toString() const { return abbr == NULL ? String("u")+ String(value) : String(abbr); }
};

constexpr UserAction UserNone() { return { UserAction::UserActionNone, NULL}; };
constexpr UserAction UserSelectInputA() { return { UserAction::UserInputA, "\1A"}; };
constexpr UserAction UserSelectInputB() { return { UserAction::UserInputB, "B\1"}; };

struct Action { 
  const MidiValue mv;
  const UserAction uv;
  const byte type;

  constexpr static byte ActionTypeNone = 0x00;
  constexpr static byte ActionTypeMidi = 0x01;
  constexpr static byte ActionTypeUser = 0x02;
  operator MidiValue(){ return mv; }
  operator UserAction(){ return uv; }
  String toString() const { 
    if(type == ActionTypeMidi) return mv.toString();
    if(type == ActionTypeUser) return uv.toString();
    return "---";
  }
};

constexpr Action NotUsed() { return { MidiNone(), UserNone(), Action::ActionTypeNone }; };
constexpr Action Note(byte v) { return { MidiNote(v), UserNone(), Action::ActionTypeMidi }; };
constexpr Action CC(byte v) { return { MidiCC(v), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Rewind() { return { MidiTransport(116,"Rew"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action FastForward() { return { MidiTransport(117,"FFw"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Stop() { return { MidiTransport(118,"Stp"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Play() { return { MidiTransport(119,"Ply"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Loop() { return { MidiTransport(115,"Lop"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Record() { return { MidiTransport(114,"Rec"), UserNone(), Action::ActionTypeMidi }; };

constexpr Action InputA() { return { MidiNone(), UserSelectInputA(), Action::ActionTypeUser }; };
constexpr Action InputB() { return { MidiNone(), UserSelectInputB(), Action::ActionTypeUser }; };

template <byte numberOfUserButtons>
struct guProgramConfig { 
  const Action assignedAction[numberOfUserButtons];
  const char* programName;
};

template <byte numberOfPrograms, 
          byte numberOfUserButtons, 
          byte lcdStringSize,
          byte defaultProgram = 0>
struct guProgramsCfg { 
  using programId = byte;
  using programConfig = guProgramConfig<numberOfUserButtons>;
  const programConfig *c;
  byte currentProgramId;
  String toString() const {
    return "Pr" + String(numberOfPrograms);
  }
  guProgramsCfg(const programConfig* config):c(config) { currentProgramId= defaultProgram; }
  Action getActionForButton(BtnId buttonId) const { return c[currentProgramId].assignedAction[buttonId]; }
  void setProgram(programId programNumber) { currentProgramId = programNumber % numberOfPrograms; }
  byte getProgramId() { return currentProgramId % numberOfPrograms; }
  programConfig* getCurrentProgram() const { return c+currentProgramId; }
  void next() { currentProgramId = (currentProgramId + 1) % numberOfPrograms; }
  void prev() { currentProgramId = (currentProgramId == 0 ? numberOfPrograms - 1 : currentProgramId - 1); }
  };

#endif
