#include "Helper.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <spdlog/spdlog.h>

namespace Addresses {
    namespace Hyperion {
        namespace FUNC {
            inline uintptr_t BitMap() {
                try {
                    std::vector<uint8_t> data = PatternHelper::readbin("dump.bin");
                    std::string ControlFlowGuard = "49 BB FF FF FF FF FF 7F 00 00"; // thats the CFG
                    auto match = PatternHelper::findhexpat(data, ControlFlowGuard);
                    if (!match.found) return 0;

                    std::vector<std::string> dataRefPatterns = {
                        "4C 8B 1D", "48 8B 05", "48 8B 15",
                        "48 8B 0D", "4C 8B 05", "4C 8B 15", "4C 8B 0D"
                    };

                    size_t searchStart = match.offset + 10;
                    size_t searchEnd = std::min(searchStart + 200, data.size());

                    for (const auto& pattern : dataRefPatterns) {
                        std::vector<uint8_t> searchData(data.begin() + searchStart, data.begin() + searchEnd);
                        auto refMatch = PatternHelper::findhexpat(searchData, pattern);

                        if (refMatch.found) {
                            size_t actualOffset = searchStart + refMatch.offset;
                            size_t ripOffsetPos = actualOffset + 3;

                            if (ripOffsetPos + 4 <= data.size()) {
                                int32_t ripOffset = PatternHelper::extractval<int32_t>(data, ripOffsetPos);
                                size_t nextInstrOffset = actualOffset + 7;
                                int64_t absoluteAddr = static_cast<int64_t>(nextInstrOffset) + ripOffset;

                                if (absoluteAddr > 0 && absoluteAddr < static_cast<int64_t>(data.size())) {
                                    return static_cast<uintptr_t>(absoluteAddr);
                                }
                            }
                        }
                    }

                    return 0;
                }
                catch (...) {
                    return 0;
                }
            }

            inline uint32_t PageHash() {
                try {
                    std::vector<uint8_t> data = PatternHelper::readbin("dump.bin");
                    std::string pattern = "89 45 D8 4C 89 F8";
                    auto match = PatternHelper::findhexpat(data, pattern);
                    if (!match.found || match.offset < 4) return 0;
                    size_t xorImmOffset = match.offset - 4;

                    if (xorImmOffset + 4 <= data.size()) {
                        return PatternHelper::extractval<uint32_t>(data, xorImmOffset);
                    }

                    return 0;
                }
                catch (...) {
                    return 0;
                }
            }

            inline uint8_t BitMapHash() {
                try {
                    std::vector<uint8_t> data = PatternHelper::readbin("dump.bin");
                    std::string pattern = "88 45 DC 48 89 F2";
                    auto match = PatternHelper::findhexpat(data, pattern);
                    if (!match.found || match.offset < 4) return 0;
                    return data[match.offset - 1];
                }
                catch (...) {
                    return 0;
                }
            }

            inline uint64_t MaxCaps() {
                try {
                    std::vector<uint8_t> data = PatternHelper::readbin("dump.bin");
                    std::string pattern = "49 81 E4 00 F0 FF FF";
                    auto match = PatternHelper::findhexpat(data, pattern);
                    if (!match.found) return 0;
                    return 0xFFFFFFFFFFFFF000;
                }
                catch (...) {
                    return 0;
                }
            }
        }
    }
}
