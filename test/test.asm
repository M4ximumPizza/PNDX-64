; PNDX-64 Assembly Test Program
; Demonstrates all major instruction types

; ============================================================================
; Section: Data setup
; ============================================================================

start:
    LI      R1, 100         ; Load immediate: R1 = 100
    LI      R2, 200         ; Load immediate: R2 = 200
    LI      R3, 50          ; Load immediate: R3 = 50

; ============================================================================
; Section: Basic arithmetic
; ============================================================================
arithmetic:
    ADD     R4, R1, R2      ; Add: R4 = R1 + R2 (300)
    SUB     R5, R4, R3      ; Subtract: R5 = R4 - R3 (250)
    MUL     R6, R3, R3      ; Multiply: R6 = R3 * R3 (2500)
    
    ; Immediate versions
    ADDI    R7, R1, 50      ; Add immediate: R7 = R1 + 50 (150)
    SUBI    R8, R2, 75      ; Subtract immediate: R8 = R2 - 75 (125)
    MULI    R9, R3, 2       ; Multiply immediate: R9 = R3 * 2 (100)

; ============================================================================
; Section: Logical operations
; ============================================================================
logical:
    LI      R10, 0xFF       ; R10 = 255
    LI      R11, 0x0F       ; R11 = 15
    
    AND     R12, R10, R11   ; Bitwise AND: R12 = 0xFF & 0x0F (0x0F = 15)
    OR      R13, R10, R11   ; Bitwise OR: R13 = 0xFF | 0x0F (0xFF = 255)
    XOR     R14, R10, R11   ; Bitwise XOR: R14 = 0xFF ^ 0x0F (0xF0 = 240)
    NOT     R15, R10        ; Bitwise NOT: R15 = ~0xFF
    
    ; Immediate versions
    ANDI    R16, R10, 0xF0  ; AND immediate
    ORI     R17, R11, 0xF0  ; OR immediate
    XORI    R18, R10, 0x0F  ; XOR immediate

; ============================================================================
; Section: Bit manipulation
; ============================================================================
bitops:
    CLZ     R19, R1         ; Count leading zeros
    CTZ     R20, R1         ; Count trailing zeros
    POPCNT  R21, R10        ; Count set bits
    
    BSET    R22, R1, R3     ; Set bit at position R3
    BCLR    R23, R1, R3     ; Clear bit at position R3
    BTST    R24, R1, R3     ; Test bit at position R3
    
    SHLI    R25, R1, 2      ; Shift left immediate: R1 << 2
    SHRI    R26, R1, 2      ; Shift right immediate: R1 >> 2
    SARI    R27, R1, 2      ; Arithmetic shift right: R1 >>> 2

; ============================================================================
; Section: Comparison and conditional operations
; ============================================================================
comparison:
    CMP     R28, R4, R5     ; Compare: R4 vs R5
    SLT     R29, R4, R5     ; Set if R4 < R5
    SLTU    R30, R4, R5     ; Unsigned less-than
    
    CMPI    R31, R1, 100    ; Compare immediate
    MIN     R1, R4, R5      ; Minimum
    MAX     R2, R4, R5      ; Maximum
    MINU    R3, R4, R5      ; Unsigned minimum
    MAXU    R4, R4, R5      ; Unsigned maximum
    ABS     R5, R1          ; Absolute value

; ============================================================================
; Section: Memory operations (commented - requires memory mapping)
; ============================================================================
memory:
    ; These would require a valid memory address
    ; LD      R6, [R1+0]     ; Load from [R1]
    ; ST      R2, [R1+8]     ; Store to [R1+8]
    ; LDW     R7, [R1+0]     ; Load word
    ; STW     R3, [R1+0]     ; Store word
    ; LEA     R8, [R1+256]   ; Load effective address
    
    NOP                     ; No-op placeholder

; ============================================================================
; Section: Control flow - branches
; ============================================================================
branches:
    LI      R10, 100
    LI      R11, 100
    
    BEQ     R10, R11, equal_case       ; Branch if equal
    JMP     not_equal_case              ; Jump over equal case
    
equal_case:
    LI      R12, 1          ; R12 = 1 if equal
    JMP     branch_done
    
not_equal_case:
    LI      R12, 0          ; R12 = 0 if not equal
    
branch_done:
    BNE     R10, R11, skip_lt_test      ; Branch if not equal
    BLT     R10, R11, less_case
    
less_case:
    LI      R13, -1
    JMP     skip_lt_test
    
skip_lt_test:
    BGE     R10, R11, greater_case      ; Branch if >=
    
greater_case:
    LI      R14, 1
    JMP     flow_test_done
    
flow_test_done:
    NOP

; ============================================================================
; Section: Jump and link
; ============================================================================
jumps:
    JAL     R15, subroutine            ; Jump and link
    
    ; After return, we'd continue here
    JMP     end_program
    
subroutine:
    LI      R20, 42
    RET                                ; Return to caller

; ============================================================================
; Section: Data with various immediate formats
; ============================================================================
immediates:
    LI      R1, 123         ; Decimal
    LI      R2, 0xFF        ; Hexadecimal
    LI      R3, 0b1010      ; Binary
    LI      R4, 0o777       ; Octal
    LI      R5, -42         ; Negative

; ============================================================================
; Program end
; ============================================================================
end_program:
    HLT                     ; Halt execution