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
    if (!regex_match(input, Util::GetInstance().VARIABLE_REGEX)) {
        throw new ParserException(input + " is not variable", nullptr);
    }
    return new Variable(input);
}

bool LambdaParser::Variable::TryCreate(std::string const &input, LambdaParser::TreeNode *&var) {
    //TODO is it true that plain Variable * will pass?
    if (regex_match(input, Util::GetInstance().VARIABLE_REGEX)) {
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
        Expression *expr = LambdaParser::Expression::Create(input.substr(1, size(input) - 2));
        var = new Atom(expr);
        done = true;
    }
    if (!done) {
        //TODO createUncheked
        Variable *variable = LambdaParser::Variable::Create(input);
        var = new Atom(variable);
        done = true;
    }
    return done;
}

bool LambdaParser::Atom::IsVariable() const {
    return dynamic_cast<Variable *>(value);
}

std::string const &LambdaParser::Atom::ToString() const {
    if (is(value, Expression *const, expr)) {
        return Util::GetInstance().OPEN + expr->ToString() + Util::GetInstance().CLOSE;
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

std::string const &LambdaParser::Use::ToString() const {
    if (next == nullptr)
        return value->ToString();
    else {
        return Util::GetInstance().OPEN + next->ToString() + " " + value->ToString() + Util::GetInstance().CLOSE;
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

////TODO is it true that plain Variable * will pass?
//todo static threadlocal random in util
////todo 2. for use stop sumbols are: \ and eof

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
}

LambdaParser::ParserException::ParserException(std::string const &_info, const LambdaParser::TreeNode *_node)
        : exception(), info(_info), node(_node) {}