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
        CYCLE = 2
    };

    // Вывод ошибки по коду error_code
    int print_err(int error_code);
    // Класс текста с заменами
    class txt_replacement{
    public:
        ~txt_replacement();
        void cipher();
        int read(std::ifstream*);
    private:
        std::pair<char, char> *mapping;
        std::string *cipher_txt;
        std::string *open_txt;
    };

    // Класс текста со сдвигом
    class txt_cycle{
    public:
        void cipher();
        int read(std::ifstream*);
    private:
        int shift;
        std::string *cipher_txt;
        std::string *open_txt;
    };

    // Класс для объединения текстов
    class text {
    public:
        ~text();
        text(int _type) {
            type = (txt_type)_type;
            next = NULL;
        }

        void set_next(text *_next) {
            next = _next;
        }

        text *get_next() {
            return next;
        }

        union {
            txt_replacement r;
            txt_cycle c;
        };
    protected:
        txt_type type;
        text *next;
    };

    // Контейнер - однонаправленный цикличный список
    class _mContainer {
    public:
        _mContainer() {
            end = start = NULL;
        }

        void write_to_file(std::ofstream *_ofstr) {
            std::string out_str = "test";
            _ofstr->write(out_str.c_str(), out_str.length());
        }

        /// Returns error_code:
        /// 0 - no error has occurred
        /// 1 - waited ">text", got other str
        /// 2 - waited ">replace", got other str
        /// 3 - waited ">wait", got other str
        /// 4 - different string-"replace" and string-"with" size
        /// 5 - waited ">type", got other str
        /// 6 - type should by either 1 or 2
        int read_from_file(std::ifstream *_ifstr) {
            char *s = new char[255];
            std::string str;
            int operation = -1; // ADD = 0
            int type = 0;
            std::string open_text;
            int map_length;
            int error_code = 0;
            char *m_first;
            std::pair <char, char> *mapping;
            while(!_ifstr->eof() && error_code == 0) {
                _ifstr->getline(s, 255);
                str = s;
                if(s[0] == '/' && s[1] == '/') continue;
                if(str == "!ADD") {
                    operation = 0;
                    continue;
                }
                if(operation == 0) {
                    if (str.substr(0, 6) == ">type ") {
                        if (str[6] == '1') type = txt_type::REPLACEMENT;
                        else if (str[6] == '2') type = txt_type::CYCLE;
                        else {
                            error_code = 6;
                            break;
                        }
                    } else {
                        error_code = 5;
                        break;
                    }
                    text *txt = new text(type);
                    if(type == txt_type::REPLACEMENT)
                        error_code = txt->r.read(_ifstr);
                    if(type == txt_type::CYCLE)
                        error_code = txt->c.read(_ifstr);
                    if(!error_code) append(txt);
                    else delete txt;
                }
            }
            delete[] s;
            return error_code;
        }

        text *text_at(int pos) {
            text *res = start;
            for(int i = 0; i < pos; i++) {
                res = res->get_next();
            }
            return res;
        }

        bool remove(text *_node) {
            text *prev = start;
            for(text *i = start; i != end; i = i->get_next()) {
                if(i == NULL) return false;
                if(i == _node) {
                    prev->set_next(i->get_next());
                    delete i;
                    return true;
                }
                prev = i;
            }
            return false;
        }

        void append(text *_node) {
            if(start) {
                end->set_next(_node);
                _node->set_next(start);
                end = _node;

            } else {
                _node->set_next(_node);
                start = end = _node;
            }
        }

    private:
        text *start;
        text *end;
    };

}
#endif //LAB1_TEXT_H
