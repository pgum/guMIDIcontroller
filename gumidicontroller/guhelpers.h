#ifndef guHelpers_H
#define guHelpers_H

using SerialBaudRate = unsigned long;
using timeMilliSeconds = unsigned long;
using timeType = unsigned long;

enum Time : timeType { ms = 1, s = 1000, min = 60*s, h = 60 * min, d = 24 * h, w = 7 * d };
constexpr timeMilliSeconds seconds = Time::s;
constexpr timeMilliSeconds oneSecond = 1 * seconds;
constexpr timeMilliSeconds halfSecond = 500;
constexpr timeMilliSeconds twoSeconds = 2 * seconds;
constexpr timeMilliSeconds threeSeconds = 3 * seconds;
constexpr timeMilliSeconds oneMinute = 60 * seconds;

using Id = byte;
using PinAddressType = byte;
using eepromAddress = byte;
using HwCommand = byte;

using UserBtnId = Id;
using CtrlBtnId = Id;
using BtnId = Id;
using ProgramId = Id;
using InputId = Id;

enum Pin : PinAddressType {Pin04 = 4, Pin05, Pin06, Pin07, Pin08, Pin09, Pin10, 
                    Pin11_NotUsable, Pin12_NotUsable, Pin13_NotUsable, 
                    Pin14, Pin15, Pin16, Pin17_NotUsable, Pin18, Pin19, Pin20, Pin21 };

const String emptyLine8Width= String("        ");
const String emptyLine64Width= String("                                                                ");
const String emptyLine128Width= emptyLine64Width+emptyLine64Width;

#endif
