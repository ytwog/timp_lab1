//
// Created by y2g on 04.02.2020.
//

// Русский текст
#include "text.h"

namespace mLab {

    void txt_replacement::cipher() {

    }

    int txt_replacement::read(std::ifstream *_ifstr) {
        char *s = new char[255];
        int error_code = 0;
        std::string str;
        std::string open_text;
        int map_length;
        int step;
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
                    open_text = str;
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
                    this->open_txt = open_text;
                    this->mapping = mapping;
                    break;
                default:
                    break;
            }
        }
        if(m_first) delete[] m_first;
        if(s) delete[] s;
        if(error_code && mapping) delete[] mapping;
        return error_code;
    }
}