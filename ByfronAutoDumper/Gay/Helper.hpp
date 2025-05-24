#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

class PatternHelper {
public:
    struct PatternMatch {
        size_t offset;
        bool found;

        PatternMatch() : offset(0), found(false) {}
        PatternMatch(size_t off) : offset(off), found(true) {}
    };

    static std::vector<uint8_t> hexstr2bytes(const std::string& hexStr);

    static PatternMatch findpat(const std::vector<uint8_t>& data,
        const std::vector<uint8_t>& pattern,
        const std::vector<bool>& mask = {});

    static PatternMatch findhexpat(const std::vector<uint8_t>& data,
        const std::string& hexPattern);

    static std::vector<uint8_t> readbin(const std::string& filename);

    template<typename T>
    static T extractval(const std::vector<uint8_t>& data, size_t offset);

    static uintptr_t getrebase(size_t fileOffset, uintptr_t baseAddress = 0x140000000);

    static void printbytes(const std::vector<uint8_t>& data, size_t start, size_t count);

private:
    static bool ishexchar(char c);

    static uint8_t hexchar2val(char c);
};

template<typename T>
T PatternHelper::extractval(const std::vector<uint8_t>& data, size_t offset) {
    if (offset + sizeof(T) > data.size()) {
        throw std::out_of_range("Offset over data size");
    }

    T value = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        value |= static_cast<T>(data[offset + i]) << (i * 8);
    }

    return value;
}
