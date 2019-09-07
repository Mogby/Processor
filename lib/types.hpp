#ifndef NCPU_TYPES_HPP
#define NCPU_TYPES_HPP

#include <string>

#include <cstdint>

namespace NCPU {

using TByte = uint8_t;
using TWord = uint16_t;
using TDWord = uint32_t;
using TOpcode = TDWord;

using TInt = int16_t;
using TSize = size_t;
using TReal = float;
using TString = std::string;

} // namespace NCPU

#endif // NCPU_TYPES_HPP