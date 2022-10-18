#include "olc6052.h"

void olc6052::write(uint16_t addr, uint8_t data) { bus->write(addr, data); }
uint8_t olc6052::read(uint16_t addr) { bus->read(addr); }
