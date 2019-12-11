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
    auto r = to->Change(var, expr);
    if (from == l && to == r) {
        return this;
    }
    return new Implication(l, r);
}

string Implication::ToString() {
    return "(" + from->ToString() + " -> " + to->ToString() + ")";
}

Implication *Implication::DeepCopy() {
    return new Implication(from->DeepCopy(), to->DeepCopy());
}

Terminal::Terminal(std::string str) {
    if(!TypeProjector.count(str)){
        TypeProjector[str] = ++TypeCounter;
    }
    type = TypeProjector[str];
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

Terminal *Terminal::DeepCopy() {
    return new Terminal(type);
}

Terminal::Terminal(int tp) : type(tp) {

}

Type::~Type() {

}

void NewLambdaType(string s) {
    TypeProjector[s] = ++TypeCounter;
}
