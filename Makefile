CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm

TARGET = pndx64

SRC_DIR = src
TEST_DIR = test

SOURCES = $(SRC_DIR)/assembler.c $(SRC_DIR)/main.c
HEADERS = $(SRC_DIR)/common.h
OBJECTS = $(SOURCES:.c=.o)

TEST_ASM = $(TEST_DIR)/test.asm
TEST_BIN = $(TEST_DIR)/test.bin
TEST_DIS = $(TEST_DIR)/test.dis

.PHONY: all clean test help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET) $(TEST_ASM)
	./$(TARGET) -a $(TEST_ASM) $(TEST_BIN)
	./$(TARGET) -d $(TEST_BIN) > $(TEST_DIS)
	@echo "Assembly test complete!"
	@echo "Binary: $(TEST_BIN)"
	@echo "Disassembly: $(TEST_DIS)"
	@head -30 $(TEST_DIS)

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f $(TEST_DIR)/*.bin $(TEST_DIR)/*.dis

help:
	@echo "PNDX-64 Assembler"
	@echo "================="
	@echo "Targets:"
	@echo "  make              - Build the assembler"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make test         - Run test assembly"
	@echo "  make help         - Show this help"