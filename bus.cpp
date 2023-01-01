#include "bus.h"

Bus::Bus() { cpu.ConnectBus(this); };
Bus::~Bus(){};
void Bus::cpuWrite(uint16_t addr, uint8_t data) {
  if (cart->cpuWrite(addr, data)) {
    // The cartridge "sees all" and has the facility to veto
    // the propagation of the bus transaction if it requires.
    // This allows the cartridge to map any address to some
    // other data, including the facility to divert transactions
    // with other physical devices. The NES does not do this
    // but I figured it might be quite a flexible way of adding
    // "custom" hardware to the NES in the future!
  } else if (addr >= 0x0000 && addr <= 0x1FFF) {
    // System RAM Address Range. The range covers 8KB, though
    // there is only 2KB available. That 2KB is "mirrored"
    // through this address range. Using bitwise AND to mask
    // the bottom 11 bits is the same as addr % 2048.
    cpuRam[addr & 0x07FF] = data;

  } else if (addr >= 0x2000 && addr <= 0x3FFF) {
    // PPU Address range. The PPU only has 8 primary registers
    // and these are repeated throughout this range. We can
    // use bitwise AND operation to mask the bottom 3 bits,
    // which is the equivalent of addr % 8.
    ppu.cpuWrite(addr & 0x0007, data);
  }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;
  if (cart->cpuRead(addr, data)) {
    // Cartridge Address Range
  } else if (addr >= 0x0000 && addr <= 0x1FFF) {
    // System RAM Address Range, mirrored every 2048
    data = cpuRam[addr & 0x07FF];
  } else if (addr >= 0x2000 && addr <= 0x3FFF) {
    // PPU Address range, mirrored every 8
    data = ppu.cpuRead(addr & 0x0007, bReadOnly);
  }

  return data;
}

void Bus::reset() {
  systemClockCounter = 0;
  cpu.reset();
}
void Bus::clock() {
  // Clocking. The heart and soul of an emulator. The running
  // frequency is controlled by whatever calls this function.
  // So here we "divide" the clock as necessary and call
  // the peripheral devices clock() function at the correct
  // times.

  // The fastest clock frequency the digital system cares
  // about is equivalent to the PPU clock. So the PPU is clocked
  // each time this function is called.
  ppu.clock();

  // The CPU runs 3 times slower than the PPU so we only call its
  // clock() function every 3 times this function is called. We
  // have a global counter to keep track of this.
  if (systemClockCounter % 3 == 0) {
    cpu.clock();
  }

  systemClockCounter++;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
  this->cart = cartridge;
  ppu.ConnectCartridge(cartridge);
}
