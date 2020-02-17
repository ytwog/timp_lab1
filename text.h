//
// Created by y2g on 04.02.2020.
//

#ifndef LAB1_TEXT_H
#define LAB1_TEXT_H
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <Windows.h>

namespace mLab {
    /// Струтуры
    struct txt_replacement;
    struct txt_cycle;
    struct text;
    struct _mContainer;

    /// Функции
    // Вывод ошибки по коду error_code
    int print_err(int error_code);
    // Класс текста с заменами

    // Шифрует открытый текст в шифротекст
    void cipher(text*);

    // Возвращает строку, содержащую информацию про текст и шифр
    std::string info_string(text*);

    // Считывает с потока строки и преобразовывает их в поля класса
    int read(std::ifstream*, text*);

    void Init(text*);
    void Init(_mContainer*);

    void write_to_file(std::ofstream *_ofstr, _mContainer *);
    int read_from_file(std::ifstream *_ifstr, _mContainer *);
    text *text_at(int pos, _mContainer *);
    bool remove(text *_node, _mContainer*);
    void append(text *_node, _mContainer*);
    /// Перечисление типов текста
    enum txt_type {
        REPLACEMENT = 1,
        CYCLE = 2
    };


    struct txt_replacement {
        int alphabet_length;
        std::pair<char, char> *mapping;
        std::string *cipher_txt;
        std::string *open_txt;
    };

    // Класс текста со сдвигом
    struct txt_cycle{
        int shift;
        std::string *cipher_txt;
        std::string *open_txt;
    };

    // Класс для объединения текстов
    struct text {
        union {
            txt_replacement r;
            txt_cycle c;
        };
        txt_type type;
        text *next;
    };

    // Контейнер - однонаправленный цикличный список
    struct _mContainer {
        text *start;
        text *end;
    };

}
#endif //LAB1_TEXT_H
