# My Virtual CPU

Toolkit for my virtual CPU. Includes a compiler, a decompiler and an emulator.

## How to build

Run CMake and then build the program with your favourite build system.
Make sure you have CMake version 3.13 or later and a compatible C++ compiler.

## Usage

```
usage: 
  ./processor <mode> <args...>
options:
  mode: e (emulator), c (compiler), or d (decompiler) 
```

### Emulator

```
usage:
  ./processor e <input_file>
options:
  input_file: file with compiled program to run
```

### Compiler

```
usage:
  ./processor c <input_file> <output_file>
options:
  input_file: file with assembly code to compile
  output_file: output binary file
```

### Decompiler

```
usage:
  ./processor d <input_file> <output_file>
options:
  input_file: binary file to decompile
  output_file: output file with assembly code 
```