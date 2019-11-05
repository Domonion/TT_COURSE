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

    //TODO is it true that he will call default constructor?

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
    std::regex const VARIABLE_REGEX;
    //TODO check if regex works
    //TODO may be static too?
    static const Util mine = Util();
};


#endif //A_UTIL_H
