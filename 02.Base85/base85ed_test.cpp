#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string.h>

#include "base85ed.h"

const std::vector<std::pair<const char *, const char * >> short_cases =
{
    { "",     ""     },
    { "F#",   "1"    },
    { "F){",  "12"   },
    { "F)}j", "123"  },
    { "F)}kW","1234" }
};

static std::vector<uint8_t> cstr2v(const char *s)
{
    return std::vector<uint8_t>(
               s,
               s + std::string(s).size()
           );
}

// Тесты encode
TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::encode(cstr2v(p.second)), cstr2v(p.first));
    }
}

// Тесты decode
TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(p.first)), cstr2v(p.second));
    }
}

TEST(Base85Evil, UnalignedLengths) {
    for (int len = 1; len <= 10; len++) {
        std::vector<uint8_t> data(len);
        for (int i = 0; i < len; i++) {
            data[i] = i * 13 % 251;
        }
        
        auto encoded = base85::encode(data);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(data, decoded) << "Failed for length " << len;
        
        int full_blocks = len / 4;
        int remainder = len % 4;
        size_t expected_size = full_blocks * 5;
        if (remainder > 0) {
            expected_size += remainder + 1;
        }
        EXPECT_EQ(encoded.size(), expected_size) << "Wrong encoded size for length " << len;
    }
}

TEST(Base85Evil, AllZeros) {
    // 4 нуля должны закодируются "!!!!!"
    auto four_zeros = std::vector<uint8_t>(4, 0);
    auto encoded = base85::encode(four_zeros);
    EXPECT_EQ(encoded.size(), 5);
    for (uint8_t c : encoded) {
        EXPECT_EQ(c, '!');
    }
    
    for (int len = 1; len <= 8; len++) {
        std::vector<uint8_t> zeros(len, 0);
        EXPECT_EQ(base85::decode(base85::encode(zeros)), zeros);
    }
}

TEST(Base85Evil, AllFF) {
    for (int len = 1; len <= 8; len++) {
        std::vector<uint8_t> ff(len, 0xFF);
        EXPECT_EQ(base85::decode(base85::encode(ff)), ff);
    }
}
