//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "Equation.h"
#include "Type.h"
#include "Substitution.h"
#include "main.h"

#ifndef A_LAMBDAPARSER_H
#define A_LAMBDAPARSER_H

struct LambdaParser {
public:

    struct TreeNode {
    public:

        virtual std::string ToString() const = 0;

        virtual ~TreeNode() = 0;

        virtual Type *GetType() = 0;

        virtual void Prove(Substitution &substitution) = 0;

        virtual pr<Equation, Type *> inferenceType() = 0;
    };

    struct Variable : public TreeNode {
    public:

        Type *myType;

        Variable(std::string_view _name);

        static Variable *CreateUnchecked(std::string_view input);

        std::string_view const name;

        std::string ToString() const;

        Type *GetType();

        pr<Equation, Type *> inferenceType();

        void Prove(Substitution &substitution);

        ~Variable();
    };

    struct Atom : public TreeNode {
    public:

        Atom(TreeNode *_value);

        Type *GetType();

        TreeNode *value;

        static Atom *Create(std::string_view input);

        std::string ToString() const;

        void Prove(Substitution &substitution);

        pr<Equation, Type *> inferenceType();

        ~Atom();
    };

    struct Use : public TreeNode {
    public:

        std::vector<Atom *> atoms;
        std::vector<Type *> types;

        Type *GetType();

        Use();

        static Use *Create(std::string_view input);

        void Prove(Substitution &substitution);

        std::string ToString() const;

        pr<Equation, Type *> inferenceType();

        ~Use();
    };

    struct Expression : public TreeNode {
    public:

        Use *usage;
        TreeNode *variable;
        Expression *expr;
        Type* myType;

        Type *GetType();

        Expression(Use *use, Variable *var, Expression *exp);

        static Expression *Create(std::string_view input);

        void Prove(Substitution &substitution);

        std::string ToString() const;

        pr<Equation, Type *> inferenceType();

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string &input);
};

extern string openVars;
Type *withSubstituition(LambdaParser::TreeNode *node, Substitution &substitution);
extern map<string_view, LambdaParser::Variable *> mapper;

#endif //A_LAMBDAPARSER_H
