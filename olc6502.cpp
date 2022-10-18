#include "olc6502.h"

void olc6502::write(uint16_t addr, uint8_t data) { bus->write(addr, data); }
uint8_t olc6502::read(uint16_t addr, bool bReadOnly) { bus->read(addr); }
