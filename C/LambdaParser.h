//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include <string>
#include <exception>
#include <string_view>
#include <vector>
#include "main.h"
#include "Equation.h"
#include "Type.h"

#ifndef A_LAMBDAPARSER_H
#define A_LAMBDAPARSER_H

struct LambdaParser {
public:

    struct TreeNode {
    public:

        virtual std::string ToString() const = 0;

        virtual ~TreeNode() = 0;

        virtual pr<Equation, Type> GetEquation() = 0;
    };

    struct Variable : public TreeNode {
    public:

        Variable(std::string_view _name);

        static Variable *CreateUnchecked(std::string_view input);

        std::string_view const name;

        std::string ToString() const;

        pr<Equation, Type> GetEquation();

        ~Variable();
    };

    struct Atom : public TreeNode {
    public:

        Atom(TreeNode *_value);

        TreeNode *value;

        static Atom *Create(std::string_view input);

        bool IsVariable() const;

        std::string ToString() const;

        pr<Equation, Type> GetEquation();

        ~Atom();
    };

    struct Use : public TreeNode {
    public:

        std::vector<Atom *> atoms;

        Use();

        static Use *Create(std::string_view input);

        std::string ToString() const;

        pr<Equation, Type> GetEquation();

        ~Use();
    };

    struct Expression : public TreeNode {
    public:

        Use *usage;
        TreeNode *variable;
        Expression *expr;

        Expression(Use *use, Variable *var, Expression *exp);

        static Expression *Create(std::string_view input);

        std::string ToString() const;

        pr<Equation, Type> GetEquation();

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string &input);
};


#endif //A_LAMBDAPARSER_H
