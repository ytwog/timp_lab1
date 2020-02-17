//
// Created by y2g on 04.02.2020.
//

#include "text.h"

namespace mLab {

    /// Функции

    int from_str_to_int(std::string _s) {
        int res = 0;
        if(_s.length() > 9) return -2; // Big string
        for(int i = 0; i < _s.length(); i++) {
            if(_s[i] >= '0' && _s[i] <= '9')
                res += (_s[i] - '0') * pow(10, _s.length() - 1 - i);
            else
                return -1; // Wrong symbol
        }
        return res;
    }

    int print_err(int error_code) {
        std::string out_str = "Error has occured: ";
        switch(error_code) {
            case 0:
                return 0;
            case 1:
                out_str += "1 - waited \">text\", got other str";
                break;
            case 2:
                out_str += "2 - waited \">replace\", got other str";
                break;
            case 3:
                out_str += "3 - waited \">with\", got other str";
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
            case 7:
                out_str += "7 - input/output file can't be opened";
                break;
            case 8:
                out_str += "8 - incorrect command line! \nUsage: lab.exe [input.txt] [output.txt]";
                break;
            case 9:
                out_str += "9 - waited !<command>, got other str";
                break;
            case 10:
                out_str += "10 - waited \">shift\", got other str";
                break;
            case 11:
                out_str += "11 - got wrong symbol in shift-integer";
                break;
            case 12:
                out_str += "12 - shift-integer is very large";
                break;
            default:
                out_str += "unexpected error.\n";
        }
        std::cout << out_str << std::endl;
        system("pause");
        return error_code;
    }


    void cipher(text*_t) {
        std::string *res = new std::string;
        if (_t->type == txt_type::REPLACEMENT) {
            *res = *_t->r.open_txt;
            for (int i = 0; i < _t->r.alphabet_length; i++) {
                int temp = 0;
                int pos = 0;
                while (temp != -1 && pos < _t->r.open_txt->length() && !_t->r.open_txt->substr(pos).empty()) {
                    temp = _t->r.open_txt->substr(pos).find(_t->r.mapping[i].first);
                    if (temp == -1) break;
                    char q = _t->r.mapping[i].first;
                    char w = _t->r.mapping[i].second;

                    (*res)[temp + pos] = _t->r.mapping[i].second;
                    pos += temp + 1;
                }
            }
            _t->r.cipher_txt = res;
        } else {
            *res = *_t->c.open_txt;
            int _shift = _t->c.shift % 26;
            for(int i = 0; i < res->length(); i++) {
                bool upper_case = ((*res)[i] >= 'A' && (*res)[i] <= 'Z');
                if(((*res)[i] >= 'a' && (*res)[i] <= 'z') || ((*res)[i] >= 'A' && (*res)[i] <= 'Z')) {
                    (*res)[i] += _t->c.shift;
                    if((((*res)[i] < 'a' || (*res)[i] > 'z') && !upper_case)
                       || (((*res)[i] < 'A' || (*res)[i] > 'Z') && upper_case))
                        (*res)[i] -= 26;
                }
            }
            _t->c.cipher_txt = res;
        }
    }

    int read(std::ifstream *_ifstr, text *_t) {
        char *s = new char[255];
        int error_code = 0;
        std::string str;
        std::string _open_text;
        int step = 0;
        if(_t->type == txt_type::REPLACEMENT) {
            char *m_first = nullptr;
            std::pair <char, char> *mapping = nullptr;
            while(!_ifstr->eof() && error_code == 0 && step < 6) {
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
                        _t->r.alphabet_length = str.length();
                        m_first = new char[_t->r.alphabet_length];
                        for (int i = 0; i < _t->r.alphabet_length; i++) {
                            m_first[i] = str[i];
                        }
                        step++;
                        break;
                    case 4:
                        if (str == ">with") step++;
                        else error_code = 3;
                        break;
                    case 5:
                        if (str.length() != _t->r.alphabet_length) {
                            error_code = 4;
                            break;
                        };
                        mapping = new std::pair<char, char>[_t->r.alphabet_length];
                        for (int i = 0; i < _t->r.alphabet_length; i++) {
                            mapping[i].first = m_first[i];
                            mapping[i].second = str[i];
                        }
                        delete[] m_first;
                        m_first = NULL;
                        _t->r.open_txt = new std::string;
                        _t->r.open_txt->append(_open_text);
                        _t->r.mapping = mapping;
                        step++;
                        break;
                    default:
                        break;
                }
            }
            if(m_first) {
                delete[] m_first;
            }
            if(s) delete[] s;
            if(error_code && mapping) delete[] mapping;
        } else { // При type == CYCLE
            int _shift = 0;
            while(!_ifstr->eof() && error_code == 0 && step < 3) {
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
                        if (str.substr(0, 7) != ">shift ") error_code = 10;
                        _shift = from_str_to_int(str.substr(7));
                        if(_shift == -1) return 11;
                        if(_shift == -2) return 12;
                        _t->c.open_txt = new std::string;
                        _t->c.open_txt->append(_open_text);
                        _t->c.shift = _shift;
                        step++;
                        break;
                    default:
                        break;
                }
            }
            if(s) delete[] s;
        }
        return error_code;
    }

    void Init(text*_t) {
        if(_t->type == txt_type::REPLACEMENT) {
            _t->r.alphabet_length = 0;
            _t->r.mapping = nullptr;
            _t->r.cipher_txt = nullptr;
            _t->r.open_txt = nullptr;
        } else {
            _t->c.shift = 0;
            _t->c.cipher_txt = nullptr;
            _t->c.open_txt = nullptr;
        }
    }

    /// Методы txt_cycle

    std::string info_string(text *_t) {
        if(_t->type == txt_type::REPLACEMENT) {
            std::string res = "Cipher type: symbol replacement\n";
            res += "Open_text:\n";
            res += *_t->r.open_txt;
            res += "\nReplace:\n";
            std::string temp = "";
            for(int i = 0; i < _t->r.alphabet_length; i++) {
                res += _t->r.mapping[i].first;
                temp += _t->r.mapping[i].second;
            }
            res += "\nWith:\n";
            res += temp;
            res += "\nCipher text:\n";
            std::string ciph;
            if(!_t->r.cipher_txt)
                cipher(_t);
            ciph = *_t->r.cipher_txt;
            res += ciph;
            res += "\n";
            return res;
        } else {
            std::string res = "Cipher type: symbol cycle\n";
            res += "Open_text:\n";
            res += *_t->c.open_txt;
            res += "\nShift: ";
            int _shift = _t->c.shift;
            char *temp_shift = new char[16];
            _itoa_s(_shift, temp_shift, 10, 10);
            res += temp_shift;
            res += "\nCipher text:\n";
            std::string ciph;
            if(!_t->c.cipher_txt)
                cipher(_t);
            ciph = *_t->c.cipher_txt;
            res += ciph;
            res += "\n";
            return res;
        }
    }


    /// Returns error_code:
    /// 0 - no error has occurred
    /// 1 - waited ">text", got other str
    /// 2 - waited ">replace", got other str
    /// 3 - waited ">with", got other str
    /// 4 - different string-"replace" and string-"with" size
    /// 5 - waited ">type", got other str
    /// 6 - type should by either 1 or 2
    /// 9 - waited !<command>, got other str
    int read_from_file(std::ifstream *_ifstr, _mContainer *_c) {
        char *s = new char[255];
        std::string str;
        int operation = -1; // ADD = 0
        int type = 0;
        std::string open_text;
        int map_length = 0;
        int error_code = 0;
        std::pair<char, char> *mapping = nullptr;
        while (!_ifstr->eof() && error_code == 0) {
            _ifstr->getline(s, 255);
            str = s;
            if (s[0] == '/' && s[1] == '/') continue;
            if (str == "!ADD") {
                operation = 0;
                continue;
            }
            if (operation == 0) {
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
                text *txt = new text;
                txt->type = (txt_type) type;
                Init(txt);
                txt->next = NULL;

                error_code = read(_ifstr, txt);
                if (!error_code) append(txt, _c);
                else delete txt;
                operation = -1;
            } else {
                error_code = 9;
            }
        }
        delete[] s;
        return error_code;
    }

    void write_to_file(std::ofstream *_ofstr, _mContainer*_c) {
        std::string out_str = "";
        if(_c->start) {
            for (text *i = _c->start; ; i = i->next) {
                out_str += info_string(i);
                out_str += "----------------\n";
                if(i == _c->end) break;
            }
        } else {
            out_str += "Empty container\n";
        }
        _ofstr->write(out_str.c_str(), out_str.length());
    }

    text *text_at(int pos, _mContainer *_c) {
        text *res = _c->start;
        for(int i = 0; i < pos; i++) {
            res = res->next;
        }
        return res;
    }

    bool remove(text *_node, _mContainer*_c) {
        text *prev = _c->start;
        for(text *i = _c->start; i != _c->end; i = i->next) {
            if(i == NULL) return false;
            if(i == _node) {
                prev->next = i->next;
                if(i->type == txt_type::REPLACEMENT) {
                    if(i->r.mapping) delete[] i->r.mapping;
                }
                delete i;
                return true;
            }
            prev = i;
        }
        return false;
    }

    void append(text *_node, _mContainer*_c) {
        if(_c->start) {
            _c->end->next = _node;
            _node->next = _c->start;
            _c->end = _node;

        } else {
            _node->next = _node;
            _c->start = _c->end = _node;
        }
    }

    void mLab::Init(_mContainer *_c) {
        _c->end = _c->start = NULL;
    }

}