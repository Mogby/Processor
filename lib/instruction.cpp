#include "instruction.hpp"

#include "util.hpp"

namespace NCPU {

IInstructionPtr TUniversalDecoder::DecodeOpcode(NCPU::TOpcode opcode) {
    auto nopcode = (opcode >>  16);
    return GetOpcodeRegister().at((opcode >> 16))->DecodeOpcode(opcode);
}

IInstructionPtr TUniversalDecoder::DecodeAssemblyLine(const NCPU::TString &line) {
    auto name = SplitString(line)[0];
    name = ToUpper(name);
    return GetNameRegister().at(name)->DecodeAssemblyLine(line);
}

void TUniversalDecoder::RegisterInstruction(NCPU::TOpcode opcode, const NCPU::TString &name,
                                            NCPU::IInstructionDecoder *decoder) {
    GetOpcodeRegister()[opcode] = decoder;
    GetNameRegister()[name] = decoder;
}

TDecoderRecorder::TDecoderRecorder(NCPU::TOpcode opcode, const NCPU::TString &name,
                                   NCPU::IInstructionDecoder *decoder) {
    TUniversalDecoder::RegisterInstruction(opcode, name, decoder);
}

TOpcode ReadInstructionCode(const NCPU::TString& opcode_pattern) {
    return (HexToInt(opcode_pattern[0]) << 12) +
           (HexToInt(opcode_pattern[1]) << 8) +
           (HexToInt(opcode_pattern[2]) << 4) +
            HexToInt(opcode_pattern[3]);
}

IInstruction::TArgs ParsePatternArgs(const TString& pattern) {
    IInstruction::TArgs  parsed_args;
    for (size_t i = 0; i < 2 * IInstruction::INSTRUCTION_SIZE; ++i) {
        if (islower(pattern[i])) {
            int arg_id;
            if (pattern[i] == 'x') {
                arg_id = 0;
            } else {
                arg_id = 1;
            }

            auto& cur_arg = parsed_args[arg_id];
            ++cur_arg.length_;
            cur_arg.offset_ = 4 * (2 * IInstruction::INSTRUCTION_SIZE - i - 1);
            cur_arg.mask_ |= 0xF << cur_arg.offset_;
        }
    }

    return parsed_args;
}

} // namespace NCPU