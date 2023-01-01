#include "Mapper_000.h"
Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks) {}

Mapper_000::~Mapper_000() {}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
  // if PRGROM is 16KB
  //     0x8000 -> 0xBFFF: first 16kb
  //     0xC000 -> 0xFFFF: Mirror of 0x8000 -> 0xBFFF

  // mapped range: 0x0000 - 7FFF

  // if PRGROM is 32KB
  //     0x8000 -> 0xFFFF: Map

  if (addr >= 0x8000 && addr <= 0xFFFF) {
    mapped_addr = addr & (prgbanks > 1 ? 0x7FFF : 0x3FFF);
    return true;
  }

  return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
  if (addr >= 0x8000 && addr <= 0xFFFF) {
    mapped_addr = addr & (prgbanks > 1 ? 0x7FFF : 0x3FFF);
    return true;
  }

  return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
  // There is no mapping required for PPU
  // PPU Address Bus          CHR ROM
  // 0x0000 -> 0x1FFF: Map    0x0000 -> 0x1FFF
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    mapped_addr = addr;
    return true;
  }

  return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    if (chrbanks == 0) {
      // Treat as RAM
      mapped_addr = addr;
      return true;
    }
  }

  return false;
}
