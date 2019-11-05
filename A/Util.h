//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include <regex>
#include <string>
#include <random>

#ifndef A_UTIL_H
#define A_UTIL_H


struct Util {
private:

    static const Util mine;//TODO is it true that he will call default constructor?

    Util();

    Util(Util const &other) = delete;

    Util(Util const &&other) = delete;

    Util const &operator=(Util const &other) = delete;

    Util const &operator=(Util const &&other) = delete;

    Util const &operator=(Util &other) = delete;

    Util const &operator=(Util &&other) = delete;

public:
    static char const OPEN = '(';
    static char const CLOSE = ')';
    std::regex const VARIABLE_REGEX;//TODO check if regex works
    static Util const &GetInstance();
};


#endif //A_UTIL_H
