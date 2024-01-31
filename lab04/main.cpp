#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>

#define LEFTROTATE(x, n) (((x) << (n)) | ((x) >> (32 - (n))))


// Прототипы функций
uint32_t F(uint32_t x, uint32_t y, uint32_t z);
uint32_t G(uint32_t x, uint32_t y, uint32_t z);
uint32_t H(uint32_t x, uint32_t y, uint32_t z);
uint32_t I(uint32_t x, uint32_t y, uint32_t z);
uint32_t J(uint32_t x, uint32_t y, uint32_t z);
// void FF(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
// void GG(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
// void HH(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
// void II(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
// void JJ(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
// void round1(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac);
// void round2(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac);
// void round3(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac);
// void round4(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac);
// void round5(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac);
// std::string ripemd160(const std::string& input);

const uint32_t K[5] = {
    0x00000000, 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xa953fd4e
};
const uint32_t KK[5] = {
    0x50a28be6, 0x5c4dd124, 0x6d703ef3, 0x7a6d76e9, 0x00000000
};
const uint32_t R[5][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8},
    {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12},
    {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2},
    {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13}
};
const uint32_t RR[5][16] = {
    {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12},
    {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2},
    {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13},
    {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14},
    {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11}
};
const uint32_t S[5][16] = {
    {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8},
    {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12},
    {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5},
    {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12},
    {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6}
};
const uint32_t SS[5][16] = {
    {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6},
    {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11},
    {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5},
    {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8},
    {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11}
};

uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
}

uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) | (~x & z);
}

uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
    return (x | ~y) ^ z;
}

uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
    return (x & z) | (y & ~z);
}

uint32_t J(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ (y | ~z);
}

// void FF(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
//     a += F(b, c, d) + x + ac;
//     a = ((a << s) | (a >> (32 - s)));
//     a += b;
// }

// void GG(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
//     a += G(b, c, d) + x + ac;
//     a = ((a << s) | (a >> (32 - s)));
//     a += b;
// }

// void HH(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
//     a += H(b, c, d) + x + ac;
//     a = ((a << s) | (a >> (32 - s)));
//     a += b;
// }

// void II(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
//     a += I(b, c, d) + x + ac;
//     a = ((a << s) | (a >> (32 - s)));
//     a += b;
// }

// void JJ(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
//     a += (c ^ (b | ~d)) + x + ac;
//     a = ((a << s) | (a >> (32 - s)));
//     a += b;
// }

// void round1(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac) {
//     FF(a, b, c, d, x, s, ac);
//     FF(e, a, b, c, x, s, ac);
// }

// void round2(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac) {
//     GG(a, b, c, d, x, s, ac);
//     GG(e, a, b, c, x, s, ac);
// }

// void round3(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac) {
//     HH(a, b, c, d, x, s, ac);
//     HH(e, a, b, c, x, s, ac);
// }

// void round4(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac) {
//     II(a, b, c, d, x, s, ac);
//     II(e, a, b, c, x, s, ac);
// }

// void round5(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, uint32_t x, uint32_t s, uint32_t ac) {
//     JJ(a, b, c, d, x, s, ac);
//     JJ(e, a, b, c, x, s, ac);
// }

// void compress(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d, uint32_t& e, const uint32_t* W, const uint32_t* R, const uint32_t* S, const uint32_t* K, size_t rounds) {
//     for (size_t i = 0; i < rounds; ++i) {
//         uint32_t temp = a + F(b, c, d) + W[R[i]] + K[i];
//         temp = ((temp << S[i]) | (temp >> (32 - S[i]))) + e;
//         a = e;
//         e = d;
//         d = (c << 10) | (c >> 22);
//         c = b;
//         b = temp;
//     }
// }

// std::string ripemd160(const std::string& input) {
//     // Дополнение входных данных
//     uint64_t total_bits = input.length() * 8;
//     uint32_t extra_bits = (448 - total_bits % 512 + 512) % 512;
//     uint32_t padding_size = (extra_bits + 64) / 8;
//     std::string padding(padding_size, '\0');
//     padding[0] = 0x80;
//     for (int i = 1; i < padding_size - 8; ++i) {
//         padding[i] = 0x00;
//     }
//     // Добавление длины исходного сообщения в битах
//     for (int i = 0; i < 8; ++i) {
//         padding[padding_size - 8 + i] = (total_bits >> (i * 8)) & 0xFF;
//     }
//     // Инициализация переменных состояния
//     uint32_t h0 = 0x67452301;
//     uint32_t h1 = 0xefcdab89;
//     uint32_t h2 = 0x98badcfe;
//     uint32_t h3 = 0x10325476;
//     uint32_t h4 = 0xc3d2e1f0;
//     // Обработка блоков сообщения
//     for (size_t i = 0; i < padding_size; i += 64) {
//         uint32_t W[16];
//         // Заполнение массива слов
//         for (int j = 0; j < 16; ++j) {
//             W[j] = (uint32_t(padding[i + 4 * j]) << 24) | (uint32_t(padding[i + 4 * j + 1]) << 16) |
//                    (uint32_t(padding[i + 4 * j + 2]) << 8) | (uint32_t(padding[i + 4 * j + 3]));
//         }
//         uint32_t A = h0, B = h1, C = h2, D = h3, E = h4;
//         // Раунды сжатия
//         compress(A, B, C, D, E, W, R[0], S[0], &K[0], 16);
//         compress(E, A, B, C, D, W, R[1], S[1], &K[1], 16);
//         compress(D, E, A, B, C, W, R[2], S[2], &K[2], 16);
//         compress(C, D, E, A, B, W, R[3], S[3], &K[3], 16);
//         compress(B, C, D, E, A, W, R[4], S[4], &K[4], 16);
//         // Обновление хеша
//         h0 += A;
//         h1 += B;
//         h2 += C;
//         h3 += D;
//         h4 += E;
//     }
//     // Форматирование хеша в строку
//     std::stringstream ss;
//     ss << std::hex << std::setw(8) << std::setfill('0') << h0
//        << std::hex << std::setw(8) << std::setfill('0') << h1
//        << std::hex << std::setw(8) << std::setfill('0') << h2
//        << std::hex << std::setw(8) << std::setfill('0') << h3
//        << std::hex << std::setw(8) << std::setfill('0') << h4;
//     return ss.str();
// }

// int main() {
//     std::string input = "The quick brown fox jumps over the lazy dog";
//     std::string hash = ripemd160(input);
//     std::cout << "RIPEMD-160 hash of \"" << input << "\": " << hash << std::endl;
//     return 0;
// }

uint32_t rols(uint32_t x, uint32_t s) {
    return (x << s) | (x >> (32 - s));
}

uint32_t rol10(uint32_t x) {
    return rols(x, 10);
}

uint32_t f(int j, uint32_t x, uint32_t y, uint32_t z) {
    if (j < 16)
        return F(x, y, z);
    else if (j < 32)
        return G(x, y, z);
    else if (j < 48)
        return H(x, y, z);
    else if (j < 64)
        return I(x, y, z);
    else
        return J(x, y, z);
}

uint32_t f_prime(int j, uint32_t x, uint32_t y, uint32_t z) {
    if (j < 16)
        return J(x, y, z);
    else if (j < 32)
        return I(x, y, z);
    else if (j < 48)
        return H(x, y, z);
    else if (j < 64)
        return G(x, y, z);
    else
        return F(x, y, z);
}

void ripemd160(const uint32_t *message, uint32_t *digest) {
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xc3d2e1f0;
    uint32_t A = h0, B = h1, C = h2, D = h3, E = h4;
    uint32_t A_prime = h0, B_prime = h1, C_prime = h2, D_prime = h3, E_prime = h4;
    uint32_t T = 0, T_prime = 0;

    for (int i = 0; i < 5; i++) {
        A = h0, B = h1, C = h2, D = h3, E = h4;
        // T = 0;
        A_prime = h0, B_prime = h1, C_prime = h2, D_prime = h3, E_prime = h4;
        
        for (int j = 0; j < 16; j++) {

            T = rols(A + f(j, B, C, D) + message[R[i][j]] + K[i], S[i][j]) + E;
            A = E; E = D; D = rol10(C); C = B; B = T;
            
            T = rols(A_prime + f(79 - j, B_prime, C_prime, D_prime) + message[RR[i][j]] + KK[i], SS[i][j]) + E_prime;
            A_prime = E_prime; E_prime = D_prime; D_prime = rol10(C_prime); C_prime = B_prime; B_prime = T;

            // T = rols(A + f(j, B, C, D) + message[R[i][j]] + K[i], S[i][j]) + E;
            // T_prime = rols(A_prime + f_prime(j, B_prime, C_prime, D_prime) + message[RR[i][j]] + KK[i], SS[i][j]) + E_prime;

            // A = E; E = D; D = rol10(C); C = B; B = T;
            // A_prime = E_prime; E_prime = D_prime; D_prime = rol10(C_prime); C_prime = B_prime; B_prime = T_prime;
        }

        T = h1 + C + D_prime;
        h1 = h2 + D + E_prime;
        h2 = h3 + E + A_prime;
        h3 = h4 + A + B_prime;
        h4 = h0 + B + C_prime;
        h0 = T;
    }

    digest[0] = h0;
    digest[1] = h1;
    digest[2] = h2;
    digest[3] = h3;
    digest[4] = h4;
}

void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
    // Добавляем 1 бит
    padded_message[0] = 0x80;

    // Заполняем оставшиеся биты нулями
    memset(padded_message + 1, 0, 56);

    // Записываем длину исходного сообщения (в битах) в конец расширенного сообщения
    uint64_t bit_length = input_len * 8;
    for (int i = 0; i < 8; i++) {
        padded_message[63 - i] = (uint8_t)(bit_length >> (i * 8));
    }
}

// 

int main() {
    // Пример входных данных для хеширования
    const char *message = "The quick brown fox jumps over the lazy dog";
    uint32_t digest[5]; // Массив для хранения результата хеширования

    // Преобразование входных данных в формат uint32_t
    size_t message_len = strlen(message);
    uint32_t message_words[16];
    memset(message_words, 0, sizeof(message_words)); // Заполняем нулями
    for (size_t i = 0; i < message_len; i++) {
        message_words[i / 4] |= (uint32_t)message[i] << (i % 4 * 8);
    }

    // Расширение сообщения до длины, кратной 512 битам
    // Добавляем бит "1"
    message_words[message_len / 4] |= 0x80 << ((message_len % 4) * 8);
    // Добавляем биты "0" до длины, кратной 512 битам
    size_t padding_len = ((message_len + 9) % 64 < 56) ? 56 - (message_len + 9) % 64 : 120 - (message_len + 9) % 64;
    memset(&message_words[(message_len + 8) / 4], 0, padding_len);

    // Вычисляем длину сообщения в битах
    uint64_t message_len_bits = (uint64_t)message_len * 8;

    // Добавляем длину сообщения в конец (в битах)
    message_words[14] = (uint32_t)(message_len_bits & 0xFFFFFFFF);
    message_words[15] = (uint32_t)((message_len_bits >> 32) & 0xFFFFFFFF);

    // Выполнение хеширования
    ripemd160(message_words, digest);

    // Вывод результата хеширования
    printf("Message: %s\n", message);
    printf("RIPEMD-160 Hash: %08x %08x %08x %08x %08x\n", digest[0], digest[1], digest[2], digest[3], digest[4]);

    return 0;
}