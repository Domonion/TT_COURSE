//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "LambdaParser.h"
#include "Util.h"
#include "main.h"

int findClosed(string const &input, int startInd = 0) {
    if (input[startInd] == Util::OPEN) {
        int balance = 1;
        forn(i, startInd + 1, size(input)) {
            if (input[i] == Util::CLOSE && --balance == 0) {
                return i;
            } else if (input[i] == Util::OPEN) {
                ++balance;
            }
        }
    }
    return -1;
}

LambdaParser::TreeNode::~TreeNode() {}

LambdaParser::Variable::Variable(std::string const &_name) : name(_name) {}

LambdaParser::Variable *LambdaParser::Variable::CreateUnchecked(std::string const &input) {
    return new Variable(input);
}

LambdaParser::Variable *LambdaParser::Variable::Create(std::string const &input) {
    if (!regex_match(input, Util::mine.VARIABLE_REGEX)) {
        throw new ParserException(input + " is not variable", nullptr);
    }
    return new Variable(input);
}

bool LambdaParser::Variable::TryCreate(std::string const &input, LambdaParser::TreeNode *&var) {
    if (regex_match(input, Util::mine.VARIABLE_REGEX)) {
        var = new Variable(input);
        return true;
    }
    return false;
}

std::string const &LambdaParser::Variable::GetName() const {
    return name;
}

std::string const &LambdaParser::Variable::ToString() const {
    return GetName();
}

LambdaParser::Variable::~Variable() {}

LambdaParser::Atom::Atom(LambdaParser::TreeNode *const _value) : value(_value) {}

LambdaParser::Atom *LambdaParser::Atom::Create(std::string const &input) {
    LambdaParser::TreeNode *var = nullptr;
    if (!TryCreate(input, var)) {
        throw ParserException("didn't create Atom from input: " + input, nullptr);
    }
    return dynamic_cast<LambdaParser::Atom *>(var);
}

bool LambdaParser::Atom::TryCreate(std::string const &input, LambdaParser::TreeNode *&var) {
    bool done = false;
    if (input[0] == Util::OPEN && input.back() == Util::CLOSE) {
        //TODO no substr optimization
        TreeNode *expr;
        string buffer = input.substr(1, size(input) - 2);
        trimSpaces(buffer);
        if (LambdaParser::Expression::TryCreate(buffer, expr)) {
            var = new Atom(expr);
            done = true;
        }
    }
    if (!done) {
        //TODO createUncheked
        TreeNode *variable;
        if (LambdaParser::Variable::TryCreate(input, variable)) {
            var = new Atom(variable);
            done = true;
        }
    }
    return done;
}

bool LambdaParser::Atom::IsVariable() const {
    return dynamic_cast<Variable *>(value);
}

std::string const &LambdaParser::Atom::ToString() const {
    if (is(value, Expression *const, expr)) {
        return Util::mine.OPEN + expr->ToString() + Util::mine.CLOSE;
    } else if (is(value, Variable *const, var)) {
        return var->ToString();
    } else {
        throw new ParserException("Atom::ToString - Impossible cast", this);
    }
}

LambdaParser::TreeNode *LambdaParser::Atom::GetValue() const {
    return value;
}

LambdaParser::Atom::~Atom() {}

LambdaParser::Use::Use(LambdaParser::Atom *const _value, LambdaParser::Use *const _next) : value(_value), next(_next) {

}

LambdaParser::Use *LambdaParser::Use::Create(std::string const &input) {
    LambdaParser::TreeNode *var = nullptr;
    if (!TryCreate(input, var)) {
        throw ParserException("didn't create Atom from input: " + input, nullptr);
    }
    return dynamic_cast<LambdaParser::Use *>(var);
}

bool LambdaParser::Use::TryCreate(std::string const &input, LambdaParser::TreeNode *&var) {
    int ind = 0;
    bool res = false;
    vec<Atom *> atoms;
    while (ind < size(input)) {
        if (input[ind] == Util::OPEN) {
            int next = findClosed(input, ind);
            if (next != -1) {
                LambdaParser::TreeNode *atom;
                if (LambdaParser::Atom::TryCreate(input.substr(ind, next - ind + 1), atom)) {
                    atoms.push_back(dynamic_cast<Atom *>(atom));
                    ind = next;
                } else {
                    break;
                }
            } else {
                break;
            }
        } else if ('a' <= input[ind] && input[ind] <= 'z') {
            int was = ind;
            while (ind < size(input) &&
                   (('a' <= input[ind] && input[ind] <= 'z') || ('0' <= input[ind] && input[ind] <= '9') ||
                    (input[ind] != '(' && input[ind] != ')' && input[ind] != '.' && input[ind] != '\\')))
                ind++;
            ind--;
            LambdaParser::TreeNode *atom;
            if (LambdaParser::Atom::TryCreate(input.substr(was, ind - was + 1), atom)) {
                atoms.push_back(dynamic_cast<Atom *>(atom));
            } else {
                break;
            }
        }
        ind++;
    }
    if (ind >= size(input)) {
        res = true;
        LambdaParser::Use *last = nullptr;
        fora(i, atoms) {
            last = new Use(i, last);
        }
        var = last;
    }
    return res;
}

std::string const &LambdaParser::Use::ToString() const {
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

}

LambdaParser::Expression *LambdaParser::Expression::Create(std::string const &input) {
    LambdaParser::TreeNode *var = nullptr;
    if (!TryCreate(input, var)) {
        throw ParserException("didn't create Atom from input: " + input, nullptr);
    }
    return dynamic_cast<LambdaParser::Expression *>(var);
}

std::string const &LambdaParser::Expression::ToString() const {
    if (IsUsage()) {
        return usage->ToString();
    } else {
        return (IsClosed() ? "(" : "(" + usage->ToString()) + "\\" + variable->ToString() + "." + expr->ToString() +
               ")";
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

}

bool LambdaParser::Expression::TryCreate(std::string const &input, LambdaParser::TreeNode *&var) {

}

LambdaParser::Expression *LambdaParser::Parse(std::string input) {
    whitespaceToSpace(input);
    trimSpaces(input);
    return Expression::Create(input);
}

void LambdaParser::whitespaceToSpace(std::string &out) {
    foraa(i, out) {
        if (isspace(i)) {
            i = ' ';
        }
    }
}

void LambdaParser::trimSpaces(std::string &out) {
    out.resize(unique(all(out)) - out.begin());
    int len = 0;
    while (len < size(out) && out[len] == ' ') ++len;
    out.erase(0, len);
    while (!out.empty() && out.back() == ' ') out.pop_back();
}

LambdaParser::ParserException::ParserException(std::string const &_info, const LambdaParser::TreeNode *_node)
        : exception(), info(_info), node(_node) {}