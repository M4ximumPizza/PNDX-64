#define _GNU_SOURCE
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

/* ============================================================================
   INSTRUCTION PARSING
   ============================================================================ */

/* Forward declarations */
static bool parse_operands(Assembler *asm_ctx, const char **operand_strs, 
                          size_t count, Operand *operands, 
                          const OperandType *expected_types, size_t expected_count);

static bool parse_address_operand(const char *str, Operand *op) {
    /* Format: [Rs1 + imm] or [Rs1] or [imm] */
    if (str[0] != '[') return false;
    
    char *copy = strdup(str);
    char *end = strchr(copy, ']');
    if (!end) {
        free(copy);
        return false;
    }
    *end = '\0';
    
    char *content = copy + 1;  /* Skip '[' */
    
    /* Trim spaces */
    while (*content && isspace(*content)) content++;
    end--;
    while (end > content && isspace(*end)) *(end--) = '\0';
    
    /* Look for '+' */
    char *plus = strchr(content, '+');
    if (plus) {
        /* [Base + offset] */
        *plus = '\0';
        char *base_str = content;
        char *offset_str = plus + 1;
        
        /* Trim */
        while (*base_str && isspace(*base_str)) base_str++;
        while (*offset_str && isspace(*offset_str)) offset_str++;
        
        uint8_t base_reg;
        if (!is_register(base_str, &base_reg)) {
            free(copy);
            return false;
        }
        
        int64_t offset;
        if (!parse_immediate(offset_str, &offset)) {
            free(copy);
            return false;
        }
        
        op->type = OPERAND_ADDR;
        op->value.addr.base = base_reg;
        op->value.addr.offset = offset;
    } else {
        /* [Base] only */
        uint8_t base_reg;
        if (is_register(content, &base_reg)) {
            op->type = OPERAND_ADDR;
            op->value.addr.base = base_reg;
            op->value.addr.offset = 0;
        } else {
            free(copy);
            return false;
        }
    }
    
    free(copy);
    return true;
}

static bool parse_operands(Assembler *asm_ctx, const char **operand_strs, 
                          size_t count, Operand *operands, 
                          const OperandType *expected_types, size_t expected_count) {
    if (count != expected_count) {
        error_message(asm_ctx, "Expected %zu operands, got %zu", expected_count, count);
        return false;
    }
    
    for (size_t i = 0; i < count; i++) {
        const char *str = operand_strs[i];
        OperandType expected = expected_types[i];
        
        /* Trim spaces */
        while (*str && isspace(*str)) str++;
        size_t len = strlen(str);
        while (len > 0 && isspace(str[len - 1])) len--;
        
        char trimmed[256];
        strncpy(trimmed, str, len);
        trimmed[len] = '\0';
        
        switch (expected) {
            case OPERAND_REG: {
                uint8_t reg;
                if (!is_register(trimmed, &reg)) {
                    error_message(asm_ctx, "Expected register, got '%s'", trimmed);
                    return false;
                }
                operands[i].type = OPERAND_REG;
                operands[i].value.reg = reg;
                break;
            }
            
            case OPERAND_FREG: {
                uint8_t reg;
                if (!is_freg(trimmed, &reg)) {
                    error_message(asm_ctx, "Expected float register, got '%s'", trimmed);
                    return false;
                }
                operands[i].type = OPERAND_FREG;
                operands[i].value.reg = reg;
                break;
            }
            
            case OPERAND_CREG: {
                uint8_t reg;
                if (!is_creg(trimmed, &reg)) {
                    error_message(asm_ctx, "Expected control register, got '%s'", trimmed);
                    return false;
                }
                operands[i].type = OPERAND_CREG;
                operands[i].value.reg = reg;
                break;
            }
            
            case OPERAND_IMM: {
                int64_t imm;
                if (!parse_immediate(trimmed, &imm)) {
                    error_message(asm_ctx, "Expected immediate, got '%s'", trimmed);
                    return false;
                }
                operands[i].type = OPERAND_IMM;
                operands[i].value.immediate = imm;
                break;
            }
            
            case OPERAND_LABEL: {
                operands[i].type = OPERAND_LABEL;
                strncpy(operands[i].value.label, trimmed, 255);
                operands[i].value.label[255] = '\0';
                break;
            }
            
            case OPERAND_ADDR: {
                if (!parse_address_operand(trimmed, &operands[i])) {
                    error_message(asm_ctx, "Expected address [base + offset], got '%s'", trimmed);
                    return false;
                }
                break;
            }
            
            default:
                error_message(asm_ctx, "Unknown operand type");
                return false;
        }
    }
    
    return true;
}

/* Split operand string by commas, respecting brackets */
static size_t split_operands(const char *operands_str, char **operands, size_t max_count) {
    size_t count = 0;
    char *copy = strdup(operands_str);
    char *p = copy;
    int bracket_depth = 0;
    char *current = p;
    
    while (*p) {
        if (*p == '[') {
            bracket_depth++;
        } else if (*p == ']') {
            bracket_depth--;
        } else if (*p == ',' && bracket_depth == 0) {
            *p = '\0';
            if (count < max_count) {
                operands[count++] = strdup(current);
            }
            current = p + 1;
        }
        p++;
    }
    
    /* Last operand */
    if (current != p && count < max_count) {
        operands[count++] = strdup(current);
    }
    
    free(copy);
    return count;
}

/* ============================================================================
   TWO-PASS ASSEMBLER
   ============================================================================ */

bool assembler_pass1(Assembler *asm_ctx) {
    /* Rewind to beginning */
    rewind(asm_ctx->input);
    asm_ctx->current_line = 1;
    asm_ctx->current_address = 0;
    asm_ctx->instruction_count = 0;
    
    char line_buf[1024];
    
    while (fgets(line_buf, sizeof(line_buf), asm_ctx->input)) {
        /* Remove newline */
        size_t len = strlen(line_buf);
        if (len > 0 && line_buf[len - 1] == '\n') {
            line_buf[len - 1] = '\0';
            len--;
        }
        
        char *line = line_buf;
        
        /* Skip leading whitespace */
        while (*line && isspace(*line)) line++;
        
        /* Skip empty lines and comments */
        if (*line == '\0' || *line == ';' || *line == '#') {
            asm_ctx->current_line++;
            continue;
        }
        
        /* Strip inline comments */
        char *comment = strchr(line, ';');
        if (!comment) comment = strchr(line, '#');
        if (comment) {
            *comment = '\0';  /* Terminate line at comment */
            /* Trim trailing whitespace before comment */
            while (comment > line && isspace(*(comment - 1))) {
                *(--comment) = '\0';
            }
        }
        

        
        /* Check for label (ends with :) */
        char *colon = strchr(line, ':');
        if (colon && colon > line) {
            /* Extract label name */
            size_t label_len = colon - line;
            char label[256];
            strncpy(label, line, label_len);
            label[label_len] = '\0';
            
            /* Trim label */
            size_t i = 0;
            while (i < label_len && isspace(label[i])) i++;
            if (i > 0) {
                memmove(label, label + i, label_len - i + 1);
            }
            
            /* Add to symbol table */
            symbol_table_add(&asm_ctx->symbols, label, asm_ctx->current_address, true, asm_ctx->current_line);
            
            /* Continue with rest of line (if any) */
            line = colon + 1;
            while (*line && isspace(*line)) line++;
        }
        
        /* Skip empty lines */
        if (*line == '\0' || *line == ';' || *line == '#') {
            asm_ctx->current_line++;
            continue;
        }
        
        /* Find mnemonic */
        char *space = strchr(line, ' ');
        if (!space) space = strchr(line, '\t');
        
        if (!space && *line != '\0') {
            /* Just mnemonic, no operands */
            space = line + strlen(line);
        }
        
        size_t mnemonic_len = space - line;
        char mnemonic[256];
        strncpy(mnemonic, line, mnemonic_len);
        mnemonic[mnemonic_len] = '\0';
        
        /* Look up instruction */
        const InstructionDef *def = NULL;
        for (size_t i = 0; i < instruction_count; i++) {
            if (strcasecmp(instruction_table[i].mnemonic, mnemonic) == 0) {
                def = &instruction_table[i];
                break;
            }
        }
        
        if (!def) {
            error_message(asm_ctx, "Line %d: Unknown instruction '%s'", asm_ctx->current_line, mnemonic);
            return false;
        }
        
        /* Parse operands */
        const char *operand_section = space;
        while (*operand_section && isspace(*operand_section)) operand_section++;
        
        char *operand_strs[4] = {0};
        size_t operand_count = split_operands(operand_section, operand_strs, 4);
        
        /* Create parsed instruction */
        if (asm_ctx->instruction_count >= asm_ctx->instruction_capacity) {
            asm_ctx->instruction_capacity *= 2;
            asm_ctx->instructions = realloc(asm_ctx->instructions, 
                                           asm_ctx->instruction_capacity * sizeof(ParsedInstruction));
        }
        
        ParsedInstruction *pi = &asm_ctx->instructions[asm_ctx->instruction_count];
        pi->def = def;
        pi->operand_count = operand_count;
        pi->line = asm_ctx->current_line;
        
        if (!parse_operands(asm_ctx, (const char **)operand_strs, operand_count, 
                           pi->operands, def->operands, def->operand_count)) {
            return false;
        }
        
        /* Free operand strings */
        for (size_t i = 0; i < operand_count; i++) {
            if (operand_strs[i]) free(operand_strs[i]);
        }
        
        asm_ctx->instruction_count++;
        asm_ctx->current_address += 8;  /* Each instruction is 8 bytes */
        asm_ctx->current_line++;
    }
    
    return true;
}

bool assembler_pass2(Assembler *asm_ctx) {
    asm_ctx->current_address = 0;
    
    /* Encode all instructions */
    for (size_t i = 0; i < asm_ctx->instruction_count; i++) {
        uint64_t encoded = encode_instruction(&asm_ctx->instructions[i], 
                                             asm_ctx->current_address, 
                                             &asm_ctx->symbols);
        
        /* Write to output */
        if (fwrite(&encoded, sizeof(uint64_t), 1, asm_ctx->output) != 1) {
            error_message(asm_ctx, "Error writing output");
            return false;
        }
        
        asm_ctx->current_address += 8;
    }
    
    return true;
}

bool assembler_run(Assembler *asm_ctx) {
    /* Pass 1: Build symbol table and parse instructions */
    printf("PNDX-64 Assembler - Pass 1: Symbol Resolution\n");
    if (!assembler_pass1(asm_ctx)) {
        fprintf(stderr, "Error: %s\n", asm_ctx->error_msg);
        return false;
    }
    printf("  Parsed %zu instructions\n", asm_ctx->instruction_count);
    printf("  Found %zu symbols\n", asm_ctx->symbols.count);
    
    /* Pass 2: Encode instructions */
    printf("PNDX-64 Assembler - Pass 2: Instruction Encoding\n");
    if (!assembler_pass2(asm_ctx)) {
        fprintf(stderr, "Error: %s\n", asm_ctx->error_msg);
        return false;
    }
    printf("  Generated %zu bytes\n", asm_ctx->instruction_count * 8);
    
    return true;
}

/* ============================================================================
   DISASSEMBLER
   ============================================================================ */

const InstructionDef* find_instruction_by_opcode(uint8_t opcode) {
    extern const InstructionDef instruction_table[];
    extern const size_t instruction_count;
    
    for (size_t i = 0; i < instruction_count; i++) {
        if (instruction_table[i].opcode == opcode) {
            return &instruction_table[i];
        }
    }
    return NULL;
}

void disassemble_instruction(uint64_t encoded, uint64_t address) {
    uint8_t opcode = (encoded >> 57) & 0x7F;
    const InstructionDef *def = find_instruction_by_opcode(opcode);
    
    if (!def) {
        printf("0x%016lx: .long 0x%016lx  (unknown opcode 0x%02x)\n", address, encoded, opcode);
        return;
    }
    
    printf("0x%016lx: %-8s", address, def->mnemonic);
    
    /* Decode operands based on format */
    switch (def->format) {
        case FORMAT_R: {
            uint8_t rd = (encoded >> 52) & 0x1F;
            uint8_t rs1 = (encoded >> 47) & 0x1F;
            uint8_t rs2 = (encoded >> 42) & 0x1F;
            printf("R%d, R%d, R%d", rd, rs1, rs2);
            break;
        }
        
        case FORMAT_RR: {
            uint8_t rd = (encoded >> 52) & 0x1F;
            uint8_t rs1 = (encoded >> 47) & 0x1F;
            printf("R%d, R%d", rd, rs1);
            break;
        }
        
        case FORMAT_RI: {
            uint8_t rd = (encoded >> 52) & 0x1F;
            uint8_t rs1 = (encoded >> 47) & 0x1F;
            int64_t imm = (int64_t)(encoded & 0x7FFFFFFFFFFFLL);
            if (imm & 0x400000000000LL) imm |= 0xFFFF800000000000LL;  /* Sign extend */
            printf("R%d, R%d, %ld", rd, rs1, imm);
            break;
        }
        
        case FORMAT_I: {
            uint8_t rd = (encoded >> 52) & 0x1F;
            int64_t imm = (int64_t)(encoded & 0xFFFFFFFFFFFFFLL);
            if (imm & 0x80000000000LL) {
                imm |= (int64_t)0xFFFFF00000000000LL;  /* Sign extend */
            }
            printf("R%d, %ld", rd, imm);
            break;
        }
        
        case FORMAT_M: {
            uint8_t rd = (encoded >> 52) & 0x1F;
            uint8_t base = (encoded >> 47) & 0x1F;
            int64_t offset = (int64_t)(encoded & 0x7FFFFFFFFFFFLL);
            if (offset & 0x400000000000LL) offset |= 0xFFFF800000000000LL;  /* Sign extend */
            printf("R%d, [R%d+%ld]", rd, base, offset);
            break;
        }
        
        case FORMAT_B: {
            uint8_t rs1 = (encoded >> 52) & 0x1F;
            uint8_t rs2 = (encoded >> 47) & 0x1F;
            int64_t offset = (int64_t)(encoded & 0x7FFFFFFFFFFFLL);
            if (offset & 0x400000000000LL) offset |= 0xFFFF800000000000LL;  /* Sign extend */
            uint64_t target = address + (offset * 8) + 8;
            printf("R%d, R%d, 0x%lx", rs1, rs2, target);
            break;
        }
        
        case FORMAT_J: {
            if (strcasecmp(def->mnemonic, "JAL") == 0) {
                /*
                 * JAL:
                 *   opcode (7) | Rd (5) | offset (52)
                 */
                uint8_t rd = (encoded >> 52) & 0x1F;

                int64_t offset = encoded & 0x000FFFFFFFFFFFFFULL;

                /* Sign-extend 52-bit offset */
                if (offset & (1LL << 51))
                    offset |= 0xFFF0000000000000LL;

                uint64_t target = address + (offset * 8) + 8;

                printf("R%d, 0x%016lx", rd, target);
            } else {
                /*
                 * JMP:
                 *   opcode (7) | offset (57)
                 */
                int64_t offset = encoded & 0x01FFFFFFFFFFFFFFULL;

                /* Sign-extend 57-bit offset */
                if (offset & (1LL << 56))
                    offset |= 0xFE00000000000000LL;

                uint64_t target = address + (offset * 8) + 8;

                printf("0x%016lx", target);
            }

            break;
        }

        case FORMAT_S: {
            /*
             * S-format:
             *   opcode (7) | func (8) | immediate (49)
             */
            int64_t imm = encoded & 0x1FFFFFFFFFFFFULL;

            /* Sign-extend 49-bit immediate */
            if (imm & (1LL << 48))
                imm |= 0xFFFFE00000000000LL;

            if (def->operand_count > 0)
                printf("%ld", imm);

            break;
        }

        case FORMAT_F:
            printf("(unimplemented F-format)");
            break;

        default:
            printf("(unimplemented format)");
            break;
    }
    
    printf("\n");
}

void disassemble_binary(FILE *input) {
    uint64_t address = 0;
    uint64_t instruction;
    
    printf("PNDX-64 Disassembly\n");
    printf("===================\n\n");
    
    while (fread(&instruction, sizeof(uint64_t), 1, input) == 1) {
        disassemble_instruction(instruction, address);
        address += 8;
    }
}

/* ============================================================================
   MAIN
   ============================================================================ */

void print_usage(const char *prog) {
    printf("PNDX-64 Assembler v1.0\n");
    printf("Usage:\n");
    printf("  %s -a <input.asm> <output.bin>   Assemble to binary\n", prog);
    printf("  %s -d <input.bin>                Disassemble binary\n", prog);
    printf("  %s -h                            Show this help\n", prog);
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (strcmp(argv[1], "-d") == 0) {
        /* Disassemble mode */
        if (argc < 3) {
            fprintf(stderr, "Error: -d requires input file\n");
            return 1;
        }
        
        FILE *input = fopen(argv[2], "rb");
        if (!input) {
            fprintf(stderr, "Error: Cannot open '%s'\n", argv[2]);
            return 1;
        }
        
        disassemble_binary(input);
        fclose(input);
        return 0;
    }
    
    if (strcmp(argv[1], "-a") == 0) {
        /* Assemble mode */
        if (argc < 4) {
            fprintf(stderr, "Error: -a requires input and output files\n");
            return 1;
        }
        
        FILE *input = fopen(argv[2], "r");
        if (!input) {
            fprintf(stderr, "Error: Cannot open input '%s'\n", argv[2]);
            return 1;
        }
        
        FILE *output = fopen(argv[3], "wb");
        if (!output) {
            fprintf(stderr, "Error: Cannot open output '%s'\n", argv[3]);
            fclose(input);
            return 1;
        }
        
        Assembler *asm_ctx = assembler_create(input, output);
        
        printf("PNDX-64 Assembler v1.0\n");
        printf("Input:  %s\n", argv[2]);
        printf("Output: %s\n\n", argv[3]);
        
        if (assembler_run(asm_ctx)) {
            printf("\nAssembly successful!\n");
        } else {
            printf("\nAssembly failed!\n");
            fclose(input);
            fclose(output);
            assembler_free(asm_ctx);
            return 1;
        }
        
        assembler_free(asm_ctx);
        fclose(input);
        fclose(output);
        return 0;
    }
    
    fprintf(stderr, "Error: Unknown option '%s'\n", argv[1]);
    print_usage(argv[0]);
    return 1;
}