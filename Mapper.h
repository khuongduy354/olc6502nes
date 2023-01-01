#include <cstdint>
class Mapper {
public:
  Mapper(uint8_t prgbanks, uint8_t chrbanks);
  ~Mapper();

public:
  // Transform CPU bus address into PRG ROM offset
  virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
  virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
  // Transform PPU bus address into CHR ROM offset
  virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
  virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
  uint8_t prgbanks = 0;
  uint8_t chrbanks = 0;
};
