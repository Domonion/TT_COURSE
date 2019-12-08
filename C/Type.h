//
// Created by Artemiy.Kononov on 12/8/2019.
//

#include "main.h"

#ifndef C_TYPE_H
#define C_TYPE_H

struct Type {
public:
    ///мне нужно гарантировать, что типы от терминалов будут совпадать при равенстве
    ///при это на каждой импликации я должен выдавать новый тип совершенно
    ///мапчик из стрингвью в инты)
    virtual bool Contains(Type * type) = 0;
    virtual bool Change(Type * var, Type * expr) = 0;
    virtual bool IsVar() = 0;
    virtual string ToString() = 0;
    virtual ~Type();
};

struct Implication : Type {
    Type *from, *to;
    Implication(Type * left, Type * right);
    bool Contains(Type * type);
    bool Change(Type * var, Type * expr);
    bool IsVar();
    string ToString();
    ~Implication();
};

struct Terminal : Type {
    int type;
    Terminal();
    Terminal(std::string);
    bool Contains(Type * type);
    bool Change(Type * var, Type * expr);
    bool IsVar();
    string ToString();
    ~Terminal();
};

#endif //C_TYPE_H
