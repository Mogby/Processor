#include "instruction.hpp"

namespace NCPU {

ADD_INSTRUCTION(PUSH, "0000xxxx") {
    UNPACK_ARG(x);
    state.stack_.Push(x);
}

ADD_INSTRUCTION(POP, "00010000") {
    state.stack_.Pop();
}

ADD_INSTRUCTION(ADD, "00020000") {
    UNPACK_STACK_TOP(x);
    UNPACK_STACK_TOP(y);
    state.stack_.Push(x + y);
}

ADD_INSTRUCTION(SUB, "00030000") {
    UNPACK_STACK_TOP(x);
    UNPACK_STACK_TOP(y);
    state.stack_.Push(x - y);
}

ADD_INSTRUCTION(MUL, "00040000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    state.stack_.Push(x * y);
}

ADD_INSTRUCTION(DIV, "00050000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    state.stack_.Push(x / y);
}

ADD_INSTRUCTION(SIN, "00060000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    state.stack_.Push(sin(x));
}

ADD_INSTRUCTION(COS, "00070000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    state.stack_.Push(cos(x));
}

ADD_INSTRUCTION(SQRT, "00080000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    state.stack_.Push(sqrt(x));
}

ADD_INSTRUCTION(READ, "00090000") {
    std::cout << "input:" << std::endl;
    TInt x;
    std::cin >> x;
    state.stack_.Push(x);
}

ADD_INSTRUCTION(WRITE, "000A0000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    std::cout << x;
}

ADD_INSTRUCTION(WRITECH, "000B0000") {
    UNPACK_STACK_TOP_AS(x, char);
    std::cout << x;
}

ADD_INSTRUCTION(DUP, "000C0000") {
    UNPACK_STACK_TOP(x);
    state.stack_.Push(x);
    state.stack_.Push(x);
}

ADD_INSTRUCTION(JMP, "000D0000") {
    UNPACK_STACK_TOP(addr);
    state.pc_ = addr;
}

ADD_INSTRUCTION(JE, "000E0000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP(x);
    UNPACK_STACK_TOP(y);
    if (x == y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(JNE, "000F0000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP(x);
    UNPACK_STACK_TOP(y);
    if (x != y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(JG, "00100000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    if (x > y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(JGE, "00110000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    if (x >= y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(JL, "00120000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    if (x < y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(JLE, "00130000") {
    UNPACK_STACK_TOP(addr);
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    if (x <= y) {
        state.pc_ = addr;
    }
}

ADD_INSTRUCTION(PUSHR, "001400xx") {
    UNPACK_ARG(x);
    state.stack_.Push(state.v_[x]);
}

ADD_INSTRUCTION(POPR, "001500xx") {
    UNPACK_ARG(x);
    UNPACK_STACK_TOP(y);
    state.v_[x] = y;
}

ADD_INSTRUCTION(CALL, "00160000") {
    UNPACK_STACK_TOP(addr);
    state.stack_.Push(state.pc_);
    state.pc_ = addr;
}

ADD_INSTRUCTION(RET, "00170000") {
    UNPACK_STACK_TOP(addr);
    state.pc_ = addr;
}

ADD_INSTRUCTION(MOD, "00190000") {
    UNPACK_STACK_TOP_AS(x, TInt);
    UNPACK_STACK_TOP_AS(y, TInt);
    state.stack_.Push(x % y);
}

} // namespace NCPU