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

    int from_str_to_int(std::string _s);
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
        void multi_method(std::ofstream &ofst);
        node *text_at(int pos);
        bool remove(node *_node);
        void append(node *_node);
        void set_end(node* n);
        void set_start(node* n);
        node *end();
        node *start();
        void sort();
        int ignore = -1;
    private:
        node *_start;
        node *_end;
    };

    // Класс для объединения текстов
    class text : public node{
    public:
        static int read_from_file(std::ifstream *_ifstr, _mContainer*cont);
        static void write_to_file(std::ofstream *_ofstr, _mContainer*cont, int ignore_type = -1);
        virtual void cipher()=0;
        virtual int counter_function()=0;
        virtual int read(std::ifstream*)=0;
        virtual std::string info_string()=0;
        virtual void multi_method(text *other, std::ofstream &ofst) = 0;
        virtual void multi_replacement(std::ofstream &ofst) = 0;
        virtual void multi_digit_repl(std::ofstream &ofst) = 0;
        virtual void multi_cycle(std::ofstream &ofst) = 0;
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
        int counter_function() override;
        void cipher() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_replacement();
        std::pair<char,char> *get_mapping();
        std::string *get_cipher_txt();
        std::string *get_open_txt();
        void multi_method(text *other, std::ofstream &ofst) override;
        void multi_replacement(std::ofstream &ofst) override;
        void multi_digit_repl(std::ofstream &ofst) override;
        void multi_cycle(std::ofstream &ofst) override;
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
        int counter_function() override;
        void cipher() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_cycle();
        std::string *get_cipher_txt();
        std::string *get_open_txt();
        void multi_method(text *other, std::ofstream &ofst) override;
        void multi_replacement(std::ofstream &ofst) override;
        void multi_digit_repl(std::ofstream &ofst) override;
        void multi_cycle(std::ofstream &ofst) override;

    private:
        int shift;
        std::string *cipher_txt;
        std::string *open_txt;
        std::string *owner_info;
    };

    class txt_digit_repl : public text{
    public:
        void cipher() override;
        int counter_function() override;
        int read(std::ifstream*) override;
        std::string info_string() override;
        txt_digit_repl();
        int *get_cipher_txt();
        std::string *get_open_txt();
        void multi_method(text *other, std::ofstream &ofst) override;
        void multi_replacement(std::ofstream &ofst) override;
        void multi_digit_repl(std::ofstream &ofst) override;
        void multi_cycle(std::ofstream &ofst) override;

    private:
        int alphabet_length;
        std::pair<char, int> *mapping;
        int *cipher_txt;
        std::string *open_txt;
        std::string *owner_info;
    };
}
#endif //LAB1_TEXT_H
