# PNDX-64 Instruction Set Architecture

THIS IS STILL IN DEVELOPMENT. INSTRUCTIONS MAY BE ADDED OR REMOVED DURING DEVELOPMENT.

## Instruction Set

PNDX-64 uses a 7-bit primary opcode, providing 128 primary opcode values from
`0x00` through `0x7F`. 

All instructions are fixed-width 64-bit instructions.

| **Opcode (Hex)** | **Mnemonic** | **Format** | **Assembly Operands** | **Architecture Description** | **Type of Instruction** |
| :--------------: | :----------: | :--------: | :-------------------: | :-------------------------- | :---------------------: |
| **0x00** | `NOP` | S | None | Performs no operation and advances the program counter. | System |
| **0x01** | `HLT` | S | None | Halts processor execution. | System |
| **0x02** | `MOV` | RR | `Rd, Rs1` | Copies the value of `Rs1` into `Rd`. | Register |
| **0x03** | `LI` | I | `Rd, imm` | Loads an immediate value into `Rd`. | Immediate |
| **0x04** | `LUI` | I | `Rd, imm` | Loads an immediate value into the upper portion of `Rd`. | Immediate |
| **0x05** | `ADD` | R | `Rd, Rs1, Rs2` | Adds `Rs1` and `Rs2` and stores the result in `Rd`. | Arithmetic |
| **0x06** | `ADDI` | RI | `Rd, Rs1, imm` | Adds an immediate value to `Rs1` and stores the result in `Rd`. | Arithmetic |
| **0x07** | `SUB` | R | `Rd, Rs1, Rs2` | Subtracts `Rs2` from `Rs1` and stores the result in `Rd`. | Arithmetic |
| **0x08** | `SUBI` | RI | `Rd, Rs1, imm` | Subtracts an immediate value from `Rs1`. | Arithmetic |
| **0x09** | `MUL` | R | `Rd, Rs1, Rs2` | Multiplies `Rs1` by `Rs2` and stores the result in `Rd`. | Arithmetic |
| **0x0A** | `MULI` | RI | `Rd, Rs1, imm` | Multiplies `Rs1` by an immediate value. | Arithmetic |
| **0x0B** | `DIV` | R | `Rd, Rs1, Rs2` | Performs signed integer division of `Rs1` by `Rs2`. | Arithmetic |
| **0x0C** | `DIVI` | RI | `Rd, Rs1, imm` | Divides `Rs1` by an immediate value. | Arithmetic |
| **0x0D** | `MOD` | R | `Rd, Rs1, Rs2` | Calculates the signed remainder of `Rs1` divided by `Rs2`. | Arithmetic |
| **0x0E** | `MODI` | RI | `Rd, Rs1, imm` | Calculates the remainder of `Rs1` divided by an immediate value. | Arithmetic |
| **0x0F** | `NEG` | RR | `Rd, Rs1` | Performs two's-complement negation of `Rs1`. | Arithmetic |
| **0x10** | `AND` | R | `Rd, Rs1, Rs2` | Performs a bitwise AND between `Rs1` and `Rs2`. | Logical |
| **0x11** | `ANDI` | RI | `Rd, Rs1, imm` | Performs a bitwise AND between `Rs1` and an immediate value. | Logical |
| **0x12** | `OR` | R | `Rd, Rs1, Rs2` | Performs a bitwise OR between `Rs1` and `Rs2`. | Logical |
| **0x13** | `ORI` | RI | `Rd, Rs1, imm` | Performs a bitwise OR between `Rs1` and an immediate value. | Logical |
| **0x14** | `XOR` | R | `Rd, Rs1, Rs2` | Performs a bitwise XOR between `Rs1` and `Rs2`. | Logical |
| **0x15** | `XORI` | RI | `Rd, Rs1, imm` | Performs a bitwise XOR between `Rs1` and an immediate value. | Logical |
| **0x16** | `NOT` | RR | `Rd, Rs1` | Inverts every bit of `Rs1` and stores the result in `Rd`. | Logical |
| **0x17** | `SHL` | R | `Rd, Rs1, Rs2` | Logically shifts `Rs1` left by the amount specified in `Rs2`. | Bit Manipulation |
| **0x18** | `SHLI` | RI | `Rd, Rs1, imm` | Logically shifts `Rs1` left by an immediate amount. | Bit Manipulation |
| **0x19** | `SHR` | R | `Rd, Rs1, Rs2` | Logically shifts `Rs1` right with zero fill. | Bit Manipulation |
| **0x1A** | `SHRI` | RI | `Rd, Rs1, imm` | Logically shifts `Rs1` right by an immediate amount. | Bit Manipulation |
| **0x1B** | `SAR` | R | `Rd, Rs1, Rs2` | Arithmetic right-shifts `Rs1` while preserving its sign bit. | Bit Manipulation |
| **0x1C** | `SARI` | RI | `Rd, Rs1, imm` | Arithmetic right-shifts `Rs1` by an immediate amount. | Bit Manipulation |
| **0x1D** | `ROL` | R | `Rd, Rs1, Rs2` | Rotates the bits of `Rs1` left by the amount in `Rs2`. | Bit Manipulation |
| **0x1E** | `ROR` | R | `Rd, Rs1, Rs2` | Rotates the bits of `Rs1` right by the amount in `Rs2`. | Bit Manipulation |
| **0x1F** | `RORI` | RI | `Rd, Rs1, imm` | Rotates the bits of `Rs1` right by an immediate amount. | Bit Manipulation |
| **0x20** | `CLZ` | RR | `Rd, Rs1` | Counts the number of leading zero bits in `Rs1`. | Bit Manipulation |
| **0x21** | `CTZ` | RR | `Rd, Rs1` | Counts the number of trailing zero bits in `Rs1`. | Bit Manipulation |
| **0x22** | `POPCNT` | RR | `Rd, Rs1` | Counts the number of set bits in `Rs1`. | Bit Manipulation |
| **0x23** | `BSET` | R | `Rd, Rs1, Rs2` | Sets the bit selected by `Rs2` in `Rs1`. | Bit Manipulation |
| **0x24** | `BCLR` | R | `Rd, Rs1, Rs2` | Clears the bit selected by `Rs2` in `Rs1`. | Bit Manipulation |
| **0x25** | `BTGL` | R | `Rd, Rs1, Rs2` | Toggles the bit selected by `Rs2` in `Rs1`. | Bit Manipulation |
| **0x26** | `BTST` | R | `Rd, Rs1, Rs2` | Tests the bit selected by `Rs2` in `Rs1`. | Bit Manipulation |
| **0x27** | `BSETI` | RI | `Rd, Rs1, imm` | Sets the bit selected by an immediate value. | Bit Manipulation |
| **0x28** | `BCLRI` | RI | `Rd, Rs1, imm` | Clears the bit selected by an immediate value. | Bit Manipulation |
| **0x29** | `BTGLI` | RI | `Rd, Rs1, imm` | Toggles the bit selected by an immediate value. | Bit Manipulation |
| **0x2A** | `BTSTI` | RI | `Rd, Rs1, imm` | Tests the bit selected by an immediate value. | Bit Manipulation |
| **0x2B** | `BREV` | RR | `Rd, Rs1` | Reverses the bit order of `Rs1`. | Bit Manipulation |
| **0x2C** | `BYTEREV` | RR | `Rd, Rs1` | Reverses the byte order of `Rs1`. | Bit Manipulation |
| **0x2D** | `ZEXT` | RR | `Rd, Rs1` | Zero-extends a value into the destination register. | Bit Manipulation |
| **0x2E** | `SEXT` | RR | `Rd, Rs1` | Sign-extends a value into the destination register. | Bit Manipulation |
| **0x2F** | `SAT` | RR | `Rd, Rs1` | Saturates a value to the defined architectural range. | Arithmetic |
| **0x30** | `LD` | M | `Rd, [Rs1 + imm]` | Loads a 64-bit value from memory. | Memory |
| **0x31** | `ST` | M | `Rs2, [Rs1 + imm]` | Stores a 64-bit value to memory. | Memory |
| **0x32** | `LDB` | M | `Rd, [Rs1 + imm]` | Loads an 8-bit byte from memory. | Memory |
| **0x33** | `STB` | M | `Rs2, [Rs1 + imm]` | Stores an 8-bit byte to memory. | Memory |
| **0x34** | `LDH` | M | `Rd, [Rs1 + imm]` | Loads a 16-bit half-word from memory. | Memory |
| **0x35** | `STH` | M | `Rs2, [Rs1 + imm]` | Stores a 16-bit half-word to memory. | Memory |
| **0x36** | `LDW` | M | `Rd, [Rs1 + imm]` | Loads a 32-bit word from memory. | Memory |
| **0x37** | `STW` | M | `Rs2, [Rs1 + imm]` | Stores a 32-bit word to memory. | Memory |
| **0x38** | `LDBS` | M | `Rd, [Rs1 + imm]` | Loads an 8-bit byte and sign-extends it to 64 bits. | Memory |
| **0x39** | `LDHS` | M | `Rd, [Rs1 + imm]` | Loads a 16-bit half-word and sign-extends it. | Memory |
| **0x3A** | `LDWS` | M | `Rd, [Rs1 + imm]` | Loads a 32-bit word and sign-extends it. | Memory |
| **0x3B** | `LEA` | M | `Rd, [Rs1 + imm]` | Calculates an effective address without accessing memory. | Memory |
| **0x3C** | `PUSH` | RR | `Rs1` | Decrements the stack pointer and stores a register value. | Stack |
| **0x3D** | `POP` | RR | `Rd` | Loads a value from the stack and increments the stack pointer. | Stack |
| **0x3E** | `FENCE` | S | None | Enforces ordering of memory operations. | Memory |
| **0x3F** | `CACHE` | S | `function, imm` | Performs an architecture-defined cache operation. | Cache |
| **0x40** | `JMP` | J | `label` | Performs an unconditional PC-relative jump. | Control Flow |
| **0x41** | `CALL` | J | `label` | Performs a PC-relative subroutine call and saves the return address. | Control Flow |
| **0x42** | `RET` | S | None | Returns from a subroutine using the link/return register. | Control Flow |
| **0x43** | `BEQ` | B | `Rs1, Rs2, label` | Branches if `Rs1` equals `Rs2`. | Conditional Branch |
| **0x44** | `BNE` | B | `Rs1, Rs2, label` | Branches if `Rs1` does not equal `Rs2`. | Conditional Branch |
| **0x45** | `BLT` | B | `Rs1, Rs2, label` | Branches if `Rs1` is signed less than `Rs2`. | Conditional Branch |
| **0x46** | `BGE` | B | `Rs1, Rs2, label` | Branches if `Rs1` is signed greater than or equal to `Rs2`. | Conditional Branch |
| **0x47** | `BLTU` | B | `Rs1, Rs2, label` | Branches if `Rs1` is unsigned less than `Rs2`. | Conditional Branch |
| **0x48** | `BGEU` | B | `Rs1, Rs2, label` | Branches if `Rs1` is unsigned greater than or equal to `Rs2`. | Conditional Branch |
| **0x49** | `BLE` | B | `Rs1, Rs2, label` | Branches if `Rs1` is signed less than or equal to `Rs2`. | Conditional Branch |
| **0x4A** | `BGT` | B | `Rs1, Rs2, label` | Branches if `Rs1` is signed greater than `Rs2`. | Conditional Branch |
| **0x4B** | `JAL` | J | `Rd, label` | Jumps to a PC-relative address and stores the return address in `Rd`. | Control Flow |
| **0x4C** | `JALR` | RI | `Rd, Rs1, imm` | Jumps to `Rs1 + imm` and stores the return address in `Rd`. | Control Flow |
| **0x4D** | `JR` | RR | `Rs1` | Jumps to the address contained in `Rs1`. | Control Flow |
| **0x4E** | `CALLR` | RR | `Rs1` | Calls an address contained in `Rs1`. | Control Flow |
| **0x4F** | `TRAP` | S | `imm` | Generates a software trap or architecture-defined exception. | Exception |
| **0x50** | `CMP` | R | `Rd, Rs1, Rs2` | Compares two registers and produces a comparison result. | Comparison |
| **0x51** | `CMPI` | RI | `Rd, Rs1, imm` | Compares a register against an immediate value. | Comparison |
| **0x52** | `SLT` | R | `Rd, Rs1, Rs2` | Sets `Rd` to one if signed `Rs1 < Rs2`, otherwise zero. | Comparison |
| **0x53** | `SLTI` | RI | `Rd, Rs1, imm` | Sets `Rd` based on signed comparison with an immediate. | Comparison |
| **0x54** | `SLTU` | R | `Rd, Rs1, Rs2` | Sets `Rd` based on unsigned `Rs1 < Rs2`. | Comparison |
| **0x55** | `SLTUI` | RI | `Rd, Rs1, imm` | Sets `Rd` based on unsigned comparison with an immediate. | Comparison |
| **0x56** | `MIN` | R | `Rd, Rs1, Rs2` | Selects the signed minimum of two values. | Comparison |
| **0x57** | `MAX` | R | `Rd, Rs1, Rs2` | Selects the signed maximum of two values. | Comparison |
| **0x58** | `MINU` | R | `Rd, Rs1, Rs2` | Selects the unsigned minimum of two values. | Comparison |
| **0x59** | `MAXU` | R | `Rd, Rs1, Rs2` | Selects the unsigned maximum of two values. | Comparison |
| **0x5A** | `ABS` | RR | `Rd, Rs1` | Calculates the absolute value of a signed integer. | Arithmetic |
| **0x5B** | `CLAMP` | R | `Rd, Rs1, Rs2` | Restricts a value to an architecture-defined range. | Arithmetic |
| **0x5C** | `TEST` | R | `Rd, Rs1, Rs2` | Performs a logical test between two register values. | Logical |
| **0x5D** | `TESTI` | RI | `Rd, Rs1, imm` | Performs a logical test against an immediate value. | Logical |
| **0x5E** | `CMOV` | R | `Rd, Rs1, Rs2` | Conditionally moves a value based on the comparison state. | Conditional |
| **0x5F** | `CSEL` | R | `Rd, Rs1, Rs2` | Selects one of two register values based on the comparison state. | Conditional |
| **0x60** | `SYSCALL` | S | `imm` | Requests a service from the operating system or supervisor. | System |
| **0x61** | `SYSRET` | S | None | Returns from a system-call handler. | System |
| **0x62** | `IRET` | S | None | Returns from an interrupt handler and restores execution state. | Interrupt |
| **0x63** | `ERET` | S | None | Returns from an exception handler. | Exception |
| **0x64** | `BREAK` | S | `imm` | Generates a debugger breakpoint exception. | Debug |
| **0x65** | `MCR` | RR | `CRd, Rs1` | Writes a general-purpose register value to a control register. | Privileged |
| **0x66** | `MRC` | RR | `Rd, CRs1` | Reads a control register into a general-purpose register. | Privileged |
| **0x67** | `CLI` | S | None | Disables maskable interrupts. | Interrupt |
| **0x68** | `STI` | S | None | Enables maskable interrupts. | Interrupt |
| **0x69** | `TLBIV` | S | None | Invalidates TLB entries according to the architectural rules. | Memory Management |
| **0x6A** | `INVLPG` | RR | `Rs1` | Invalidates the translation associated with an address. | Memory Management |
| **0x6B** | `SRET` | S | None | Returns from supervisor-level execution. | Privileged |
| **0x6C** | `WFI` | S | None | Waits for an interrupt before continuing execution. | System |
| **0x6D** | `WFE` | S | None | Waits for an architecture-defined event. | System |
| **0x6E** | `FLSH` | S | None | Flushes architecture-defined processor state. | System |
| **0x6F** | `HALT` | S | None | Places the processor into a halted state. | System |
| **0x70** | `TAINT` | RR | `Rd, Rs1` | Applies or propagates security taint metadata. | Security |
| **0x71** | `UNTAINT` | RR | `Rd, Rs1` | Removes security taint metadata according to privilege rules. | Security |
| **0x72** | `CPYT` | RR | `Rd, Rs1` | Copies taint metadata associated with a register. | Security |
| **0x73** | `CLRT` | RR | `Rd, Rs1` | Clears taint metadata associated with a register. | Security |
| **0x74** | `TAGLD` | M | `Rd, [Rs1 + imm]` | Loads security-tag metadata associated with a memory location. | Security |
| **0x75** | `TAGST` | M | `Rs2, [Rs1 + imm]` | Stores security-tag metadata associated with a memory location. | Security |
| **0x76** | `TAGCHK` | M | `Rd, [Rs1 + imm]` | Checks the security tag associated with a memory location. | Security |
| **0x77** | `CFI` | S | `imm` | Performs an architecture-defined control-flow integrity operation. | Security |
| **0x78** | `RNG` | RR | `Rd` | Obtains a hardware or implementation-defined random value. | Security |
| **0x79** | `HASH` | R | `Rd, Rs1, Rs2` | Performs an architecture-defined hardware hashing operation. | Security |
| **0x7A** | `ATOMIC` | R | `Rd, Rs1, Rs2` | Performs an atomic operation selected by the function field. | Atomic |
| **0x7B** | `DISPLAY` | S | `function, imm` | Performs an operation on the PNDX display controller. | Hardware |
| **0x7C** | `DEBUG` | S | `function, imm` | Performs an architecture-defined debugging operation. | Debug |
| **0x7D** | `PMU` | S | `function, imm` | Controls or queries performance-monitoring hardware. | Hardware |
| **0x7E** | `EXT` | S | `function, imm` | Provides an extension opcode space for additional instructions. | Extension |
| **0x7F** | `CUSTOM` | S | `function, imm` | Reserved for experimental or implementation-specific instructions. | Extension |

---

# Extended Instruction Spaces

Several PNDX-64 instructions use a primary opcode as an
instruction-class selector rather than defining only one operation.

## Floating-Point Extension

Primary opcode:

    0x7E

The `Function` field selects the floating-point operation.

| **Function** | **Mnemonic** | **Format** | **Assembly Operands** | **Architecture Description** | **Type of Instruction** |
| :----------: | :----------: | :--------: | :-------------------: | :-------------------------- | :---------------------: |
| `0x00` | `FADD` | F | `Fd, Fs1, Fs2` | Adds two floating-point values. | Floating Point |
| `0x01` | `FSUB` | F | `Fd, Fs1, Fs2` | Subtracts two floating-point values. | Floating Point |
| `0x02` | `FMUL` | F | `Fd, Fs1, Fs2` | Multiplies two floating-point values. | Floating Point |
| `0x03` | `FDIV` | F | `Fd, Fs1, Fs2` | Divides two floating-point values. | Floating Point |
| `0x04` | `FNEG` | RR/F | `Fd, Fs1` | Negates a floating-point value. | Floating Point |
| `0x05` | `FABS` | RR/F | `Fd, Fs1` | Calculates the floating-point absolute value. | Floating Point |
| `0x06` | `FSQRT` | RR/F | `Fd, Fs1` | Calculates the square root of a floating-point value. | Floating Point |
| `0x07` | `FMIN` | F | `Fd, Fs1, Fs2` | Selects the smaller floating-point value. | Floating Point |
| `0x08` | `FMAX` | F | `Fd, Fs1, Fs2` | Selects the larger floating-point value. | Floating Point |
| `0x09` | `FCMP` | F | `Fd, Fs1, Fs2` | Compares two floating-point values. | Floating Point |
| `0x0A` | `ITOF` | F | `Fd, Rs1` | Converts an integer to floating point. | Conversion |
| `0x0B` | `FTOI` | F | `Rd, Fs1` | Converts floating point to integer. | Conversion |
| `0x0C` | `FCVT` | F | `Fd, Fs1` | Converts between supported floating-point formats. | Conversion |
| `0x0D` | `FMADD` | F | `Fd, Fs1, Fs2` | Performs a fused multiply-add operation. | Floating Point |
| `0x0E` | `FMSUB` | F | `Fd, Fs1, Fs2` | Performs a fused multiply-subtract operation. | Floating Point |
| `0x0F` | `FROUND` | RR/F | `Fd, Fs1` | Rounds a floating-point value according to the active rounding mode. | Floating Point |

## Atomic Extension

Primary opcode:

    0x7A

| **Function** | **Mnemonic** | **Format** | **Assembly Operands** | **Architecture Description** | **Type of Instruction** |
| :----------: | :----------: | :--------: | :-------------------: | :-------------------------- | :---------------------: |
| `0x00` | `CAS` | R | `Rd, Rs1, Rs2` | Atomically compares and conditionally exchanges values. | Atomic |
| `0x01` | `XCHG` | R | `Rd, Rs1` | Atomically exchanges a register and memory value. | Atomic |
| `0x02` | `ATADD` | R | `Rd, Rs1, Rs2` | Performs an atomic addition. | Atomic |
| `0x03` | `ATSUB` | R | `Rd, Rs1, Rs2` | Performs an atomic subtraction. | Atomic |
| `0x04` | `ATAND` | R | `Rd, Rs1, Rs2` | Performs an atomic AND operation. | Atomic |
| `0x05` | `ATOR` | R | `Rd, Rs1, Rs2` | Performs an atomic OR operation. | Atomic |
| `0x06` | `ATXOR` | R | `Rd, Rs1, Rs2` | Performs an atomic XOR operation. | Atomic |
| `0x07` | `LL` | M | `Rd, [Rs1]` | Performs a load-linked operation. | Atomic |
| `0x08` | `SC` | M | `Rd, [Rs1]` | Performs a store-conditional operation. | Atomic |

## Display Extension

Primary opcode:

    0x7B

| **Function** | **Mnemonic** | **Format** | **Assembly Operands** | **Architecture Description** | **Type of Instruction** |
| :----------: | :----------: | :--------: | :-------------------: | :-------------------------- | :---------------------: |
| `0x00` | `DISP_CLEAR` | S | None | Clears the display framebuffer. | Display |
| `0x01` | `DISP_FLUSH` | S | None | Flushes the framebuffer to the physical display. | Display |
| `0x02` | `DISP_VSYNC` | S | None | Waits for the display vertical synchronization interval. | Display |
| `0x03` | `DISP_SETMODE` | S | `imm` | Changes the active display mode. | Display |
| `0x04` | `DISP_GETMODE` | S | `Rd` | Reads the current display mode. | Display |
| `0x05` | `DISP_CURSOR` | S | `imm` | Controls the hardware cursor. | Display |
| `0x06` | `DISP_FBBASE` | S | `Rs1` | Sets the framebuffer base address. | Display |
| `0x07` | `DISP_FBINFO` | S | `Rd` | Returns framebuffer configuration information. | Display |

---

# Instruction Categories

| **Category** | **Opcode Range** | **Purpose** |
|---|---|---|
| Basic Integer | `0x00–0x0F` | Register movement and arithmetic |
| Logical | `0x10–0x1F` | Boolean and shift operations |
| Bit Manipulation | `0x20–0x2F` | Bit-level operations |
| Memory | `0x30–0x3F` | Loads, stores, stack and memory ordering |
| Control Flow | `0x40–0x4F` | Jumps, calls and branches |
| Comparison | `0x50–0x5F` | Comparisons and conditional operations |
| System / Privileged | `0x60–0x6F` | OS, interrupts and privileged operations |
| Security / Hardware | `0x70–0x7F` | Security, atomic, display and extension operations |

---