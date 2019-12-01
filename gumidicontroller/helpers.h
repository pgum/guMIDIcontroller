#ifndef Helpers_H
#define Helpers_H

using SerialBaudRate = unsigned long;
using TimerOneFireRateUS = unsigned long;
using lcdStayAwakeTimeMS = unsigned long;
enum class CfgId: byte {Id0 = 0x0, Id1, Id2, Id3, Id4, Id5, Id6, Id7, Id8, Id9, Id10};
constexpr byte Id2Byte(CfgId id){ return static_cast<byte>(id); }

#endif
