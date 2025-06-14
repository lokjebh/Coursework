## Implementing MiniJVM
This program uses mini version of .class files that we will call .mclass files. Our .mclass files will contain only bytecodes, and only bytecodes for one method. However, the format of the bytecodes is the same as that found in real .class files.
MiniJVM is a lightweight stack-based interpreter that simulates a subset of Java Virtual Machine (JVM) behavior. Written in C, it reads and executes `.mclass` files that contain simplified bytecode instructions, mimicking core features of JVM such as stack-based arithmetic, local variable handling, and control flow.

---

### Features
- Stack-based bytecode interpreter
- Supports arithmetic, logic, local variable operations, and branching
- Operates on a fixed-size operand stack and local variable array
- Reads bytecode files (`*.mclass`) and executes instructions linearly
- Test automation with Makefile targets

---

### Requirements
- `gcc` (C Compiler)
- `make`
- `libadt.a` (provided ADT library)
- A Unix-like environment (Linux, macOS, or WSL)

---

### Compilation
To compile, run:
`make`
This will:
- Compile `minijvm.c` into `minijvm.o`
- Link it against the ADT library (`libadt.a`)
- Produce an executable named `mjava`

---

### Usage
Run the MiniJVM by passing the base filename (without extension) of a .mclass file:
`./mjava testfiles/test1`
This command executes `testfiles/test1.mclass`.

### Bytecode Instruction Supported
| Bytecode         | Description                                  |
|------------------|----------------------------------------------|
| `iconst_0`        | Push constant 0                              |
| `bipush <byte>`   | Push signed byte                             |
| `pop`             | Discard top of operand stack                 |
| `dup`             | Duplicate top of stack                       |
| `iadd`            | Add top two integers                         |
| `isub`            | Subtract top from second top                 |
| `imul`            | Multiply top two integers                    |
| `idiv`            | Divide second top by top (error if zero)     |
| `irem`            | Remainder of second top divided by top       |
| `ishr`            | Arithmetic shift right                       |
| `iload <n>`       | Load local variable `n` to stack             |
| `istore <n>`      | Store top of stack to local variable `n`     |
| `iinc <n> <d>`    | Increment local variable `n` by `d`          |
| `ifeq <offset>`   | Branch if top of stack is zero               |
| `goto <offset>`   | Unconditional branch                         |
| `print`           | Print top of stack                           |
| `return`          | End execution                                |

### Testing
To run all test cases, run:
`make test`

To run a specific test (e.g. test3), run:
`make test3`

### cleaning Up
To remove compiled files:
`make clean`
This deletes:
- mjava
- minijvm.o

### Notes
- All .mclass files must be preperly formatted as butecode understood by MiniJVm.
- The ADT stack library (libadt.a) must be present in the `lib/` directory.
- Local variable index range: 0 - 9
- Maximum bytecode file size is limited (`MAX_CLASS_SIZE` defined in `minijvm.h`).

