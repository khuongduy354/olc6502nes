#include "olc6502.h"
#include "bus.h"

void olc6502::write(uint16_t addr, uint8_t data) { bus->write(addr, data); }
uint8_t olc6502::read(uint16_t addr) {
  bus->read(addr);
  return 0
}

void olc6502::clock() {
  if (cycles == 0) {
    opcode = read(pc++);
    cycles = lookup[opcode].clock_cycles_required;
    uint8_t cycles_for_address = (*this->lookup[opcode].address_mode_prep)();
    uint8_t cycles_for_ops = (*this->lookup[opcode].operate)();
    cycles = cycles_for_address + cycles_for_ops;
  }
  cycles--;
};
uint8_t olc6502::fetch() { fetched = read(pc++); }

// ADDRESS MODES
// read next byte value
uint8_t olc6502::IMM() {
  addr = pc++;
  return 0;
};

// no data needed;
uint8_t olc6502::IMP() {
  fetched = a;
  return 0;
}

// OPCODES
// load into accumulator
uint8_t olc6502::LDA() {
  fetch();
  a = fetched;
  return 0;
};

// ZP0 with X offset
uint8_t olc6502::ZPX() {
  addr = read(pc + x);
  pc++;
  return 0;
}

// only take offset (low) bytes as address -> optimize speed
uint8_t olc6502::ZP0() {
  addr = read(pc++);
  return 0;
};

// ZP0 with Y offset
uint8_t olc6502::ZPY() {
  addr = read(pc + y);
  pc++;
  return 0;
}

// return 16 bit address
uint8_t olc6502::ABS() {
  uint8_t high = read(pc++);
  uint8_t low = read(pc++);
  addr = (high << 8) | low;
  return 0;
}

// ABS with X offset
uint8_t olc6502::ABX() {
  uint8_t high = read(pc++);
  uint8_t low = read(pc++);
  addr = (high << 8) | low + x;

  // if x offest causes change pages, 1 additional cc is needed;
  if ((addr & 0xFF00) != (high << 8)) {
    return 1;
  }
  return 0;
}

uint8_t olc6502::ABY() {
  uint8_t high = read(pc++);
  uint8_t low = read(pc++);
  addr = (high << 8) | low + y;

  // if x offest causes change pages, 1 additional cc is needed;
  if ((addr & 0xFF00) != (high << 8)) {
    return 1;
  }
  return 0;
}

// relative address
uint8_t olc6502::REL() {
  uint8_t next_byte = read(pc++);
  // next 8 bits bytes has 256 options, range from -128 to 127
  // first bit in that is sign or not.
  if (next_byte & 0x80) {
    addr_rel = pc + next_byte & 0x7F;
  } else {
    addr_rel = pc - next_byte & 0x7F;
  }
  return 0;
}

// indirect address, value from address in RAM, is another address :D
uint8_t olc6502::IND() {
  uint8_t low = read(pc++);
  uint8_t high = read(pc++);
  uint16_t ind_addr = (high << 8) | low + y;
  addr = (read(ind_addr + 1) << 8 | read(ind_addr));
  return 0;
}

// indirect address, but page 0x00 with offset
uint8_t olc6502::IZX() {
  uint8_t offset = read(pc++);
  addr = 0x0000 | (offset + x);
  addr = read(addr + 1) << 8 | read(addr);

  return 0;
}

uint8_t olc6502::IZY() {
  uint8_t offset = read(pc++);
  addr = 0x0000 | (offset + y);
  addr = read(addr + 1) << 8 | read(addr);

  return 0;
}
