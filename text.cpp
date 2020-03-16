//
// Created by y2g on 04.02.2020.
//

#include "text.h"

namespace mLab {

    /// Функции

    std::string int_to_str(int ask) {
        std::string res = "";
        if(ask == 0) return "0";
        while(ask) {
            res = char(ask % 10 + 48) + res;
            ask /= 10;
        }
        return res;
    }

    bool comparat(text *_f, text *_s) {
        int l1 = _f->counter_function();
        int l2 = _s->counter_function();
        return l1 > l2;
    }

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

    void sort(_mContainer *cont) {
        int _size = 0;
        if((text*)cont->start() == nullptr) return;
        text *prev_i = nullptr;
        for(text* i = (text*)cont->start(); i != (text*)cont->end(); i = (text*)i->get_next()) {
            text *prev_j = nullptr;
            for(text* j = (text*)i->get_next(); j != (text*)cont->start(); j = (text*)j->get_next()) {
                if(comparat(i, j)) {
                    // Обновляем start
                    if(cont->start() == i) {
                        cont->set_start(j);
                    } else if(cont->start() == j) {
                        cont->set_start(i);
                    }
                    // Обновляем end
                    if(cont->end() == i) {
                        cont->set_end(j);
                    } else if(cont->end() == j) {
                        cont->set_end(i);
                    }
                    if(prev_i)
                        prev_i->set_next(j);
                    if(prev_j)
                        prev_j->set_next(i);
                    node*z1 = i->get_next();
                    i->set_next(j->get_next());
                    j->set_next(z1);
                    auto z2 = i;
                    i = j;
                    j = z2;
                }
                prev_j = j;
            }
            prev_i = i;
        }
    }

    /// Методы text

    txt_replacement::txt_replacement() {
        alphabet_length = 0;
        mapping = nullptr;
        cipher_txt = nullptr;
        open_txt = nullptr;
    }

    void node::set_next(node *_next) {
        next = _next;
    }

    node *node::get_next() {
        return next;
    }

    void text::set_type(int _type) {type = (txt_type)_type;}

    int text::read_from_file(std::ifstream *_ifstr, _mContainer*cont) {
        char *s = new char[255];
        std::string str;
        int operation = -1; // ADD = 0
        int type = 0;
        std::string open_text;
        int error_code = 0;
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
                text *txt = nullptr;
                if (type == txt_type::REPLACEMENT) txt = new txt_replacement();
                if (type == txt_type::CYCLE) txt = new txt_cycle();
                txt->set_type(type);
                error_code = txt->read(_ifstr);

                if (!error_code && txt) cont->append(txt);
                else delete txt;
                operation = -1;
            } else {
                error_code = 9;
            }
        }
        delete[] s;
        return error_code;
    }

    /// Методы txt_replacement
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
        res += "\nOpenText length:\n";
        res += int_to_str(counter_function());
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

    int txt_replacement::counter_function() {
        return open_txt->length();
    }

    /// Методы txt_cycle

    int txt_cycle::read(std::ifstream *_ifstr) {
        char *s = new char[255];
        int error_code = 0;
        std::string str;
        std::string _open_text;
        int step = 0;
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
                    open_txt = new std::string;
                    open_txt->append(_open_text);
                    shift = _shift;
                    step++;
                    break;
                default:
                    break;
            }
        }
        if(s) delete[] s;
        return error_code;
    }

    std::string *txt_cycle::get_open_txt() {
        return open_txt;
    }

    std::string *txt_cycle::get_cipher_txt() {
        if(cipher_txt == nullptr) cipher();
        return cipher_txt;
    }

    void txt_cycle::cipher() {
        std::string *res = new std::string;
        *res = *get_open_txt();
        int _shift = shift % 26;
        for(int i = 0; i < res->length(); i++) {
            bool upper_case = ((*res)[i] >= 'A' && (*res)[i] <= 'Z');
            if(((*res)[i] >= 'a' && (*res)[i] <= 'z') || ((*res)[i] >= 'A' && (*res)[i] <= 'Z')) {
                (*res)[i] += shift;
                if((((*res)[i] < 'a' || (*res)[i] > 'z') && !upper_case)
                    || (((*res)[i] < 'A' || (*res)[i] > 'Z') && upper_case))
                    (*res)[i] -= 26;
            }
        }
        cipher_txt = res;
    }

    std::string txt_cycle::info_string() {
        std::string res = "Cipher type: symbol cycle\n";
        res += "Open_text:\n";
        res += *get_open_txt();
        res += "\nShift: ";
        int _shift = shift;
        char *temp_shift = new char[16];
        _itoa_s(_shift, temp_shift, 10, 10);
        res += temp_shift;
        res += "\nCipher text:\n";
        res += *get_cipher_txt();
        res += "\nOpenText length:\n";
        res += int_to_str(counter_function());
        res += "\n";
        return res;
    }

    txt_cycle::txt_cycle() {
        shift = 0;
        cipher_txt = nullptr;
        open_txt = nullptr;
    }

    int txt_cycle::counter_function() {
        return open_txt->length();
    }

    /// Методы контейнера _mContainer

    /// Returns error_code:
    /// 0 - no error has occurred
    /// 1 - waited ">text", got other str
    /// 2 - waited ">replace", got other str
    /// 3 - waited ">with", got other str
    /// 4 - different string-"replace" and string-"with" size
    /// 5 - waited ">type", got other str
    /// 6 - type should by either 1 or 2
    /// 9 - waited !<command>, got other str

    _mContainer::_mContainer() {
        _end = _start = NULL;
    }

    void text::write_to_file(std::ofstream *_ofstr, _mContainer*cont) {
        std::string out_str = "";
        sort(cont);
        if(cont->start()) {
            for (text *i = (text*)cont->start(); ; i = (text*)i->get_next()) {
                out_str += i->info_string();
                out_str += "----------------\n";
                if(i == cont->end()) break;
            }
        } else {
            out_str += "Empty container\n";
        }
        _ofstr->write(out_str.c_str(), out_str.length());
    }

    node *_mContainer::text_at(int pos) {
        node *res = _start;
        for(int i = 0; i < pos; i++) {
            res = res->get_next();
        }
        return res;
    }

    bool _mContainer::remove(node *_node) {
        node *prev = _start;
        for(node *i = _start; i != _end; i = i->get_next()) {
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

    void _mContainer::append(node *_node) {
        if(_start) {
            _end->set_next(_node);
            _node->set_next(_start);
            _end = _node;

        } else {
            _node->set_next(_node);
            _start = _end = _node;
        }
    }

    node *_mContainer::end() {return _end;}

    node *_mContainer::start() {return _start;}

    void _mContainer::set_start(node *n) {
        _start = n;
    }

    void _mContainer::set_end(node *n) {
        _end = n;
    }

}