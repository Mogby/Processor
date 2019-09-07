#include <iostream>

#include "lib/program.hpp"
#include "lib/instruction_set.hpp"

void PrintEmulatorUsage(char* cmd) {
    puts("usage:");
    printf("  %s e <input_file>\n", cmd);
    puts("options:");
    puts("  input_file: file with compiled program to run");
}

void PrintCompilerUsage(char* cmd) {
    puts("usage:");
    printf("  %s c <input_file> <output_file>\n", cmd);
    puts("options:");
    puts("  input_file: file with assembly code to compile");
    puts("  output_file: output binary file");
}

void PrintDecompilerUsage(char* cmd) {
    puts("usage:");
    printf("  %s d <input_file> <output_file>\n", cmd);
    puts("options:");
    puts("  input_file: binary file to decompile");
    puts("  output_file: output file with assembly code");
}

void PrintUsage(char* cmd) {
    puts("usage:");
    printf("  %s <mode> <args...>", cmd);
    puts("options:");
    puts("  mode: e (emulator), c (compiler) or d (decompiler)");
}

int emulator_main(int argc, char** argv) {
    if (argc < 3) {
        PrintEmulatorUsage(argv[0]);
        return -1;
    }

    NCPU::TProgram program;
    program.ReadFromBinaryFile(argv[2]);
    program.Run();

    return 0;
}

int compiler_main(int argc, char** argv) {
    if (argc < 4) {
        PrintCompilerUsage(argv[0]);
        return -1;
    }

    NCPU::TProgram program;
    program.ReadFromAsmFile(argv[2]);
    program.WriteToBinaryFile(argv[3]);

    return 0;
}

int decompiler_main(int argc, char **argv) {
    if (argc < 4) {
        PrintDecompilerUsage(argv[0]);
        return -1;
    }

    NCPU::TProgram program;
    program.ReadFromBinaryFile(argv[2]);
    program.WriteToAsmFile(argv[3]);

    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        PrintUsage(argv[0]);
        return -1;
    }

    switch(argv[1][0]) {
        case 'e':
            return emulator_main(argc, argv);
        case 'c':
            return compiler_main(argc, argv);
        case 'd':
            return decompiler_main(argc, argv);
        default:
            PrintUsage(argv[0]);
            return -1;
    }
}