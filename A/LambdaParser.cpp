//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "LambdaParser.h"
#include "Util.h"
#include "main.h"

bool valid(char c) {
    return c != '.' && c != '\\' && c != ' ' && c != '(' && c != ')';
}

int findCharWithBalance(string_view const &input, char c, int balance = 0, int startInd = 0, int endInd = 2000000000) {
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

bool LambdaParser::Variable::TryCreate(string_view input, TreeNode *&var) {
    fora(i, input)
        if (!valid(i))
            return false;
    string res;
    fora(i, input)res.push_back(i);
    var = new Variable(res);
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

LambdaParser::Atom * LambdaParser::Atom::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    if (input[0] == Util::OPEN && input.back() == Util::CLOSE) {
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

LambdaParser::TreeNode *LambdaParser::Atom::GetValue() const {
    return value;
}

LambdaParser::Atom::~Atom() {
    delete value;
}

LambdaParser::Use::Use(LambdaParser::Atom *const _value, LambdaParser::Use *const _next) : value(_value), next(_next) {}

LambdaParser::Use* LambdaParser::Use::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
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
            ind = 0;
            break;
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
    } else {
        fora(i, atoms)delete i;
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

LambdaParser::Use::~Use() {
    delete value;
    delete next;
}

LambdaParser::Expression::Expression(LambdaParser::Use *use, LambdaParser::Variable *var, LambdaParser::Expression *exp)
        : usage(use), variable(var), expr(exp) {}

LambdaParser::Expression* LambdaParser::Expression::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    if (LambdaParser::Use::TryCreate(input, var)) {
        return new Expression(Use::Create(input), nullptr, nullptr);
    } else if (input[0] == '\\') {
        int ind = 0;
        while (input[ind] != '.') ind++;
        return new Expression(nullptr, LambdaParser::Variable::CreateUnchecked(input.substr(1, ind - 1)), LambdaParser::Expression::Create(input.substr(ind + 1)));
    } else {
        ///expression with use
        int ind = findCharWithBalance(input, '\\');
        if (ind == -1) {
            fora(i, input)
                if (i == '\\')
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
    //todo tostring maybe very very long
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

LambdaParser::Expression::~Expression() {
    delete usage;
    delete variable;
    delete expr;
}

LambdaParser::Expression *LambdaParser::Parse(std::string &input) {
    whitespaceToSpace(input);
    uniqueSpaces(input);
    string_view now = trimSpaces(input);
    return Expression::Create(now);;
}

void LambdaParser::whitespaceToSpace(std::string &out) {
    foraa(i, out) {
        if (isspace(i)) {
            i = ' ';
        }
    }
}

string_view LambdaParser::trimSpaces(string const &input) {
    string_view res(input.c_str(), size(input));
    return trimSpaces(res);
}

std::string_view LambdaParser::trimSpaces(std::string_view const &input) {
    string_view res(input);
    int len = 0;
    while (len < size(res) && res[len] == ' ') ++len;
    res.remove_prefix(len);
    len = 0;
    while (len < size(res) && res[size(res) - len - 1] == ' ') len++;
    res.remove_suffix(len);
    return res;
}

void LambdaParser::uniqueSpaces(std::string &out) {
    int now = 0;
    forn(i, 1, size(out)) {
        if (out[i] != ' ' || out[i] != out[now]) {
            out[++now] = out[i];
        }
    }
    out.resize(now + 1);
}