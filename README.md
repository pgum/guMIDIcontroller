# guMIDIcontroller - Dependencies  
* AceButton - for everything to work :)
* midiUSB - to send MIDI over USB
* LiquidCrystal_I2C - for guLCD - to connect to hd44780 with I2C multiplexer
* EEPROM - to save and restore last used program and input jack on startup .  

All of these are available from Package Manager in Arduino IDE.  

## Features
* 7 program banks
* Sends MIDI CC or notes over USB
* Can control A/B input audio switch
* Can be extended to control other features over user buttons
* Keeps last used program bank on power-off
* Keeps input audio switch on power-off
* Turns off LCD sreen after set timeout

## Known issues:  
* Names could be better  
* Should refactor HWConfig to incorporate more things or change name to ButtonConfig  
* UserAction, MidiValue and Action structs are mutants, because of them there are if's everywhere  
* Dunno how to deal with EEPROM access. For now its handled in main file. 
* Not all "raw" types are changed into meaningful aliases.
* Control Switches (ctrlBtn2 and next in future) need to be checked upon setup also.

## Pictures

https://imgur.com/a/rM1fs5H

## Take it and modify it

Feel free to leave issue on github if you have comments or feature requests although I don't plan on developing this much more.  
Code is used in "production" (made one copy of device, and it work :) ).  

Feel free to modify and use as defined by GNU GPL3, it would be awesome if you can give me a notice that you use this - it's always nice to know that this helped someone :)
