//
// Created by Artemiy.Kononov on 12/8/2019.
//

#include "Substitution.h"
#include "Equation.h"
#include "Type.h"

Substitution Substitution::Unificate(Equation equation) {
    bool done;
    do {
        done = false;
        forn(i, 0, size(equation.statements)) {
            if (equation.statements[i].Same()) {
                swap(equation.statements[i], equation.statements.back());
                equation.statements.pop_back();
                i--;
            } else if (equation.statements[i].r->IsVar() && !equation.statements[i].l->IsVar()) {
                swap(equation.statements[i].l, equation.statements[i].r);
                i--;
            } else if (!equation.statements[i].l->IsVar() && !equation.statements[i].r->IsVar()) {
                is(equation.statements[i].l, Implication*, lImpl);
                is(equation.statements[i].r, Implication*, rImpl);
                equation.statements.push_back(Equation::Statement(lImpl->from, rImpl->from));
                equation.statements.push_back(Equation::Statement(lImpl->to, rImpl->to));
                swap(equation.statements.back(), equation.statements[i]);
                equation.statements.pop_back();
                i--;
            } else if (equation.statements[i].l->IsVar() && !equation.statements[i].r->IsVar() &&
                       equation.statements[i].r->Contains(equation.statements[i].l)) {
                return Substitution(false);
            }
        }
        forn(i, 0, size(equation.statements)) {
            if (equation.statements[i].l->IsVar()) {
                forn(j, 0, size(equation.statements)) if (i != j) {
                        if (equation.statements[j].l->Contains(equation.statements[i].l)) {
                            equation.statements[j].l = equation.statements[j].l->Change(equation.statements[i].l,  equation.statements[i].r);
                            done = true;
                        }
                        if (equation.statements[j].r->Contains(equation.statements[i].l)) {
                            equation.statements[j].r = equation.statements[j].r->Change(equation.statements[i].l,  equation.statements[i].r);
                            done = true;
                        }
                    }
                if (done) {
                    break;
                }
            }
        }
    } while (done);
    return Substitution(equation);
}

Substitution::Substitution(bool _isValid) : isValid(_isValid) {}

Substitution::Substitution(Equation equation) : myEquation(), isValid(true) {
    myEquation.statements.insert(myEquation.statements.end(), all(equation.statements));
}

Type * Substitution::Substitute(Type *type) {
    if (type->IsVar()) {
        foraa(i, myEquation.statements) {
            if (type->Contains(i.l)) {
                return i.r->DeepCopy();
            }
        }
        return type->DeepCopy();
    }
    is(type, Implication*, pImpl);
    return new Implication(Substitute(pImpl->from), Substitute(pImpl->to));
}