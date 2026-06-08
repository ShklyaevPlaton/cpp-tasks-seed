#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "base85ed.h"

namespace
{

std::vector<uint8_t> cstr2v(const char *str)
{
    std::vector<uint8_t> result;

    for (const char *ptr = str; *ptr != '\0'; ++ptr)
    {
        result.push_back(static_cast<uint8_t>(*ptr));
    }

    return result;
}

std::vector<uint8_t> make_data(std::size_t length)
{
    std::vector<uint8_t> data;
    data.reserve(length);

    for (std::size_t i = 0; i < length; ++i)
    {
        data.push_back(static_cast<uint8_t>((i * 13) % 251));
    }

    return data;
}

}

TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    const std::vector<std::pair<const char *, const char *>> cases =
    {
        {"", ""},
        {"1", "F#"},
        {"12", "F){"},
        {"123", "F)}j"},
        {"1234", "F)}kW"},
    };

    for (const auto &p : cases)
    {
        EXPECT_EQ(base85::encode(cstr2v(p.first)), cstr2v(p.second));
    }
}

TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    const std::vector<std::pair<const char *, const char *>> cases =
    {
        {"", ""},
        {"F#", "1"},
        {"F){", "12"},
        {"F)}j", "123"},
        {"F)}kW", "1234"},
    };

    for (const auto &p : cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(p.first)), cstr2v(p.second));
    }
}

TEST(Base85RoundTrip, DifferentLengths)
{
    const std::vector<std::size_t> lengths =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 31, 32, 33, 100,
    };

    for (std::size_t length : lengths)
    {
        const std::vector<uint8_t> data = make_data(length);
        const std::vector<uint8_t> encoded = base85::encode(data);
        const std::vector<uint8_t> decoded = base85::decode(encoded);

        EXPECT_EQ(decoded, data) << "Failed for length " << length;
    }
}

TEST(Base85RoundTrip, EncodedSizeForDifferentLengths)
{
    for (std::size_t length = 0; length <= 20; ++length)
    {
        const std::vector<uint8_t> data = make_data(length);
        const std::vector<uint8_t> encoded = base85::encode(data);

        const std::size_t full_blocks = length / 4;
        const std::size_t remainder = length % 4;
        std::size_t expected_size = full_blocks * 5;

        if (remainder != 0)
        {
            expected_size += remainder + 1;
        }

        EXPECT_EQ(encoded.size(), expected_size) << "Wrong size for length " << length;
    }
}

TEST(Base85RoundTrip, AllZeros)
{
    const std::vector<uint8_t> four_zeros(4, 0);
    const std::vector<uint8_t> encoded = base85::encode(four_zeros);

    EXPECT_EQ(encoded, cstr2v("00000"));

    for (std::size_t length = 1; length <= 12; ++length)
    {
        const std::vector<uint8_t> zeros(length, 0);

        EXPECT_EQ(base85::decode(base85::encode(zeros)), zeros)
                << "Failed for length " << length;
    }
}

TEST(Base85RoundTrip, AllFF)
{
    for (std::size_t length = 1; length <= 12; ++length)
    {
        const std::vector<uint8_t> ff(length, 0xFF);

        EXPECT_EQ(base85::decode(base85::encode(ff)), ff)
                << "Failed for length " << length;
    }
}

TEST(Base85RoundTrip, BytesGreaterThan127)
{
    const std::vector<uint8_t> data =
    {
        0, 1, 2, 127, 128, 129, 200, 254, 255,
    };

    EXPECT_EQ(base85::decode(base85::encode(data)), data);
}

TEST(Base85Decode, InvalidCharacterReturnsEmptyVector)
{
    EXPECT_TRUE(base85::decode(cstr2v("\"")).empty());
}
