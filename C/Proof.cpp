//
// Created by domonion on 09.12.2019.
//

#include "Proof.hpp"

int currentDepth = -1;

map<string, int> hypos;

void Proof::Prove(LambdaParser::Expression *expr, Type *type) {
    currentDepth++;
    if (expr->IsUsage()) {
        Prove(expr->usage, type);
    } else if (expr->IsClosed()) {
        is(type, Implication*, pImplication);
        is(pImplication->from, Terminal*, pTerminal);
        string var = expr->variable->ToString();
        hypos[var] = pTerminal->type;
        forn(i, 0, currentDepth) {
            cout << "*   ";
        }
        cout << "|- " << var << " : " << type->ToString() << " [rule #3]\n";
        Prove(expr, pImplication->to);
        cout << type->ToString() << ' ';
        hypos.erase(var);
    } else {

    }
    currentDepth--;
}

void Proof::Prove(LambdaParser::Use *usage, Type *type) {
    currentDepth++;
    if (size(usage->atoms) == 1) {
        Prove(usage->atoms[0], type);
    } else {
        
    }
    currentDepth--;
}

void Proof::Prove(LambdaParser::Atom *expr, Type *type) {
    if (is(expr, LambdaParser::Variable*, var)) {
        Prove(var, type);
    } else {
        is(expr, LambdaParser::Expression*, exp);
        Prove(exp, type);
    }
}

void Proof::Prove(LambdaParser::Variable *expr, Type *type) {
    currentDepth++;
    forn(i, 0, currentDepth) {
        cout << "*   ";
    }
    string var = expr->ToString();
    string typeS = type->ToString();
    cout << var << " : " << typeS << " |- " << var << " : " << typeS << " [rule #3]\n";\
    currentDepth++;
}
