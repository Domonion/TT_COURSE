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
                done = true;
            } else if (equation.statements[i].r->IsVar() && !equation.statements[i].l->IsVar()) {
                swap(equation.statements[i].l, equation.statements[i].r);
                i--;
                done = true;
            } else if (!equation.statements[i].l->IsVar() && !equation.statements[i].r->IsVar()) {
                is(equation.statements[i].l, Implication*, lImpl);
                is(equation.statements[i].r, Implication*, rImpl);
                equation.statements.push_back(Equation::Statement(lImpl->from, rImpl->from));
                equation.statements.push_back(Equation::Statement(lImpl->to, rImpl->to));
                swap(equation.statements.back(), equation.statements[i]);
                equation.statements.pop_back();
                done = true;
                i--;
            } else if (equation.statements[i].l->IsVar() && !equation.statements[i].r->IsVar() &&
                       equation.statements[i].r->Contains(equation.statements[i].l)) {
                return Substitution(false);
            }
        }
        //I should break every once i changed something, because otherwise it might break into infinite loop
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

Substitution::Substitution(Equation equation) : myEquation(equation), isValid(true) {}

bool Substitution::Substitute(Type *&type) {
    if (type->IsVar()) {
        foraa(i, myEquation.statements) {
            if (type->Contains(i.l)) {
                type = i.r;
                return true;
            }
        }

    }
    is(type, Implication*, pImpl);
    if (pImpl == nullptr) return true;
    bool res = false;
    if (pImpl->from->IsVar()) {
        foraa(i, myEquation.statements) {
            if (pImpl->from->Contains(i.l)) {
                pImpl->from = i.r;
                res = true;
                break;
            }
        }
    } else {
        res |= Substitute(pImpl->from);
    }
    if (pImpl->to->IsVar()) {
        foraa(i, myEquation.statements) {
            if (pImpl->to->Contains(i.l)) {
                pImpl->to = i.r;
                res = true;
                break;
            }
        }
    } else {
        res |= Substitute(pImpl->to);
    }
    return res;
}