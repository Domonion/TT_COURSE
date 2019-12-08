//
// Created by Artemiy.Kononov on 12/8/2019.
//
#include "Type.h"
#include "main.h"

#ifndef C_EQUATION_H
#define C_EQUATION_H


struct Equation {
public:
    struct Statement {
    public:
        Type *l, *r;
        Statement(Type* left, Type *right);
        bool Same();
    private:
        static bool CheckSame(Type * l1, Type* r1);
    };

    vec<Statement> statements;
};


#endif //C_EQUATION_H
