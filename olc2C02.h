#pragma once
#include "cartridge.h"
#include "olcPixelGameEngine.h"
#include <cstdint>
#include <memory>
class olc2C02 {
public:
  olc2C02();
  ~olc2C02();

private:
  std::shared_ptr<Cartridge> cart;

public:
  // Communication with Main Bus
  uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
  void cpuWrite(uint16_t addr, uint8_t data);

  // Communication with PPU Bus
  uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);
  void ppuWrite(uint16_t addr, uint8_t data);

  void ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge);
  void clock();

private:
  uint8_t tblName[2][1024];
  uint8_t tblPattern[2][4096];
  uint8_t tblPalette[32];

private:
  olc::Pixel palScreen[0x40];
  // error here call to deleted constructor, see javidx
  olc::Sprite sprScreen = olc::Sprite(256, 240);
  olc::Sprite sprNameTable[2] = {olc::Sprite(256, 240), olc::Sprite(256, 240)};
  olc::Sprite sprPatternTable[2] = {olc::Sprite(128, 128),
                                    olc::Sprite(128, 128)};

public:
  // Debugging Utilities
  olc::Sprite &GetScreen();
  olc::Sprite &GetNameTable(uint8_t i);
  olc::Sprite &GetPatternTable(uint8_t i);
  bool frame_complete = false;

private:
  int16_t scanline = 0;
  int16_t cycle = 0;
};
