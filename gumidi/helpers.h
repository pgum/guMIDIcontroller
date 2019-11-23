#ifndef Helpers_H
#define Helpers_H

enum class CfgId: byte {Id0 = 0x0, Id1, Id2, Id3};
constexpr byte Id2Byte(CfgId id){ return static_cast<byte>(id); }

#endif
