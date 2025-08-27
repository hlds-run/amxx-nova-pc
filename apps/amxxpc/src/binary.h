// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

#ifndef _INCLUDE_BINARY_H
#define _INCLUDE_BINARY_H

#include "amx.h"
#include <stdio.h>

#ifdef HAVE_STDINT_H
    #include <stdint.h>
#else
using int8_t = __int8;
using uint8_t = unsigned __int8;
using int16_t = __int16;
using uint16_t = unsigned __int16;
#endif

class BinaryReader {
  public:
    BinaryReader(FILE* fp);
    //~BinaryReader();
    uint32_t ReadUInt32() const;
    int32_t ReadInt32() const;
    uint16_t ReadUInt16() const;
    int16_t ReadInt16() const;
    uint8_t ReadUInt8() const;
    int8_t ReadInt8() const;
    char* ReadChars(char buffer[], size_t chars) const;

  private:
    bool ReadAddr(void* buffer, size_t size) const;

    FILE* m_Fp;
};

class BinaryWriter {
  public:
    BinaryWriter(FILE* fp);

    void WriteUInt32(uint32_t num) const;
    void WriteInt32(int32_t num) const;
    void WriteUInt16(uint16_t num) const;
    void WriteInt16(int16_t num) const;
    void WriteUInt8(uint8_t num) const;
    void WriteInt8(int8_t num) const;
    void WriteChars(const char buffer[], size_t chars) const;

  private:
    bool WriteAddr(const void* buffer, size_t size) const;

    FILE* m_Fp;
};

#endif //_INCLUDE_BINARY_H
