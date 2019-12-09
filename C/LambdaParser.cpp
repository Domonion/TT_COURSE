//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "LambdaParser.h"
#include "main.h"

map<string_view, LambdaParser::Variable *> mapper;
vector<string> answer;
vector<string> hypos;
string openVars;
static int currentDepth = -1;

void writePrefix(LambdaParser::TreeNode *node, Substitution &substitution) {
    forn(i, 0, currentDepth) {
        cout << "*   ";
    }
    cout << openVars;
    if (size(hypos)) {
        cout << ", ";
    }
    forn(i, 0, size(hypos)) {
        cout << hypos[i];
        if (i != size(hypos) - 1) {
            cout << ", ";
        }
    }
    cout << " |- " << node->ToString() << " : ";
    auto nodeType = node->GetType();
    substitution.Substitute(nodeType);
    cout << nodeType->ToString() << '\n';

}

Type *withSubstituition(LambdaParser::TreeNode *node, Substitution &substitution) {
    auto res = node->GetType();
    substitution.Substitute(res);
    return res;
}
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
    if (!mapper.count(input)) {
        mapper[input] = new Variable(input);
    }
    return mapper[input];
}

std::string LambdaParser::Variable::ToString() const {
    string now;
    now.reserve(size(name));
    fora(i, name)now.push_back(i);
    return now;
}

LambdaParser::Variable::~Variable() {}

pr<Equation, Type *> LambdaParser::Variable::inferenceType() {
    if (myType == nullptr) {
        myType = new Terminal(ToString());
    }
    return pr<Equation, Type *>(Equation(), myType->DeepCopy());
}

Type *LambdaParser::Variable::GetType() {
    return myType;
}

void LambdaParser::Variable::Prove(Substitution &substitution) {
    currentDepth++;
    writePrefix(this, substitution);
    currentDepth--;
}

LambdaParser::Atom::Atom(LambdaParser::TreeNode *const _value) : value(_value) {}

LambdaParser::Atom *LambdaParser::Atom::Create(string_view input_raw) {
    string_view input = trimSpaces(input_raw);
    if (input[0] == '(' && input.back() == ')') {
        return new Atom(LambdaParser::Expression::Create(input.substr(1, size(input) - 2)));
    }
    return new Atom(LambdaParser::Variable::CreateUnchecked(input));
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

pr<Equation, Type *> LambdaParser::Atom::inferenceType() {
    return value->inferenceType();
}

Type *LambdaParser::Atom::GetType() {
    return value->GetType();
}

void LambdaParser::Atom::Prove(Substitution &substitution) {
    value->Prove(substitution);
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

pr<Equation, Type *> Combine(pr<Equation, Type *> p, pr<Equation, Type *> r) {
    Equation equation = Equation();
    equation.statements.insert(equation.statements.end(), all(p.first.statements));
    equation.statements.insert(equation.statements.end(), all(r.first.statements));
    Type *newTerm = new Terminal();
    equation.statements.push_back(Equation::Statement(p.sc, new Implication(r.sc, newTerm)));
    return pr<Equation, Type *>(equation, newTerm);
}

pr<Equation, Type *> LambdaParser::Use::inferenceType() {
    if (size(atoms) == 1) {
        return atoms.back()->inferenceType();
    }
    Atom *last = atoms.back();
    atoms.pop_back();
    auto resOther = inferenceType();
    auto resBack = last->inferenceType();
    atoms.push_back(last);
    auto combined = Combine(resOther, resBack);
    types.push_back(combined.sc->DeepCopy());
    return combined;
}

Type *LambdaParser::Use::GetType() {
    return types.back()->DeepCopy();
}

void LambdaParser::Use::Prove(Substitution &substitution) {
    if(size(atoms) == 1){
        atoms.back()->Prove(substitution);
    } else {
        currentDepth++;
        writePrefix(this, substitution);
        Type* lastType = types.back();
        types.pop_back();
        Atom* lastAtom = atoms.back();
        atoms.pop_back();
        this->Prove(substitution);
        lastAtom->Prove(substitution);
        atoms.push_back(lastAtom);
        types.push_back(lastType);
        currentDepth--;
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
        } else {
            mapper.erase(var_s);
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
        } //TODO check if this is ok?
        /* else {
            mapper.erase(var_s);
        }*/
        mapper[var_s] = var;
        Expression *res = new Expression(dynamic_cast<LambdaParser::Use *>(use),
                                         var,
                                         LambdaParser::Expression::Create(input.substr(ind + 1)));
        if (old != nullptr) {
            mapper[var_s] = old;
        } else {
            mapper.erase(var_s);
        }
        return res;
    }
}

std::string LambdaParser::Expression::ToString() const {
    if (IsUsage()) {
        return usage->ToString();
    } else {
        return (IsClosed() ? "" : "(" + usage->ToString() + " ") + "(\\" + variable->ToString() + "." +
               expr->ToString() +
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

pr<Equation, Type *> LambdaParser::Expression::inferenceType() {
    if (IsUsage()) {
        auto res = usage->inferenceType();
        myType = res.sc;
        return res;
    }
    if (IsClosed()) {
        NewLambdaType(variable->ToString());
        auto res = expr->inferenceType();
        auto kek =  pr<Equation, Type *>(res.fs, new Implication(new Terminal(variable->ToString()), res.sc));
        myType = kek.sc;
        return kek;
    }
    auto usage2 = usage;
    usage = nullptr;
    auto resExpr = inferenceType();
    usage = usage2;
    auto resUsage = usage->inferenceType();
    auto res =  Combine(resUsage, resExpr);
    myType = res.sc;
    return res;
}

Type *LambdaParser::Expression::GetType() {
    return myType->DeepCopy();
}

void LambdaParser::Expression::Prove(Substitution &substitution) {
    if (IsUsage()) {
        usage->Prove(substitution);
        return;
    } else if (IsClosed()) {
        currentDepth++;
        writePrefix(this, substitution);
        hypos.push_back(variable->ToString() + " : " + withSubstituition(variable, substitution)->ToString());
        expr->Prove(substitution);
        hypos.pop_back();
        currentDepth--;
    } else {
        currentDepth++;
        writePrefix(this, substitution);
        auto use = usage;
        usage = nullptr;
        this->Prove(substitution);
        usage = use;
        auto expression = expr;
        auto var = variable;
        variable = nullptr;
        expr = nullptr;
        this->Prove(substitution);
        expr = expression;
        variable = var;
        currentDepth--;
    }
}

LambdaParser::Expression *LambdaParser::Parse(std::string &input) {
    whitespaceToSpace(input);
    uniqueSpaces(input);
    string_view now = trimSpaces(input);
    return Expression::Create(now);
}