#include "gulcd.h"

guLcd::guLcd(i2cAddress i2cAddr, lcdDimention rows, lcdDimention cols): lcd(i2cAddr, rows, cols), rows(rows), cols(cols) {};

constexpr uint8_t customCharacterMusicNote[] = { 0x08, 0x0C, 0x0E, 0x0B, 0x09, 0x19, 0x1B, 0x03 };
void guLcd::init(){
  lcd.init();
  lcd.createChar(1, customCharacterMusicNote);
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
}

void guLcd::printWithTimeout(const String& text, timeMilliSeconds duration, lcdRowIndex line) {
  print(text, line, duration);
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

void guLcd::printProgram(const guProgramConfig<4>* program) {
    const String emptyLine5Width= String("     ");
    const String emptyLine64Width= String("                                                                ");
    
    String programNameRaw= String(program->programName) + emptyLine64Width;
    String leftSideRaw= program->assignedAction[0].toString() + emptyLine5Width;
    String rightSideRaw = emptyLine5Width + program->assignedAction[3].toString();
    
    const byte sidesWidth =4;
    String programName= programNameRaw.substring(0,cols- (2*sidesWidth));
    String leftSide= leftSideRaw.substring(0, sidesWidth);
    String rightSide= rightSideRaw.substring(rightSideRaw.length()-sidesWidth);
    String upperLine= leftSide + programName + rightSide;
    
    String lowerLeftSideRaw= String("  ") + program->assignedAction[1].toString() + emptyLine64Width;
    String lowerRightSideRaw= emptyLine64Width + program->assignedAction[2].toString() + String("  ");
    String lowerLeftSide = lowerLeftSideRaw.substring(0,cols/2);
    String lowerRightSide = lowerRightSideRaw.substring(lowerRightSideRaw.length()- (cols/2));
    String lowerLine= lowerLeftSide + lowerRightSide;
  print(upperLine, firstLine);
  print(lowerLine, secondLine);
  extendLcdBacklight();
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
