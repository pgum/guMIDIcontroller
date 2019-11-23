# guMIDI - Dependencies  
* AceButton - for everything to work :)
* midiUSB - to send MIDI over USB
* LiquidCrystal_I2C - for guLCD - to connect to hd44780 with I2C multiplexer
* EEPROM - to save and restore last used program on startup  

All of these are available from Package Manager in Arduino IDE.  

To modify programs (there are 7 here) in ino file change `midiButtonsConfig` and sadly (no time for full refactoring) change numOfModes in gushift.h.  

Feel free to leave issue on github if you have comments or feature requests although I don't plan on developing this much more.  
Code is used in "production" (made one copy of device, and it work :) ).  

Feel free to modify and use as defined by GNU GPL3, it would be awesome if you can give me a notice that you use this - it's always nice to know that this helped someone :)