#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>


// Прототипы функций
uint32_t F(uint32_t x, uint32_t y, uint32_t z);
uint32_t G(uint32_t x, uint32_t y, uint32_t z);
uint32_t H(uint32_t x, uint32_t y, uint32_t z);
uint32_t I(uint32_t x, uint32_t y, uint32_t z);
uint32_t J(uint32_t x, uint32_t y, uint32_t z);

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

// void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
//     // Добавляем 1 бит
//     padded_message[input_len] = 0x80;

//     // Заполняем оставшиеся биты нулями
//     for (size_t i = input_len + 1; i < 64; ++i) {
//         padded_message[i] = 0;
//     }

//     // Добавляем длину сообщения (в битах) в виде двух 32-битных слов
//     uint64_t bit_length = input_len * 8;
//     for (int i = 0; i < 8; ++i) {
//         padded_message[56 + i] = (uint8_t)(bit_length >> (i * 8));
//     }
// }

// void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
//     // Добавляем 1 бит после последнего байта входного сообщения
//     padded_message[input_len] = 0x80;

//     // Заполняем оставшиеся биты нулями до добавления длины сообщения
//     for (size_t i = input_len + 1; i < 64; ++i) {
//         padded_message[i] = 0;
//     }

//     // Добавляем длину сообщения (в битах) в виде двух 32-битных слов
//     uint64_t bit_length = input_len * 8;
//     for (int i = 0; i < 8; ++i) {
//         padded_message[63 - i] = (uint8_t)(bit_length >> (i * 8));
//     }
// }

// void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
//     // Добавляем 1 бит после последнего байта входного сообщения
//     padded_message[input_len] = 0x80;

//     // Заполняем оставшиеся биты нулями до добавления длины сообщения
//     size_t padding_len = ((input_len + 8) % 64 < 56) ? 56 - (input_len + 8) % 64 : 120 - (input_len + 8) % 64;
//     memset(&padded_message[input_len + 1], 0, padding_len);

//     // Добавляем длину сообщения (в битах) в виде двух 32-битных слов
//     uint64_t bit_length = input_len * 8;
//     for (int i = 0; i < 8; ++i) {
//         padded_message[input_len + padding_len + i] = (uint8_t)(bit_length >> (i * 8));
//     }
// }

// void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
//     // Добавляем 1 бит после последнего байта входного сообщения
//     padded_message[input_len] = 0x80;

//     // Заполняем оставшиеся биты нулями до добавления длины сообщения
//     size_t padding_len = ((input_len + 8) % 64 < 56) ? 56 - (input_len + 8) % 64 : 120 - (input_len + 8) % 64;
//     memset(&padded_message[input_len + 1], 0, padding_len);

//     // Добавляем длину сообщения (в битах) в виде двух 32-битных слов
//     uint64_t bit_length = input_len * 8;
//     for (int i = 0; i < 8; ++i) {
//         padded_message[input_len + padding_len + i] = (uint8_t)(bit_length >> (i * 8));
//     }

//     // Преобразуем последовательность байтов в little-endian порядке в 32-битные слова
//     for (size_t i = 0; i < input_len; i += 4) {
//         uint32_t word = 0;
//         for (size_t j = 0; j < 4 && i + j < input_len; ++j) {
//             word |= (uint32_t)padded_message[i + j] << (j * 8);
//         }
//         padded_message[i / 4] = word;
//     }
// }

void pad_message(const uint8_t *input, size_t input_len, uint8_t *padded_message) {
    // Добавляем 1 бит после последнего байта входного сообщения
    padded_message[input_len] = 0x80;

    // Заполняем оставшиеся биты нулями до добавления длины сообщения
    size_t padding_len = ((input_len + 8) % 64 < 56) ? 56 - (input_len + 8) % 64 : 120 - (input_len + 8) % 64;
    memset(&padded_message[input_len + 1], 0, padding_len);

    // Добавляем длину сообщения (в битах) в виде двух 32-битных слов
    uint64_t bit_length = input_len * 8;
    for (int i = 0; i < 8; ++i) {
        padded_message[input_len + padding_len + i] = (uint8_t)(bit_length >> (i * 8));
    }

    // Преобразуем последовательность байтов в little-endian порядке в 32-битные слова
    for (size_t i = 0; i < input_len; i += 4) {
        uint32_t word = 0;
        for (size_t j = 0; j < 4 && i + j < input_len; ++j) {
            word |= (uint32_t)padded_message[i + j] << (j * 8);
        }
        padded_message[i / 4] = word;
    }

    // Дополняем блок нулевыми байтами до конца блока
    memset(&padded_message[input_len + padding_len + 8], 0, 64 - (input_len + padding_len + 8));
}


int main() {
    // Входное сообщение для хеширования
    const uint8_t input[] = "The quick brown fox jumps over the lazy dog";
    size_t input_len = strlen((char*)input);

    // Вычисляем длину расширенного сообщения
    size_t padded_len = ((input_len + 8) / 64 + 1) * 64;

    // Выделяем память под расширенное сообщение
    uint8_t* padded_message = new uint8_t[padded_len];

    std::cout << "Input_len: " << input_len << std::endl;
    std::cout << "Padded_len: " << padded_len << std::endl;

    // Копируем входное сообщение в расширенное и дополняем его нулями и длиной
    memcpy(padded_message, input, input_len);
    std::cout << "Padded message: " << padded_message << std::endl;
    for (int i = 0; i < padded_len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)padded_message[i];
    }
    std::cout << std::endl;

    pad_message(input, input_len, padded_message);
    std::cout << "Padded message: " << padded_message << std::endl;
    
    for (int i = 0; i < padded_len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)padded_message[i];
    }
    std::cout << std::endl;

    // Преобразуем расширенное сообщение в массив 32-битных слов
    uint32_t* message = reinterpret_cast<uint32_t*>(padded_message);

    // Выделяем память под результат хеширования
    uint32_t digest[5];

    // Вызываем функцию хеширования RIPEMD-160
    ripemd160(message, digest);

    // Выводим результат хеширования
    std::cout << "RIPEMD-160 hash of \"" << input << "\": ";
    for (int i = 0; i < 5; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << digest[i];
    }
    std::cout << std::endl;

    // Освобождаем выделенную память
    delete[] padded_message;

    return 0;
}