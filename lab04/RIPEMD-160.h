#include <cstdint>
#include <ctime>
#include <fstream>   //Для файлов
#include <iostream>  //Для cout
#include <sstream>   //Для ostringstream и функции hex
#include <string>    //Для string
#include <unordered_map>
#include <vector>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))  // Битовый сдвиг на n бит

using namespace std;  // Для cout, hex и файлов

class RIPEMD_160  // Класс
{
    string message;  // Исходное сообщение

    unsigned long long bitlen;  // Исходная длина сообщения (до шага 1) в битах

    unsigned int** X;  // Массивы блоков по 64 байта(512 бит)

    unsigned int blocks;  // Количество блоков для обработки

    unsigned int H0, H1, H2, H3, H4;  // Регистры RIPEMD буфера

    unsigned int A1, B1, C1, D1, E1, A2, B2, C2, D2, E2,
        T;  // Переменные для сохранения значений регистров во время алгоритма

    unsigned int F(unsigned int j, unsigned int x, unsigned int y,
                   unsigned int z);  // Выбор битовой функции в соответсвии с номером цикла
    unsigned int K1(
        unsigned int j);  // Выбор константы в соответсвии с номером цикла для первого потока
    unsigned int K2(
        unsigned int j);  // Выбор константы в соответсвии с номером цикла для второго потока
    unsigned int inv(unsigned int value);  // Смена порядка бит (endians)
    unsigned int bytes_to_uint(char* bytes);  // Преоразование 4х байтов в unsigned int

    void extension();           // Шаг 1 - Расширение сообщения
    void adding_length();       // Шаг 2 - Добавление длины сообщения
    void initialize_ripemd();   // Шаг 3 - Инициализация RIPEMD буфера
    void message_processing();  // Шаг 4 - Обработка сообщения в блоках
    ostringstream result;  // Шаг 5 - Результат в виде хэш-сообщения
   public:
    bool read_file(char* fileName);               // Чтение из файла
    bool write_file(char* fileName, string str);  // Запись в файл

    string ripemd_160(string mess);  // Алгоритм преобразования
};