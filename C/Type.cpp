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

Type *Implication::Change(Type *var, Type *expr) {
    auto l = from->Change(var, expr);
    auto r = from->Change(var, expr);
    if (from == l && to == r) {
        return this;
    }
    return new Implication(l, r);
}

string Implication::ToString() {
    return "(" + from->ToString() + " -> " + to->ToString() + ")";
}

Terminal::Terminal(std::string str) {
    type = TypeProjector[str];
    if (type == 0) {
        type = TypeProjector[str] = ++TypeCounter;
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

Type *Terminal::Change(Type *var, Type *expr) {
    is(var, Terminal*, v);
    if (v->type == type) {
        return expr;
    }
    return this;
}

string Terminal::ToString() {
    return "t" + to_string(type);
}

Type::~Type() {

}
