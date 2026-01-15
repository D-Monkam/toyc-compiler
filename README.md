# ToyC Compiler

A statically-typed, functional programming language compiler written in C++ using the LLVM compiler infrastructure.

ToyC compiles custom source code directly into **native machine code** (object files), enabling interoperability with C and C++. It features a custom recursive descent parser, an AST-driven backend, and an object file emitter.

## Features
* **LLVM Backend:** Generates optimized LLVM IR (Intermediate Representation).
* **Native Compilation:** Emits standard `.o` object files compatible with system linkers.
* **C++ Interoperability:** Functions written in ToyC can be called directly from C++ applications.
* **Recursive Descent Parser:** Custom lexer and parser handling operator precedence climbing.

## Getting Started

### Prerequisites
* LLVM (10.0 or later)
* CMake (3.10 or later)
* C++ Compiler (Clang or GCC)

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make