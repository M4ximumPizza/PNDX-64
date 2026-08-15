#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <strings.h>

/* ============================================================================
   INSTRUCTION TABLE - PNDX-64 ISA
   ============================================================================ */

const InstructionDef instruction_table[] = {
    /* 0x00-0x0F: Basic Integer Operations */
    {"NOP",     0x00, FORMAT_S, 0, {}, "No operation"},
    {"HLT",     0x01, FORMAT_S, 0, {}, "Halt processor"},
    {"MOV",     0x02, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Copy register"},
    {"LI",      0x03, FORMAT_I, 2, {OPERAND_REG, OPERAND_IMM}, "Load immediate"},
    {"LUI",     0x04, FORMAT_I, 2, {OPERAND_REG, OPERAND_IMM}, "Load upper immediate"},
    {"ADD",     0x05, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Add"},
    {"ADDI",    0x06, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Add immediate"},
    {"SUB",     0x07, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Subtract"},
    {"SUBI",    0x08, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Subtract immediate"},
    {"MUL",     0x09, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Multiply"},
    {"MULI",    0x0A, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Multiply immediate"},
    {"DIV",     0x0B, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Divide"},
    {"DIVI",    0x0C, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Divide immediate"},
    {"MOD",     0x0D, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Modulo"},
    {"MODI",    0x0E, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Modulo immediate"},
    {"NEG",     0x0F, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Negate"},

    /* 0x10-0x1F: Logical Operations */
    {"AND",     0x10, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Bitwise AND"},
    {"ANDI",    0x11, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "AND immediate"},
    {"OR",      0x12, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Bitwise OR"},
    {"ORI",     0x13, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "OR immediate"},
    {"XOR",     0x14, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Bitwise XOR"},
    {"XORI",    0x15, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "XOR immediate"},
    {"NOT",     0x16, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Bitwise NOT"},
    {"SHL",     0x17, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Shift left"},
    {"SHLI",    0x18, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Shift left immediate"},
    {"SHR",     0x19, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Shift right"},
    {"SHRI",    0x1A, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Shift right immediate"},
    {"SAR",     0x1B, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Arithmetic shift right"},
    {"SARI",    0x1C, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Arithmetic shift right immediate"},
    {"ROL",     0x1D, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Rotate left"},
    {"ROR",     0x1E, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Rotate right"},
    {"RORI",    0x1F, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Rotate right immediate"},

    /* 0x20-0x2F: Bit Manipulation */
    {"CLZ",     0x20, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Count leading zeros"},
    {"CTZ",     0x21, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Count trailing zeros"},
    {"POPCNT",  0x22, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Count set bits"},
    {"BSET",    0x23, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Set bit"},
    {"BCLR",    0x24, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Clear bit"},
    {"BTGL",    0x25, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Toggle bit"},
    {"BTST",    0x26, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Test bit"},
    {"BSETI",   0x27, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Set bit immediate"},
    {"BCLRI",   0x28, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Clear bit immediate"},
    {"BTGLI",   0x29, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Toggle bit immediate"},
    {"BTSTI",   0x2A, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Test bit immediate"},
    {"BREV",    0x2B, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Reverse bits"},
    {"BYTEREV", 0x2C, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Reverse bytes"},
    {"ZEXT",    0x2D, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Zero extend"},
    {"SEXT",    0x2E, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Sign extend"},
    {"SAT",     0x2F, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Saturate"},

    /* 0x30-0x3F: Memory */
    {"LD",      0x30, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load 64-bit"},
    {"ST",      0x31, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Store 64-bit"},
    {"LDB",     0x32, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load byte"},
    {"STB",     0x33, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Store byte"},
    {"LDH",     0x34, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load half-word"},
    {"STH",     0x35, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Store half-word"},
    {"LDW",     0x36, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load word"},
    {"STW",     0x37, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Store word"},
    {"LDBS",    0x38, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load signed byte"},
    {"LDHS",    0x39, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load signed half-word"},
    {"LDWS",    0x3A, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load signed word"},
    {"LEA",     0x3B, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load effective address"},
    {"PUSH",    0x3C, FORMAT_RR, 1, {OPERAND_REG}, "Push register"},
    {"POP",     0x3D, FORMAT_RR, 1, {OPERAND_REG}, "Pop register"},
    {"FENCE",   0x3E, FORMAT_S, 0, {}, "Memory ordering barrier"},
    {"CACHE",   0x3F, FORMAT_S, 1, {OPERAND_IMM}, "Cache control"},

    /* 0x40-0x4F: Control Flow */
    {"JMP",     0x40, FORMAT_J, 1, {OPERAND_LABEL}, "Jump"},
    {"CALL",    0x41, FORMAT_J, 1, {OPERAND_LABEL}, "Call"},
    {"RET",     0x42, FORMAT_S, 0, {}, "Return"},
    {"BEQ",     0x43, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch equal"},
    {"BNE",     0x44, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch not equal"},
    {"BLT",     0x45, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch less-than"},
    {"BGE",     0x46, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch greater/equal"},
    {"BLTU",    0x47, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch unsigned less-than"},
    {"BGEU",    0x48, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch unsigned greater/equal"},
    {"BLE",     0x49, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch less/equal"},
    {"BGT",     0x4A, FORMAT_B, 3, {OPERAND_REG, OPERAND_REG, OPERAND_LABEL}, "Branch greater-than"},
    {"JAL",     0x4B, FORMAT_J, 2, {OPERAND_REG, OPERAND_LABEL}, "Jump and link"},
    {"JALR",    0x4C, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Jump and link register"},
    {"JR",      0x4D, FORMAT_RR, 1, {OPERAND_REG}, "Jump register"},
    {"CALLR",   0x4E, FORMAT_RR, 1, {OPERAND_REG}, "Call register"},
    {"TRAP",    0x4F, FORMAT_S, 1, {OPERAND_IMM}, "Trap"},

    /* 0x50-0x5F: Comparison */
    {"CMP",     0x50, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Compare"},
    {"CMPI",    0x51, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Compare immediate"},
    {"SLT",     0x52, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Signed less-than"},
    {"SLTI",    0x53, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Signed less-than immediate"},
    {"SLTU",    0x54, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Unsigned less-than"},
    {"SLTUI",   0x55, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Unsigned less-than immediate"},
    {"MIN",     0x56, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Signed minimum"},
    {"MAX",     0x57, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Signed maximum"},
    {"MINU",    0x58, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Unsigned minimum"},
    {"MAXU",    0x59, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Unsigned maximum"},
    {"ABS",     0x5A, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Absolute value"},
    {"CLAMP",   0x5B, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Clamp"},
    {"TEST",    0x5C, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Test"},
    {"TESTI",   0x5D, FORMAT_RI, 3, {OPERAND_REG, OPERAND_REG, OPERAND_IMM}, "Test immediate"},
    {"CMOV",    0x5E, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Conditional move"},
    {"CSEL",    0x5F, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Conditional select"},

    /* 0x60-0x6F: System/Privileged */
    {"SYSCALL", 0x60, FORMAT_S, 1, {OPERAND_IMM}, "System call"},
    {"SYSRET",  0x61, FORMAT_S, 0, {}, "System return"},
    {"IRET",    0x62, FORMAT_S, 0, {}, "Interrupt return"},
    {"ERET",    0x63, FORMAT_S, 0, {}, "Exception return"},
    {"BREAK",   0x64, FORMAT_S, 1, {OPERAND_IMM}, "Debug breakpoint"},
    {"MCR",     0x65, FORMAT_RR, 2, {OPERAND_CREG, OPERAND_REG}, "Move to control register"},
    {"MRC",     0x66, FORMAT_RR, 2, {OPERAND_REG, OPERAND_CREG}, "Move from control register"},
    {"CLI",     0x67, FORMAT_S, 0, {}, "Clear interrupts"},
    {"STI",     0x68, FORMAT_S, 0, {}, "Set interrupts"},
    {"TLBIV",   0x69, FORMAT_S, 0, {}, "TLB invalidate"},
    {"INVLPG",  0x6A, FORMAT_RR, 1, {OPERAND_REG}, "Invalidate page"},
    {"SRET",    0x6B, FORMAT_S, 0, {}, "Supervisor return"},
    {"WFI",     0x6C, FORMAT_S, 0, {}, "Wait for interrupt"},
    {"WFE",     0x6D, FORMAT_S, 0, {}, "Wait for event"},
    {"FLSH",    0x6E, FORMAT_S, 0, {}, "Flush state"},
    {"HALT",    0x6F, FORMAT_S, 0, {}, "Halt"},

    /* 0x70-0x7F: Security/Hardware/Display */
    {"TAINT",   0x70, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Apply taint"},
    {"UNTAINT", 0x71, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Remove taint"},
    {"CPYT",    0x72, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Copy taint"},
    {"CLRT",    0x73, FORMAT_RR, 2, {OPERAND_REG, OPERAND_REG}, "Clear taint"},
    {"TAGLD",   0x74, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Load tag"},
    {"TAGST",   0x75, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Store tag"},
    {"TAGCHK",  0x76, FORMAT_M, 2, {OPERAND_REG, OPERAND_ADDR}, "Check tag"},
    {"CFI",     0x77, FORMAT_S, 1, {OPERAND_IMM}, "Control flow integrity"},
    {"RNG",     0x78, FORMAT_RR, 1, {OPERAND_REG}, "Random number"},
    {"HASH",    0x79, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Hash"},
    {"ATOMIC",  0x7A, FORMAT_R, 3, {OPERAND_REG, OPERAND_REG, OPERAND_REG}, "Atomic op"},
    {"DISPLAY", 0x7B, FORMAT_S, 2, {OPERAND_IMM, OPERAND_IMM}, "Display control"},
    {"DEBUG",   0x7C, FORMAT_S, 2, {OPERAND_IMM, OPERAND_IMM}, "Debug op"},
    {"PMU",     0x7D, FORMAT_S, 2, {OPERAND_IMM, OPERAND_IMM}, "Performance monitor"},
    {"EXT",     0x7E, FORMAT_S, 2, {OPERAND_IMM, OPERAND_IMM}, "Extended instruction"},
    {"CUSTOM",  0x7F, FORMAT_S, 2, {OPERAND_IMM, OPERAND_IMM}, "Custom opcode"},
};

const size_t instruction_count = sizeof(instruction_table) / sizeof(InstructionDef);

/* ============================================================================
   SYMBOL TABLE IMPLEMENTATION
   ============================================================================ */

void symbol_table_init(SymbolTable *st) {
    st->capacity = 256;
    st->count = 0;
    st->symbols = malloc(st->capacity * sizeof(Symbol));
}

void symbol_table_add(SymbolTable *st, const char *name, uint64_t address, bool is_label, int line) {
    if (st->count >= st->capacity) {
        st->capacity *= 2;
        st->symbols = realloc(st->symbols, st->capacity * sizeof(Symbol));
    }
    strncpy(st->symbols[st->count].name, name, 255);
    st->symbols[st->count].name[255] = '\0';
    st->symbols[st->count].address = address;
    st->symbols[st->count].is_label = is_label;
    st->symbols[st->count].line = line;
    st->count++;
}

bool symbol_table_lookup(SymbolTable *st, const char *name, uint64_t *address) {
    for (size_t i = 0; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            *address = st->symbols[i].address;
            return true;
        }
    }
    return false;
}

void symbol_table_free(SymbolTable *st) {
    if (st->symbols) {
        free(st->symbols);
        st->symbols = NULL;
    }
    st->count = 0;
    st->capacity = 0;
}

/* ============================================================================
   IMMEDIATE PARSING
   ============================================================================ */

bool parse_immediate(const char *str, int64_t *value) {
    char *endptr;
    
    if (!str || str[0] == '\0') return false;
    
    /* Character literal */
    if (str[0] == '\'' && str[2] == '\'') {
        *value = (int64_t)str[1];
        return true;
    }
    
    /* Binary: 0b... */
    if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        *value = strtoll(str + 2, &endptr, 2);
        return *endptr == '\0';
    }
    
    /* Octal: 0o... */
    if (str[0] == '0' && (str[1] == 'o' || str[1] == 'O')) {
        *value = strtoll(str + 2, &endptr, 8);
        return *endptr == '\0';
    }
    
    /* Hex: 0x... */
    if ((str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) ||
        (str[0] == '-' && str[1] == '0' && (str[2] == 'x' || str[2] == 'X'))) {
        *value = strtoll(str, &endptr, 16);
        return *endptr == '\0';
    }
    
    /* Decimal */
    *value = strtoll(str, &endptr, 10);
    return *endptr == '\0';
}

/* ============================================================================
   REGISTER PARSING
   ============================================================================ */

bool is_register(const char *str, uint8_t *reg) {
    if (str[0] != 'R' && str[0] != 'r') return false;
    
    /* Aliases */
    if (strcmp(str, "ZERO") == 0 || strcmp(str, "zero") == 0) { *reg = 0; return true; }
    if (strcmp(str, "SP") == 0 || strcmp(str, "sp") == 0) { *reg = 29; return true; }
    if (strcmp(str, "LR") == 0 || strcmp(str, "lr") == 0) { *reg = 30; return true; }
    if (strcmp(str, "FP") == 0 || strcmp(str, "fp") == 0) { *reg = 28; return true; }
    
    /* Numeric */
    int r = atoi(str + 1);
    if (r >= 0 && r <= 31) {
        *reg = (uint8_t)r;
        return true;
    }
    return false;
}

bool is_freg(const char *str, uint8_t *reg) {
    if (str[0] != 'F' && str[0] != 'f') return false;
    int r = atoi(str + 1);
    if (r >= 0 && r <= 15) {
        *reg = (uint8_t)r;
        return true;
    }
    return false;
}

bool is_creg(const char *str, uint8_t *reg) {
    if (str[0] != 'C' && str[0] != 'c') return false;
    int r = atoi(str + 1);
    if (r >= 0 && r <= 63) {
        *reg = (uint8_t)r;
        return true;
    }
    return false;
}

/* ============================================================================
   LEXER / TOKENIZER
   ============================================================================ */

static bool is_valid_label_start(char c) {
    return isalpha(c) || c == '_';
}

static bool is_valid_label_char(char c) {
    return isalnum(c) || c == '_';
}

Token tokenize_next(FILE *input, int *line, int *column) {
    Token tok = {0};
    int c;
    
    /* Skip whitespace (except newlines) */
    while ((c = fgetc(input)) != EOF && c != '\n' && isspace(c)) {
        (*column)++;
    }
    
    if (c == EOF) {
        tok.type = TOK_EOF;
        return tok;
    }
    
    if (c == '\n') {
        tok.type = TOK_NEWLINE;
        tok.line = *line;
        tok.column = *column;
        (*line)++;
        *column = 0;
        return tok;
    }
    
    if (c == ';' || c == '#') {
        tok.type = TOK_COMMENT;
        tok.line = *line;
        tok.column = *column;
        while ((c = fgetc(input)) != EOF && c != '\n');
        if (c == '\n') ungetc(c, input);
        return tok;
    }
    
    tok.line = *line;
    tok.column = *column;
    
    /* Single character tokens */
    if (c == ',') { tok.type = TOK_COMMA; tok.value[0] = ','; return tok; }
    if (c == '[') { tok.type = TOK_LBRACKET; tok.value[0] = '['; return tok; }
    if (c == ']') { tok.type = TOK_RBRACKET; tok.value[0] = ']'; return tok; }
    if (c == '+') { tok.type = TOK_PLUS; tok.value[0] = '+'; return tok; }
    if (c == ':') { tok.type = TOK_COLON; tok.value[0] = ':'; return tok; }
    
    /* String tokens */
    int idx = 0;
    
    /* Immediate or label/mnemonic */
    if (c == '-' || isdigit(c)) {
        tok.value[idx++] = c;
        while ((c = fgetc(input)) != EOF && (isdigit(c) || c == 'x' || c == 'X' || 
               c == 'b' || c == 'B' || c == 'o' || c == 'O' || isalpha(c))) {
            if (idx < 255) tok.value[idx++] = c;
        }
        ungetc(c, input);
        tok.value[idx] = '\0';
        (*column) += idx;
        
        int64_t imm_val;
        if (parse_immediate(tok.value, &imm_val)) {
            tok.numval = (uint64_t)imm_val;
            tok.type = TOK_IMMEDIATE;
        }
        return tok;
    }
    
    /* Register or mnemonic */
    if (isalpha(c) || c == '_') {
        tok.value[idx++] = c;
        while ((c = fgetc(input)) != EOF && is_valid_label_char(c)) {
            if (idx < 255) tok.value[idx++] = c;
        }
        ungetc(c, input);
        tok.value[idx] = '\0';
        (*column) += idx;
        
        /* Check type */
        uint8_t dummy;
        if (is_register(tok.value, &dummy)) {
            tok.type = TOK_REGISTER;
        } else if (is_freg(tok.value, &dummy)) {
            tok.type = TOK_FREG;
        } else if (is_creg(tok.value, &dummy)) {
            tok.type = TOK_CREG;
        } else {
            /* Could be label or mnemonic */
            tok.type = TOK_LABEL;  /* Parser decides */
        }
        return tok;
    }
    
    tok.type = TOK_EOF;
    return tok;
}

/* ============================================================================
   INSTRUCTION LOOKUP
   ============================================================================ */

static const InstructionDef* lookup_instruction(const char *mnemonic) {
    for (size_t i = 0; i < instruction_count; i++) {
        if (strcasecmp(instruction_table[i].mnemonic, mnemonic) == 0) {
            return &instruction_table[i];
        }
    }
    return NULL;
}

/* ============================================================================
   ERROR HANDLING
   ============================================================================ */

void error_message(Assembler *asm_ctx, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(asm_ctx->error_msg, sizeof(asm_ctx->error_msg), fmt, args);
    va_end(args);
    asm_ctx->has_error = true;
}

/* ============================================================================
   INSTRUCTION ENCODING
   ============================================================================ */

uint64_t encode_instruction(const ParsedInstruction *instr, uint64_t pc, SymbolTable *symbols) {
    uint64_t encoded = 0;
    uint8_t opcode = instr->def->opcode;
    
    /* Opcode is always bits 63-57 */
    encoded |= ((uint64_t)opcode & 0x7F) << 57;
    
    switch (instr->def->format) {
        case FORMAT_R: {
            /* opcode(7) | Rd(5) | Rs1(5) | Rs2(5) | Func(5) | reserved(37) */
            uint8_t rd = instr->operands[0].value.reg;
            uint8_t rs1 = instr->operands[1].value.reg;
            uint8_t rs2 = instr->operands[2].value.reg;
            encoded |= ((uint64_t)rd & 0x1F) << 52;
            encoded |= ((uint64_t)rs1 & 0x1F) << 47;
            encoded |= ((uint64_t)rs2 & 0x1F) << 42;
            break;
        }
        
        case FORMAT_RR: {
            /* opcode(7) | Rd(5) | Rs1(5) | Func(5) | reserved(42) */
            uint8_t rd = instr->operands[0].value.reg;
            uint8_t rs1 = instr->operands[1].value.reg;
            encoded |= ((uint64_t)rd & 0x1F) << 52;
            encoded |= ((uint64_t)rs1 & 0x1F) << 47;
            break;
        }
        
        case FORMAT_RI: {
            /* opcode(7) | Rd(5) | Rs1(5) | imm(47) */
            uint8_t rd = instr->operands[0].value.reg;
            uint8_t rs1 = instr->operands[1].value.reg;
            int64_t imm = instr->operands[2].value.immediate;
            encoded |= ((uint64_t)rd & 0x1F) << 52;
            encoded |= ((uint64_t)rs1 & 0x1F) << 47;
            encoded |= ((uint64_t)imm & 0x7FFFFFFFFFFFLL);
            break;
        }
        
        case FORMAT_I: {
            /* opcode(7) | Rd(5) | imm(52) */
            uint8_t rd = instr->operands[0].value.reg;
            int64_t imm = instr->operands[1].value.immediate;
            encoded |= ((uint64_t)rd & 0x1F) << 52;
            encoded |= ((uint64_t)imm & 0xFFFFFFFFFFFFFLL);
            break;
        }
        
        case FORMAT_M: {
            /* opcode(7) | Rd(5) | Base(5) | offset(47) */
            uint8_t rd = instr->operands[0].value.reg;
            uint8_t base = instr->operands[1].value.addr.base;
            int64_t offset = instr->operands[1].value.addr.offset;
            encoded |= ((uint64_t)rd & 0x1F) << 52;
            encoded |= ((uint64_t)base & 0x1F) << 47;
            encoded |= ((uint64_t)offset & 0x7FFFFFFFFFFFLL);
            break;
        }
        
        case FORMAT_B: {
            /* opcode(7) | Rs1(5) | Rs2(5) | offset(47) */
            uint8_t rs1 = instr->operands[0].value.reg;
            uint8_t rs2 = instr->operands[1].value.reg;
            int64_t offset = 0;
            
            if (instr->operands[2].type == OPERAND_LABEL) {
                uint64_t label_addr;
                if (symbol_table_lookup(symbols, instr->operands[2].value.label, &label_addr)) {
                    offset = (int64_t)(label_addr - (pc + 8)) / 8;  /* PC-relative */
                }
            }
            
            encoded |= ((uint64_t)rs1 & 0x1F) << 52;
            encoded |= ((uint64_t)rs2 & 0x1F) << 47;
            encoded |= ((uint64_t)offset & 0x7FFFFFFFFFFFLL);
            break;
        }
        
        case FORMAT_J: {
            /* opcode(7) | offset(57) */
            int64_t offset = 0;
            
            if (instr->operands[instr->operand_count - 1].type == OPERAND_LABEL) {
                uint64_t label_addr;
                if (symbol_table_lookup(symbols, instr->operands[instr->operand_count - 1].value.label, &label_addr)) {
                    offset = (int64_t)(label_addr - (pc + 8)) / 8;  /* PC-relative */
                }
            }
            
            encoded |= ((uint64_t)offset & 0x1FFFFFFFFFFFFFFLL);
            break;
        }
        
        case FORMAT_S: {
            /* opcode(7) | func(8) | imm(49) */
            /* For system instructions, just encode as-is for now */
            break;
        }
        
        case FORMAT_F: {
            /* opcode(7) | Fd(5) | Fs1(5) | Fs2(5) | Func(5) | reserved(28) */
            uint8_t fd = instr->operands[0].value.reg;
            uint8_t fs1 = instr->operands[1].value.reg;
            uint8_t fs2 = instr->operands[2].value.reg;
            encoded |= ((uint64_t)fd & 0x1F) << 52;
            encoded |= ((uint64_t)fs1 & 0x1F) << 47;
            encoded |= ((uint64_t)fs2 & 0x1F) << 42;
            break;
        }
    }
    
    return encoded;
}

/* ============================================================================
   ASSEMBLER IMPLEMENTATION
   ============================================================================ */

Assembler* assembler_create(FILE *input, FILE *output) {
    Assembler *asm_ctx = malloc(sizeof(Assembler));
    asm_ctx->input = input;
    asm_ctx->output = output;
    asm_ctx->current_address = 0;
    asm_ctx->current_line = 1;
    asm_ctx->has_error = false;
    asm_ctx->error_msg[0] = '\0';
    asm_ctx->instruction_capacity = 1024;
    asm_ctx->instruction_count = 0;
    asm_ctx->instructions = malloc(asm_ctx->instruction_capacity * sizeof(ParsedInstruction));
    symbol_table_init(&asm_ctx->symbols);
    return asm_ctx;
}

void assembler_free(Assembler *asm_ctx) {
    if (asm_ctx->instructions) {
        free(asm_ctx->instructions);
    }
    symbol_table_free(&asm_ctx->symbols);
    free(asm_ctx);
}