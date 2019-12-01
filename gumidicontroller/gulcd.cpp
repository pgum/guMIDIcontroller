#include "gulcd.h"

guLcd::guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols): lcd(i2cAddr, rows, cols) {};
 byte customCharacterMusicNote[] = {
  0x08,
  0x0C,
  0x0E,
  0x0B,
  0x09,
  0x19,
  0x1B,
  0x03
};
void guLcd::intro(const String date, const String time, const String name, const String ver, byte buttons) {
  lcd.init();
  lcd.createChar(1, customCharacterMusicNote);
  lcd.clear();
  lcd.noBacklight();
  delay(500);
  lcd.backlight();  
  printIntroScreen(date, time);
  printIntroScreen(String(name), String("ver.") + String(ver));
  printIntroScreen(String("Buttons: ") + String(buttons) + String(" "), String("Setup Complete!"));
  lcd.home();
  lcd.clear();
  lcd.cursor();
}
void guLcd::printMidiSend(const byte id, const String text) {
  String line = "#" + String(id) + String("  ") + text;
  //lcd.backlight();
  printSecondLine(line);
}
void guLcd::printProgramChange(const String status, const String tooltip) {
  //lcd.backlight();
  printFirstLine(status);
  //printSecondLine(tooltip);
  printWithDelay(tooltip, 1);
  extendLcdBacklight(extendLcdBacklightMs);
  //Serial.print(status);
  //Serial.print(" ");
  //Serial.println(tooltip);
}
void guLcd::printFirstLine(const String text) {
  lcd.setCursor(0, 0);
  lcd.print(text);
  //Serial.println(text);
}
/*bool guLcd::isBacklight() {
  return isBacklit;
}*/
void guLcd::printSecondLine(const String text) {
  lcd.setCursor(0, 1);
  lcd.print(text);
  //Serial.println(text);
}
void guLcd::printWithDelay(String text, unsigned long timeMs) {
  timeMs = timeMs == 0 ? lcdRedrawTooltipAfterMs : timeMs;
  auto newTime = millis() + timeMs;
  lcdDelayPrintTime = newTime;
  delayString = text;
  //Serial.print("New print with delay on ");
  //Serial.print(lcdBacklightTime);
  //Serial.print(" text: ");
  //Serial.println(delayString);
}
void guLcd::printIntroScreen(const String first, const String second) {
  lcd.backlight();  
  lcd.home();
  lcd.print(first);
  lcd.setCursor(0,1);
  lcd.print(second);
  delay(3000);
  //Serial.println(first);
  //Serial.println(second);
}
void guLcd::noBacklight() {
  lcd.noBacklight();

}
void guLcd::backlight() {
  lcd.backlight();

}
void guLcd::enableAlwaysOn(){ alwaysOn = true; };
void guLcd::disableAlwaysOn(){ alwaysOn = false; };
void guLcd::setExtendLcdBacklightMs(timeMilliSeconds timeMs){ extendLcdBacklightMs = timeMs; }
void guLcd::setLcdRedrawTooltipAfterMs(timeMilliSeconds timeMs){ lcdRedrawTooltipAfterMs = timeMs; }

void guLcd::extendLcdBacklight(unsigned long timeMs) {
  auto newTime = millis() + timeMs;
  lcdBacklightTime = lcdBacklightTime < newTime ? newTime : lcdBacklightTime;
  //Serial.print("New backlight timeout: ");
  //Serial.println(lcdBacklightTime);
}
void guLcd::update(){
  auto now = millis();
if(not alwaysOn){
  if(lcdBacklightTime != 0){
    if(now > lcdBacklightTime){
      //Serial.print(now);
      //Serial.println(" Lights off!");
      lcd.noBacklight();
      lcdBacklightTime = 0;
    }else{
      lcd.backlight();
    }
  } 
}else{
  lcd.backlight();
}

if(lcdDelayPrintTime != 0){
  if(now > lcdDelayPrintTime){
      //Serial.print(now);
      //Serial.print(" reprint with delay: ");
      //Serial.println(delayString);
      printSecondLine(delayString);
      lcdDelayPrintTime = 0;
    }
  } 

}
