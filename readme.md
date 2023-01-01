# Todo 
- debug,demo2 not up  
 
# important 
 in olcPixelGameEngine i commented the Sprite = delete in class Sprite 
 
# Notes  
- cpu and bus read RAM. 
  bus read RAM data 
  cpu access RAM from bus
-> bus.read() access the RAM directly
-> cpu.bus.read() 
-> cpu.read() is just a shortcut function 
  ```cpp 
  cpu::read(addr){ 
    bus->read(addr)
  }
  ``` 
- Catridge reads from its own PRG, hence no bus.read 
- PPU reads from Catridge's PRG //unknown yet
 
- learn some NES address mode and opcode
ADC: supports both signed and unsigned 
for example: 1000100 can be understood by both 132 and -124 
check formula for this addition, XOR is to check if 2 bits differnt 
 
- flag usage 
https://www.nesdev.org/wiki/Status_flags  flags usage after set a value   

- bitwise operators: 
sign 2's compilements 

- RAM 2kb, but total 8kb due to mirroring, some weird addresses are also due to mirroring

- weird design in read/write in bus, ppu, cartridge comes first due to some of javidx future intentions  

- mapping, mirroing mechanism https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=6 

- learn about javidx olcPixelEngine, to understand more about ppu 
