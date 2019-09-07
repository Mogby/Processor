#include "program.hpp"

#include "instruction.hpp"
#include "util.hpp"

#include <fstream>
#include <regex>

namespace NCPU {

void TProgram::ReadFromAsmFile(const TString &filename) {
    Wipe();

    TString asm_line;
    std::vector<TString> asm_lines;
    std::ifstream input(filename);
    while (std::getline(input, asm_line)) {
        RemoveComment(asm_line);
        asm_line = StripString(asm_line);
        if (asm_line.empty()) {
            continue;
        }

        if (asm_line.back() == ':') {
            asm_line = asm_line.substr(0, asm_line.size() - 1); // removing trailing ':'
            label_offsets_[asm_line] = asm_lines.size();
        } else {
            asm_lines.push_back(asm_line);
        }
    }

    for (auto& line : asm_lines) {
        ReplaceLabels(line);
        instructions_.push_back(TUniversalDecoder::DecodeAssemblyLine(line));
    }
}

void TProgram::WriteToAsmFile(const TString &filename) const {
    std::ofstream output(filename);
    for (auto instruction : instructions_) {
        output << instruction->ToAssembly() << std::endl;
    }
}

void TProgram::ReadFromBinaryFile(const TString &filename) {
    Wipe();

    std::ifstream input(filename, std::ios::binary);
    std::vector<char> opcodes;
    std::copy(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(),
        std::back_inserter(opcodes)
    );

    if (opcodes.size() % sizeof(TOpcode) != 0) {
        throw TException{"Invalid opcode length"};
    }

    for (int i = 0; i < opcodes.size(); i += sizeof(TOpcode)) {
        TOpcode cur_opcode = 0;
        for (int j = 0; j < sizeof(TOpcode); ++j) {
            cur_opcode = ((cur_opcode << 8) | (TByte)opcodes[i + j]);
        }
        instructions_.push_back(TUniversalDecoder::DecodeOpcode(cur_opcode));
    }
}

void TProgram::WriteToBinaryFile(const NCPU::TString &filename) const {
    std::ofstream output(filename, std::ios::binary);
    for (auto instruction : instructions_) {
        // print opcode byte by byte
        for (int i = sizeof(TOpcode) - 1; i >= 0; --i) {
            output << static_cast<char>((instruction->ToOpcode() >> (8 * i)) & 0xFF);
        }
    }
}

void TProgram::Run() {
    TCPUState state;
    do {
        instructions_[state.pc_++]->Execute(state);
    } while (state.pc_ < instructions_.size());
}

void TProgram::Wipe() {
    label_offsets_.clear();
    instructions_.clear();
}

void TProgram::RemoveComment(TString &assembly_line) const {
    static const std::regex COMMENT_REGEX(";.*");
    assembly_line = std::regex_replace(assembly_line, COMMENT_REGEX, "");
}

void TProgram::ReplaceLabels(TString &assembly_line) const {
    auto words = SplitString(assembly_line);
    assembly_line = words[0];
    for (int i = 1; i < words.size(); ++i) {
        auto& word = words[i];
        if (label_offsets_.count(word)) {
            word = ToString(label_offsets_.at(word));
        }
        assembly_line += " " + word;
    }
}

} // namespace NCPU