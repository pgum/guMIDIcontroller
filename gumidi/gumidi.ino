#include <MIDIUSB.h>
#include <AceButton.h>
#include <ButtonConfig.h>
#include "helpers.h"
#include "gumidi.h"
#include "gulcd.h"
#include "gushift.h"

using namespace ace_button;

namespace {
const char* ProductName = "GuMIDI mk.2";
const char* ProductVersion = "0.9003";

constexpr byte btn0pin6 = 6;
constexpr byte btn1pin7 = 7;
constexpr byte btn2pin8 = 8;
constexpr byte btn3pin9 = 9;
constexpr byte shiftPrevPin10 = 10;
constexpr byte shiftNextPin16 = 16;

constexpr byte lcdI2C = 0x27;
constexpr byte lcdRows = 16;
constexpr byte lcdCols = 2;
guLcd lcd(lcdI2C, lcdRows, lcdCols); 

guShift shiftButtons {};

void handleShiftEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto id = button->getId();
  if(eventType == AceButton::kEventPressed) {
    if(id == Id2Byte(CfgId::Id0)){ shiftButtons.prevMode(); Serial.println("Prev Program"); }
    if(id == Id2Byte(CfgId::Id1)){ shiftButtons.nextMode(); Serial.println("Next Program"); }
  lcd.printProgramChange(shiftButtons.status(), shiftButtons.tooltip());
  }
}

constexpr MidiCfg<7> midiButtonsConfig[] {
  { btn0pin6, CfgId::Id0, { Note(36), Note(41), Note(45), Note(50), Stop(), CC(21), CC(25) } }, 
  { btn1pin7, CfgId::Id1, { Note(37), Note(42), Note(46), Note(51), Play(), CC(22), CC(26) } },
  { btn2pin8, CfgId::Id2, { Note(38), Note(43), Note(48), Note(53), Loop(), CC(23), CC(27) } },
  { btn3pin9, CfgId::Id3, { Note(39), Note(44), Note(49), Note(54), Record(), CC(24), CC(28) } }                                                                                                
};
constexpr byte numOfMidiButtons = sizeof(midiButtonsConfig)/sizeof(midiButtonsConfig[0]);

ButtonConfig midiConfig {};
AceButton midiButtons[numOfMidiButtons];

}

void handleMidiEvent(AceButton* button, uint8_t eventType, uint8_t) {
  const auto id = button->getId();
  const auto mode = shiftButtons.Mode();
  if(eventType == AceButton::kEventPressed) {
    const auto midi = midiButtonsConfig[id].getMidi(mode);
    if(midi.isNote()){
      sendToggleMidi(midi);
      lcd.printSendToggleMidi(id, midi);
    }
    else{
      sendOnceMidi(midi);
      lcd.printSendOnceMidi(id, midi);
    }
  }
}

void sendOnceMidi(const MidiValue& midi) {
  MidiUSB.sendMIDI(midi.getPacket());
  MidiUSB.flush();
}

void sendToggleMidi(const MidiValue& midi) {
  MidiUSB.sendMIDI(midi.turnOnPacket());
  MidiUSB.flush();
  delay(1);
  MidiUSB.sendMIDI(midi.turnOffPacket());
  MidiUSB.flush();
}

void initializeMidiButtons() {
  for(const auto &b : midiButtonsConfig){
    pinMode(b.pin, INPUT_PULLUP);
    auto id = Id2Byte(b.id);
    midiButtons[id].setButtonConfig(&midiConfig);
    midiButtons[id].init(b.pin, HIGH, id);
  }
}

void setupMidi() {
  midiConfig.setEventHandler(handleMidiEvent);
  midiConfig.setFeature(ButtonConfig::kFeatureSuppressAll);
  initializeMidiButtons();
}

void setup() {
  constexpr long serialBaud = 115200;
  Serial.begin(serialBaud);
  lcd.intro(__DATE__, __TIME__, ProductName, ProductVersion, numOfMidiButtons);
  shiftButtons.init(handleShiftEvent, shiftPrevPin10, shiftNextPin16);
  setupMidi();
  lcd.printProgramChange(shiftButtons.status(), shiftButtons.tooltip());
}

void loop() {
  shiftButtons.check();
  for(auto &midiButton : midiButtons) midiButton.check();
}
