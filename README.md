# Compiler 2 - Parser and Codegen for simple.jive

## 📋 Assignment Requirements

Implement a simple compiler that translates `.jive` language to x86-64 assembly code.

## 📂 Project File Structure

### ✅ Required Files (Assignment Submission)

```
📝 Source Code Files:
├── main.c          # Main program (command line argument handling)
├── lexer.c         # Lexical analyzer (completed)
├── parser.c        # Syntax parser (completed)
├── codegen.c       # Code generator (completed)
└── string.c        # String utilities

🧪 Test Files:
├── simple.jive     # Basic test (single function) - assignment requirement
└── simple2.jive    # Extended test (multiple functions) - additional testing

🔧 Other Files:
├── build.sh        # Build script
└── Compiler 2 - Parser and Codegen for simple.jive.pdf  # Assignment document
```

### ❌ Files Not Required for Submission (will be cleaned)

```
├── jive            # Compiled executable
├── *.asm           # Generated assembly files
├── *.dSYM/         # Debug symbol directories
└── test_simple.c   # Temporary test files
```

## 🎯 Implemented Features

### 1. Lexical Analyzer (lexer.c) ✅
- Recognizes keywords: `fn`, `return`
- Recognizes types: `int`
- Recognizes symbols: `(`, `)`, `{`, `}`, `->`
- Recognizes integer literals and identifiers

### 2. Syntax Parser (parser.c) ✅
- `ast_list_append()` - Doubly linked list operations
- `parse_block()` - Parse code blocks
- `parse_statement()` - Parse statements (return)
- `parse_expression()` - Parse expressions (integers)
- `parse_fn_def()` - Parse function definitions

### 3. Code Generator (codegen.c) ✅
- `generate_asm_for_fn()` - Generate function assembly code
- `generate_asm_for_stmt()` - Generate statement assembly code
- `generate_asm_for_expr()` - Generate expression assembly code

## 🚀 Usage Instructions

### Compile the Compiler
```bash
gcc -Wall -ggdb main.c -o jive
```

### Run the Compiler
```bash
# Compile simple.jive
./jive simple.jive -o simple.asm

# Compile simple2.jive
./jive simple2.jive -o simple2.asm
```

### Test the Compiler
```bash
# Assemble generated code with nasm
nasm -felf64 simple.asm
ld simple.o -o simple

# Run the program
./simple

# Check return value
echo $?  # Should output 42
```

## 📝 Supported Syntax

```jive
fn function_name() -> int
{
    return 42
}
```

## 🧪 Test Cases

### simple.jive (Basic Test)
```jive
fn main() -> int
{
    return 42
}
```

### simple2.jive (Multiple Functions Test)
```jive
fn main() -> int { return 42 }
fn foo() -> int { return 64 }
fn bar() -> int { return 123 }
fn baz() -> int { return 17 }
```

## 🧹 Project Cleanup

Run cleanup script (removes all generated files):
```bash
bash cleanup.sh
```

After cleanup, only source code and test files remain.

## ✅ Assignment Completion Status

### 📝 Instructor's Required TODOs (Implemented)

**parser.c:**
- [x] `ast_list_append()` - Implement doubly linked list node insertion
- [x] `parse_block()` - Parse statements within code blocks  
- [x] `parse_fn_def()` - Populate function definition data structure

**codegen.c:**
- [x] `generate_asm_for_fn()` - Generate assembly code for functions

### 🔧 Code Completed by Student

**lexer.c:**
- [x] Complete lexical analyzer implementation (from scratch)

**parser.c:**
- [x] `parse_statement()` - Parse return statements
- [x] `parse_expression()` - Parse integer literals
- [x] `parse_program()` - Fix infinite loop issues

**codegen.c:**
- [x] `generate_asm_for_expr()` - Expression code generation
- [x] `generate_asm_for_stmt()` - Statement code generation

### 🧪 Test Results
- [x] Test passed for simple.jive (assignment requirement)
- [x] Test passed for simple2.jive (additional testing)

### 📋 TODO Comments in Code

The code preserves the instructor's original TODO comments, clearly showing:
- **Instructor's TODOs** - Marked required functionality to implement
- **Student's Implementation** - Added specific implementation below TODO comments

## 🎓 Learning Outcomes

This project demonstrates understanding of:
- **Compiler Design**: Lexical analysis, parsing, and code generation
- **Data Structures**: Abstract Syntax Trees (AST) and doubly linked lists
- **Assembly Language**: x86-64 assembly code generation
- **C Programming**: Memory management, file I/O, and modular design

## 📚 Technical Details

- **Language**: C
- **Target Architecture**: x86-64
- **Assembly Format**: NASM
- **Build System**: GCC with debug symbols
- **Testing**: Multiple test cases with different function structures