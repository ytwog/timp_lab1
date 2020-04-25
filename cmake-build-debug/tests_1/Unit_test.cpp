#include "../text.h"
#include <gtest/gtest.h>

using namespace mLab;

int executeProgram(std::ofstream &ofstr, std::ifstream &ifstr) {
    setlocale(LC_ALL, 0);

    if(ifstr.bad() || ifstr.fail() || ofstr.bad() || ofstr.fail()) {
        return print_err(7);
    }
    std::cout << "Program started"<< std::endl;
    _mContainer container;
    int error_code = print_err(text::read_from_file(&ifstr, &container));
    container.sort();
    text::write_to_file(&ofstr, &container);
    std::cout << "Program finished" << std::endl;
    return error_code;
}

// Проверка на отсутствие ошибок при стандартном раскладе
TEST(Unit_test, TestNoErrors) {
    char *arg1 = new char[10]{'i', 'n', 'p', 'u', 't', '.', 't', 'x', 't', '\0'};
    char *arg2 = new char[12]{'o', 'u', 't', 'p', 'u', 't', '1', '.', 't', 'x', 't','\0'};
    std::ifstream ifstr(arg1);
    std::ofstream ofstr(arg2);

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 0);
    delete [] arg1;
    delete [] arg2;
}

// Проверка на обработку ошибки при невозможности открытия файла
TEST(Unit_test, TestError1) {
    char *arg1 = new char[22]{'t', 'e', 's', 't', 's', '_', '1', '/', 'i', 'n', 'p', 'e', 'r', 'r', '1', '.', 't', 'x', 't', '\0'};
    char *arg2 = new char[12]{'o', 'u', 't', 'p', 'u', 't', 'Q', '.', 't', 'x', 't', '\0'};
    std::ifstream ifstr(arg1);
    std::ofstream ofstr(arg2);

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 7);
    delete [] arg1;
    delete [] arg2;
}


// Проверка на обработки ошибки при невозможности открытия файла
TEST(Unit_test, TestError2) {
    char *arg1 = new char[22]{'t', 'e', 's', 't', 's', '_', '1', '/', 'i', 'n', 'p', 'e', 'r', 'r', '2', '.', 't', 'x', 't', '\0'};
    char *arg2 = new char[12]{'o', 'u', 't', 'p', 'u', 't', 'Q', '.', 't', 'x', 't', '\0'};
    std::ifstream ifstr(arg1);
    std::ofstream ofstr(arg2);

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 7);
    delete [] arg1;
    delete [] arg2;
}
