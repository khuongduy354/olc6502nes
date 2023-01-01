#pragma once
#include "olc2C02.h"
#include "olc6502.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

class Bus {
public:
  Bus();
  ~Bus();

public: // Devices on bus
  olc6502 cpu;
  olc2C02 ppu;

  // Fake RAM for this part of the series
  std::array<uint8_t, 2 * 1024> cpuRam;

private:
  std::shared_ptr<Cartridge> cart;

public: // Bus Read & Write
  void cpuWrite(uint16_t addr, uint8_t data);
  uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public:
  void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
  void reset();
  void clock();

public:
  uint32_t systemClockCounter = 0;
};
