//
// Created by Artemiy.Kononov on 12/8/2019.
//

#include "Type.h"

map<string, int> TypeProjector;

int TypeCounter = 0;

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
    bool res = false;
    is(var, Terminal*, varTerm);
    if(!from->IsVar()) {
        res |= from->Change(var, expr);
    }
    else{
        is(from, Terminal*, fromTerm);
        if(fromTerm->type == varTerm->type) {
            from = expr;
            res = true;
        }
    }
    if(!to->IsVar()) {
        res |= to->Change(var, expr);
    }
    else{
        is(to, Terminal*, toTerm);
        if(toTerm->type == varTerm->type) {
            to = expr;
            res = true;
        }
    }
    return res;
}

string Implication::ToString() {
    return "(" + from->ToString() + " -> " + to->ToString() + ")";
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

string Terminal::ToString() {
    return "t" + to_string(type);
}

Type::~Type() {

}
