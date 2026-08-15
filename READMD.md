# Pandimonimum 64 Bit (PNDX-64)

This repository is going to be a 64-bit custom computer architecture intended to be a cybersecurity sandbox for myself. It is designed specifically for educational purposes and security research. The PNDX architecture is planned to replace the previous 3rd Generation processor called PND32.

## Instruction Encoding

PNDX-64 uses a fixed-width 64-bit instruction encoding. Instruction formats are designed around the type and number of operands required by each instruction. The architecture uses a 7-bit primary opcode, providing 128 possible instruction classes, with function fields used to extend individual instruction classes where necessary.

### Format R (Register Operations)

Used for operations involving three registers, such as arithmetic and logical operations.

- **Opcode** (7 bits): Instruction class
- **Rd** (5 bits): Destination register
- **Rs1** (5 bits): Source register 1
- **Rs2** (5 bits): Source register 2
- **Function** (5+ bits): Operation subtype
- **Reserved**: Remaining bits

Example:

    ADD Rd, Rs1, Rs2
    SUB Rd, Rs1, Rs2
    AND Rd, Rs1, Rs2
    XOR Rd, Rs1, Rs2

### Format RI (Register + Immediate)

Used for operations involving two registers and an immediate constant.

- **Opcode** (7 bits): Instruction class
- **Rd** (5 bits): Destination register
- **Rs1** (5 bits): Source register
- **Immediate** (47 bits): Constant value

Example:

    ADDI Rd, Rs1, Immediate
    ANDI Rd, Rs1, Immediate
    ORI Rd, Rs1, Immediate

### Format RR (Register + Register)

Used for operations requiring two registers.

- **Opcode** (7 bits): Instruction class
- **Rd** (5 bits): Destination register
- **Rs1** (5 bits): Source register
- **Function** (5+ bits): Operation subtype
- **Reserved**: Remaining bits

Example:

    MOV Rd, Rs1
    NEG Rd, Rs1
    NOT Rd, Rs1
    CMP Rd, Rs1

### Format I (Immediate)

Used for instructions requiring a destination register and a large immediate value.

- **Opcode** (7 bits): Instruction class
- **Rd** (5 bits): Destination register
- **Immediate** (52 bits): Immediate constant

Example:

    LI Rd, Immediate
    LUI Rd, Immediate

### Format M (Memory Operations)

Used for load and store operations using a base register and an address offset.

- **Opcode** (7 bits): Memory operation
- **Rd** (5 bits): Destination or source register
- **Base** (5 bits): Base address register
- **Offset** (47 bits): Address offset

Example:

    LD Rd, [Base + Offset]
    ST Rd, [Base + Offset]
    LDB Rd, [Base + Offset]
    STB Rd, [Base + Offset]

For load instructions, `Rd` is the destination register. For store instructions, `Rd` represents the source register containing the value being stored.

### Format B (Conditional Branch)

Used for conditional control-flow operations.

- **Opcode** (7 bits): Branch condition
- **Rs1** (5 bits): Source register 1
- **Rs2** (5 bits): Source register 2
- **Offset** (47 bits): PC-relative branch offset

Example:

    BEQ Rs1, Rs2, Offset
    BNE Rs1, Rs2, Offset
    BLT Rs1, Rs2, Offset
    BGE Rs1, Rs2, Offset

Branch offsets are PC-relative, allowing programs to be relocated without requiring absolute branch addresses.

### Format J (Jump Operations)

Used for unconditional jumps, function calls, and other long-range control-flow operations.

- **Opcode** (7 bits): Jump operation
- **Offset** (57 bits): PC-relative jump offset

Example:

    JMP Offset
    CALL Offset

Jump instructions may use additional function or control bits if required by future revisions of the architecture.

### Format S (System and Special Operations)

Used for system-level, control, and special-purpose instructions that do not require the standard register layouts.

- **Opcode** (7 bits): System instruction class
- **Function** (8 bits): Specific system operation
- **Immediate** (49 bits): Optional instruction-specific value

Example:

    NOP
    HLT
    RET
    IRET
    SYSCALL
    BREAK

### Format F (Floating-Point Operations)

Used for floating-point arithmetic and other floating-point operations.

- **Opcode** (7 bits): Floating-point instruction class
- **Fd** (5 bits): Destination floating-point register
- **Fs1** (5 bits): Source floating-point register 1
- **Fs2** (5 bits): Source floating-point register 2
- **Function** (5+ bits): Operation subtype
- **Reserved**: Remaining bits

Example:

    FADD Fd, Fs1, Fs2
    FSUB Fd, Fs1, Fs2
    FMUL Fd, Fs1, Fs2
    FDIV Fd, Fs1, Fs2

## Instruction Format Summary

| Format | Purpose | Primary Operands |
|--------|---------|------------------|
| R | Register operations | Rd, Rs1, Rs2 |
| RI | Register + immediate operations | Rd, Rs1, Immediate |
| RR | Two-register operations | Rd, Rs1 |
| I | Large immediate operations | Rd, Immediate |
| M | Memory operations | Rd, Base, Offset |
| B | Conditional branches | Rs1, Rs2, Offset |
| J | Jumps and calls | Offset |
| S | System and special operations | Function, Immediate |
| F | Floating-point operations | Fd, Fs1, Fs2 |

## Instruction Set Design

The PNDX-64 instruction set is intended to support:

- Integer arithmetic and logical operations
- Bitwise and shift operations
- Register manipulation
- Memory access
- Conditional and unconditional control flow
- Function calls and returns
- System calls and privileged operations
- Floating-point operations
- Atomic operations
- Bit manipulation
- Security-oriented instructions
- Hardware and architecture experimentation

The instruction set is expected to contain more than 64 instructions. Rather than assigning every instruction a unique encoding structure, PNDX-64 uses instruction classes and function fields to allow related instructions to share the same encoding format.

This provides additional opcode space for future expansion while keeping the instruction decoder, assembler, disassembler, and emulator relatively simple.

## Register Architecture

PNDX-64 is designed around a 64-bit register architecture. Register fields are 5 bits wide, allowing up to 32 general-purpose registers.

### General-Purpose Registers

Each general-purpose register is 64 bits wide.

    R0  - General-purpose register
    R1  - General-purpose register
    ...
    R31 - General-purpose register

The exact register conventions, including the stack pointer, program counter, link register, and special-purpose registers, will be defined as the architecture develops.

## Design Goals

PNDX-64 is intended to prioritize:

1. **Educational Value** — The architecture should be understandable enough to study and modify.
2. **Security Research** — The architecture should provide a controlled environment for experimenting with security mechanisms.
3. **Extensibility** — The ISA should have sufficient opcode space for future instructions and architectural features.
4. **Deterministic Behavior** — The emulator and reference implementation should provide predictable architectural behavior.
5. **Implementation Simplicity** — The instruction formats should remain straightforward to decode and implement.
6. **Architectural Experimentation** — Features such as pipelining, branch prediction, privilege levels, memory protection, and other CPU concepts can be implemented and tested.

PNDX-64 is a new architectural generation rather than simply a 64-bit extension of PND32. The encoding, register architecture, instruction set, and execution model are being redesigned to provide a cleaner foundation for future development.