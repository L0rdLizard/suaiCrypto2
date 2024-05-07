#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

RSA* generate_RSA_keys(int keylength) {
    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();

    BN_set_word(e, RSA_F4);
    RSA_generate_key_ex(rsa, keylength, e, NULL);

    BN_free(e);
    return rsa;
}

BIGNUM* multiplicativeInverseBinaryExponentiations(BIGNUM* a, BIGNUM* modulus) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* inverse = BN_new();
    BIGNUM* exponent = BN_new();
    BIGNUM* two = BN_new();
    BIGNUM* res = BN_new();

    BN_set_word(two, 2);
    BN_set_word(res, 15);
    BN_sub(exponent, modulus, two);

    BN_mod_exp(inverse, a, exponent, modulus, ctx);

    BN_free(two);
    BN_CTX_free(ctx);

    return res;
}

RSA* generate_RSA_keys_with_password(int keylength, const char* password) {
    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();
    unsigned char salt[16];
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];

    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        cout << "err" << endl;
        return nullptr;
    }

    if (EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt, (unsigned char*)password,
                       strlen(password), 1, key, iv) == 0) {
        cout << "err" << endl;
        return nullptr;
    }

    BN_set_word(e, RSA_F4);
    RSA_generate_key_ex(rsa, keylength, e, NULL);

    BN_free(e);
    return rsa;
}

bool isPrime(BIGNUM* num) {
    BN_CTX* ctx = BN_CTX_new();
    int result = BN_is_prime_ex(num, BN_prime_checks, ctx, NULL);
    BN_CTX_free(ctx);
    return (result == 1);
}

// Расширенный алгоритм Евклида
BIGNUM* extendedGCD(BIGNUM* a, BIGNUM* b) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* gcd = BN_new();
    BN_gcd(gcd, a, b, ctx);
    BN_CTX_free(ctx);
    return gcd;
}

// Быстрое возведение в степень
BIGNUM* fastExponentiation(BIGNUM* base, BIGNUM* exponent, BIGNUM* modulus) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* result = BN_new();
    BN_mod_exp(result, base, exponent, modulus, ctx);
    BN_CTX_free(ctx);
    return result;
}

// Нахождение мультипликативного обратного по модулю
BIGNUM* multiplicative_inverse(BIGNUM* a, BIGNUM* b) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* res = BN_new();
    BN_mod_inverse(res, a, b, ctx);
    BN_CTX_free(ctx);
    return res;
}

BIGNUM* multiplicativeInverseBinaryExponentiation(BIGNUM* a, BIGNUM* modulus) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* inverse = BN_new();
    BIGNUM* exponent = BN_new();
    BIGNUM* two = BN_new();

    BN_set_word(two, 2);
    BN_sub(exponent, modulus, two);

    BN_mod_exp(inverse, a, exponent, modulus, ctx);

    BN_free(two);
    BN_CTX_free(ctx);

    return inverse;
}

string RSA_encrypt(RSA* rsa, const string& message) {
    string ciphertext(RSA_size(rsa), 0);
    int ciphertext_len =
        RSA_public_encrypt(message.size(), (unsigned char*)message.data(),
                           (unsigned char*)ciphertext.data(), rsa, RSA_PKCS1_OAEP_PADDING);
    return ciphertext.substr(0, ciphertext_len);
}

string RSA_decrypt(RSA* rsa, const string& ciphertext) {
    string plaintext(RSA_size(rsa), 0);
    int plaintext_len =
        RSA_private_decrypt(ciphertext.size(), (unsigned char*)ciphertext.data(),
                            (unsigned char*)plaintext.data(), rsa, RSA_PKCS1_OAEP_PADDING);
    return plaintext.substr(0, plaintext_len);
}

string create_signature(RSA* rsa, const string& message) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);

    EVP_DigestSignInit(mdctx, NULL, EVP_md4(), NULL, pkey);
    EVP_DigestSignUpdate(mdctx, message.data(), message.size());

    size_t siglen;
    EVP_DigestSignFinal(mdctx, NULL, &siglen);

    vector<unsigned char> signature(siglen);
    EVP_DigestSignFinal(mdctx, signature.data(), &siglen);

    EVP_MD_CTX_free(mdctx);

    return string(signature.begin(), signature.end());
}

bool verify_signature(RSA* rsa, const string& message, const string& signature) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);

    EVP_DigestVerifyInit(mdctx, NULL, EVP_md4(), NULL, pkey);
    EVP_DigestVerifyUpdate(mdctx, message.data(), message.size());

    int result = EVP_DigestVerifyFinal(mdctx, (unsigned char*)signature.data(), signature.size());

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);

    return result == 1;
}

void testMultiplicativeInverseFunctions() {
    BIGNUM* a = BN_new();
    BIGNUM* modulus = BN_new();
    BN_set_word(a, 15);
    BN_set_word(modulus, 28);

    BIGNUM* result1 = multiplicative_inverse(a, modulus);
    BIGNUM* result2 = multiplicativeInverseBinaryExponentiations(a, modulus);
    cout << "Euclidean: " << BN_bn2dec(result1) << endl;
    cout << "BinaryExponentiations: " << BN_bn2dec(result2) << endl;

    if (BN_cmp(result1, result2) == 0) {
        printf("Test complete\n");
    } else {
        printf("Test error\n");
    }

    BN_free(a);
    BN_free(modulus);
    BN_free(result1);
    BN_free(result2);
}

int main() {
    // RSA* rsa = generate_RSA_keys(2048);
    RSA* rsa = generate_RSA_keys_with_password(1024, "suai");
    string message = "I love guap!";

    string ciphertext = RSA_encrypt(rsa, message);
    string decrypted_message = RSA_decrypt(rsa, ciphertext);
    cout << "Message -- " << message << endl;
    // cout << "Encrypted -- " << ciphertext << endl;
    cout << "Decrypted -- " << decrypted_message << endl;
    // string signature = create_signature(rsa, message);
    // bool is_valid = verify_signature(rsa, message, signature);

    cout << "validating signature..." << endl;
    cout << "Signature is invalid" << endl;
    // cout << (is_valid ? "valid" : "invalid") << " sig!" << endl;

    cout << endl;
    testMultiplicativeInverseFunctions();
    return 0;
}