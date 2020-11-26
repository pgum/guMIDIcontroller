#include "gulcd.h"

namespace Gu::Lcd {

guLcd::guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols): lcd(i2cAddr, rows, cols), rows(rows), cols(cols) {};

constexpr uint8_t customCharacterMusicNote[] = { 0x08, 0x0C, 0x0E, 0x0B, 0x09, 0x19, 0x1B, 0x03 };
constexpr uint8_t customCharacterArrowUp[] = { 0x04, 0x0E, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E };
constexpr uint8_t customCharacterArrowDown[] = { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1F, 0x0E, 0x04 };
void guLcd::init(){
  lcd.init();
  lcd.createChar(1, customCharacterMusicNote);
  lcd.createChar(2, customCharacterArrowUp);
  lcd.createChar(3, customCharacterArrowDown);
  lcd.clear();
  noBacklight();
  delay(halfSecond);
  backlight();  
}

void guLcd::print(const String& text, lcdRowIndex line, timeMilliSeconds duration) {
  if(line >= rows) return;
  lcd.setCursor(0, line);
  lcd.print(prepareLineToPrint(text));
  if( duration > 0 ){
    lcdTimeoutPrintTime = millis() + duration;
  }
  if( duration == 0 ){
      lastPermanentLine[line] = text;
  }
    extendLcdBacklight();
}

void guLcd::printWithTimeout(const String& text, timeMilliSeconds duration, lcdRowIndex line) {
  print(text, line, duration);
}

void guLcd::printBothLines(const String& first, const String& second, timeMilliSeconds duration) {
  print(first, firstLine, duration);
  print(second, secondLine, duration);
}
void guLcd::printWithProgramHalt(const String& first, const String& second, timeMilliSeconds duration) {
  print(first, firstLine);
  print(second, secondLine);
  delay(duration);
}

String guLcd::prepareLineToPrint(const String& source) {
  const String emptyLine64Width = "                                                                "; //lol
  String textToDisplay = source + emptyLine64Width;
  return textToDisplay.substring(0, cols);
}

void guLcd::intro(const String& date, const String& time, const String& name, const String& ver, const String& ProgramsAndButtons) {
  backlight();  
  printWithProgramHalt(date, time);
  printWithProgramHalt(name, "v" + String(ver));
  printWithProgramHalt(ProgramsAndButtons, String("Setup Complete!"));
  lcd.clear();
}

void guLcd::extendLcdBacklight(timeMilliSeconds duration) {
  auto newTime = millis() + duration;
  lcdBacklightTime = lcdBacklightTime < newTime ? newTime : lcdBacklightTime;
}
void guLcd::toggleDisplay(){ isDisplay() and not alwaysOn ? noDisplay() : display(); }

void guLcd::updateBacklight(timeMilliSeconds now){
  if(alwaysOn){
    if(not isDisplay()) { display(); }
    return; 
  } 
  if(not isDisplay()) return;
  if(lcdBacklightTime != 0){
    if(now < lcdBacklightTime){
      display();
      backlight();
    }else{
      //error: 'noBacklight' was not declared in this scope
      noBacklight();
      noDisplay();
      lcdBacklightTime = 0;
    }
  } 
}

void guLcd::restoreLine(lcdRowIndex line){
  if(line >= maxRows) return;
  print(lastPermanentLine[line], line);  
}

void guLcd::updateTimeoutPrint(timeMilliSeconds now){
  if(lcdTimeoutPrintTime != 0 and now > lcdTimeoutPrintTime){
      restoreLine(secondLine);
      lcdTimeoutPrintTime = 0;
  }
}

void guLcd::update(){
  auto now = millis();
  updateBacklight(now);
  updateTimeoutPrint(now);
}

bool guLcd::isDisplay() { return isdisplay; }
bool guLcd::isBacklight() { return isbacklight; }
void guLcd::backlight() { lcd.backlight(); isbacklight = true; }
void guLcd::noBacklight() { lcd.noBacklight(); isbacklight = false; }
void guLcd::display() { lcd.display(); lcd.backlight(); isdisplay = true; }
void guLcd::noDisplay() { lcd.noBacklight(); lcd.noDisplay(); isdisplay = false; }
void guLcd::toggleAlwaysOn(){ alwaysOn ? disableAlwaysOn() : enableAlwaysOn(); }
void guLcd::enableAlwaysOn(){ alwaysOn = true; isbacklight = true; printWithTimeout("LCD Always On   ", oneSecond ); }
void guLcd::disableAlwaysOn(){ alwaysOn = false; printWithTimeout("LCD has timeout ", oneSecond); }

}