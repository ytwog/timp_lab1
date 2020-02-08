//
// Created by y2g on 04.02.2020.
//

#ifndef LAB1_TEXT_H
#define LAB1_TEXT_H
#include <string>
#include <iostream>
#include <fstream>
#include <locale>

namespace mLab {


    class text {
    public:
        virtual void cipher() = 0;
        virtual int read(std::ifstream*);
        text() {
            next = NULL;
        }

        void set_next(text *_next) {
            next = _next;
        }

        text *get_next() {
            return next;
        }

    protected:
        text *next;
        std::string open_txt;
    };


    class txt_replacement : text {
    public:
        void cipher();
        int read(std::ifstream*);
    private:
        std::pair<char, char> *mapping;
        std::string cipher_txt;
    };

    class txt_cycle : text {
    public:
        void cipher();
        int read(std::ifstream*);
    private:
        int shift;
        std::string cipher_txt;
    };

    class _mContainer {
    public:
        _mContainer() {
            end = start = NULL;
        }


        /// Returns error_code:
        /// 0 - no error has occurred
        /// 1 - waited ">text", got other str
        /// 2 - waited ">replace", got other str
        /// 3 - waited ">wait", got other str
        /// 4 - different string-"replace" and string-"with" size
        /// 5 - reserved
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
                }
                if(operation == 0) {
                    if (str.substr(0, 6) == ">type ") {
                        if (str[7] == '1') type = 1;
                        else if (str[7] == '2') type = 2;
                        else {
                            error_code = 6;
                            break;
                        }
                    }
                    text *txt = (text*)new txt_replacement;
                    error_code = txt->read(_ifstr);
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
