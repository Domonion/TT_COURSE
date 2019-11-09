//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "LambdaParser.h"
#include "main.h"

map<string_view, LambdaParser::Variable *> mapper;

bool valid(char c) {
    return c != '.' && c != '\\' && c != ' ' && c != '(' && c != ')';
}

int findCharWithBalance(string_view const &input, char c, int balance = 0, int startInd = 0, int endInd = 2000000000) {
    int now_balance = 0;
    endInd = min(endInd, size(input));
    forn(i, startInd, endInd) {
        if (input[i] == '(') {
            --now_balance;
        } else if (input[i] == ')') {
            ++now_balance;
        }
        if (now_balance == balance && input[i] == c)
            return i;
    }
    return -1;
}


void whitespaceToSpace(string &out) {
    foraa(i, out) {
        if (isspace(i)) {
            i = ' ';
        }
    }
}

string_view trimSpaces(string_view const &input) {
    string_view res(input);
    int len = 0;
    while (len < size(res) && res[len] == ' ') ++len;
    res.remove_prefix(len);
    len = 0;
    while (len < size(res) && res[size(res) - len - 1] == ' ') len++;
    res.remove_suffix(len);
    return res;
}


string_view trimSpaces(string const &input) {
    string_view res(input.c_str(), size(input));
    return trimSpaces(res);
}

void uniqueSpaces(string &out) {
    int now = 0;
    forn(i, 1, size(out)) {
        if (out[i] != ' ' || out[i] != out[now]) {
            out[++now] = out[i];
        }
    }
    out.resize(now + 1);
}

LambdaParser::TreeNode::~TreeNode() {}

LambdaParser::Variable::Variable(std::string_view const _name) : name(_name) {}

LambdaParser::Variable *LambdaParser::Variable::CreateUnchecked(std::string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    if (mapper.count(input)) {
        return mapper[input];
    }
    return new Variable(input);
}

std::string LambdaParser::Variable::ToString() const {
    string now;
    now.reserve(size(name));
    fora(i, name)now.push_back(i);
    return now;
}

LambdaParser::Variable::~Variable() {}

bool LambdaParser::Variable::Reduct() {
    return false;
}

LambdaParser::Atom::Atom(LambdaParser::TreeNode *const _value) : value(_value) {}

LambdaParser::Atom *LambdaParser::Atom::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    if (input[0] == '(' && input.back() == ')') {
        return new Atom(LambdaParser::Expression::Create(input.substr(1, size(input) - 2)));
    }
    return new Atom(LambdaParser::Variable::CreateUnchecked(input));
}

bool LambdaParser::Atom::IsVariable() const {
    return dynamic_cast<Variable *>(value);
}

std::string LambdaParser::Atom::ToString() const {
    if (is(value, Expression *const, expr)) {
        return expr->ToString();
    } else if (is(value, Variable *const, var)) {
        return var->ToString();
    } else {
        return "";
    }
}

LambdaParser::Atom::~Atom() {
    delete value;
}

bool LambdaParser::Atom::Reduct() {
    return value->Reduct();
}

LambdaParser::Use::Use() {}

LambdaParser::Use *LambdaParser::Use::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    int ind = 0;
    LambdaParser::Use *last = new Use();
    while (ind < size(input)) {
        if (input[ind] == '(') {
            int next = findCharWithBalance(input, ')', 0, ind);
            last->atoms.push_back(LambdaParser::Atom::Create(input.substr(ind, next - ind + 1)));
            ind = next;
        } else if ('a' <= input[ind] && input[ind] <= 'z') {
            int was = ind;
            while (ind < size(input) && valid(input[ind]))
                ind++;
            last->atoms.push_back(LambdaParser::Atom::Create(input.substr(was, ind - was)));
            ind--;
        }
        ind++;
    }

    return last;
}

std::string LambdaParser::Use::ToString() const {
    string res;
    int ind = 0;
    while (ind < size(atoms)) {
        if (ind == 0)
            res = atoms[ind]->ToString();
        else {
            res = '(' + res + " " + atoms[ind]->ToString() + ')';
        }
        ind++;
    }
    return res;
}

LambdaParser::Use::~Use() {
    fora(i, atoms)delete i;
}

bool LambdaParser::Use::Reduct() {
    forn(i, 0, size(atoms)) {
        if(i < size(atoms) - 1){
            if(!atoms[i]->IsVariable()){
                REDUCTON;
                return true;
            }
            //todo what if i am the only one and left of me is expr?
            //todo is it right order of reduction:
            if(atoms[i]->Reduct()){
                return true;
            }
        }
    }
}

LambdaParser::Expression::Expression(LambdaParser::Use *use, LambdaParser::Variable *var, LambdaParser::Expression *exp)
        : usage(use), variable(var), expr(exp) {}

LambdaParser::Expression *LambdaParser::Expression::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    bool kek = findCharWithBalance(input, '.') == -1 && findCharWithBalance(input, '\\') == -1;
    if (kek) {
        return new Expression(Use::Create(input), nullptr, nullptr);
    } else if (input[0] == '\\') {
        int ind = 0;
        while (input[ind] != '.') ind++;
        string_view var_s = input.substr(1, ind - 1);
        Variable *var = LambdaParser::Variable::CreateUnchecked(var_s);
        Variable *old = nullptr;
        if (mapper.count(var_s)) {
            old = mapper[var_s];
        }
        mapper[var_s] = var;
        Expression *res = new Expression(nullptr, var,
                                         LambdaParser::Expression::Create(input.substr(ind + 1)));
        if (old != nullptr) {
            mapper[var_s] = old;
        }
        return res;
    } else {
        int ind = findCharWithBalance(input, '\\');
        LambdaParser::Use *use = LambdaParser::Use::Create(input.substr(0, ind));
        int was = ind + 1;
        while (input[ind] != '.') ind++;
        string_view var_s = input.substr(was, ind - was);
        Variable *var = LambdaParser::Variable::CreateUnchecked(var_s);
        Variable *old = nullptr;
        if (mapper.count(var_s)) {
            old = mapper[var_s];
        }
        mapper[var_s] = var;
        Expression *res = new Expression(dynamic_cast<LambdaParser::Use *>(use),
                                         var,
                                         LambdaParser::Expression::Create(input.substr(ind + 1)));
        if (old != nullptr) {
            mapper[var_s] = old;
        }
        return res;
    }
}

std::string LambdaParser::Expression::ToString() const {
    //TODO that is very very long(
    if (IsUsage()) {
        return usage->ToString();
    } else {
        return (IsClosed() ? "" : "(" + usage->ToString() + " ") + "(\\" + variable->ToString() + "." + expr->ToString() +
               ")" + (IsClosed() ? "" : ")");
    }
}

bool LambdaParser::Expression::IsUsage() const {
    return expr == nullptr;
}

bool LambdaParser::Expression::IsClosed() const {
    return !IsUsage() && usage == nullptr;
}

LambdaParser::Expression::~Expression() {
    delete usage;
    delete variable;
    delete expr;
}

bool LambdaParser::Expression::Reduct() {
    if (IsUsage()) {
        return usage->Reduct();
    } else {
        if (IsClosed()) {
            return expr->Reduct();
        } else {
            if (usage->Reduct()) {
                return true;
            } else {
                if (size(usage->atoms) == 1) {
                    Atom *last = usage->atoms.back();
                    if (!last->IsVariable()) {
                        REDUCTION;
                        return true;
                    }
                }
                return expr->Reduct();
            }
        }
    }
}

LambdaParser::Expression *LambdaParser::Parse(std::string &input) {
    whitespaceToSpace(input);
    uniqueSpaces(input);
    string_view now = trimSpaces(input);
    return Expression::Create(now);
}