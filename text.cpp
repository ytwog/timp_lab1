//
// Created by y2g on 04.02.2020.
//

#include "text.h"

namespace mLab {

    // Вывод ошибки по коду error_code
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
            default:
                out_str += "unexpected error.\n";
        }
        std::cout << out_str << std::endl;
        system("pause");
        return error_code;
    }

    void txt_replacement::cipher() {
        std::string *res = new std::string;
        *res = *get_open_txt();
        for(int i = 0; i < alphabet_length; i++) {
            int temp = 0;
            int pos = 0;
            while(temp != -1 && pos < open_txt->length() && !open_txt->substr(pos).empty()) {
                temp = open_txt->substr(pos).find(mapping[i].first);
                if(temp == -1) break;
                char q = mapping[i].first;
                char w = mapping[i].second;

                (*res)[temp + pos] = mapping[i].second;
                pos += temp+1;
            }
        }
        cipher_txt = res;
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
        int step = 0;
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
                    alphabet_length = str.length();
                    m_first = new char[alphabet_length];
                    for (int i = 0; i < alphabet_length; i++) {
                        m_first[i] = str[i];
                    }
                    step++;
                    break;
                case 4:
                    if (str == ">with") step++;
                    else error_code = 3;
                    break;
                case 5:
                    if (str.length() != alphabet_length) {
                        error_code = 4;
                        break;
                    };
                    mapping = new std::pair<char, char>[alphabet_length];
                    for (int i = 0; i < alphabet_length; i++) {
                        mapping[i].first = m_first[i];
                        mapping[i].second = str[i];
                    }
                    delete[] m_first;
                    m_first = NULL;
                    open_txt = new std::string;
                    open_txt->append(_open_text);
                    this->mapping = mapping;
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
        return error_code;
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
    int _mContainer::read_from_file(std::ifstream *_ifstr) {
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
                text *txt = new text((txt_type) type);
                if (type == txt_type::REPLACEMENT)
                    error_code = txt->r.read(_ifstr);
                if (type == txt_type::CYCLE)
                    error_code = txt->c.read(_ifstr);
                if (!error_code) append(txt);
                else delete txt;
                operation = -1;
            } else {
                error_code = 9;
            }
        }
        delete[] s;
        return error_code;
    }

    _mContainer::_mContainer() {
        end = start = NULL;
    }

    void _mContainer::write_to_file(std::ofstream *_ofstr) {
        std::string out_str = "";
        if(start) {
            for (text *i = start; ; i = i->get_next()) {
                if(i->get_type() == txt_type::REPLACEMENT) {
                    out_str += i->r.info_string();
                } else {
                    out_str += "Cipher type: cycle shift\n";
                }
                out_str += "----------------\n";
                if(i == end) break;
            }
        } else {
            out_str += "Empty container\n";
        }
        _ofstr->write(out_str.c_str(), out_str.length());
    }

    text *_mContainer::text_at(int pos) {
        text *res = start;
        for(int i = 0; i < pos; i++) {
            res = res->get_next();
        }
        return res;
    }

    bool _mContainer::remove(text *_node) {
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

    void _mContainer::append(text *_node) {
        if(start) {
            end->set_next(_node);
            _node->set_next(start);
            end = _node;

        } else {
            _node->set_next(_node);
            start = end = _node;
        }
    }

    void txt_replacement::Init() {
        alphabet_length = 0;
        mapping = nullptr;
        cipher_txt = nullptr;
        open_txt = nullptr;
    }

    std::string txt_replacement::info_string() {
        std::string res = "Cipher type: symbol replacement\n";
        res += "Open_text:\n";
        res += *get_open_txt();
        res += "\nReplace:\n";
        std::string temp = "";
        for(int i = 0; i < alphabet_length; i++) {
            res += mapping[i].first;
            temp += mapping[i].second;
        }
        res += "\nWith:\n";
        res += temp;
        res += "\nCipher text:\n";
        res += *get_cipher_txt();
        res += "\n";
        return res;
    }

    std::pair<char, char> *txt_replacement::get_mapping() {return mapping;}

    std::string *txt_replacement::get_cipher_txt() {
        if(cipher_txt == nullptr) cipher();
        return cipher_txt;
    }

    std::string *txt_replacement::get_open_txt() {
        return open_txt;
    }
}