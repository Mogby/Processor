#include "util.hpp"

#include <algorithm>
#include <locale>
#include <regex>

#include <cctype>

namespace NCPU {

TByte HexToInt(TByte hex) {
    hex = tolower(hex);
    if ('a' <= hex && hex <= 'f') {
        return hex - 'a' + 10;
    } else if ('0' <= hex && hex <= '9'){
        return hex - '0';
    } else {
        throw TException{"Invalid hex digit"};
    }
}

TString ToUpper(NCPU::TString string) {
    std::transform(string.begin(), string.end(), string.begin(), toupper);
    return string;
}

TString StripString(NCPU::TString string) {
    string.erase(string.begin(), std::find_if(string.begin(), string.end(),
            [] (int c) {
                return !std::isspace(c);
            }));
    string.erase(std::find_if(string.rbegin(), string.rend(),
            [](int ch) {
                return !std::isspace(ch);
            }).base(), string.end());
    return string;
}

// As proposed in
// https://stackoverflow.com/questions/236129
std::vector<TString> SplitString(const TString& string) {
    std::vector<TString> result;
    std::istringstream iss(string);
    std::copy(std::istream_iterator<TString>(iss),
              std::istream_iterator<TString>(),
              std::back_inserter(result));
    return result;
}

TString ReplaceString(const TString &haystack, const TString &needle, const TString &replacement) {
    std::regex regex(needle);
    return std::regex_replace(haystack, regex, replacement);
}

} // namespace NCPU