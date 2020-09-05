## mcpu-emu : an emulator for MCPU

MCPU is a neat minimal 8-bit CPU (CPU design by Tim Boescke, cpldcpu@opencores.org) - see its [opencores project file](https://opencores.org/projects/mcpu) and [GitHub repo](https://github.com/cpldcpu/MCPU). It fits into 32 macrocells on a CPLD and can operate on 64 bytes of RAM.

Being a minimal CPU it supports only four 8-bit instructions, that consist of 2 bit opcode and 6 bit address/immediate field.

It has two registers - an 8-bit accumulator and 6-bit program counter. MCPU supports 64 bytes of RAM shared between program and data.

The instruction set is:

| mnemonic | opcode | description
| -- | -- | --
| NOR | 00AAAAAA | Acc = Acc NOR mem\[AAAAAA\]
| ADD | 01AAAAAA | Acc = Acc +  mem\[AAAAAA\], update carry
| STA | 10AAAAAA | mem\[AAAAAA\] = Acc
| JCC | 11AAAAAA | PC = AAAAAA if carry = 0, clear carry


