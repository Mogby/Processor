#ifndef NCPU_STATE_HPP
#define NCPU_STATE_HPP

#include "types.hpp"
#include "stack.hpp"

namespace NCPU {

struct TCPUState {
  public:
    static const TSize MEMORY_SIZE                      = 0x1000;
    static const TSize STACK_SIZE                       = 0x100;
    static const TSize GENERAL_PURPOSE_REGISTERS_NUM    = 0x10;

  public:
    ////////////////////////////////////////////////////////////
    // Registers
    ////////////////////////////////////////////////////////////

    TWord v_[GENERAL_PURPOSE_REGISTERS_NUM] = {0}; // general purpose

    TWord pc_{0}; // program counter

    ////////////////////////////////////////////////////////////
    // Memory
    ////////////////////////////////////////////////////////////

    TWord ram_[MEMORY_SIZE] = {0};
    TStack<TWord> stack_{STACK_SIZE};
};

} // namespace NCPU

#endif // NCPU_STATE_HPP