//
// Created by Artemiy.Kononov on 12/8/2019.
//
#include "main.h"
#include "Equation.h"

#ifndef C_SUBSTITUTION_H
#define C_SUBSTITUTION_H


class Substitution {
public:
    static Substitution Unificate(Equation equation);
    const bool isValid;
    Substitution(bool _isValid);
    Substitution(Equation equation);
    bool Substitute(Type*& type);
    Equation myEquation;
};


#endif //C_SUBSTITUTION_H
