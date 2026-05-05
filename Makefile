# ──────────────────────────────────────────────────────────────────
#  Makefile for the DSL Compiler
# ──────────────────────────────────────────────────────────────────
CC      = gcc
CFLAGS  = -Wall -Wextra -g -I src
FLEX    = flex
BISON   = bison
SRC     = src
BUILD   = build
TARGET  = $(BUILD)/dslc

# Source files compiled directly with gcc
C_SRCS  = $(SRC)/ast.c $(SRC)/semantic.c $(SRC)/codegen.c $(SRC)/main.c

# Generated files
LEX_C   = $(BUILD)/lex.yy.c
PAR_C   = $(BUILD)/parser.tab.c
PAR_H   = $(BUILD)/parser.tab.h

.PHONY: all clean test test1 test2 test3

all: $(BUILD) $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

# ── 1. Run Bison to generate parser.tab.c + parser.tab.h ─────────
$(PAR_C) $(PAR_H): $(SRC)/parser.y | $(BUILD)
	$(BISON) -d -o $(PAR_C) $(SRC)/parser.y

# ── 2. Run Flex to generate lex.yy.c ──────────────────────────────
$(LEX_C): $(SRC)/lexer.l $(PAR_H) | $(BUILD)
	$(FLEX) -o $(LEX_C) $(SRC)/lexer.l

# ── 3. Compile everything together ────────────────────────────────
$(TARGET): $(LEX_C) $(PAR_C) $(C_SRCS)
	$(CC) $(CFLAGS) -I$(BUILD) $^ -o $@

# ── Convenience test targets ──────────────────────────────────────
test1: all
	./$(TARGET) test/test1.dsl output/test1.tac

test2: all
	./$(TARGET) test/test2.dsl output/test2.tac

test3: all
	-./$(TARGET) test/test3_error.dsl   # expected to fail with semantic error

test: test1 test2 test3

clean:
	rm -rf $(BUILD) output/*.tac
