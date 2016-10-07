// Inspired by https://github.com/bundz/simpletest

#include <iostream>
#include <string>

// Colors
#define KNRM  std::string("\x1B[0m")
#define KRED  std::string("\x1B[31m")
#define KGRN  std::string("\x1B[32m")
#define KYEL  std::string("\x1B[33m")
#define KBLU  std::string("\x1B[34m")
#define KMAG  std::string("\x1B[35m")
#define KCYN  std::string("\x1B[36m")
#define KWHT  std::string("\x1B[37m")
#define PASSED (KGRN + "        PASSED!" + KNRM + "\n")
#define FAILED(text) (KRED + "        NOT PASSED!    " + (text) + KNRM + "\n")

// Text functions
void DESCRIBE(const std::string text);
void WHEN(const std::string text);
void IF(const std::string text);
void THEN(const std::string text);
void IFTHEN(const std::string text_if, const std::string text_then);

// Test functions
template <typename T> void isEqual (T a, T b);
template <typename T> void isDifferent (T a, T b);
template <typename T> void isLess (T a, T b);
template <typename T> void isLessOrEqual (T a, T b);
template <typename T> void isGreater (T a, T b);
template <typename T> void isGreaterOrEqual (T a, T b);
template <typename T> void isWithin (T a, T b, T c);
template <typename T> void isNull (T a);
template <typename T> void isNotNull (T a);
template <typename T> void isTrue (T a);
template <typename T> void isFalse (T a);

// Implementation of text functions

void DESCRIBE (const std::string text) {
    std::cout << KCYN + "-- " + text + " --" + KNRM + "\n";
}

void WHEN (const std::string text) {
    std::cout << KYEL + "  when:" + KNRM + " " + text + "\n";
}

void IF (const std::string text) {
    std::cout << KYEL + "    if:" + KNRM + " " + text + "\n";
}

void THEN (const std::string text) {
    std::cout << KYEL + "      then:" + KNRM + " " + text + "\n";
}

void IFTHEN(const std::string text_if, const std::string text_then) {
    IF(text_if);
    THEN(text_then);
}

// Implementation of test functions

template <typename T> void isEqual (T a, T b) {
    if ( a == b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " == " + std::to_string(b));
}

template <typename T> void isDifferent (T a, T b) {
    if ( a != b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " != " + std::to_string(b));
}

template <typename T> void isLess (T a, T b) {
    if ( a < b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " < " + std::to_string(b));
}

template <typename T> void isLessOrEqual (T a, T b) {
    if ( a <= b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " <= " + std::to_string(b));
}

template <typename T> void isGreater (T a, T b) {
    if ( a > b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " > " + std::to_string(b));
}

template <typename T> void isGreaterOrEqual (T a, T b) {
    if ( a >= b ) std::cout << PASSED;
    else std::cout << FAILED("got " + std::to_string(a) + " >= " + std::to_string(b));
}

template <typename T> void isWithin (T a, T b, T c) {
    if ( b > c ) std::swap(b,c);
    if ( a >= b )
        if ( a <= c ) std::cout << PASSED;
        else std::cout << FAILED("got " + std::to_string(a) + " <= " + std::to_string(c));
    else std::cout << FAILED("got " + std::to_string(a) + " >= " + std::to_string(b));
}

template <typename T> void isNull (T a) {
    if ( a == nullptr ) std::cout << PASSED;
    else std::cout << FAILED("pointer is not null");
}

template <typename T> void isNotNull (T a) {
    if ( a != nullptr ) std::cout << PASSED;
    else std::cout << FAILED("pointer is null");
}

template <typename T> void isTrue (T a) {
    if ( a ) std::cout << PASSED;
    else std::cout << FAILED("value is false");
}

template <typename T> void isFalse (T a) {
    if ( ! a ) std::cout << PASSED;
    else std::cout << FAILED("value is true");
}

