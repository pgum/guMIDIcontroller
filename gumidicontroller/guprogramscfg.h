#ifndef guProgramsCfg_H
#define guProgramsCfg_H

using eepromAddress = byte;
using ActionType = byte;

struct UserAction {
  using UserActionType = byte;
  const UserActionType value;
  const char* abbr8;
  constexpr static UserActionType UserActionNone = 0;
  constexpr static UserActionType UserInputA = 10;
  constexpr static UserActionType UserInputB = 11;
  String toString() const { return (String(value) +  String(" ") + ((abbr8 != NULL) ? String(abbr8) : String("        ")) + String("     ")); }
};

constexpr UserAction UserNone() { return {UserAction::UserActionNone, NULL}; }
constexpr UserAction UserSelectInputA() { return {UserAction::UserInputA, "A -> Out"}; }
constexpr UserAction UserSelectInputB() { return {UserAction::UserInputB, "B -> Out"}; }

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
    return "   None Type!   ";
  }
};

constexpr Action NotUsed() { return { MidiNone(), UserNone(), Action::ActionTypeNone }; };
constexpr Action Note(byte v) { return { MidiNote(v), UserNone(), Action::ActionTypeMidi }; };
constexpr Action CC(byte v) { return { MidiCC(v), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Rewind() { return { MidiTransport(116,"Rew "), UserNone(), Action::ActionTypeMidi }; };
constexpr Action FastForward() { return { MidiTransport(117,"FFwd"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Stop() { return { MidiTransport(118,"Stop"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Play() { return { MidiTransport(119,"Play"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Loop() { return { MidiTransport(115,"Loop"), UserNone(), Action::ActionTypeMidi }; };
constexpr Action Record() { return { MidiTransport(114,"Rec "), UserNone(), Action::ActionTypeMidi }; };

constexpr Action InputA() { return { MidiNone(), UserSelectInputA(), Action::ActionTypeUser }; };
constexpr Action InputB() { return { MidiNone(), UserSelectInputB(), Action::ActionTypeUser }; };

template <byte numberOfUserButtons, 
          byte lengthOfLcdDescription>
struct guProgramConfig { 
  const ProgramId id;
  const Action v[numberOfUserButtons];
  const char programHeader[lengthOfLcdDescription];
  const char programIdleTooltip[lengthOfLcdDescription];
};

template <byte numberOfPrograms, 
          byte numberOfUserButtons, 
          byte lengthOfLcdDescription>
struct guProgramsCfg { 
  using programConfig = guProgramConfig<numberOfUserButtons, lengthOfLcdDescription>;
  const programConfig *c;
  byte currentProgram;
  
  guProgramsCfg(const programConfig* config):c(config) {};
  Action getActionForButton(BtnId buttonId) const { return c[currentProgram].v[buttonId]; }
  void setProgram(byte programNumber) { currentProgram = programNumber % numberOfPrograms; }
  byte getProgram() { return currentProgram % numberOfPrograms; }
  void next() { currentProgram = (currentProgram + 1) % numberOfPrograms; }
  void prev() { currentProgram = (currentProgram == 0 ? currentProgram = numberOfPrograms - 1 : currentProgram - 1); }
  String header(){ return c[currentProgram].programHeader; }
  String tooltip(){ return c[currentProgram].programIdleTooltip; }
  };


#endif
