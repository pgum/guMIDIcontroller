#include "gulcd.h"

guLcd::guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols): lcd(i2cAddr, rows, cols) {};
const byte customCharacterMusicNote[] = { 0x08, 0x0C, 0x0E, 0x0B, 0x09, 0x19, 0x1B, 0x03 };
void guLcd::init(){
  lcd.init();
  lcd.createChar(1, customCharacterMusicNote);
  lcd.clear();
  lcd.noBacklight();
  delay(500);
  lcd.backlight();  
}
void guLcd::intro(const String date, const String time, const String name, const String ver, byte buttons) {
  printIntroScreen(date, time);
  printIntroScreen(name, "ver." + String(ver));
  printIntroScreen("Buttons: " + String(buttons) + " ", String("Setup Complete!"));
  lcd.home();
  lcd.clear();
  lcd.cursor();
}
void guLcd::printProgramChange(const String status, const String tooltip) {
  printFirstLine(status);
  printSecondLine(tooltip);
  lcdDelayPrintTime=0;
  extendLcdBacklight(extendLcdBacklightMs);
}
void guLcd::printFirstLine(const String text) {
  if(text.length() == 0) return;
  lcd.setCursor(0, 0);
  lcd.print(text);
}
void guLcd::printSecondLine(const String text) {
  if(text.length() == 0) return;
  lcd.setCursor(0, 1);
  lcd.print(text);
}
void guLcd::printWithDelay(String text, unsigned long timeMs) {
  auto newTime = millis() + (timeMs == 0 ? lcdRedrawTooltipAfterMs : timeMs);
  lcdDelayPrintTime = newTime;
  delayString = text;
}
void guLcd::printIntroScreen(const String first, const String second) {
  lcd.backlight();  
  lcd.home();
  lcd.print(first);
  lcd.setCursor(0,1);
  lcd.print(second);
  delay(2000);
}
bool guLcd::isBacklight() { return isbacklight; }
void guLcd::backlight() { lcd.backlight(); isbacklight = true;}
void guLcd::noBacklight() { lcd.noBacklight(); isbacklight = false;}
void guLcd::enableAlwaysOn(){ alwaysOn = true; isbacklight = true;};
void guLcd::disableAlwaysOn(){ alwaysOn = false; };
void guLcd::setExtendLcdBacklightMs(timeMilliSeconds timeMs){ extendLcdBacklightMs = timeMs; }
void guLcd::setLcdRedrawTooltipAfterMs(timeMilliSeconds timeMs){ lcdRedrawTooltipAfterMs = timeMs; }

void guLcd::extendLcdBacklight(unsigned long timeMs) {
  auto extension = (timeMs == 0 ? extendLcdBacklightMs : timeMs);
  auto newTime = millis() + extension;
  lcdBacklightTime = lcdBacklightTime < newTime ? newTime : lcdBacklightTime;
}
void guLcd::updateBacklight(timeMilliSeconds now){
  if(alwaysOn){
    backlight();
    return;
  }
  if(lcdBacklightTime != 0){
    if(now < lcdBacklightTime){
      backlight();
    }else{
      noBacklight();
      lcdBacklightTime = 0;
    }
  } 
}
void guLcd::updateDelayPrint(timeMilliSeconds now){
  if(lcdDelayPrintTime != 0){
    if(now > lcdDelayPrintTime){
      printSecondLine(delayString);
      lcdDelayPrintTime = 0;
    }
  } 
}
void guLcd::update(){
  auto now = millis();
  updateBacklight(now);
  updateDelayPrint(now);
}
