#ifndef NCPU_INSTRUCTION_HPP
#define NCPU_INSTRUCTION_HPP

#include "state.hpp"
#include "types.hpp"

#include <array>
#include <memory>
#include <unordered_map>

namespace NCPU {

class IInstruction {
  public:
    static const TSize MAX_ARGS_NUM = 2;
    static const TSize INSTRUCTION_SIZE = 4;

  public:
    struct TArg {
        uint8_t length_ = 0;
        uint8_t offset_ = 0;
        TOpcode mask_ = 0;
    };

    using TArgs = std::array<TArg, MAX_ARGS_NUM>;
    using TArgsStorage = std::array<TWord, MAX_ARGS_NUM>;

  public:
    virtual void Execute(TCPUState& state) const = 0;

    virtual TOpcode ToOpcode() const = 0;
    virtual TString ToAssembly() const = 0;
};

using IInstructionPtr = std::shared_ptr<IInstruction>;

class IInstructionDecoder {
  public:
    virtual IInstructionPtr DecodeOpcode(TOpcode opcode) const = 0;
    virtual IInstructionPtr DecodeAssemblyLine(const TString& line) const = 0;
};

template<typename TInstruction>
class TSingleInstructionDecoder : public IInstructionDecoder {
  public:
    IInstructionPtr DecodeOpcode(TOpcode opcode) const override {
        auto instruciton = std::make_shared<TInstruction>();
        int cur_arg = 0;
        for (const auto& arg : TInstruction::ARGS) {
            if (arg.length_ == 0) {
                break;
            }

            instruciton->arg_values_[cur_arg++] = ((opcode & arg.mask_) >> arg.offset_);
        }
        return instruciton;
    }

    IInstructionPtr DecodeAssemblyLine(const TString& line) const override {
        auto instruction = std::make_shared<TInstruction>();
        auto words = SplitString(line);
        for (int i = 1; i < words.size(); ++i) {
            instruction->arg_values_[i - 1] = FromString<TWord>(words[i]);
        }
        return instruction;
    }
};

class TUniversalDecoder {
  private:
    using TOpcodeRegister = std::unordered_map<TOpcode, IInstructionDecoder*>;
    using TNameRegister = std::unordered_map<TString, IInstructionDecoder*>;

  public:
    static IInstructionPtr DecodeOpcode(TOpcode opcode);
    static IInstructionPtr DecodeAssemblyLine(const TString& line);

  private:
    static void RegisterInstruction(TOpcode opcode,
                                    const TString& name,
                                    IInstructionDecoder* decoder);

  private:
    static TOpcodeRegister& GetOpcodeRegister() {
        static TOpcodeRegister register_;
        return register_;
    }

    static TNameRegister& GetNameRegister() {
        static TNameRegister register_;
        return register_;
    }

  friend class TDecoderRecorder;
};

class TDecoderRecorder {
  public:
    TDecoderRecorder(TOpcode opcode,
                     const TString& name,
                     IInstructionDecoder* decoder);
};

TOpcode ReadInstructionCode(const TString& opcode_pattern);
IInstruction::TArgs ParsePatternArgs(const TString& pattern);

} // namespace NCPU

#define DECLARE_INSTRUCTION(INSTRUCTION_CLASS, INSTRUCTION_NAME, OPCODE_PATTERN) \
    class INSTRUCTION_CLASS : public NCPU::IInstruction { \
      private: \
        static const NCPU::TOpcode OPCODE; \
        static const NCPU::TString NAME; \
        static const NCPU::IInstruction::TArgs ARGS; \
        static const NCPU::TDecoderRecorder DECODER_RECORDER; \
        \
      public: \
        void Execute(NCPU::TCPUState& state) const override; \
        \
        NCPU::TOpcode ToOpcode() const override; \
        NCPU::TString ToAssembly() const override; \
        \
      private: \
        NCPU::IInstruction::TArgsStorage arg_values_{}; \
        \
      friend class NCPU::TSingleInstructionDecoder<INSTRUCTION_CLASS>; \
    }; \
    \
    const NCPU::TOpcode INSTRUCTION_CLASS::OPCODE = NCPU::ReadInstructionCode(OPCODE_PATTERN); \
    const NCPU::IInstruction::TArgs INSTRUCTION_CLASS::ARGS = NCPU::ParsePatternArgs(OPCODE_PATTERN); \
    const NCPU::TString INSTRUCTION_CLASS::NAME = #INSTRUCTION_NAME; \
    const NCPU::TDecoderRecorder INSTRUCTION_CLASS::DECODER_RECORDER( \
        INSTRUCTION_CLASS::OPCODE, \
        #INSTRUCTION_NAME, \
        new NCPU::TSingleInstructionDecoder<INSTRUCTION_CLASS>() \
    );

#define DEFINE_ENCODERS(INSTRUCTION_CLASS) \
    NCPU::TOpcode INSTRUCTION_CLASS::ToOpcode() const { \
        NCPU::TOpcode opcode = (OPCODE << 16); \
        for (int i = 0; i < NCPU::IInstruction::MAX_ARGS_NUM; ++i) { \
            if (ARGS[i].length_ == 0) { \
                break; \
            } \
            opcode |= (arg_values_[i] << ARGS[i].offset_); \
        } \
        return opcode; \
    } \
    \
    NCPU::TString INSTRUCTION_CLASS::ToAssembly() const { \
        std::stringstream assembly; \
        assembly << NAME; \
        for (int i = 0; i < NCPU::IInstruction::MAX_ARGS_NUM; ++i) { \
            if (ARGS[i].length_ == 0) { \
                break; \
            } \
            assembly << " " << arg_values_[i]; \
        } \
        return assembly.str(); \
    }

#define ADD_INSTRUCTION(INSTRUCTION_NAME, OPCODE_PATTERN) \
    DECLARE_INSTRUCTION(T ## INSTRUCTION_NAME ## Instruction, INSTRUCTION_NAME, OPCODE_PATTERN) \
    DEFINE_ENCODERS(T ## INSTRUCTION_NAME ## Instruction) \
    void T ## INSTRUCTION_NAME ## Instruction::Execute(NCPU::TCPUState& state) const

#define UNPACK_ARG(arg) \
    auto arg = arg_values_.at(#arg[0] - 'x');

#define UNPACK_ARG_AS(arg, type) \
    auto arg = static_cast<type>(#arg[0] - 'x');

#define UNPACK_STACK_TOP(var) \
    auto var = state.stack_.GetFrontElement(); \
    state.stack_.Pop();

#define UNPACK_STACK_TOP_AS(var, type) \
    auto var = static_cast<type>(state.stack_.GetFrontElement()); \
    state.stack_.Pop();

#endif // NCPU_INSTRUCTION_HPP