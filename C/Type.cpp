//
// Created by Artemiy.Kononov on 12/8/2019.
//

#include "Type.h"


Implication::~Implication() {
    delete from;
    delete to;
}

Implication::Implication(Type *left, Type *right) {
    from = left;
    to = right;
}

bool Implication::IsVar() {
    return false;
}

bool Implication::Contains(Type *type) {
    return from->Contains(type) || to->Contains(type);
}

bool Implication::Change(Type *var, Type *expr) {
    is(var, Terminal*, varTerm);
    if(!from->IsVar()) {
        from->Change(var, expr);
    }
    else{
        is(from, Terminal*, fromTerm);
        if(fromTerm->type == varTerm->type) {
            from = expr;
        }
    }
    if(!to->IsVar()) {
        to->Change(var, expr);
    }
    else{
        is(to, Terminal*, toTerm);
        if(toTerm->type == varTerm->type) {
            to = expr;
        }
    }
}

Terminal::Terminal(std::string str) {
    type = TypeProjector[str];
    if (type == 0) {
        TypeProjector[str] = ++TypeCounter;
    }
}


Terminal::~Terminal() {}

Terminal::Terminal() {
    type = ++TypeCounter;
}

bool Terminal::IsVar() {
    return true;
}

bool Terminal::Contains(Type *jopa) {
    is(jopa, Terminal*, term);
    return term->type == type;
}

bool Terminal::Change(Type *var, Type *expr) {
    return true;
}
