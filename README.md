# DSL Compiler — Compiler Design Project

<div align="center">

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![Lexer](https://img.shields.io/badge/Lexer-Flex-green?style=for-the-badge)
![Parser](https://img.shields.io/badge/Parser-Bison-orange?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Ubuntu-purple?style=for-the-badge&logo=ubuntu)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge)

**A fully functional compiler for a custom Domain-Specific Language (DSL)**  
built using C, Flex (Lex), and Bison (YACC) — covering all five classic compiler phases.

</div>

---

## 👩‍💻 Student Details

| Field            | Details                                                  |
|------------------|----------------------------------------------------------|
| **Name**         | HARSITHA G P                                             |
| **Register No.** | RA2311026050197                                          |
| **Subject**      | Compiler Design                                          |
| **Institution**  | SRM INSTITUTE OF SCIENCE and TECHNOLOGY                  |

---

## 📁 Project Structure

```
Compiler-Design-DSL_RegNo_Name/
│
├── src/
│   ├── lexer.l          # Flex – Lexical Analyser
│   ├── parser.y         # Bison – Parser & Grammar Rules
│   ├── ast.h            # AST Node type definitions
│   ├── ast.c            # AST construction & utilities
│   ├── semantic.h       # Semantic analysis header
│   ├── semantic.c       # Semantic analysis (symbol table)
│   ├── codegen.h        # Code generation header
│   ├── codegen.c        # Three-address intermediate code generator
│   └── main.c           # Main driver – connects all phases
│
├── test/
│   ├── test1.dsl        # Basic arithmetic assignments
│   ├── test2.dsl        # Complex expression chain
│   └── test3_error.dsl  # Semantic error test case
│
├── output/
│   ├── test1.tac        # TAC output for test1
│   └── test2.tac        # TAC output for test2
│
├── docs/
│   └── DSL_Compiler_Report.pdf
│
├── Makefile
└── README.md
```

---

## 📌 Project Description

This project implements a **minimal but complete compiler** for a custom Domain-Specific Language (DSL). The DSL supports arithmetic expressions, variable assignments, and multi-statement programs.

### ✅ Supported DSL Features

| Feature                   | Example               |
|---------------------------|-----------------------|
| Integer assignment        | `x = 5;`              |
| Addition                  | `z = x + y;`          |
| Subtraction               | `z = x - y;`          |
| Multiplication            | `z = x * y;`          |
| Division                  | `z = x / y;`          |
| Parenthesised expression  | `z = (x + y) * 2;`    |
| Unary minus               | `z = -x;`             |
| Single-line comment       | `# this is a comment` |
| Multiple statements       | Any number of lines   |

---

## ⚙️ Tools Used

| Tool       | Version | Purpose                          |
|------------|---------|----------------------------------|
| **GCC**    | 11.4    | C compiler                       |
| **Flex**   | 2.6.4   | Lexical analyser generator       |
| **Bison**  | 3.8.2   | Parser generator (LALR grammar)  |
| **Make**   | 4.3     | Build automation                 |
| **Ubuntu** | 22.04   | Development & execution platform |

---

## 🏗️ Compiler Architecture

The compiler follows the classic **5-phase pipeline**:

```
Source Code (.dsl)
        │
        ▼
┌─────────────────┐
│  Phase 1 & 2    │  lexer.l + parser.y
│  Lexing/Parsing │  Token stream → AST
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│    Phase 3      │  ast.c
│  AST Building   │  Syntax tree construction
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│    Phase 4      │  semantic.c
│ Semantic Check  │  Symbol table validation
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│    Phase 5      │  codegen.c
│ Code Generation │  Three-Address Code (TAC)
└─────────────────┘
```

---

## 🚀 Steps to Run

### Prerequisites

```bash
sudo apt-get update
sudo apt-get install gcc flex bison make -y
```

### Build

```bash
make all
```

### Run

```bash
# Run test 1 – basic arithmetic
./build/dslc test/test1.dsl

# Run test 2 – complex expressions
./build/dslc test/test2.dsl

# Run test 3 – semantic error detection
./build/dslc test/test3_error.dsl

# Save output to a file
./build/dslc test/test1.dsl output/test1.tac

# Run all tests at once
make test

# Clean build files
make clean
```

---

## 📥 Sample Input & Output

### Input — `test/test1.dsl`

```
x = 5;
y = 10;
z = x + y;
result = z * 2;
```

### Output

```
DSL Compiler  -  source: test/test1.dsl

+------------------------------------------+
|  Phase 1 & 2 : Lexing / Parsing          |
+------------------------------------------+
[Parser] AST constructed successfully

+------------------------------------------+
|  Phase 3 : Abstract Syntax Tree          |
+------------------------------------------+
STMTLIST
  ASSIGN
    IDENT(x)
    NUMBER(5)
  STMTLIST
    ASSIGN
      IDENT(y)
      NUMBER(10)
    ...

+------------------------------------------+
|  Phase 4 : Semantic Analysis             |
+------------------------------------------+
[Semantic] OK - no errors found

+------------------------------------------+
|  Phase 5 : Intermediate Code Generation  |
+------------------------------------------+
; -- Three-Address Code ----------------------
begin:
    x = 5
    y = 10
    t0 = x + y
    z = t0
    t1 = z * 2
    result = t1
end:
; --------------------------------------------

[Done] Compilation finished successfully.
```

### Semantic Error Detection — `test/test3_error.dsl`

**Input:**
```
a = 10;
c = a + b;   # ERROR: b is not defined
```

**Output:**
```
Semantic error: variable 'b' used before assignment
[Semantic] 1 error(s) found
(compilation aborted - code generation skipped)
```

---

## 📂 Module Descriptions

| Module        | Description |
|---------------|-------------|
| `lexer.l`     | Flex rules that tokenise the source into NUMBER, IDENTIFIER, operators, and punctuation. Comments and whitespace are discarded. |
| `parser.y`    | Bison LALR grammar. Operator precedence declared with `%left`. Each grammar action calls an AST constructor. |
| `ast.c`       | Defines 5 node types. Provides `make_*` constructors, `print_ast()` for debug, and `free_ast()` for cleanup. |
| `semantic.c`  | Single-pass AST walk with a hash-map symbol table. Reports use-before-assignment errors and aborts on failure. |
| `codegen.c`   | Post-order recursive walk that emits three-address code with numbered temporaries `t0, t1, t2...` |
| `main.c`      | Wires all phases together. Prints phase banners. Accepts an optional output file argument. |

---

<div align="center">
Made with ❤️ for Compiler Design
</div>
