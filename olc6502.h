#include "bus.h"
class olc6502 {
public:
  olc6502();
  void connectBus(Bus *b) { bus = b; }
  void write(uint16_t addr, uint8_t data);
  uint8_t read(uint16_t addr, bool bReadOnly);

private:
  Bus *bus = nullptr;
};
