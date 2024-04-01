#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <openssl/md4.h>
#include <openssl/bn.h>
#include <openssl/rand.h>

// Здесь вы можете использовать библиотеку больших чисел, такую как GMP или OpenSSL

class RSAfun {
private:
    BIGNUM *p, *q, *n, *phi, *e, *d;
public:
    RSAfun() {
        p = BN_new();
        q = BN_new();
        n = BN_new();
        phi = BN_new();
        e = BN_new();
        d = BN_new();
    }

    ~RSAfun() {
        BN_free(p);
        BN_free(q);
        BN_free(n);
        BN_free(phi);
        BN_free(e);
        BN_free(d);
    }

    // Функция для вычисления мультипликативного обратного с помощью расширенного алгоритма Евклида
    long long multiplicative_inverse(long long a, long long b) {
        long long b0 = b, t, q;
        long long x0 = 0, x1 = 1;
        if (b == 1) return 1;
        while (a > 1) {
            q = a / b;
            t = b, b = a % b, a = t;
            t = x0, x0 = x1 - q * x0, x1 = t;
        }
        if (x1 < 0) x1 += b0;
        return x1;
    }

    BIGNUM* multiplicative_inverse(BIGNUM* a, BIGNUM* b) {
        BN_CTX *ctx = BN_CTX_new();
        BIGNUM *res = BN_new();
        BN_mod_inverse(res, a, b, ctx);
        BN_CTX_free(ctx);
        return res;
    }

    // Функция для генерации ключей
    void generate_keys(int bitLength) {
        BN_CTX *ctx = BN_CTX_new();


        // Генерация двух простых чисел p и q
        BN_generate_prime_ex(p, bitLength / 2, 0, NULL, NULL, NULL);
        BN_generate_prime_ex(q, bitLength / 2, 0, NULL, NULL, NULL);

        // Вычисление n = p * q
        BN_mul(n, p, q, ctx);

        // Вычисление функции Эйлера: phi = (p - 1) * (q - 1)
        BIGNUM *p1 = BN_new();
        BIGNUM *q1 = BN_new();
        BN_sub_word(p1, 1);
        BN_sub_word(q1, 1);
        BN_mul(phi, p1, q1, ctx);

        // Выбор целого числа e такое, что 1 < e < phi и наибольший общий делитель (e, phi) = 1
//        do {
//            BN_rand(e, bitLength / 2, 0, 0);
//        } while (BN_gcd(NULL, e, phi, ctx) != 1);
        BIGNUM *gcd = BN_new();  // Создаем объект BIGNUM для хранения результата

        do {
            BN_rand(e, bitLength / 2, 0, 0);
        } while (BN_gcd(gcd, e, phi, ctx) != 1);

        BN_free(gcd);  // Освобождаем память после использования


        // Вычисление d, мультипликативного обратного к e по модулю phi
        BN_mod_inverse(d, e, phi, ctx);

        BN_CTX_free(ctx);
        BN_free(p1);
        BN_free(q1);
    }

    // Функция для шифрования
    BIGNUM* encrypt(BIGNUM* message) {
        BN_CTX *ctx = BN_CTX_new();
        BIGNUM *encrypted = BN_new();
        BN_mod_exp(encrypted, message, e, n, ctx);
        BN_CTX_free(ctx);
        return encrypted;
    }

    // Функция для расшифровки
    BIGNUM* decrypt(BIGNUM* encrypted) {
        BN_CTX *ctx = BN_CTX_new();
        BIGNUM *decrypted = BN_new();
        BN_mod_exp(decrypted, encrypted, d, n, ctx);
        BN_CTX_free(ctx);
        return decrypted;
    }

    // Функция для создания подписи
    BIGNUM* sign(BIGNUM* message) {
        unsigned char hash[MD4_DIGEST_LENGTH];
        MD4((unsigned char*)BN_bn2hex(message), BN_num_bytes(message), hash);

        BIGNUM *hashNum = BN_bin2bn(hash, MD4_DIGEST_LENGTH, NULL);

        BN_CTX *ctx = BN_CTX_new();
        BIGNUM *signature = BN_new();
        BN_mod_exp(signature, hashNum, d, n, ctx);
        BN_CTX_free(ctx);

        BN_free(hashNum);

        return signature;
    }

    // Функция для проверки подписи
    bool verify(BIGNUM* message, BIGNUM* signature) {
        unsigned char hash[MD4_DIGEST_LENGTH];
        MD4((unsigned char*)BN_bn2hex(message), BN_num_bytes(message), hash);

        BIGNUM *hashNum = BN_bin2bn(hash, MD4_DIGEST_LENGTH, NULL);

        BN_CTX *ctx = BN_CTX_new();
        BIGNUM *decryptedSignature = BN_new();
        BN_mod_exp(decryptedSignature, signature, e, n, ctx);
        BN_CTX_free(ctx);

        bool isValid = BN_cmp(hashNum, decryptedSignature) == 0;

        BN_free(hashNum);
        BN_free(decryptedSignature);

        return isValid;
    }
};

// int main() {
//     RSAfun rsa;

//     // Генерация ключей
//     rsa.generate_keys(1024);

//     // Шифрование и расшифровка сообщения
//     BIGNUM *message = BN_new();
//     BN_set_word(message, 123456789);  // Пример сообщения

//     BIGNUM *encrypted = rsa.encrypt(message);
//     BIGNUM *decrypted = rsa.decrypt(encrypted);

//     // Создание и проверка подписи
//     BIGNUM *signature = rsa.sign(message);
//     bool isValid = rsa.verify(message, signature);

//     // Вывод результатов
//     std::cout << "Original message: " << BN_bn2dec(message) << std::endl;
//     std::cout << "Encrypted message: " << BN_bn2dec(encrypted) << std::endl;
//     std::cout << "Decrypted message: " << BN_bn2dec(decrypted) << std::endl;
//     std::cout << "Signature is " << (isValid ? "valid" : "invalid") << std::endl;

//     // Освобождение памяти
//     BN_free(message);
//     BN_free(encrypted);
//     BN_free(decrypted);
//     BN_free(signature);

//     return 0;
// }
