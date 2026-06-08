#include "base85ed.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace base85 {

namespace {

constexpr char ALPHABET[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

constexpr std::size_t BASE = 85;
constexpr std::size_t BLOCK_BYTES = 4;
constexpr std::size_t BLOCK_CHARS = 5;

std::array<int, 256> make_lookup() {
    std::array<int, 256> lookup{};
    lookup.fill(-1);

    for (int i = 0; i < static_cast<int>(BASE); ++i) {
        lookup[static_cast<unsigned char>(ALPHABET[i])] = i;
    }

    return lookup;
}

}  // namespace

std::vector<uint8_t> encode(std::vector<uint8_t> const &bytes) {
    std::vector<uint8_t> out;
    out.reserve((bytes.size() + BLOCK_BYTES - 1) / BLOCK_BYTES * BLOCK_CHARS);

    for (std::size_t i = 0; i < bytes.size(); i += BLOCK_BYTES) {
        const std::size_t count = std::min(BLOCK_BYTES, bytes.size() - i);

        uint32_t value = 0;

        for (std::size_t j = 0; j < count; ++j) {
            value = (value << 8) | bytes[i + j];
        }

        value <<= 8 * (BLOCK_BYTES - count);

        std::array<uint8_t, BLOCK_CHARS> block{};

        for (int j = static_cast<int>(BLOCK_CHARS) - 1; j >= 0; --j) {
            block[j] = static_cast<uint8_t>(ALPHABET[value % BASE]);
            value /= BASE;
        }

        const std::size_t output_count = count < BLOCK_BYTES ? count + 1 : BLOCK_CHARS;

        out.insert(out.end(), block.begin(), block.begin() + output_count);
    }

    return out;
}

std::vector<uint8_t> decode(std::vector<uint8_t> const &b85str) {
    if (b85str.empty()) {
        return {};
    }

    const auto lookup = make_lookup();

    std::vector<uint8_t> out;
    out.reserve(b85str.size() / BLOCK_CHARS * BLOCK_BYTES + BLOCK_BYTES);

    for (std::size_t i = 0; i < b85str.size(); i += BLOCK_CHARS) {
        const std::size_t count = std::min(BLOCK_CHARS, b85str.size() - i);

        uint64_t value = 0;

        for (std::size_t j = 0; j < count; ++j) {
            const int digit = lookup[b85str[i + j]];

            if (digit < 0) {
                return {};
            }

            value = value * BASE + static_cast<uint64_t>(digit);
        }

        for (std::size_t j = count; j < BLOCK_CHARS; ++j) {
            value = value * BASE + (BASE - 1);
        }

        if (value > 0xFFFFFFFFULL) {
            return {};
        }

        const std::size_t output_count = count < BLOCK_CHARS ? count - 1 : BLOCK_BYTES;

        for (std::size_t j = 0; j < output_count; ++j) {
            const int shift = static_cast<int>(8 * (BLOCK_BYTES - 1 - j));
            out.push_back(static_cast<uint8_t>((value >> shift) & 0xFF));
        }
    }

    return out;
}

}  // namespace base85
