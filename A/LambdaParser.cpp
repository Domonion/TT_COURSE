//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "LambdaParser.h"
#include "Util.h"
#include "main.h"

bool valid(char c) {
    return c != '.' && c != '\\' && c != ' ' && c != '(' && c != ')';
}

int findCharWithBalance(string const &input, char c, int balance = 0, int startInd = 0, int endInd = 2000000000) {
    int now_balance = 0;
    endInd = min(endInd, size(input));
    forn(i, startInd, endInd) {
        if (input[i] == Util::CLOSE) {
            --now_balance;
        } else if (input[i] == Util::OPEN) {
            ++now_balance;
        }
        if (now_balance == balance && input[i] == c)
            return i;
    }
    return -1;
}

LambdaParser::TreeNode::~TreeNode() {}

LambdaParser::Variable::Variable(std::string const _name) : name(_name) {}

LambdaParser::Variable *LambdaParser::Variable::CreateUnchecked(std::string input) {
    return new Variable(input);
}

bool LambdaParser::Variable::TryCreate(std::string input, LambdaParser::TreeNode *&var) {
    fora(i, input)if (!valid(i))
            return false;
    var = new Variable(input);
    return true;
}

std::string LambdaParser::Variable::GetName() const {
    return name;
}

std::string LambdaParser::Variable::ToString() const {
    return GetName();
}

LambdaParser::Variable::~Variable() {}

LambdaParser::Atom::Atom(LambdaParser::TreeNode *const _value) : value(_value) {}

bool LambdaParser::Atom::TryCreate(std::string input, LambdaParser::TreeNode *&var) {
    trimSpaces(input);
    if (input.empty())
        throw exception();
    if (input[0] == Util::OPEN && input.back() == Util::CLOSE) {
        TreeNode *expr;
        if (LambdaParser::Expression::TryCreate(input.substr(1, size(input) - 2), expr)) {
            var = new Atom(expr);
            return true;
        }
        throw exception();
    }
    TreeNode *variable;
    if (LambdaParser::Variable::TryCreate(input, variable)) {
        var = new Atom(variable);
        return true;
    }
    throw exception();
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

LambdaParser::TreeNode *LambdaParser::Atom::GetValue() const {
    return value;
}

LambdaParser::Atom::~Atom() {}

LambdaParser::Use::Use(LambdaParser::Atom *const _value, LambdaParser::Use *const _next) : value(_value), next(_next) {}

bool LambdaParser::Use::TryCreate(std::string input, LambdaParser::TreeNode *&var) {//TODO shit there shit happens
    trimSpaces(input);
    if (input.empty())
        throw exception();
    int ind = 0;
    bool res = false;
    vec<Atom *> atoms;
    while (ind < size(input)) {
        if (input[ind] == Util::OPEN) {
            int next = findCharWithBalance(input, Util::CLOSE, 0, ind);
            if (next != -1) {
                LambdaParser::TreeNode *atom;
                if (LambdaParser::Atom::TryCreate(input.substr(ind, next - ind + 1), atom)) {
                    atoms.push_back(dynamic_cast<Atom *>(atom));
                    ind = next;
                } else {
                    throw exception();
                }
            } else {
                throw exception();
            }
        } else if ('a' <= input[ind] && input[ind] <= 'z') {
            int was = ind;
            while (ind < size(input) && valid(input[ind]))
                ind++;
            LambdaParser::TreeNode *atom;
            if (LambdaParser::Atom::TryCreate(input.substr(was, ind - was), atom)) {
                atoms.push_back(dynamic_cast<Atom *>(atom));
                ind--;
            } else {
                throw exception();
            }
        } else if (input[ind] == '\\' || input[ind] == '.') {
            return false;
        }
        ind++;
    }
    if (ind >= size(input) && size(atoms) > 0) {
        res = true;
        LambdaParser::Use *last = nullptr;
        fora(i, atoms) {
            last = new Use(i, last);
        }
        var = last;
    }
    return res;
}

std::string LambdaParser::Use::ToString() const {
    if (next == nullptr)
        return value->ToString();
    else {
        return Util::OPEN + next->ToString() + " " + value->ToString() + Util::CLOSE;
    }
}

LambdaParser::Atom *LambdaParser::Use::GetValue() const {
    return value;
}

bool LambdaParser::Use::IsLast() const {
    return next == nullptr;
}

LambdaParser::Use *LambdaParser::Use::GetNext() const {
    return next;
}

LambdaParser::Use::~Use() {}

LambdaParser::Expression::Expression(LambdaParser::Use *use, LambdaParser::Variable *var, LambdaParser::Expression *exp)
        : usage(use), variable(var), expr(exp) {}

bool LambdaParser::Expression::TryCreate(std::string input, LambdaParser::TreeNode *&var) {
    trimSpaces(input);
    if (input.empty())
        throw exception();
    if (LambdaParser::Use::TryCreate(input, var)) {
        var = new Expression(dynamic_cast<Use *>(var), nullptr, nullptr);
        return true;
    } else if (input[0] == '\\') {
        int ind = 0;
        while (ind < size(input) && input[ind] != '.') ind++;
        if (ind == size(input))
            throw exception();
        LambdaParser::TreeNode *variable = nullptr;
        if (!LambdaParser::Variable::TryCreate(input.substr(1, ind - 1), variable)) {
            throw exception();
        }
        LambdaParser::TreeNode *expr = nullptr;
        if (!LambdaParser::Expression::TryCreate(input.substr(ind + 1), expr)) {
            throw exception();
        }
        var = new Expression(nullptr, dynamic_cast<LambdaParser::Variable *>(variable),
                             dynamic_cast<LambdaParser::Expression *>(expr));
        return true;
        ///expression without use
    } else {
        ///expression with use
        int ind = findCharWithBalance(input, '\\');
        if (ind == -1) {
            fora(i, input)if (i == '\\')
                    throw exception();
            return false;
        }
        LambdaParser::TreeNode *use = nullptr;
        if (!LambdaParser::Use::TryCreate(input.substr(0, ind), use)) {
            throw exception();
        }
        /// here ind should be \.
        int was = ind + 1;
        while (ind < size(input) && input[ind] != '.') ind++;
        if (ind == size(input))
            throw exception();
        LambdaParser::TreeNode *variable = nullptr;
        if (!LambdaParser::Variable::TryCreate(input.substr(was, ind - was), variable)) {
            throw exception();
        }
        LambdaParser::TreeNode *expr = nullptr;
        if (!LambdaParser::Expression::TryCreate(input.substr(ind + 1), expr)) {
            throw exception();
        }
        var = new Expression(dynamic_cast<LambdaParser::Use *>(use), dynamic_cast<LambdaParser::Variable *>(variable),
                             dynamic_cast<LambdaParser::Expression *>(expr));
        return true;
    }
}

std::string LambdaParser::Expression::ToString() const {
    if (IsUsage()) {
        return usage->ToString();
    } else {
        return (IsClosed() ? "" : "(" + usage->ToString() + " ") + "(\\" + variable->ToString() + "." + expr->ToString() +
               ")" + (IsClosed() ? "" : ")");
    }
}

LambdaParser::Use *LambdaParser::Expression::GetUsage() const {
    return usage;
}

LambdaParser::Variable *LambdaParser::Expression::GetVariable() const {
    return variable;
}

LambdaParser::Expression *LambdaParser::Expression::GetExpression() const {
    return expr;
}

bool LambdaParser::Expression::IsUsage() const {
    return expr == nullptr;
}

bool LambdaParser::Expression::IsClosed() const {
    return !IsUsage() && usage == nullptr;
}

LambdaParser::Expression::~Expression() {}

LambdaParser::Expression *LambdaParser::Parse(std::string input) {
    whitespaceToSpace(input);
    trimSpaces(input);
    TreeNode *res = nullptr;
    Expression::TryCreate(input, res);
//    if(!Expression::TryCreate(input, res))
//        throw exception();
    return dynamic_cast<Expression *>(res);
}

void LambdaParser::whitespaceToSpace(std::string &out) {
    foraa(i, out) {
        if (isspace(i)) {
            i = ' ';
        }
    }
}

void LambdaParser::trimSpaces(std::string &out) {
    if (out.empty()) return;
    string res;
    res.push_back(out[0]);
    forn(i, 1, size(out))if (out[i] != ' ' || out[i] != res.back())
            res.push_back(out[i]);
    out = res;
    int len = 0;
    while (len < size(out) && out[len] == ' ') ++len;
    out.erase(0, len);
    while (!out.empty() && out.back() == ' ') out.pop_back();
}