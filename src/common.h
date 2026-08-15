#ifndef PNDX64_ASM_H
#define PNDX64_ASM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Instruction format types */
typedef enum {
    FORMAT_R,       /* 3 registers: opcode | Rd | Rs1 | Rs2 | Func | reserved */
    FORMAT_RR,      /* 2 registers: opcode | Rd | Rs1 | Func | reserved */
    FORMAT_RI,      /* Register + immediate: opcode | Rd | Rs1 | imm47 */
    FORMAT_I,       /* Large immediate: opcode | Rd | imm52 */
    FORMAT_M,       /* Memory: opcode | Rd | Base | offset47 */
    FORMAT_B,       /* Branch: opcode | Rs1 | Rs2 | offset47 */
    FORMAT_J,       /* Jump: opcode | offset57 */
    FORMAT_S,       /* System: opcode | func8 | imm49 */
    FORMAT_F,       /* Floating point: opcode | Fd | Fs1 | Fs2 | Func | reserved */
} InstructionFormat;

/* Operand types */
typedef enum {
    OPERAND_NONE,
    OPERAND_REG,
    OPERAND_FREG,
    OPERAND_CREG,
    OPERAND_IMM,
    OPERAND_LABEL,
    OPERAND_ADDR,   /* [Base + offset] */
} OperandType;

/* Instruction definition */
typedef struct {
    const char *mnemonic;
    uint8_t opcode;
    InstructionFormat format;
    uint8_t operand_count;
    OperandType operands[4];
    const char *description;
} InstructionDef;

/* Token types */
typedef enum {
    TOK_EOF,
    TOK_MNEMONIC,
    TOK_REGISTER,
    TOK_FREG,
    TOK_CREG,
    TOK_IMMEDIATE,
    TOK_LABEL,
    TOK_COMMA,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_PLUS,
    TOK_COLON,
    TOK_NEWLINE,
    TOK_COMMENT,
} TokenType;

/* Token */
typedef struct {
    TokenType type;
    char value[256];
    uint64_t numval;
    int line;
    int column;
} Token;

/* Symbol table entry */
typedef struct {
    char name[256];
    uint64_t address;
    bool is_label;
    int line;
} Symbol;

/* Symbol table */
typedef struct {
    Symbol *symbols;
    size_t count;
    size_t capacity;
} SymbolTable;

/* Operand value */
typedef struct {
    OperandType type;
    union {
        uint8_t reg;        /* 0-31 for general registers, 0-15 for float */
        int64_t immediate;
        char label[256];
        struct {
            uint8_t base;
            int64_t offset;
        } addr;
    } value;
} Operand;

/* Parsed instruction */
typedef struct {
    const InstructionDef *def;
    Operand operands[4];
    uint8_t operand_count;
    int line;
} ParsedInstruction;

/* Assembler context */
typedef struct {
    FILE *input;
    FILE *output;
    SymbolTable symbols;
    ParsedInstruction *instructions;
    size_t instruction_count;
    size_t instruction_capacity;
    uint64_t current_address;
    int current_line;
    char error_msg[512];
    bool has_error;
} Assembler;

/* Function declarations */

/* Lexer */
Token tokenize_next(FILE *input, int *line, int *column);

/* Symbol table */
void symbol_table_init(SymbolTable *st);
void symbol_table_add(SymbolTable *st, const char *name, uint64_t address, bool is_label, int line);
bool symbol_table_lookup(SymbolTable *st, const char *name, uint64_t *address);
void symbol_table_free(SymbolTable *st);

/* Parser */
bool parse_instruction(Assembler *asm_ctx, Token *tokens, size_t token_count, ParsedInstruction *instr);

/* Encoder */
uint64_t encode_instruction(const ParsedInstruction *instr, uint64_t pc, SymbolTable *symbols);

/* Assembler */
Assembler* assembler_create(FILE *input, FILE *output);
void assembler_free(Assembler *asm_ctx);
bool assembler_pass1(Assembler *asm_ctx);
bool assembler_pass2(Assembler *asm_ctx);
bool assembler_run(Assembler *asm_ctx);

/* Utilities */
bool parse_immediate(const char *str, int64_t *value);
bool is_register(const char *str, uint8_t *reg);
bool is_freg(const char *str, uint8_t *reg);
bool is_creg(const char *str, uint8_t *reg);
void error_message(Assembler *asm_ctx, const char *fmt, ...);

/* External instruction table */
extern const InstructionDef instruction_table[];
extern const size_t instruction_count;

#endif /* PNDX64_ASM_H */