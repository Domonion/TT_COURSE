//
// Created by Artemiy.Kononov on 12/8/2019.
//

#include "Equation.h"

Equation::Statement::Statement(Type *left, Type *right) : l(left), r(right) {}

bool Equation::Statement::CheckSame(Type * nowl, Type* nowr) {
    if (nowl->IsVar() != nowr->IsVar()) {
        return false;
    }
    if (nowl->IsVar()) {
        return dynamic_cast<Terminal *>(nowl)->type == dynamic_cast<Terminal *>(nowr)->type;
    }
    is(nowl, Implication*, lImpl);
    is(nowr, Implication*, rImpl);
    return CheckSame(lImpl->from, rImpl->from) && CheckSame(lImpl->to, rImpl->to);
}

bool Equation::Statement::Same() {
    return CheckSame(l, r);
}
