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

I've also added an "OUT" instruction according to Jean-Claude Wippler's [TFoC - A minimal computer](https://jeelabs.org/2017/11/tfoc---a-minimal-computer/) article, with the opcode of 0xFF that prints the accumulator to the standard output.
