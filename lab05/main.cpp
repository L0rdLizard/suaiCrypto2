#include <iostream>
#include <openssl/bn.h>
#include "RSAfun.cpp"

int main() {
    RSAfun rsa;
    std::cout << "123";
    // Генерация ключей
    rsa.generate_keys(1024);
    
    // Шифрование и расшифровка сообщения
    BIGNUM *message = BN_new();
    
    BN_set_word(message, 123456789);  // Пример сообщения

    BIGNUM *encrypted = rsa.encrypt(message);
    BIGNUM *decrypted = rsa.decrypt(encrypted);

    // Создание и проверка подписи
    BIGNUM *signature = rsa.sign(message);
    bool isValid = rsa.verify(message, signature);

    // Вывод результатов
    std::cout << "Original message: " << BN_bn2dec(message) << std::endl;
    std::cout << "Encrypted message: " << BN_bn2dec(encrypted) << std::endl;
    std::cout << "Decrypted message: " << BN_bn2dec(decrypted) << std::endl;
    std::cout << "Signature is " << (isValid ? "valid" : "invalid") << std::endl;

    // Освобождение памяти
    BN_free(message);
    BN_free(encrypted);
    BN_free(decrypted);
    BN_free(signature);

    return 0;
}
