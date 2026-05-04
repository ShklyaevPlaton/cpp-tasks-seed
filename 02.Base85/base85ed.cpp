#include "base85ed.h"
#include <array>
#include <algorithm>

namespace base85 {

// Алфавит
const char ALPHABET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

std::vector<uint8_t> encode(std::vector<uint8_t> const &bytes) {
    std::vector<uint8_t> out;
    out.reserve((bytes.size() + 3) / 4 * 5);
    
    for (size_t i = 0; i < bytes.size(); i += 4) {
        uint32_t val = 0;
        size_t cnt = 0;
        
        // 4 байт
        for (size_t j = 0; j < 4 && i + j < bytes.size(); ++j) {
            val = (val << 8) | bytes[i + j];
            cnt++;
        }
        
        if (cnt == 0) break;
        val <<= (8 * (4 - cnt)); // нули
        
        // -> 5 символов
        uint8_t block[5];
        for (int j = 4; j >= 0; --j) {
            block[j] = ALPHABET[val % 85];
            val /= 85;
        }
        
        out.insert(out.end(), block, block + (cnt < 4 ? cnt + 1 : 5));
    }
    return out;
}

std::vector<uint8_t> decode(std::vector<uint8_t> const &b85str) {
    if (b85str.empty()) return {};
    
    std::array<int, 256> lookup;
    lookup.fill(-1);
    for (int i = 0; i < 85; ++i) {
        lookup[(unsigned char)ALPHABET[i]] = i;
    }
    
    std::vector<uint8_t> out;
    out.reserve(b85str.size() / 5 * 4 + 4);
    
    for (size_t i = 0; i < b85str.size(); i += 5) {
        size_t cnt = std::min<size_t>(5, b85str.size() - i);
        
        uint32_t val = 0;
        for (size_t j = 0; j < cnt; ++j) {
            int v = lookup[b85str[i + j]];
            if (v == -1) return {};
            val = val * 85 + v;
        }
        
        for (size_t j = cnt; j < 5; ++j) {
            val = val * 85 + 84; 
        }
        
        for (int j = cnt - 1; j > 0; --j) {
            out.push_back((val >> (8 * (j - 1))) & 0xFF);
        }
    }
    return out;
}

} // namespace base85
