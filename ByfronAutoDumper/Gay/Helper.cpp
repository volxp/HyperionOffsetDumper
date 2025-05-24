#include "Helper.hpp"
#include <algorithm>
#include <cctype>

std::vector<uint8_t> PatternHelper::hexstr2bytes(const std::string& hexStr) {
    std::vector<uint8_t> bytes;
    std::string cleanHex;
    for (char c : hexStr) {
        if (c != ' ' && c != '\t' && c != '\n') {
            cleanHex += std::toupper(c);
        }
    }
    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        if (i + 1 < cleanHex.length()) {
            if (ishexchar(cleanHex[i]) && ishexchar(cleanHex[i + 1])) {
                uint8_t byte = (hexchar2val(cleanHex[i]) << 4) | hexchar2val(cleanHex[i + 1]);
                bytes.push_back(byte);
            }
        }
    }

    return bytes;
}

PatternHelper::PatternMatch PatternHelper::findpat(const std::vector<uint8_t>& data,
    const std::vector<uint8_t>& pattern,
    const std::vector<bool>& mask) {
    if (pattern.empty() || data.size() < pattern.size()) {
        return PatternMatch();
    }

    bool usemask = !mask.empty() && mask.size() == pattern.size();

    for (size_t i = 0; i <= data.size() - pattern.size(); ++i) {
        bool match = true;

        for (size_t j = 0; j < pattern.size(); ++j) {
            if (usemask && !mask[j]) {
                continue;
            }

            if (data[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            return PatternMatch(i);
        }
    }

    return PatternMatch();
}

PatternHelper::PatternMatch PatternHelper::findhexpat(const std::vector<uint8_t>& data,
    const std::string& hexPattern) {
    std::vector<uint8_t> pattern = hexstr2bytes(hexPattern);
    return findpat(data, pattern);
}

std::vector<uint8_t> PatternHelper::readbin(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);

    if (file.gcount() != static_cast<std::streamsize>(fileSize)) {
        throw std::runtime_error("Failed to read entire file");
    }

    return data;
}

uintptr_t PatternHelper::getrebase(size_t fileOffset, uintptr_t baseAddress) {
    return baseAddress + fileOffset;
}

void PatternHelper::printbytes(const std::vector<uint8_t>& data, size_t start, size_t count) {
    std::cout << "Bytes at offset 0x" << std::hex << start << ": ";

    size_t end = start + count;
    if (end > data.size()) {
        end = data.size();
    }

    for (size_t i = start; i < end; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

bool PatternHelper::ishexchar(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

uint8_t PatternHelper::hexchar2val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}
