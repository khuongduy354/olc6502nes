# Todo 
- why use supportive variable instead of plain registers. 
for example: accumulator -> fetched -> ALU. 
  why not:    accumulator -> ALU.
- cpu and bus 
  A bus is used to read or write data.when present in code,
  should i do cpu.bus.write() or bus.cpu.write();  

# Notes 
ADC: supports both signed and unsigned 
for example: 1000100 can be understood by both 132 and -124 
check formula for this addition, XOR is to check if 2 bits differnt 
 
https://www.nesdev.org/wiki/Status_flags  flags usage after set a value 
