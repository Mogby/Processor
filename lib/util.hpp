#ifndef NCPU_UTIL_HPP
#define NCPU_UTIL_HPP

#include "types.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace NCPU {

class TException : public std::exception {
  public:
    TException() = default;

    explicit TException(TString message)
        : message_{std::move(message)} {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

  private:
    const std::string message_{};
};

template <typename TType>
class TDumper {
  private:
    // see
    // https://habr.com/post/205772
    template <typename TType>
    struct TIsPrintable {
        static void detect(...);

        template <typename TType>
        static decltype(std::cout << std::declval<TType>()) detect(const TType&);

        static constexpr bool value_ = std::negation<
                std::is_same<void, decltype(detect(std::declval<TType>()))>
        >::value;
    };

    template <typename TType, bool IsPrintable>
    struct TInnerDumper {};

    template <typename TType>
    struct TInnerDumper<TType, true> {
        static TString GetDump(const TType& value) {
            std::ostringstream dump;
            dump << value;
            return dump.str();
        }
    };

    template <typename TType>
    struct TInnerDumper<TType, false> {
        static TString GetDump(const TType& value) {
            std::ostringstream dump;
            dump << typeid(TType).name() << "@0x" << &value;
            return dump.str();
        }
    };

  public:
    static TString GetDump(const TType& value) {
        return TInnerDumper<TType, TIsPrintable<TType>::value_>::GetDump(value);
    }
};

TByte HexToInt(TByte hex);

TString ToUpper(TString string);
TString StripString(TString string);
std::vector<TString> SplitString(const TString& string);
TString ReplaceString(const TString &haystack, const TString &needle, const TString &replacement);

template<typename TValue>
TValue FromString(const TString& string) {
    TValue value;
    std::stringstream converter;
    converter << string;
    converter >> value;
    return value;
}

template<typename TValue>
TString ToString(const TValue& value) {
    std::stringstream converter;
    converter << value;
    return converter.str();
}

} // namespace NCPU

#endif // NCPU_UTIL_HPP