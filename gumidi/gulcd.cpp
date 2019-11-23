#include "gulcd.h"

guLcd::guLcd(byte i2cAddr, byte rows, byte cols): lcd(i2cAddr, rows, cols) {};
constexpr byte customCharacterMusicNote[] = {
  0x08,
  0x0C,
  0x0E,
  0x0B,
  0x09,
  0x19,
  0x1B,
  0x03
};
void guLcd::intro(String date, String time, String name, String ver, int buttons) {
  lcd.init();
  lcd.createChar(1, customCharacterMusicNote);
  lcd.clear();
  lcd.noBacklight();
  delay(250);
  lcd.backlight();  
  printIntroScreen(date, time);
  printIntroScreen(String(name), String("ver.") + String(ver));
  printIntroScreen(String("Buttons: ") + String(buttons) + String(" "), String("Setup Complete!"));
  lcd.home();
  lcd.clear();
  lcd.cursor();
}
void guLcd::printProgramChange(String status, String tooltip) {
  printFirstLine(status);
  printSecondLine(tooltip);
}
void guLcd::printFirstLine(String text) {
  lcd.setCursor(0, 0);
  lcd.print(text);
  Serial.println(text);
}
void guLcd::printSecondLine(String text) {
  lcd.setCursor(0, 1);
  lcd.print(text);
}
void guLcd::printIntroScreen(const String first, const String second) {
  lcd.home();
  lcd.print(first);
  lcd.setCursor(0,1);
  lcd.print(second);
  delay(1000);
  Serial.println(first);
  Serial.println(second);
}
void guLcd::printSendToggleMidi(const byte id, const MidiValue m){
  String line = "#" + String(id) + String("  ") +  m.toString();
  Serial.println(line);
  printSecondLine(line);
}
void guLcd::printSendOnceMidi(const byte id, const MidiValue m){
  String line = "#" + String(id) + String("  ") +  m.toString();
  Serial.println(line);
  printSecondLine(line);
}
