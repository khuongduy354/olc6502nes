#include "olc6502.h"
#include "bus.h"
// Constructor
olc6502::olc6502() {
  // Assembles the translation table. It's big, it's ugly, but it yields a
  // convenient way to emulate the 6502. I'm certain there are some "code-golf"
  // strategies to reduce this but I've deliberately kept it verbose for study
  // and alteration

  // It is 16x16 entries. This gives 256 instructions. It is arranged to that
  // the bottom 4 bits of the instruction choose the column, and the top 4 bits
  // choose the row.

  // For convenience to get function pointers to members of this class, I'm
  // using this or else it will be much much larger :D

  // The table is one big initialiser list of initialiser lists...
  using a = olc6502;
  lookup = {
      {"BRK", &a::BRK, &a::IMM, 7}, {"ORA", &a::ORA, &a::IZX, 6},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 3}, {"ORA", &a::ORA, &a::ZP0, 3},
      {"ASL", &a::ASL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"PHP", &a::PHP, &a::IMP, 3}, {"ORA", &a::ORA, &a::IMM, 2},
      {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABS, 4},
      {"ASL", &a::ASL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ZPX, 4},
      {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABX, 4},
      {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
      {"JSR", &a::JSR, &a::ABS, 6}, {"AND", &a::AND, &a::IZX, 6},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"BIT", &a::BIT, &a::ZP0, 3}, {"AND", &a::AND, &a::ZP0, 3},
      {"ROL", &a::ROL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},
      {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"BIT", &a::BIT, &a::ABS, 4}, {"AND", &a::AND, &a::ABS, 4},
      {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ZPX, 4},
      {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"SEC", &a::SEC, &a::IMP, 2}, {"AND", &a::AND, &a::ABY, 4},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ABX, 4},
      {"ROL", &a::ROL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
      {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IZX, 6},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 3}, {"EOR", &a::EOR, &a::ZP0, 3},
      {"LSR", &a::LSR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2},
      {"LSR", &a::LSR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4},
      {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BVC", &a::BVC, &a::REL, 2}, {"EOR", &a::EOR, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ZPX, 4},
      {"LSR", &a::LSR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ABX, 4},
      {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
      {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IZX, 6},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 3}, {"ADC", &a::ADC, &a::ZP0, 3},
      {"ROR", &a::ROR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"PLA", &a::PLA, &a::IMP, 4}, {"ADC", &a::ADC, &a::IMM, 2},
      {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4},
      {"ROR", &a::ROR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ZPX, 4},
      {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ABX, 4},
      {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 2}, {"STA", &a::STA, &a::IZX, 6},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
      {"STY", &a::STY, &a::ZP0, 3}, {"STA", &a::STA, &a::ZP0, 3},
      {"STX", &a::STX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
      {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},
      {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"STY", &a::STY, &a::ABS, 4}, {"STA", &a::STA, &a::ABS, 4},
      {"STX", &a::STX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
      {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IZY, 6},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
      {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4},
      {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
      {"TYA", &a::TYA, &a::IMP, 2}, {"STA", &a::STA, &a::ABY, 5},
      {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
      {"???", &a::NOP, &a::IMP, 5}, {"STA", &a::STA, &a::ABX, 5},
      {"???", &a::XXX, &a::IMP, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IZX, 6},
      {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6},
      {"LDY", &a::LDY, &a::ZP0, 3}, {"LDA", &a::LDA, &a::ZP0, 3},
      {"LDX", &a::LDX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
      {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2},
      {"TAX", &a::TAX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4},
      {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
      {"BCS", &a::BCS, &a::REL, 2}, {"LDA", &a::LDA, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
      {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4},
      {"LDX", &a::LDX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
      {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4},
      {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4},
      {"LDY", &a::LDY, &a::ABX, 4}, {"LDA", &a::LDA, &a::ABX, 4},
      {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::XXX, &a::IMP, 4},
      {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IZX, 6},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"CPY", &a::CPY, &a::ZP0, 3}, {"CMP", &a::CMP, &a::ZP0, 3},
      {"DEC", &a::DEC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"INY", &a::INY, &a::IMP, 2}, {"CMP", &a::CMP, &a::IMM, 2},
      {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
      {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4},
      {"DEC", &a::DEC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ZPX, 4},
      {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4},
      {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ABX, 4},
      {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
      {"CPX", &a::CPX, &a::IMM, 2}, {"SBC", &a::SBC, &a::IZX, 6},
      {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"CPX", &a::CPX, &a::ZP0, 3}, {"SBC", &a::SBC, &a::ZP0, 3},
      {"INC", &a::INC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
      {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},
      {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2},
      {"CPX", &a::CPX, &a::ABS, 4}, {"SBC", &a::SBC, &a::ABS, 4},
      {"INC", &a::INC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IZY, 5},
      {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
      {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ZPX, 4},
      {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
      {"SED", &a::SED, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABY, 4},
      {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
      {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ABX, 4},
      {"INC", &a::INC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
  };
}

void olc6502::write(uint16_t addr, uint8_t data) { bus->write(addr, data); }
uint8_t olc6502::read(uint16_t addr) {
  bus->read(addr);
  return 0;
}
bool olc6502::complete() { return cycles == 0; }
void olc6502::set_flag(FLAGBITS flag, bool is_set) {
  if (is_set) {
    status |= flag;
  } else {
    status &= ~flag;
  }
}
bool olc6502::get_flag(FLAGBITS flag) {
  return (status & flag) != 0 ? true : false;
}

void olc6502::clock() {
  if (cycles == 0) {
    opcode = read(pc++);
    cycles = lookup[opcode].clock_cycles_required;
    uint8_t cycles_for_address = (this->*lookup[opcode].addrmode)();
    uint8_t cycles_for_ops = (this->*lookup[opcode].operate)();
    cycles = cycles_for_address + cycles_for_ops;
  }
  cycles--;
};
uint8_t olc6502::fetch() {
  // TODO IMP mode
  fetched = read(pc++);
  return fetched;
}
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
  // first bit from left in that is sign flag
  if (next_byte & 0x80) { // if sign bit is 1 mean plus
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

// branch if carry set
uint8_t olc6502::BCS() {
  if (get_flag(C) == 1) {
    cycles++;
    uint16_t temp = pc + addr_rel;

    // if cross 1 page boundary, 1 more cycle
    if ((temp & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }
    pc = temp;
  };
  return 0;
}

// instructions
uint8_t olc6502::AND() {
  fetch();
  a = a & fetched;
  set_flag(Z, a == 0x00);
  set_flag(N, a & 0x80);
  return 1;
}

uint8_t olc6502::CLC() {
  set_flag(C, false);
  return 0;
}

uint8_t olc6502::PHA() {
  // 0x0100 is stack hardcoded base position
  write(0x0100 + sp--, a);
  return 0;
}
uint8_t olc6502::PLA() {
  sp++;
  a = read(0x0100 + sp);
  set_flag(Z, a == 0x00);
  set_flag(N, a & 0x80);
  return 0;
}

void olc6502::reset() {
  // Get address to set program counter to
  addr = 0xFFFC;
  uint16_t lo = read(addr + 0);
  uint16_t hi = read(addr + 1);
  pc = (hi << 8) | lo;

  // Reset internal registers
  a = 0;
  x = 0;
  y = 0;
  sp = 0xFD;
  status = 0x00 | U; // unused flag

  // Clear internal helper variables
  addr_rel = 0x0000;
  addr = 0x0000;
  fetched = 0x00;

  // Reset takes time
  cycles = 8;
}

void olc6502::irq() {
  if (get_flag(I) == 0) {
    // write pc to stack (2 locations due to 8 bit stack)
    write(0x010 + sp--, (pc >> 8) & 0xFF);
    write(0x010 + sp--, pc & 0xFF);

    // Then Push the status register to the stack
    set_flag(B, 0);
    set_flag(U, 1);
    set_flag(I, 1);
    write(0x0100 + sp--, status);

    // Read new program counter location from fixed address
    addr = 0xFFFE;
    uint16_t lo = read(addr + 0);
    uint16_t hi = read(addr + 1);
    pc = (hi << 8) | lo;

    cycles = 7;
  }
}

// exactly like irq, but with different address and cant be disabled
void olc6502::nmi() {
  // write pc to stack (2 locations due to 8 bit stack)
  write(0x010 + sp--, (pc >> 8) & 0xFF);
  write(0x010 + sp--, pc & 0xFF);

  // Then Push the status register to the stack
  set_flag(B, 0);
  set_flag(U, 1);
  set_flag(I, 1);
  write(0x0100 + sp--, status);

  // Read new program counter location from fixed address
  addr = 0xFFFA;
  uint16_t lo = read(addr + 0);
  uint16_t hi = read(addr + 1);
  pc = (hi << 8) | lo;

  cycles = 7;
}

std::map<uint16_t, std::string> olc6502::disassemble(uint16_t nStart,
                                                     uint16_t nStop) {
  uint32_t addr = nStart;
  uint8_t value = 0x00, lo = 0x00, hi = 0x00;
  std::map<uint16_t, std::string> mapLines;
  uint16_t line_addr = 0;

  // A convenient utility to convert variables into
  // hex strings because "modern C++"'s method with
  // streams is atrocious
  auto hex = [](uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
      s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
  };

  // Starting at the specified address we read an instruction
  // byte, which in turn yields information from the lookup table
  // as to how many additional bytes we need to read and what the
  // addressing mode is. I need this info to assemble human readable
  // syntax, which is different depending upon the addressing mode

  // As the instruction is decoded, a std::string is assembled
  // with the readable output
  while (addr <= (uint32_t)nStop) {
    line_addr = addr;

    // Prefix line with instruction address
    std::string sInst = "$" + hex(addr, 4) + ": ";

    // Read instruction, and get its readable name
    uint8_t opcode = bus->read(addr, true);
    addr++;
    sInst += lookup[opcode].instr_name + " ";

    // Get oprands from desired locations, and form the
    // instruction based upon its addressing mode. These
    // routines mimmick the actual fetch routine of the
    // 6502 in order to get accurate data as part of the
    // instruction
    if (lookup[opcode].addrmode == &olc6502::IMP) {
      sInst += " {IMP}";
    } else if (lookup[opcode].addrmode == &olc6502::IMM) {
      value = bus->read(addr, true);
      addr++;
      sInst += "#$" + hex(value, 2) + " {IMM}";
    } else if (lookup[opcode].addrmode == &olc6502::ZP0) {
      lo = bus->read(addr, true);
      addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + " {ZP0}";
    } else if (lookup[opcode].addrmode == &olc6502::ZPX) {
      lo = bus->read(addr, true);
      addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + ", X {ZPX}";
    } else if (lookup[opcode].addrmode == &olc6502::ZPY) {
      lo = bus->read(addr, true);
      addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
    } else if (lookup[opcode].addrmode == &olc6502::IZX) {
      lo = bus->read(addr, true);
      addr++;
      hi = 0x00;
      sInst += "($" + hex(lo, 2) + ", X) {IZX}";
    } else if (lookup[opcode].addrmode == &olc6502::IZY) {
      lo = bus->read(addr, true);
      addr++;
      hi = 0x00;
      sInst += "($" + hex(lo, 2) + "), Y {IZY}";
    } else if (lookup[opcode].addrmode == &olc6502::ABS) {
      lo = bus->read(addr, true);
      addr++;
      hi = bus->read(addr, true);
      addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
    } else if (lookup[opcode].addrmode == &olc6502::ABX) {
      lo = bus->read(addr, true);
      addr++;
      hi = bus->read(addr, true);
      addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
    } else if (lookup[opcode].addrmode == &olc6502::ABY) {
      lo = bus->read(addr, true);
      addr++;
      hi = bus->read(addr, true);
      addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
    } else if (lookup[opcode].addrmode == &olc6502::IND) {
      lo = bus->read(addr, true);
      addr++;
      hi = bus->read(addr, true);
      addr++;
      sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
    } else if (lookup[opcode].addrmode == &olc6502::REL) {
      value = bus->read(addr, true);
      addr++;
      sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
    }

    // Add the formed string to a std::map, using the instruction's
    // address as the key. This makes it convenient to look for later
    // as the instructions are variable in length, so a straight up
    // incremental index is not sufficient.
    mapLines[line_addr] = sInst;
  }

  return mapLines;
}
