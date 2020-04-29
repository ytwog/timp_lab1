#include "../text.h"
#include <gtest/gtest.h>

using namespace mLab;

int executeProgram(std::ofstream &ofstr, std::ifstream &ifstr) {
    setlocale(LC_ALL, 0);

    if(ifstr.bad() || ifstr.fail() || ofstr.bad() || ofstr.fail()) {
        return print_err(7);
    }
    _mContainer container;
    int error_code = text::read_from_file(&ifstr, &container);
    container.sort();
    return error_code;
}

// Проверка на отсутствие ошибок при стандартном раскладе
TEST(Unit_test, TestNoErrors) {
    std::ifstream ifstr("input.txt");
    std::ofstream ofstr("output.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 0);
}

// Проверка на обработку ошибки при некорректном вводе метки >text
TEST(Unit_test, TestError1) {
    std::ifstream ifstr("tests_1/inperr1.txt");
    std::ofstream ofstr("outputQ.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 1);
}

// Проверка на обработку ошибки при некорректном вводе метки >replace
TEST(Unit_test, TestError2) {

    std::ifstream ifstr("tests_1/inperr2.txt");
    std::ofstream ofstr("outputQ.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 2);
}

// Проверка на обработку ошибки при некорректном вводе метки >with
TEST(Unit_test, TestError3) {
    std::ifstream ifstr("tests_1/inperr3.txt");
    std::ofstream ofstr("outputQ.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 3);
}


// Проверка на обработку ошибки при различии длины исходного и заменяемого алфавита
TEST(Unit_test, TestError4) {

    std::ifstream ifstr("tests_1/inperr4.txt");
    std::ofstream ofstr("outputQ.txt");
    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 4);
}

// Проверка на обработку ошибки при некорректном вводе метки >type
TEST(Unit_test, TestError5) {
    std::ifstream ifstr("tests_1/inperr5.txt");
    std::ofstream ofstr("outputQ.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 5);
}


// Проверка на обработку ошибки
TEST(Unit_test, TestError6) {
    std::ifstream ifstr("tests_1/inperr6.txt");
    std::ofstream ofstr("outputQ.txt");
    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 13);
}


// Проверка на обработку ошибки при вводе некорректного типа
TEST(Unit_test, TestError9) {

    std::ifstream ifstr("tests_1/inperr9.txt");
    std::ofstream ofstr("outputQ.txt");

    int finalCode = executeProgram(ofstr, ifstr);
    EXPECT_EQ(finalCode, 9);
}


// Проверка на корректность сортировки
TEST(Unit_test, TestSort) {
    std::ifstream ifstr("tests_1/inptest.txt");
    std::ofstream ofstr("outputQ.txt");
    if(ifstr.bad() || ifstr.fail() || ofstr.bad() || ofstr.fail()) {
        // Тест провален
        EXPECT_FALSE(true);
    }
    _mContainer container;
    text::read_from_file(&ifstr, &container);
    text::write_to_file(&ofstr, &container);
    container.sort();

    int prev = ((text*)container.start())->counter_function();
    for(auto i = container.start()->get_next(); i != nullptr; i = i->get_next()) {
        if(prev < ((text*)i)->counter_function()) {
            //EXPECT_FALSE(true);
            return;
        }
        prev = ((text*)i)->counter_function();
    }
    text::write_to_file(&ofstr, &container);
    ofstr.close();
}
