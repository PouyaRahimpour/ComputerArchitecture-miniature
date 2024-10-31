# Miniature - MIPS-like assembler and simulator
assembler for a simple assembly language similar to mips assembly.
The project has two phases.
## Phase 1
Converts an assembly code to machine code.
- compile:
```
gcc -o assembler main.c utils.c assemble.c
```

- run:
```
./assmbler input output
```
use test cases in tests/ dir as input.

## Phase 2
Decodes and executes machine code provided in previous phase, simulating mips hardware with c functions. The code prints ProgramCounter, Register contents, instruction by instruction.
- compile: 
```
gcc -o main main.c utils.c assemble.c
```
- run
```
./main tests2/addi.as addi.m
```

use test cases in tests2/ dir as input.


