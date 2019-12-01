#ifndef guProgramsCfg_H
#define guProgramsCfg_H
#include <EEPROM.h>

using eepromAddress = byte;

struct UserValue {
  const byte value;
  const char* abbr8;
  constexpr static byte UserInputA = 10;
  constexpr static byte UserInputB = 11;
  String toString() const { return (String(value) +  String(" ") + ((abbr8 != NULL) ? String(abbr8) : String("        ")) + String("     ")); }
};

constexpr UserValue UserNone() { return {0, NULL}; }
constexpr UserValue UserSelectInputA() { return {UserValue::UserInputA, "A -> Out"}; }
constexpr UserValue UserSelectInputB() { return {UserValue::UserInputB, "B -> Out"}; }

struct Action {
  const MidiValue mv;
  const UserValue uv;
  const byte type;

  constexpr static byte ActionTypeNone = 0x00;
  constexpr static byte ActionTypeMidi = 0x01;
  constexpr static byte ActionTypeUser = 0x02;
  operator MidiValue(){ return mv; }
  operator UserValue(){ return uv; }
  String toString() const { 
    if(type == ActionTypeMidi) return mv.toString();
    if(type == ActionTypeUser) return uv.toString();
    return "None Type";
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
  const CfgId id;
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
  eepromAddress eepromAddrLastProgram;
  byte currentProgram;
  
  guProgramsCfg(const programConfig* config, const eepromAddress &eepromMemoryAddress):c(config), eepromAddrLastProgram(eepromMemoryAddress) {};
  void saveProgramIndex(){ EEPROM.update(eepromAddrLastProgram, currentProgram); }
  void loadProgramIndex(){ currentProgram = EEPROM.read(eepromAddrLastProgram) % numberOfPrograms; }
  Action get(byte id) const { return c[currentProgram].v[id]; }
  void next() { currentProgram = (currentProgram + 1) % numberOfPrograms; saveProgramIndex(); }
  void prev() { currentProgram = (currentProgram == 0 ? currentProgram = numberOfPrograms - 1 : currentProgram - 1); saveProgramIndex(); }
  String header(){ return c[currentProgram].programHeader; }
  String tooltip(){ return c[currentProgram].programIdleTooltip; }
  };


#endif
