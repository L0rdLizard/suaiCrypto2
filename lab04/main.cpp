#include "RIPEMD-160.h"

uint16_t firstBits(string hash, unsigned int n) {
    string hashTail = hash.substr(hash.length() - 4);

    unsigned long long hashValue = std::stoull(hashTail, nullptr, 16);
    uint16_t firstTenBits = hashValue & n;

    return firstTenBits;
}

std::string generateRandomMessage() {
    int length = std::rand() % 40 + 1;

    std::string message;
    for (int i = 0; i < length; ++i) {
        char randomChar = static_cast<char>(std::rand() % 95 + 32);

        message.push_back(randomChar);
    }

    return message;
}

size_t countSteps(std::vector<uint16_t> y) {
    size_t steps = 0;

    for (int i = 0; i < y.size(); i++) {
        for (int j = i + 1; j < y.size(); j++) {
            steps++;
            if (y[i] == y[j]) {
                return steps;
            }
        }
    }

    return steps;
}

void writeArrayToFile(const int (&arr)[5][3][1000], std::string fileName1, std::string fileName2,
                      std::string fileName3) {
    std::ofstream file1("data\\" + fileName1);
    std::ofstream file2("data\\" + fileName2);
    std::ofstream file3("data\\" + fileName3);

    if (!file1.is_open() || !file2.is_open() || !file3.is_open()) {
        std::cerr << "Open error" << std::endl;
        return;
    }

    for (int i = 0; i < 5; ++i) {
        for (int k = 0; k < 1000; ++k) {
            file1 << arr[i][0][k] << " ";
            file2 << arr[i][1][k] << " ";
            file3 << arr[i][2][k] << " ";
        }
        file1 << "\n";
        file2 << "\n";
        file3 << "\n";
    }

    file1.close();
    file2.close();
    file3.close();

    std::cout << "Succes!" << std::endl;
}

int main(int argc, char** argv) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // if (argc != 2){
    //     cout << endl << "Usage: ./RIPEMD-160 file_in" << endl << endl;
    //     return -1;
    // }
    // else
    // {
    //     RIPEMD_160 hash;

    //     if (!hash.read_file(argv[1]))
    //         cout << "Read error!" << endl << endl;
    //     else if (!hash.write_file((char*)"RIPEMD-160sum.txt", hash.ripemd_160()))
    //         cout << "Write error!" << endl << endl;

    // }
    // string message = "The quick brown fox jumps over the lazy dog";
    // RIPEMD_160 hash;
    // hash.ripemd_160("The quick brown fox jumps over the lazy dog");

    // unsigned int masks[] = {0xFF, 0x3FF, 0xFFF, 0x3FFF, 0xFFFF};
    unsigned int masks[] = {0x0F, 0x3F, 0xFF, 0x3FF, 0xFFF};

    string message1 = "Love";
    string message2 = "borzoi";
    string message3 = "\x00\x00\x00\x00\x00";
    RIPEMD_160 hash1;
    std::string result1 = hash1.ripemd_160(message1);

    RIPEMD_160 hash2;
    std::string result2 = hash2.ripemd_160(message2);

    RIPEMD_160 hash3;
    std::string result3 = hash3.ripemd_160(message3);

    std::string massResult[] = {result1, result2, result3};

    int countN[5][3][1000];
    int countS[5][3][1000];
    // std::vector<uint16_t> y = {};

    // 5 bit length
    for (int i = 0; i < 5; i++) {
        // 3 key words
        for (int j = 0; j < 3; j++) {
            std::cout << "i = " << i << "; j = " << j << std::endl;
            uint16_t firstB = firstBits(massResult[j], masks[i]);

            // 1000 exp
            for (int k = 0; k < 1000; k++) {
                int nCount = 0;
                std::vector<uint16_t> y = {};

                while (true) {
                    nCount++;
                    RIPEMD_160 rHash;
                    
                    std::string rStr = generateRandomMessage();
                    std::string rResult = rHash.ripemd_160(rStr);

                    uint16_t rFirstB = firstBits(rResult, masks[i]);

                    y.push_back(rFirstB);
                    if (rFirstB == firstB) {
                        countN[i][j][k] = nCount;
                        // std::cout << nCount << std::endl;
                        break;
                    }
                }
                countS[i][j][k] = countSteps(y);
            }
        }
    }

    writeArrayToFile(countN, "countN1.txt", "countN2.txt", "countN3.txt");
    writeArrayToFile(countS, "countS1.txt", "countS2.txt", "countS3.txt");

    // for (int i = 0; i < 1000; i++)
    // {
    //     std::cout << countN[i] << "  ";
    // }

    return 0;
}