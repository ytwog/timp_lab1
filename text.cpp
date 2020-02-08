//
// Created by y2g on 04.02.2020.
//

#include "text.h"

namespace mLab {

    // Вывод ошибки по коду error_code
    int print_err(int error_code) {
        std::string out_str = "Error has occured: ";
        switch(error_code) {
            case 1:
                out_str += "1 - waited \">text\", got other str";
                break;
            case 2:
                out_str += "2 - waited \">replace\", got other str";
                break;
            case 3:
                out_str += "3 - waited \">wait\", got other str";
                break;
            case 4:
                out_str += R"(4 - different string-"replace" and string-"with" size)";
                break;
            case 5:
                out_str += "5 - waited \">type\", got other str";
                break;
            case 6:
                out_str += "6 - type should by either 1 or 2";
                break;
            default:
                out_str += "unexpected error.\n";
        }
        std::cout << out_str << std::endl;
        int WHAT;
        std::cin >> WHAT;
        system("pause");
        return error_code;
    }

    void txt_replacement::cipher() {

    }

    text::~text() {

    }

    int txt_cycle::read(std::ifstream *) {
        return 0;
    }

    txt_replacement::~txt_replacement() {
        if(mapping) delete[] mapping;
    }

    int txt_replacement::read(std::ifstream *_ifstr) {
        char *s = new char[255];
        int error_code = 0;
        std::string str;
        std::string _open_text;
        int map_length;
        int step = 0;
        char *m_first;
        std::pair <char, char> *mapping;
        while(!_ifstr->eof() && error_code == 0) {
            _ifstr->getline(s, 255);
            str = s;
            if (s[0] == '/' && s[1] == '/') continue;
            switch (step) {
                case 0:
                    if (str == ">text") step++;
                    else error_code = 1;
                    break;
                case 1:
                    _open_text = str;
                    step++;
                    break;
                case 2:
                    if (str == ">replace") step++;
                    else error_code = 2;
                    break;
                case 3:
                    map_length = str.length();
                    m_first = new char[map_length];
                    for (int i = 0; i < map_length; i++) {
                        m_first[i] = str[i];
                    }
                    step++;
                    break;
                case 4:
                    if (str == ">with") step++;
                    else error_code = 3;
                    break;
                case 5:
                    if (str.length() != map_length) {
                        error_code = 4;
                        break;
                    };
                    mapping = new std::pair<char, char>;
                    for (int i = 0; i < map_length; i++) {
                        mapping->first = m_first[i];
                        mapping->second = str[i];
                    }
                    delete[] m_first;
                    open_txt = new std::string(_open_text);
                    this->mapping = mapping;
                    break;
                default:
                    break;
            }
        }
        if(m_first) {
            delete[] m_first;
            m_first = NULL;
        }
        if(s) delete[] s;
        if(error_code && mapping) delete[] mapping;
        return error_code;
    }

}