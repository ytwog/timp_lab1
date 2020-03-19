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

    // Перечисление типов текста
    enum txt_type {
        REPLACEMENT = 1,
        CYCLE = 2,
        DIGIT_REPL = 3
    };


    class node {
    public:
        void set_next(node *_next);
        node *get_next();
    protected:
        node *next = nullptr;
    };


    // Контейнер - однонаправленный цикличный список
    class _mContainer {
    public:
        _mContainer();
        node *text_at(int pos);
        bool remove(node *_node);
        void append(node *_node);
        node *end();
        node *start();
    private:
        node *_start;
        node *_end;
    };

    // Класс для объединения текстов
    class text : public node{
    public:
        static int read_from_file(std::ifstream *_ifstr, _mContainer*cont);
        static void write_to_file(std::ofstream *_ifstr, _mContainer*cont);
        virtual void cipher()=0;
        virtual int read(std::ifstream*)=0;
        virtual std::string info_string()=0;
        txt_type get_type() {return type;}
        void set_type(int _type);

    protected:
        txt_type type;
    };


    // Вывод ошибки по коду error_code
    int print_err(int error_code);
    // Класс текста с заменами
    class txt_replacement : public text{
    public:
        void cipher() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_replacement();
        std::pair<char,char> *get_mapping();
        std::string *get_cipher_txt();
        std::string *get_open_txt();

    private:
        int alphabet_length;
        std::pair<char, char> *mapping;
        std::string *cipher_txt;
        std::string *open_txt;
        std::string *owner_info;
    };

    // Класс текста со сдвигом
    class txt_cycle : public text{
    public:
        void cipher() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_cycle();
        std::string *get_cipher_txt();
        std::string *get_open_txt();

    private:
        int shift;
        std::string *cipher_txt;
        std::string *open_txt;
        std::string *owner_info;
    };

    class txt_digit_repl : public text{
    public:
        void cipher() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_digit_repl();
        int *get_cipher_txt();
        std::string *get_open_txt();
    private:
        int alphabet_length;
        std::pair<char, int> *mapping;
        int *cipher_txt;
        std::string *open_txt;
        std::string *owner_info;
    };
}
#endif //LAB1_TEXT_H
